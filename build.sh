#!/bin/bash
CPU_NUM=$(grep processor /proc/cpuinfo | awk '{field=$NF};END{print field+1}')
txtrst='\e[0m'  # Color off
txtred='\e[0;31m' # Red
txtgrn='\e[0;32m' # Green
txtylw='\e[0;33m' # Yellow
txtblu='\e[0;34m' # Blue

setup ()
{
    UBOOT_DIR="$(dirname "$(readlink -f "$0")")"
    BUILD_DIR="$UBOOT_DIR/build"
    CROSS_COMPILE="$ANDROID_BUILD_TOP/arm-2010q1/bin/arm-none-eabi-"
}

build ()
{
    local target=$1
    echo "Building for $target"
    local target_dir="$BUILD_DIR/$target"
    mkdir -p "$target_dir"
    make $target
    make -j$CPU_NUM
}

if [ x = "x$ANDROID_BUILD_TOP" ] ; then
	echo -e "${txtred}Android build environment must be configured${txtrst}"
	exit 1
fi
    
setup

if [ "$1" = clean ] ; then
    rm -fr "$BUILD_DIR"/*
    exit 0
fi

targets=("$@")

START=$(date +%s)

for target in "${targets[@]}" ; do 
    build $target
done

END=$(date +%s)
ELAPSED=$((END - START))
E_MIN=$((ELAPSED / 60))
E_SEC=$((ELAPSED - E_MIN * 60))
printf "Elapsed: "
[ $E_MIN != 0 ] && printf "%d min(s) " $E_MIN
printf "%d sec(s)\n" $E_SEC
