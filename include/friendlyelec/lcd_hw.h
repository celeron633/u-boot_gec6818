
#ifndef __LCD_HW_H_
#define __LCD_HW_H_


extern struct t_lcd_param {
	unsigned
	_LCD_TYPE,
	_LCD_WIDTH,
	_LCD_HEIGHT,
	_LCD_VBPD_VALUE,
	_LCD_VFPD_VALUE,
	_LCD_VSPW_VALUE,
	_LCD_HBPD_VALUE,
	_LCD_HFPD_VALUE,
	_LCD_HSPW_VALUE,
	_LCD_VCLK_POLARITY,
	_LCD_HSYNC_POLARITY,
	_LCD_VSYNC_POLARITY,
	_LCD_VDEN_POLARITY,
	_LCD_PNR_MODE,
	_LCD_VCLK_SOURCE,
	_LCD_VCLK_DIRECTION,
	_LCD_TFT_CLKVAL;
	const char *_LCD_NAME;
	unsigned
	_HDMI_ONLY;
} lcd_param_tab[];
extern int lcd_index;
extern unsigned LCD_ADDR;

#define	LCD_TYPE           (lcd_param_tab[lcd_index]._LCD_TYPE          )
#define	LCD_WIDTH          (lcd_param_tab[lcd_index]._LCD_WIDTH         )
#define	LCD_HEIGHT         (lcd_param_tab[lcd_index]._LCD_HEIGHT        )
#define	LCD_VBPD_VALUE     (lcd_param_tab[lcd_index]._LCD_VBPD_VALUE    )
#define	LCD_VFPD_VALUE     (lcd_param_tab[lcd_index]._LCD_VFPD_VALUE    )
#define	LCD_VSPW_VALUE     (lcd_param_tab[lcd_index]._LCD_VSPW_VALUE    )
#define	LCD_HBPD_VALUE     (lcd_param_tab[lcd_index]._LCD_HBPD_VALUE    )
#define	LCD_HFPD_VALUE     (lcd_param_tab[lcd_index]._LCD_HFPD_VALUE    )
#define	LCD_HSPW_VALUE     (lcd_param_tab[lcd_index]._LCD_HSPW_VALUE    )
#define	LCD_VCLK_POLARITY  (lcd_param_tab[lcd_index]._LCD_VCLK_POLARITY )
#define	LCD_HSYNC_POLARITY (lcd_param_tab[lcd_index]._LCD_HSYNC_POLARITY)
#define	LCD_VSYNC_POLARITY (lcd_param_tab[lcd_index]._LCD_VSYNC_POLARITY)
#define	LCD_VDEN_POLARITY  (lcd_param_tab[lcd_index]._LCD_VDEN_POLARITY )
#define	LCD_PNR_MODE       (lcd_param_tab[lcd_index]._LCD_PNR_MODE      )
#define	LCD_VCLK_SOURCE    (lcd_param_tab[lcd_index]._LCD_VCLK_SOURCE   )
#define	LCD_VCLK_DIRECTION (lcd_param_tab[lcd_index]._LCD_VCLK_DIRECTION)
#define	LCD_TFT_CLKVAL     (lcd_param_tab[lcd_index]._LCD_TFT_CLKVAL    )
#define	LCD_NAME           (lcd_param_tab[lcd_index]._LCD_NAME          )
#define	HDMI_ONLY          (lcd_param_tab[lcd_index]._HDMI_ONLY         )

void InitLcdHw(unsigned bpp);
const char *LcdNameFromNumber(int lcd);

#endif /* __LCD_HW_H_ */
