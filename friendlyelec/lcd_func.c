#include <stdarg.h>
#include <common.h>

#include <friendlyelec/lcd_hw.h>
#include <friendlyelec/lcd_func.h>
#include "font_8x16.inc"
#include <linux/types.h>
#include <asm-arm/sizes.h>

#define BACKGROUND_COLOR    ( ( (  BACKGROUND_R >> 3) << 11) | ((  BACKGROUND_G >> 2) << 5) | (  BACKGROUND_B >> 3) )

int lcd_index = -1;
unsigned LCD_ADDR;
#define DRAW_PIXEL(x,y,c) ( ((unsigned short *)LCD_ADDR + LCD_WIDTH * (y))[x] = (c) )

static int set_color(int bpp, unsigned int color, int ystart, int yend) 
{
	int i;

	int xres = LCD_WIDTH;
	if (bpp == 16) {
		ushort *p = (ushort *) LCD_ADDR+(ystart*xres);
		for (i = (ystart*xres); i < (yend * xres); i++) {
			*p++ = color;
		}
	} else if (bpp == 32) {
		unsigned int *p = (unsigned int *) LCD_ADDR+(ystart*xres);
		for (i = (ystart*xres); i < (yend * xres); i++) {
			*p++ = color;
		}
	} else {
		printf("Unsupported BPP %d\n", bpp);
	}

	return 0;
}

void DrawColor(int bpp)
{
	int yres = LCD_HEIGHT;
	switch(bpp) {
	    case 16:
	        // blue, bit[0-4]
	        set_color(bpp, 0x1f, 0, yres/3);
	        printf("outputting blue\n");
            // green, bit[5-10]
            set_color(bpp, 0x7e0, yres/3, yres/3*2);
            printf("outputting green\n");
	        // red, bit[11-15]
            set_color(bpp, 0x1f800, yres/3*2, yres);
            printf("outputting red\n");
	        break;
	    case 32:
            // blue, bit[0-7]
            set_color(bpp, 0xff, 0, yres/3);
            printf("outputting blue\n");
            // green, bit[8-15]
            set_color(bpp, 0xff00, yres/3, yres/3*2);
            printf("outputting green\n");
            // red, bit[16-23]
            set_color(bpp, 0xff0000, yres/3*2, yres);
            printf("outputting red\n");
            break;
	        break;
	    default:
	        printf("unsupported bpp\n");
	        break;
	}
}

void DrawBoarder(void)
{
	// 16 bpp green color: 0x7e0
	DrawLine(0, 0, LCD_WIDTH-1, 0, 0x7e0);
	DrawLine(0, LCD_HEIGHT-1, LCD_WIDTH-1, LCD_HEIGHT, 0x7e0);
	DrawLine(0, 0, 0, LCD_HEIGHT-1, 0x7e0);
	DrawLine(LCD_WIDTH-1, 0, LCD_WIDTH-1, LCD_HEIGHT-1, 0x7e0);
}

/* Draw line (x0, y0)~(x1, y1) with specific RGB888 color (r,g,b) */
void DrawLine(int x0, int y0, int x1, int y1, unsigned short color)
{
	int dx, dy, dm, dn, m, n, k, u, v, l, sum;

	dx = x1 - x0;
	dy = y1 - y0;
	dm = 1;
	dn = 1;

	if (dx < 0) {
		dx = -dx;
		dm = -1;
	}
	if (dy < 0) {
		dy = -dy;
		dn = -1;
	}

	m = dx;
	n = dy;
	k = 1;
	u = x0;
	v = y0;

	if (dx < dy) {
		m = dy;
		n = dx;
		k = dm;
		dm = dn;
		dn = k;
		k = 0;
		u = y0;
		v = x0;
	}

	l = 0;
	sum = m;

	DRAW_PIXEL(x0,y0, color);
	while (sum != 0) {
		sum = sum - 1;
		l = l + n;
		u = u + dm;
		if (l >= m) {
			v = v + dn;
		}
		l = l % m;

		if (k == 1) {
			DRAW_PIXEL(u, v, color);
		} else {
			DRAW_PIXEL(v, u, color);
		}
	}
}

static void DrawChar(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	const unsigned char *pZK;
	unsigned char mask,buf;

	pZK = &Font8x16[ch*16];
	for( i = 0 ; i < 16 ; i++ )
	{
		mask = 0x80;
		buf = pZK[i];
		for( j = 0 ; j < 8 ; j++ )
		{
			if( buf & mask )
			{
				DRAW_PIXEL(x+j,y+i,c);
			}else
			{
				if( !st )
				{
					DRAW_PIXEL(x+j,y+i,bk_c);
				}
			}

			mask = mask >> 1;
		}
	}
}

void DrawString(int x, int y,const char *str,unsigned int c,unsigned int bk_c,unsigned int st)
{
	if (lcd_index < 0) {
		return;
	}
	if (x < 0) {
		x += LCD_WIDTH;
	}
	if (y < 0) {
		y += LCD_HEIGHT;
	}
	for(;;) {
		unsigned ch;
		ch = *str++;
		if (ch == 0) {
			break;
		}
		if (ch < ' ' || ch > 0x7F) {
			continue;
		}
		DrawChar(x, y, ch, c, bk_c, st);
		x += 8;
	}
}

