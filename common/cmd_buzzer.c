#include <common.h>
#include <command.h>
#include <net.h>
#include <s5pc110.h>
#include <asm/io.h>

void buzzer_enable(int on)
{
	writel((readl(GPD0CON) & (~0x3<<0)) | (1<<0), GPD0CON);
	if (on) {
		writel(readl(GPD0DAT) | (0x1<<0), GPD0DAT);
	} else {
		writel(readl(GPD0DAT) & (~0x1<<0), GPD0DAT);
	}
}

void buzzer_play(unsigned int times)
{
	writel((readl(GPD0CON) & (~0x3<<0)) | (1<<0), GPD0CON);
	while ((times--)>0) {
		writel(readl(GPD0DAT) | (0x1<<0), GPD0DAT);
		udelay(1000000);
		writel(readl(GPD0DAT) & (~0x1<<0), GPD0DAT);
		udelay(1000000);
	}
}

int do_buzzer (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *cmd;
	int times = 2;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	cmd = argv[1];

	if (strcmp(cmd, "on") == 0) {
		buzzer_enable(1);
	} else 	if (strcmp(cmd, "off") == 0) {
		buzzer_enable(0);
	} else 	if (strcmp(cmd, "play") == 0) {
		if (argc == 3)
			times = simple_strtoul(argv[2], NULL, 10);
		buzzer_play(times);
	}
	return 0;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	buzzer, CFG_MAXARGS, 1,	do_buzzer,
	"buzzer     - control buzzer\n",
	"on	  - turn on all buzzer\n"
	"off  - turn off all buzzer\n"
	"play [times]- buzzer play.WARNING: Can't not stop manually\n"
);