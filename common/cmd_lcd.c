#include <common.h>
#include <command.h>
#include <net.h>
#include <s5pc110.h>
#include <asm/io.h>
#include <friendlyelec/lcd_func.h>
#include <friendlyelec/lcd_hw.h>

#define DISPLAY_BASE	0xF8000000
#define rWINCON1		__REG(DISPLAY_BASE +0x024)

#define RGB16(r,g,b) ( ( (  (r) >> 3) << 11) | ((  (g) >> 2) << 5) | (  (b) >> 3) )
#define BACKGROUND_R  0x00
#define BACKGROUND_G  0x00
#define BACKGROUND_B  0x7F
#define BACKGROUND_COLOR    ( ( (  BACKGROUND_R >> 3) << 11) | ((  BACKGROUND_G >> 2) << 5) | (  BACKGROUND_B >> 3) )

static int x = LCD_CHAR_START_X;
static int y = LCD_CHAR_START_Y + LCD_CHAR_STEP_Y;

void lcd_enable(int on)
{
	if (on) {
		rWINCON1 |= 0x1;
	} else {
		rWINCON1 &= ~0x1;
	}
}

int do_lcd (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *cmd;
	unsigned color = RGB16(0xFF, 0xFF, 0xFF);

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	cmd = argv[1];

	if (strcmp(cmd, "on") == 0) {
		lcd_enable(1);
	} else if (strcmp(cmd, "off") == 0) {
		lcd_enable(0);
	} else if (strcmp(cmd, "boarder") == 0) {
		DrawBoarder();	
	} else if (strcmp(cmd, "color") == 0) {
		DrawColor(16);
	} else  {	
		PrintLCD(x, y, color, BACKGROUND_COLOR, 1, cmd);
		y += LCD_CHAR_STEP_Y;
		if (y > LCD_HEIGHT) {
			y = LCD_CHAR_START_Y;
			ClearScreen();
		}
	}
	return 0;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	lcd, CFG_MAXARGS, 1,	do_lcd,
	"lcd	  	- control lcd\n",
	"string	  - info need to be printed\n"
	"on	  - turn on lcd\n"
	"off  - turn off lcd\n"
	"boarder  - draw boarder\n"
);
