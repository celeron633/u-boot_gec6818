#include <common.h>
#include <command.h>
#include <net.h>
#include <s5pc110.h>
#include <asm/io.h>
#include <friendlyelec/lcd_func.h>
#include <friendlyelec/lcd_hw.h>
#include <friendlyelec/1-wire.h>

int do_backlight (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int brightness;
	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	brightness = simple_strtoul(argv[1],NULL,10);
	if (brightness < 0)
		brightness = 0;
	else if (brightness > 127)
		brightness = 127;
	SetBacklightOfLCD(brightness);
	return 0;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	backlight, CFG_MAXARGS, 1,	do_backlight,
	"backlight - control onewre backlight\n",
	"value	  - Brightness(0-127)\n"
);
