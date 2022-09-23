#!/bin/bash

function pt_error()
{
    echo -e "\033[1;31mERROR: $*\033[0m"
}

function pt_warn()
{
    echo -e "\033[1;31mWARN: $*\033[0m"
}

function pt_info()
{
    echo -e "\033[1;32mINFO: $*\033[0m"
}

function execute_cmd() 
{
    eval $@ || exit $?
}

function usage()
{
    echo -e "\033[1;32mUsage: `basename $0` /dev/sdX\033[0m"
    exit 1
}

function parse_arg()
{
    if [ $# -ne 1 ]; then
        usage
    fi
    FUSE_DEVICE=$1
}

FUSE_DEVICE=none
signed_bl1_position=1
bl2_position=9
uboot_position=57

parse_arg $@

DEV_NAME=`basename ${FUSE_DEVICE}`
BLOCK_CNT=`cat /sys/block/${DEV_NAME}/size`
if [ $? -ne 0 ]; then
    pt_error "Fail to find device ${DEV_NAME}"
    exit 1
fi

if [ ${BLOCK_CNT} -le 0 ]; then
    pt_error "NO media found in card reader."
    exit 1
fi

if [ ${BLOCK_CNT} -gt 64000000 ]; then
    pt_error "Block device size ${BLOCK_CNT} is too large"
    exit 1
fi


pt_info "BL1 fusing"
dd iflag=dsync oflag=dsync if=c110.signedBL1_bin of=${FUSE_DEVICE} seek=$signed_bl1_position

pt_info "u-boot fusing"
dd iflag=dsync oflag=dsync if=../u-boot.bin of=${FUSE_DEVICE} seek=$bl2_position count=16
dd iflag=dsync oflag=dsync if=../u-boot.bin of=${FUSE_DEVICE} seek=$uboot_position
