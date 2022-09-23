//#include "stdlib.h"
#include <friendlyelec/lcd_hw.h>

#include <friendlyelec/fa_public.h>
#include <s5pc110.h>

#define IVCLK_FALL_EDGE        (0<<7)
#define IVCLK_RISE_EDGE        (1<<7)
#define IHSYNC_HIGH_ACTIVE    (0<<6)
#define IHSYNC_LOW_ACTIVE    (1<<6)
#define IVSYNC_HIGH_ACTIVE    (0<<5)
#define IVSYNC_LOW_ACTIVE    (1<<5)
#define IVDEN_HIGH_ACTIVE    (0<<4)
#define IVDEN_LOW_ACTIVE    (1<<4)

#define PNRMODE_RGB_P        (0<<17)
#define PNRMODE_BGR_P        (1<<17)
#define PNRMODE_RGB_S        (2<<17)
#define PNRMODE_BGR_S        (3<<17)
#define PNRMODE_MASK        (3<<17)

#define CLKSEL_F_HCLK        (0<<2)
#define CLKSEL_F_LCDCLK        (1<<2)
#define CLKSEL_F_EXT27M    (3<<2)
#define CLKSEL_F_MASK        (3<<2)

#define CLKDIR_DIRECT        (0<<4)
#define CLKDIR_DIVIDED        (1<<4)

// VIDCON0
#define PROGRESSIVE            (0<<29)
#define INTERLACE            (1<<29)

#define VIDOUT_RGBIF        (0<<26)
#define VIDOUT_TVENC        (1<<26)
#define VIDOUT_I80IF0        (2<<26)
#define VIDOUT_I80IF1        (3<<26)
#define VIDOUT_TV_RGBIF    (4<<26)
#define VIDOUT_TV_I80IF0    (6<<26)
#define VIDOUT_TV_I80IF1    (7<<26)

#define LDI1_16_MODE        (0<<23)
#define LDI1_16_2_MODE        (1<<23)
#define LDI1_9_9_MODE        (2<<23)
#define LDI1_16_8_MODE        (3<<23)
#define LDI1_18_MODE        (4<<23)
#define LDI1_8_8_MODE        (5<<23)

#define LDI0_16_MODE        (0<<20)
#define LDI0_16_2_MODE        (1<<20)
#define LDI0_9_9_MODE        (2<<20)
#define LDI0_16_8_MODE        (3<<20)
#define LDI0_18_MODE        (4<<20)
#define LDI0_8_8_MODE        (5<<20)

#define PNRMODE_RGB_P        (0<<17)
#define PNRMODE_BGR_P        (1<<17)
#define PNRMODE_RGB_S        (2<<17)
#define PNRMODE_BGR_S        (3<<17)
#define PNRMODE_MASK        (3<<17)

#define CLKVALUP_ALWAYS    (0<<16)
#define CLKVALUP_ST_FRM    (1<<16)

#define CLKVAL_F(n)            (((n-1)&0xff)<<6)

#define VCLK_NORMAL            (0<<5)
#define VCLK_FREERUN        (1<<5)

#define CLKDIR_DIRECT        (0<<4)
#define CLKDIR_DIVIDED        (1<<4)

#define CLKSEL_F_HCLK        (0<<2)
#define CLKSEL_F_LCDCLK        (1<<2)
#define CLKSEL_F_EXT27M    (3<<2)
#define CLKSEL_F_MASK        (3<<2)

#define ENVID_DISABLE        (0<<1)
#define ENVID_ENABLE        (1<<1)
#define ENVID_F_DISABLE        (0<<0)
#define ENVID_F_ENABLE        (1<<0)

	// VIDCON2
#define TVIF_FMT_YUV444    (0x7<<7)
	// VIDTCON0
#define VBPDE(n)                (((n-1)&0xff)<<24)
#define VBPD(n)                (((n-1)&0xff)<<16)
#define VFPD(n)                (((n-1)&0xff)<<8)
#define VSPW(n)                (((n-1)&0xff)<<0)

	// VIDTCON1
#define VFPDE(n)                (((n-1)&0xff)<<24)
#define HBPD(n)                (((n-1)&0xff)<<16)
#define HFPD(n)                (((n-1)&0xff)<<8)
#define HSPW(n)                (((n-1)&0xff)<<0)

	// VIDTCON2
#define LINEVAL(n)            (((n-1)&0x7ff)<<11)
#define HOZVAL(n)            (((n-1)&0x7ff)<<0)
	// WINCON0
