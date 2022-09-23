#include <common.h>
#include <command.h>
#include <net.h>
#include <s5pc110.h>
#include <asm/io.h>

void led_enable(int on)
{
	writel((readl(GPJ2CON) & (~0xFFFF<<0)) | (0x1111<<0), GPJ2CON);
	if (on) {
		writel(readl(GPJ2DAT) & (~0xF<<0), GPJ2DAT);
	} else {
		writel(readl(GPJ2DAT) | (0xF<<0), GPJ2DAT);
	}
}

void led_play(unsigned int times)
{
	writel((readl(GPJ2CON) & (~0xFFFF<<0)) | (0x1111<<0), GPJ2CON);
	while ((times--) > 0) {
		udelay(1000000);
		writel((readl(GPJ2DAT) & (~0xF<<0)) | (0x1<<(times%4)), GPJ2DAT);
	}
	led_enable(1);
}

int do_led (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *cmd;
	int times = 10;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	cmd = argv[1];

	if (strcmp(cmd, "on") == 0) {
		led_enable(1);
	} else 	if (strcmp(cmd, "off") == 0) {
		led_enable(0);
	} else 	if (strcmp(cmd, "play") == 0) {
		if (argc == 3)
			times = simple_strtoul(argv[2], NULL, 10);
		led_play(times);
	}
	return 0;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	led, CFG_MAXARGS, 1,	do_led,
	"led     - control led\n",
	"on	  - turn on all leds\n"
	"off  - turn off all leds\n"
	"play [times]- leds play.WARNING: Can't not stop manually\n"
);