void PrintLCD(int x,int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...)
{
	char buf[256];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	DrawString(x, y, buf, c, bk_c, st);
}

void ClearScreen(void)
{
	if (lcd_index < 0) {
		return;
	}
	memset( (void *)LCD_ADDR, 0, LCD_WIDTH * LCD_HEIGHT * 2);
}

static void _InitLCD_(int lcd_type, int booting, int boot_linux)
{
    unsigned i;

    if (lcd_index >= 0) {
        return;
    }

    lcd_index = -1;
    for (i = 0; lcd_param_tab[i]._LCD_TYPE != 0; i++) {
        if (lcd_param_tab[i]._LCD_TYPE == lcd_type) {
            lcd_index = i;
            break;
        }
    }
    if (lcd_index < 0) {
        return;
    }

    i = LCD_WIDTH * LCD_HEIGHT;
    // Fill LCD with background color

    if (boot_linux) {
        // bpp = 32
        unsigned c = 0; // (BACKGROUND_R << 16) | (BACKGROUND_G << 8) | ((BACKGROUND_B));
        while (i--) {
            ((unsigned *)LCD_ADDR)[i] = c;
        }
    } else if (booting) {
#if 0    
        PROGRESS_BAR_TOP    =  (LCD_HEIGHT - 5 - 20);
        PROGRESS_BAR_LEFT   = 25;
        PROGRESS_BAR_WIDTH  = (LCD_WIDTH - 2 * PROGRESS_BAR_LEFT);
        PROGRESS_BAR_HEIGHT = 4;
        PROGRESS_BAR_COLOR  = ( ( (PROGRESS_BAR_R >> 3) << 11) | ((PROGRESS_BAR_G >> 2) << 5) | (PROGRESS_BAR_B >> 3) );
#endif
        while (i--) {
            ((unsigned short *)LCD_ADDR)[i] = BACKGROUND_COLOR;
        }
    } else {
#if 0
        PROGRESS_BAR_TOP    = (LCD_HEIGHT - 5 - 20) - 16 - 8;
        PROGRESS_BAR_LEFT   = 22;
        PROGRESS_BAR_WIDTH  = (LCD_WIDTH - 2 * PROGRESS_BAR_LEFT);
        PROGRESS_BAR_HEIGHT = 4;
        PROGRESS_BAR_COLOR  = 0xFFFFU;
#endif
        memset( (void *)LCD_ADDR, 0, i * 2);
    }
    SetBacklightOfLCD(127);
#if 0
    if (booting) {
        LoadPicture(boot_linux);
    }

    if (!RUN_FROM_NAND) {
        SetBacklightOfLCD(127);
    }

#if DEBUG_LCD
        // draw a red rect, to check if LCD parameters is OK
    (boot_linux ? DrawRect32bpp : DrawRect)(0, 0, LCD_WIDTH, LCD_HEIGHT, RGB16(0xFF, 0x00, 0x00));
    (boot_linux ? DrawRect32bpp : DrawRect)(1, 1, LCD_WIDTH - 2, LCD_HEIGHT - 2, RGB16(0x00, 0x00, 0x00));
#endif
#endif
    InitLcdHw(boot_linux ? 32 : 16);

}

void InitLCD(int lcd_type, int booting)
{
	DECLARE_GLOBAL_DATA_PTR;
	if (s5pc110_chip == S5PC110_CHIP_TYPE_D) {		
		gd->bd->bi_dram[0].start = MEMORY_BASE_ADDRESS;
		gd->bd->bi_dram[0].size = 80 * SZ_1M;
		gd->bd->bi_dram[1].start = MEMORY_BASE_ADDRESS + 256 * SZ_1M;
		gd->bd->bi_dram[1].size = 256 * SZ_1M;
		gd->bd->bi_dram[2].start = gd->bd->bi_dram[1].start + gd->bd->bi_dram[1].size;
		gd->bd->bi_dram[2].size = 128 * SZ_1M;
		LCD_ADDR = gd->bd->bi_dram[2].start + gd->bd->bi_dram[2].size - 10 * SZ_1M;
	} else if (s5pc110_chip == S5PC110_CHIP_TYPE_N) {
		gd->bd->bi_dram[0].start = MEMORY_BASE_ADDRESS;
		gd->bd->bi_dram[0].size = 256 * SZ_1M;
		gd->bd->bi_dram[1].start = gd->bd->bi_dram[0].start + gd->bd->bi_dram[0].size;
		gd->bd->bi_dram[1].size = 256 * SZ_1M;
		LCD_ADDR = gd->bd->bi_dram[1].start + gd->bd->bi_dram[1].size - 10 * SZ_1M;
	} else {
		printf("unknown s5c110 variant\n");
		hang();
	}
	_InitLCD_(lcd_type, booting, 0);
	 //PrintLCD(x, y, color, BACKGROUND_COLOR, 1, "LCD Type: %s(%s)", LCD_NAME,  "Auto");
}