#define CSC_WIDE_RANGE        (0<<26)
#define CSC_NARROW_RANGE    (3<<26)

#define LOCALSEL_TVSCALER    (0<<23)
#define LOCALSEL_CIPREVIEW    (1<<23)
#define LOCALSEL_CICODEC    (1<<23)

#define LOCAL_PATH_DISABLE    (0<<22)
#define LOCAL_PATH_ENABLE    (1<<22)

#define BUFSEL_BUF0            (0<<20)
#define BUFSEL_BUF1            (1<<20)

#define BUFAUTO_DISABLE    (0<<19)
#define BUFAUTO_ENABLE        (1<<19)

#define BITSWP_DISABLE        (0<<18)
#define BITSWP_ENABLE        (1<<18)
#define BYTSWP_DISABLE        (0<<17)
#define BYTSWP_ENABLE        (1<<17)
#define HAWSWP_DISABLE    (0<<16)
#define HAWSWP_ENABLE        (1<<16)
#define WSWP_ENABLE          (1<<15)
#define WSWP_DISABLE         (0<<15)

#define LOCAL_IN_RGB888    (0<<13)
#define LOCAL_IN_YUV444    (1<<13)

#define BURSTLEN_16WORD    (0<<9)
#define BURSTLEN_8WORD        (1<<9)
#define BURSTLEN_4WORD        (2<<9)
#define BURSTLEN_MASK        (3<<9)

#define BLEND_PER_PLANE    (0<<6)
#define BLEND_PER_PIXEL        (1<<6)

#define BPPMODE_F_1BPP            (0<<2)
#define BPPMODE_F_2BPP            (1<<2)
#define BPPMODE_F_4BPP            (2<<2)
#define BPPMODE_F_8BPP_PAL    (3<<2)
#define BPPMODE_F_8BPP_NOPAL    (4<<2)
#define BPPMODE_F_16BPP_565    (5<<2)
#define BPPMODE_F_16BPP_A555    (6<<2)
#define BPPMODE_F_16BPP_I555    (7<<2)
#define BPPMODE_F_18BPP_666    (8<<2)
#define BPPMODE_F_18BPP_A665    (9<<2)
#define BPPMODE_F_19BPP_A666    (0xa<<2)
#define BPPMODE_F_24BPP_888    (0xb<<2)
#define BPPMODE_F_24BPP_A887    (0xc<<2)
#define BPPMODE_F_25BPP_A888    (0xd<<2)
#define BPPMODE_F_MASK            (0xf<<2)
#define BPPMODE_F(n)            (((n)&0xf)<<2)

#define ALPHASEL_ALPHA0    (0<<1)    // Per Plane
#define ALPHASEL_ALPHA1    (1<<1)
#define ALPHASEL_AEN        (0<<1)    // Per Pixel
#define ALPHASEL_DATA        (1<<1)

#define ENWIN_F_DISABLE    (0<<0)
#define ENWIN_F_ENABLE        (1<<0)

	// VIDOSDxA
#define OSD_LEFTTOPX_F(n)    (((n)&0x7ff)<<11)
#define OSD_LEFTTOPY_F(n)    ((n)&0x7ff)

	// VIDOSDxB
#define OSD_RIGHTBOTX_F(n)    (((n)&0x7ff)<<11)
#define OSD_RIGHTBOTY_F(n)    ((n)&0x7ff)

	// VIDOSDxC
#define ALPHA0_R(n)            (((n)&0xf)<<20)
#define ALPHA0_G(n)            (((n)&0xf)<<16)
#define ALPHA0_B(n)            (((n)&0xf)<<12)
#define ALPHA1_R(n)            (((n)&0xf)<<8)
#define ALPHA1_G(n)            (((n)&0xf)<<4)
#define ALPHA1_B(n)            ((n)&0xf)

	// VIDOSD0C, VIDOSDxD
#define OSD_SIZE(n)            ((n)&0xffffff)

	// VIDW0xADD0
#define VBANK_F(n)            (((n)&0xff)<<24)
#define VBASEU_F(n)            ((n)&0xffffff)

	// VIDW0xADD1
#define VBASEL_F(n)            ((n)&0xffffff)

	// VIDW0xADD2
#define OFFSIZE_F(n)            (((n)&0x1fff)<<13)
#define PAGEWIDTH_F(n)        ((n)&0x1fff)


