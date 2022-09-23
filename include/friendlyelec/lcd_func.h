#if !defined(__LCD_FUNC_H__)
#define __LCD_FUNC_H__

#define BACKGROUND_R  0x00
#define BACKGROUND_G  0x00
#define BACKGROUND_B  0x7F

#define PROGRESS_BAR_R 0xFF
#define PROGRESS_BAR_G 0xFF
#define PROGRESS_BAR_B 0x00

#define LOGO_POS_TOP   0
#define LOGO_POS_LEFT  0

#define RGB16(r,g,b) ( ( (  (r) >> 3) << 11) | ((  (g) >> 2) << 5) | (  (b) >> 3) )

#define LCD_CHAR_START_X	(10)
#define LCD_CHAR_START_Y	(4)
#define LCD_CHAR_STEP_Y		(17)

void InitLCD(int lcd_type, int booting);
void ClearScreen(void);

void ProgressBarInit(unsigned total);
void ProgressBarStep(void);

void DrawColor(int bpp);
void DrawBoarder(void);
void DrawPixel(int x, int y, unsigned c);
void DrawString(int x, int y,const char *str,unsigned int c,unsigned int bk_c,unsigned int st);
void DrawInvString(int x, int y,const char *str,unsigned int c,unsigned int bk_c);
void FillRect(int x, int y, unsigned w, unsigned h, unsigned c);
void DrawRect(int x, int y, unsigned w, unsigned h, unsigned c);
void DrawHLine(int x, int y, unsigned len, unsigned c);

void PrintLCD(int x, int y, unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...);

void ShowProcessInfoLCD(char *fmt, ...);
void ShowFileInfoLCD(const char *fmt, ...);
void ShowSummaryInfoLCD(char *fmt, ...);
void ShowErrorInfoLCD(char *fmt, ...);
void InstallingProgressBarInit(unsigned total);
void InstallingProgressBarStep(void);
void InstallingProgressBarClose(void);


void DrawFinalInfo(int OK, const char OS[]);
void ShowBaseInfoLCD(void);

#endif