#
# (C) Copyright 2002
# Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
#
# SPDX-License-Identifier:	GPL-2.0+
#
PLATFORM_RELFLAGS += -fno-common -ffixed-x18

PF_NO_UNALIGNED := $(call cc-option, -mstrict-align)
PLATFORM_CPPFLAGS += $(PF_NO_UNALIGNED)

# With the D-cache off memory is Device type and unaligned accesses fault.
# -mstrict-align doesn't help when GCC (>= 7) merges field stores through a
# pointer it assumes aligned, e.g. the IP header at pkt + 14.
PLATFORM_CPPFLAGS += $(call cc-option, -fno-store-merging)
