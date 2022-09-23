/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * S5PV210 - LCD Driver for U-Boot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <video_fb.h>
#include "videomodes.h"

#define LCD_WIDTH		1024
#define LCD_HEIGHT		600	//766

#define	S70_LCD_PANEL	1

/* VIDTCON0 */
#define S3C_VIDTCON0_VBPDE(x)			(((x) & 0xff) << 24)
#define S3C_VIDTCON0_VBPD(x)			(((x) & 0xff) << 16)
#define S3C_VIDTCON0_VFPD(x)			(((x) & 0xff) << 8)
#define S3C_VIDTCON0_VSPW(x)			(((x) & 0xff) << 0)

/* VIDTCON1 */
#define S3C_VIDTCON1_VFPDE(x)			(((x) & 0xff) << 24)
#define S3C_VIDTCON1_HBPD(x)			(((x) & 0xff) << 16)
#define S3C_VIDTCON1_HFPD(x)			(((x) & 0xff) << 8)
#define S3C_VIDTCON1_HSPW(x)			(((x) & 0xff) << 0)

#define	T_V (	S3C_VIDTCON0_VBPDE(1) | \
		S3C_VIDTCON0_VBPD(7) | \
		S3C_VIDTCON0_VFPD(5) | \
		S3C_VIDTCON0_VSPW(1) \
	)

#define	T_H (	S3C_VIDTCON1_VFPDE(1) | \
		S3C_VIDTCON1_HBPD(13) | \
		S3C_VIDTCON1_HFPD(0) | \
		S3C_VIDTCON1_HSPW(10) \
	)

#define S3C_VIDCON1_IVCLK_FALLING_EDGE		(0 << 7)
#define S3C_VIDCON1_IVCLK_RISING_EDGE		(1 << 7)
#define S3C_VIDCON1_IHSYNC_NORMAL		(0 << 6)
#define S3C_VIDCON1_IHSYNC_INVERT		(1 << 6)
#define S3C_VIDCON1_IVSYNC_NORMAL		(0 << 5)
#define S3C_VIDCON1_IVSYNC_INVERT		(1 << 5)
#define S3C_VIDCON1_IVDEN_NORMAL		(0 << 4)
#define S3C_VIDCON1_IVDEN_INVERT		(1 << 4)

#define	LCD_POLARITY	(S3C_VIDCON1_IVCLK_RISING_EDGE | S3C_VIDCON1_IHSYNC_INVERT | S3C_VIDCON1_IVSYNC_INVERT)


#define Inp32(_addr)		readl(_addr)
#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))



void lcd_set_blank();

GraphicDevice gd_s70;

