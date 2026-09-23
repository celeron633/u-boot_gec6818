/*
 * S5P6818 in AArch32 (CONFIG_S5P6818_AARCH32): the SoC code is the same
 * as the AArch64 build's, plus what armv7's start.S and board_r expect.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#include "../../armv8/s5p6818/cpu.c"

void s_init(void)
{
}

void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
	dcache_enable();
}
