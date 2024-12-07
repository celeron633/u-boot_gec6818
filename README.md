# 粤嵌GEC6818开发板u-boot-2016.01 （基于FriendlyARM nanopi3修改）

## uboot README

[uboot README](README_raw)

## 信息：

* SOC：SAMSUNG S5P6818（官方已经不维护）
* RAM：1G
* EMMC：8G
* 开发toolchain：https://github.com/friendlyarm/prebuilts/tree/master/gcc-x64/toolchain-6.4-aarch64.tar.gz
* 开发平台：openSUSE Leap 15.6（使用自己喜欢的即可，HOSTCC版本最好不要大于12）
* 资料：https://github.com/celeron633/docs_gec6818（包含电路图，官方手册，三星6818手册）

粤嵌的这个开发板和友善的nanopi fire 3基本一样，友善的u-boot甚至可以直接运行，需要简单修改适配下网络，屏幕

## 编译方法：

```shell
source profile_env.sh
make s5p6818_gec6818_defconfig
make -j 12
```

## 烧写方法：

参考FriendlyARM的https://github.com/friendlyarm/sd-fuse_s5p6818/blob/master/README_cn.md

用编译产物的fip-nonsecure.img替换prebuilt目录下面原来的再烧写即可，或者已经可以进系统了，使用下面的脚本烧写：

https://github.com/celeron633/sd-fuse_s5p6818/blob/master/update-bl33.sh

**若现在使用的uboot 2014.07版本的32位请不要直接烧，会砖掉**，需要重建emmc上面的分区表，然后写入bl2（bl1-emmcboot.bin），bl31（fip-loader.img），bl32（fip-secure.img）（bl1在ARM ATF里的概念为芯片上面的bootrom，名字有点绕），以及我们编译出来的bl33（fip-nonsecure.img），再使用fdisk重新分区，重新恢复rootfs（我使用的targz+nc，也可以用sftp等等），需要一定linux操作基础

## 开发进度：

* [X] 启动，运行Linux
* [X] 网络（10M/100M可用，**1000M由于时钟配置有点问题**，GMAC给到PHY的时钟不太对，暂时不能用）
* [X] 存储（支持从EMMC，SD启动操作系统，不过从EMMC启动需要修改下bl1的NISH才行）
* [X] 屏幕（OK，使用S70配置RGB控制器，at070tn92也可以，不过进系统有问题，需要修改下dts）
* [X] 串口（波特率115200，使用UART0口，板上后面的RS232）

## 致谢：

* NexellCorp：https://github.com/NexellCorp/u-boot-2016.01
* FriendlyARM：https://github.com/friendlyarm/u-boot/tree/nanopi2-v2016.01
* u-boot