void *video_hw_init(void)
{
	if(!check_special_boot())
		return 0;
	u32 uFbAddr = CFG_LCD_FBUFFER;

	u32* pBuffer = (u32*)uFbAddr;

#if S70_LCD_PANEL
//	GPD0_3 as pwm3 as output
	Outp32(0xe02000a0, 0x1000);
	Outp32(0xe02000a4, 0x08);
	Outp32(0xe02000ac, 0xc0);
#endif

	
	Outp32(0xe0200120, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe0200128,0x0);			//set pull-up,down disable
	Outp32(0xe0200140, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(0xe0200148,0x0);			//set pull-up,down disable
	Outp32(0xe0200160, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(0xe0200168,0x0);			//set pull-up,down disable
	Outp32(0xe0200180, 0x00002222);	//set GPF3 as VD[23:20]
	Outp32(0xe0200188,0x0);			//set pull-up,down disable

	
	
	//--------- S5PC110 EVT0 needs MAX drive strength	---------//
	Outp32(0xe020012c,0xffffffff);			//set GPF0 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020014c,0xffffffff);			//set GPF1 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020016c,0xffffffff);			//set GPF2 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020018c,0x3ff);					//set GPF3 drive strength max by WJ.KIM(09.07.17)
	
	
	//I just read these registers under system
	Outp32(0xf8000004, 0x0112C0E0);//LCD_POLARITY);
	Outp32(0xf8000010, 0x00060400);//T_V);
	Outp32(0xf8000014, 0x000CFF09);//T_H);

	Outp32(0xf8000170, 0x0);
	Outp32(0xf8000018, 0x12BBFF);
	Outp32(0xf8000000, 0x0);
	Outp32(0xf8000000, 0x254);
	Outp32(0xf8000130, 0x20);
//window control registers
	Outp32(0xf8000020, 0x0);
	Outp32(0xf8000024, 0x0);
	Outp32(0xf8000028, 0x0);
	Outp32(0xf800002c, 0x0);
	Outp32(0xf8000030, 0x0);
// shadow control
	Outp32(0xf8000034, 0x0);
//window color registers
	Outp32(0xf8000180, 0x0);
	Outp32(0xf8000184, 0x0);
	Outp32(0xf8000188, 0x0);
	Outp32(0xf800018c, 0x0);
	Outp32(0xf8000190, 0x0);
//color key,color value registers
	Outp32(0xf8000140, 0x6000000);
	Outp32(0xf8000144, 0x0);


	Outp32(0xf8000148, 0x0);
	Outp32(0xf8000150, 0x0);
	Outp32(0xf8000158, 0x0);


	Outp32(0xf8000058, 0x0); //window1 alpha control	
	Outp32(0xf8000208, 0x0); //alpha value0
	Outp32(0xf800020c, 0x0); //alpha value1

	Outp32(0xf8000068, 0x0); //window2's
	Outp32(0xf8000210, 0x0);
	Outp32(0xf8000214, 0x0);

	Outp32(0xf8000078, 0x0); //window3's
	Outp32(0xf8000218, 0x0);
	Outp32(0xf800021c, 0x0);

	Outp32(0xf8000088, 0x0); //window4's
	Outp32(0xf8000220, 0x0);
	Outp32(0xf8000224, 0x0);

	Outp32(0xf8000260, 0x1); //blending registers,set as 8-bit value

//window0's
	Outp32(0xf8000048, 0x258000); //window0 size control
	Outp32(0xf8000200, 0xffffff); //window0 alpha value0
	Outp32(0xf8000204, 0xffffff); //window0 alpha value1
//window1's
	Outp32(0xf8000058, 0xFFFFFF); //window1 alpha control
	Outp32(0xf800005c, 0x258000); //window0 size control
	Outp32(0xf8000208, 0xFFFFFF); //window0 alpha value0
	Outp32(0xf800020c, 0xFFFFFF); //window0 alpha value1

	Outp32(0xf8000034, 0x0); //shadow control
	
//	specify window0
	Outp32(0xf8000020, 0x802c);//control register
	Outp32(0xf80000a0, uFbAddr + 0x00000000);
	Outp32(0xf80000d0, uFbAddr + 0x00400800);
	Outp32(0xf80000a4, uFbAddr + 0x00000000);
	Outp32(0xf80000d4, uFbAddr + 0x00400800);
	Outp32(0xf80020a0, uFbAddr + 0x00000000);
	Outp32(0xf80020d0, uFbAddr + 0x00400800);
	Outp32(0xf8000100, 0x1558); //buffer size
	Outp32(0xf8000040, 0x0); //position
	Outp32(0xf8000044, 0x2aaaff); //position
	

//	specify window1
	Outp32(0xf8000024, 0x802c);
	Outp32(0xf80000a8, CONFIG_VIDEO_LOGO_ADDRESS + 0x00000000);
	Outp32(0xf80000d8, CONFIG_VIDEO_LOGO_ADDRESS + 0x00400800);
	Outp32(0xf80000ac, CONFIG_VIDEO_LOGO_ADDRESS + 0x00000000);
	Outp32(0xf80000dc, CONFIG_VIDEO_LOGO_ADDRESS + 0x00400800);
	Outp32(0xf80020a8, CONFIG_VIDEO_LOGO_ADDRESS + 0x00000000);
	Outp32(0xf80020d8, CONFIG_VIDEO_LOGO_ADDRESS + 0x00400800);
	Outp32(0xf8000104, 0x1558);
	Outp32(0xf8000050, 0x0);
	Outp32(0xf8000054, 0x2aaaff);
//	end
	
	
	Outp32(0xf8000034, 0x3);

	Outp32(0xf8000020, 0x802d);
	Outp32(0xf8000024, 0x802d);

	Outp32(0xf8000034, 0x3);
	
//	Outp32(0xf8000000, 0x257);
//	Outp32(0xf8000000, 0x57); //===> MPLL should be 667 !!!!
	Outp32(0xf8000000, 0x53);
	Outp32(0xf8000010, 0x60400);
	Outp32(0xf80001a4, 0x3);

	Outp32(0xe0107008,0x2); //syscon output path
	Outp32(0xe0100204,0x700000); //syscon fimdclk = mpll


//	lcd pannel power up

#if S70_LCD_PANEL
	//udelay(500);
	Outp32(0xe0200c60, (Inp32(0xe0200c60) & ~(0xFFF00000)) | 0x11100000);

	Outp32(0xe0200c64, Inp32(0xe0200c64) | 0xE0); //set gph3_5 gph3_6 gph3_7
	//udelay(1000);
	Outp32(0xe0200c64, Inp32(0xe0200c64) & 0x7f); //set gph3_7 low
	//udelay(1000);
	Outp32(0xe0200c64, Inp32(0xe0200c64) | 0x80); //set gph3_7 high
#endif
	GraphicDevice *pGD = (GraphicDevice *)&gd_s70;
	pGD->winSizeX = LCD_WIDTH;
	pGD->winSizeY = LCD_HEIGHT;
	pGD->plnSizeX = LCD_WIDTH;
	pGD->plnSizeY = LCD_HEIGHT;

	pGD->gdfBytesPP = 4;
	pGD->gdfIndex = GDF_32BIT_X888RGB;

	pGD->frameAdrs = CFG_LCD_FBUFFER;
	pGD->memSize = 1024*600*4;
	
	memset(CFG_LCD_FBUFFER,0x00,1024*600*4);
	memset(CONFIG_VIDEO_LOGO_ADDRESS,0x00,1024*600*4);

	//put_video_logo(0,0);

	return &gd_s70;
}
void video_set_lut(unsigned int index,
			unsigned char r,
			unsigned char g,
			unsigned char b)
{
	
}
/*
#include "2.bmp.c"
#define 	SANGU_IMAGE_WIDTH x_2_bmp
#define 	SANGU_IMAGE_HEIGHT y_2_bmp

#define		SG_IMAGE_X ((1024 - SANGU_IMAGE_WIDTH)/2)
#define		SG_IMAGE_Y ((600 - SANGU_IMAGE_HEIGHT)/2)


void put_video_logo(int x,int y)
{
	int* Buf,*pLogo,i,j,k=0;
	
	pLogo = (int*)pic_2_bmp;
	
	for (i = 0; i < SANGU_IMAGE_HEIGHT; i++)
	{
		Buf =(int*)(CONFIG_VIDEO_LOGO_ADDRESS + (i + y)*1024*4 + x*4);
		for (j = 0; j < SANGU_IMAGE_WIDTH; j++)
		{
			*Buf++ = pLogo[k++];
		}
	}
}
*/





