#define DISPLAY_BASE	0xF8000000
#define rVIDCON0		__REG(DISPLAY_BASE +0x000)
#define rVIDCON1		__REG(DISPLAY_BASE +0x004)
#define rVIDCON2		__REG(DISPLAY_BASE +0x008)
#define rVIDCON3		__REG(DISPLAY_BASE +0x00C)
#define rVIDTCON0		__REG(DISPLAY_BASE +0x010)
#define rVIDTCON1		__REG(DISPLAY_BASE +0x014)
#define rVIDTCON2		__REG(DISPLAY_BASE +0x018)
#define rVIDTCON3		__REG(DISPLAY_BASE +0x01C)
#define rWINCON1		__REG(DISPLAY_BASE +0x024)
#define rSHADOWCON      __REG(DISPLAY_BASE +0x034)
#define rVIDOSD1B		__REG(DISPLAY_BASE +0x054)
#define rVIDOSD1C		__REG(DISPLAY_BASE +0x058)
#define rVIDOSD1D		__REG(DISPLAY_BASE +0x05C)
#define rVIDW01ADD0B0	__REG(DISPLAY_BASE +0x0A8)
#define rVIDW01ADD1B0	__REG(DISPLAY_BASE +0x0D8)
#define rVIDW01ADD2		__REG(DISPLAY_BASE +0x104)

#define DISPLAY_CONTROL_REG __REG(0xE0107008)


struct t_lcd_param lcd_param_tab[] = {

		{
				LCD_A70,
				800,
				480,
				29,
				17,
				24,
				40,
				40,
				48,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				4,
				"A70",
		},
		{
				LCD_S70,
				800,
				480,
				21,
				22,
				2,
				44,
				210,
				2,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				4,
				"S70",
		},
		{
				LCD_S70D,
				800,
				480,
				30,
				22,
				2,
				86,
				210,
				2,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				4,
				"S70D",
		},		
		{
				LCD_S702,
				800,
				480,
				21,
				22,
				2,
				44,
				210,
				2,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				4,
				"S702",
		},
		{
				LCD_S701,
				800,
				480,
				21,
				22,
				2,
				44,
				210,
				2,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				4,
				"S701",
		},

