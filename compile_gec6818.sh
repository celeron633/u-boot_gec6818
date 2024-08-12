#!/bin/bash

toolchain_prefix="aarch64-elf-"

make CROSS_COMPILE=${toolchain_prefix} -j$(nproc)