		{
				LCD_W50,
				800,
				480,
				29,
				13,
				3,
				40,
				40,
				48,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				4,
				"W50",
		},
		{
				LCD_H43,
				480,
				272,
				1,
				8,
				2,
				5,
				40,
				2,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				9,
				"H43",
		},
		{
				LCD_N43,
				480,
				272,
				0x01,
				0x01,
				0x01,
				0x02,
				0x03,
				0x28,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				9,
				"N43",
		},
		{
				LCD_VGA1024X768,
				1024,
				768,
				0x02,
				0x02,
				0x10,
				0xA8,
				0x11,
				0x2A,
				IVCLK_FALL_EDGE,
				IHSYNC_HIGH_ACTIVE,
				IVSYNC_HIGH_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				5,
				"VGA1024X768",
		},
		{
				LCD_VGA800X600,
				800,
				600,
				0x02,
				0x02,
				0x10,
				0xA8,
				0x11,
				0x2A,
				IVCLK_FALL_EDGE,
				IHSYNC_HIGH_ACTIVE,
				IVSYNC_HIGH_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				5,
				"VGA800X600",
		},
		{
				LCD_VGA640X480,
				640,
				480,
				0x02,
				0x02,
				0x10,
				0xA8,
				0x11,
				0x2A,
				IVCLK_FALL_EDGE,
				IHSYNC_HIGH_ACTIVE,
				IVSYNC_HIGH_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				5,
				"VGA640X480",
		},
		{
				LCD_EZVGA1024X768,
				1024,
				768,
				0x02,
				0x02,
				0x10,
				0xA8,
				0x11,
				0x2A,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				1,
				"EZVGA1024X768",
		},
		{
				LCD_EZVGA1024X600,
				1024,
				600,
				0x02,
				0x02,
				0x10,
				0xA8,
				0x11,
				0x2A,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				4,
				"EZVGA1024X600",
		},
		{
				LCD_EZVGA800X600,
				800,
				600,
				0x02,
				0x02,
				0x10,
				0xA8,
				0x11,
				0x2A,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				5,
				"EZVGA800X600",
		},
		{
				LCD_L80,
				640,
				480,
				29,
				3,
				1,
				53,
				35,
				73,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				3,
				"L80",
		},
		{
				LCD_T35,
				240,
				320,
				0x01,
				0x01,
				0x04,
				0x01,
				0x04,
				0x1E,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				11,
				"T35",
		},
		{
				LCD_G10,
				640,
				480,
				0x22,
				0x0a,
				0x01,
				0x63,
				0x3c,
				0x01,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				3,
				"G10",
		},
		{
				LCD_W35,
				320,
				240,
				0x0c,
				0x04,
				0x01,
				0x46,
				0x04,
				0x01,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				22,
				"W35",
		},
		{
				LCD_X35,
				240,
				320,
				4,
				2,
				10,
				8,
				6,
				18,
				IVCLK_RISE_EDGE,
				IHSYNC_HIGH_ACTIVE,
				IVSYNC_HIGH_ACTIVE,
				IVDEN_LOW_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				23,
				"X35",
		},
		{
				LCD_W101,
				1024,
				600,
				21,
				22,
				2,
				44,
				210,
				2,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				4,
				"W101",
		},
		{
				LCD_A121,
				1024,
				768,
				8,
				8,
				16,
				32,
				32,
				64,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				3,
				"A121",
		},
		{
				LCD_U80,
				800,
				600,
				10,
				10,
				12,
				80,
				80,
				120,
				IVCLK_RISE_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				3,
				"U80",
		},
		{
				LCD_P43,
				480,
				272,
				9,
				1,
				2,
				40,
				5,
				2,
				IVCLK_RISE_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				9,
				"P43",
		},
		{
				LCD_PA43,
				480,
				272,
				9,
				1,
				2,
				40,
				5,
				2,
				IVCLK_FALL_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				19,
				"PA43",
		},
		{
				LCD_PB43,
				480,
				272,
				9,
				1,
				2,
				40,
				5,
				2,
				IVCLK_RISE_EDGE,
				IHSYNC_LOW_ACTIVE,
				IVSYNC_LOW_ACTIVE,
				IVDEN_HIGH_ACTIVE,
				PNRMODE_RGB_P,
				CLKSEL_F_LCDCLK,
				CLKDIR_DIVIDED,
				19,
				"PB43",
		},
		{
			LCD_P35,
			320,
			240,
			0x0c,
			0x04,
			0x01,
			0x07,
			0x04,
			0x01,
			IVCLK_RISE_EDGE,
			IHSYNC_LOW_ACTIVE,
			IVSYNC_LOW_ACTIVE,
			IVDEN_HIGH_ACTIVE,
			PNRMODE_RGB_P,
			CLKSEL_F_LCDCLK,
			CLKDIR_DIVIDED,
			22,
			"P35",     	
		},
		{
			LCD_TD35,
			240,
			320,
			1,
			1,
			10,
			1,
			101,
			5,
			IVCLK_RISE_EDGE,
			IHSYNC_LOW_ACTIVE,
			IVSYNC_LOW_ACTIVE,
			IVDEN_HIGH_ACTIVE,
			PNRMODE_RGB_P,
			CLKSEL_F_LCDCLK,
			CLKDIR_DIVIDED,
			20,
			"TD35",     	
		},
		{
				HDMI1080P60,
				1920,
				1280,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI1080P60",
				1,
		},
		{
				HDMI1080I60,
				1920,
				1280,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI1080I60",
				1,
		},
		{
				HDMI1080P30,
				1920,
				1280,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI1080P30",
				1,
		},
		{
				HDMI720P60,
				1280,
				720,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI720P60",
				1,
		},
		{
				HDMI576P16X9,
				1024,
				576,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI576P16X9",
				1,
		},
		{
				HDMI576P4X3,
				768,
				576,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI576P4X3",
				1,
		},
		{
				HDMI480P16X9,
				800,
				480,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI480P16X9",
				1,
		},
		{
				HDMI480P4X3,
				640,
				480,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI480P4X3",
				1,
		},
		{
				HDMI1080P60D,
				1920/2,
				1280/2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI1080P60D",
				1,
		},
		{
				HDMI1080I60D,
				1920/2,
				1280/2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI1080I60D",
				1,
		},
		{
				HDMI1080P30D,
				1920/2,
				1280/2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI1080P30D",
				1,
		},
		{
				HDMI720P60D,
				1280/2,
				720/2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI720P60D",
				1,
		},
		{
				HDMI576P16X9D,
				1024/2,
				576/2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI576P16X9D",
				1,
		},
		{
				HDMI576P4X3D,
				768/2,
				576/2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI576P4X3D",
				1,
		},
		{
				HDMI480P16X9D,
				800/2,
				480/2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI480P16X9D",
				1,
		},
		{
				HDMI480P4X3D,
				640/2,
				480/2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23,
				"HDMI480P4X3D",
				1,
		},

		{
				0,
		}
};

void InitLcdHw(unsigned bpp)
{
	if (bpp != 16 && bpp != 32) {
		return;
	}
#if 0	
	if (LCD_TYPE == LCD_PA43) {
		// do SPI init for P43
		LcdPa43SpiInit();
	}
#endif
	printf("LCD: %d X %d\n", LCD_WIDTH, LCD_HEIGHT);
	// Make LCD Interface Pin as there function and drive max current
	__REG(GPF0CON) = 0x11111111U * 2;
	__REG(GPF1CON) = 0x11111111U * 2;
	__REG(GPF2CON) = 0x11111111U * 2;
	__REG(GPF3CON) = 0x00001111U * 2;
	__REG(GPF0DRV) = 0x55555555U * 2;
	__REG(GPF1DRV) = 0x55555555U * 2;
	__REG(GPF2DRV) = 0x55555555U * 2;
	__REG(GPF3DRV) = 0x00000055U * 2;

	// LCD PWR
	__REG(GPD0CON) = (__REG(GPD0CON) & ~0xF0U) | 0x10U;
	__REG(GPD0DAT) |= 0x2U;


	// Choose FIMD with MPLL/4
	CLK_SRC1_REG = (CLK_SRC1_REG & ~(0xFU << 20)) | (0x6U << 20); // FIMD_SEL:MPLL
	CLK_DIV1_REG = (CLK_DIV1_REG & ~(0xFU << 20)) | (0x3U << 20);


	rVIDCON0 = PROGRESSIVE | VIDOUT_RGBIF | LCD_PNR_MODE | CLKVALUP_ALWAYS |
			CLKVAL_F(LCD_TFT_CLKVAL) | VCLK_NORMAL | LCD_VCLK_DIRECTION |
			LCD_VCLK_SOURCE | ENVID_DISABLE | ENVID_F_DISABLE;
	rVIDCON1 = LCD_VCLK_POLARITY | LCD_HSYNC_POLARITY | LCD_VSYNC_POLARITY | LCD_VDEN_POLARITY | (0x3U<<9);

	rVIDTCON0 = VBPDE(1) | VBPD(LCD_VBPD_VALUE) |   VFPD(LCD_VFPD_VALUE) | VSPW(LCD_VSPW_VALUE);
	rVIDTCON1 = VFPDE(1) | HBPD(LCD_HBPD_VALUE) |   HFPD(LCD_HFPD_VALUE) | HSPW(LCD_HSPW_VALUE);
	rVIDTCON2 = LINEVAL(LCD_HEIGHT) | HOZVAL(LCD_WIDTH);


	rWINCON1 = ( bpp == 32 ? WSWP_ENABLE : HAWSWP_ENABLE) | ( bpp == 32 ? BPPMODE_F_25BPP_A888 : BPPMODE_F_16BPP_565) | ENWIN_F_ENABLE | (1U<<6) | (1U <<1);
	rSHADOWCON = (1<<1);
	rVIDOSD1B = OSD_RIGHTBOTX_F(LCD_WIDTH - 1) | OSD_RIGHTBOTY_F(LCD_HEIGHT - 1);
	rVIDOSD1C = 0x0;
	rVIDOSD1D = LCD_WIDTH * LCD_HEIGHT;

	rVIDW01ADD0B0 = VBANK_F(LCD_ADDR >> 24) | VBASEU_F(LCD_ADDR);
	rVIDW01ADD1B0 = VBASEL_F(VBASEU_F(LCD_ADDR) + (bpp / 8)* LCD_WIDTH * LCD_HEIGHT);
	rVIDW01ADD2 = PAGEWIDTH_F(LCD_WIDTH * (bpp/8));

	DISPLAY_CONTROL_REG = 0x2;

	if (!HDMI_ONLY) {
		rVIDCON0 |= 0x3;
	}
#if 0	
	if (LCD_TYPE == LCD_PB43) {
		// do SPI init for P43
		LcdPb43SpiInit();
	}
#endif	
}

const char *LcdNameFromNumber(int lcd)
{
	int i;
	for (i = 0; lcd_param_tab[i]._LCD_TYPE != 0; i++) {
		if (lcd_param_tab[i]._LCD_TYPE == lcd) {
			return lcd_param_tab[i]._LCD_NAME;
		}
	}
	return NULL;
}
