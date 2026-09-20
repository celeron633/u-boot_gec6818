# u-boot_gec6818

[![Build](https://github.com/celeron633/u-boot_gec6818/actions/workflows/build.yml/badge.svg)](https://github.com/celeron633/u-boot_gec6818/actions/workflows/build.yml)

U-Boot 2016.01 for the GEC6818 board (Nexell S5P6818 SoC), based on
FriendlyARM's NanoPi3 support (the GEC6818 board is nearly identical to
FriendlyARM's NanoPi Fire3 - FriendlyARM's own u-boot mostly runs as-is,
needing only network/screen adaptation).

**English** | [中文](#中文)

## English

### Hardware

- SoC: Samsung/Nexell S5P6818 (no longer officially maintained)
- RAM: 1GB
- eMMC: 8GB
- Reference docs (schematics, official manuals, S5P6818 manual):
  https://github.com/celeron633/docs_gec6818

### This is part of three repos

- [bl1-gec6818](https://github.com/celeron633/bl1-gec6818) (branch `artik`) - BL1
- **u-boot_gec6818** (this repo) - BL33 / u-boot
- [linux_kernel_gec6818](https://github.com/celeron633/linux_kernel_gec6818)
  (branch `gec6818-v4.4.y`) - kernel

bl1-gec6818 supports two boot modes - the normal ARM Trusted Firmware
chain, and a newer `SKIP_ATF` mode that jumps from BL1 straight to
u-boot. See [bl1-gec6818's BOOT_MODES.md](https://github.com/celeron633/bl1-gec6818/blob/artik/BOOT_MODES.md)
for the full picture - it determines which image below you actually
need.

### Building

Toolchain - tested with [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
13.2.rel1, `aarch64-none-elf` target (any aarch64 GCC works, just match
`CROSS_COMPILE` to whatever prefix you have - `profile_env.sh` in this
repo uses `aarch64-linux-`, `compile_gec6818.sh` uses `aarch64-elf-`;
below uses `aarch64-none-elf-`, all equally fine):

```sh
wget https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf.tar.xz
tar xf arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf.tar.xz -C ~/
export PATH=~/arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf/bin:$PATH
sudo apt-get install -y device-tree-compiler  # dtc, for the .dtb build
```

`ARCH`/`CROSS_COMPILE` need to be **environment variables**, not
`make ARCH=... CROSS_COMPILE=...` command-line arguments - `config.mk`'s
`ARCH := $(CONFIG_SYS_ARCH:...)` is a plain assignment, which can only
override an environment variable, not one set on the make command line
(you'd otherwise get a stale `arch/aarch64/...` path instead of the
`arch/arm/...` this tree - `CONFIG_ARM64=y` under `arch/arm` - actually
needs):

```sh
export ARCH=aarch64
export CROSS_COMPILE=aarch64-none-elf-
make s5p6818_gec6818_defconfig
make -j"$(nproc)"          # -> u-boot.bin, fip-nonsecure.img
make u-boot-direct.img     # -> for bl1-gec6818's SKIP_ATF mode
```

GitHub Actions builds both `fip-nonsecure.img` and `u-boot-direct.img`
on every push and uploads them as a workflow artifact - see the badge
above, or `.github/workflows/build.yml`.

`fip-nonsecure.img` and `u-boot-direct.img` are **not interchangeable**:
the former is a real ARM TF-A FIP container (built via
`tools/fip_create`), meant to be loaded by BL31; the latter is
`u-boot.bin` wrapped directly in an NSIH header with a real entry
address, meant to be jumped to straight from BL1 in `SKIP_ATF` mode.
Putting the wrong one in the wrong slot won't boot.

### Flashing

See FriendlyARM's guide:
https://github.com/friendlyarm/sd-fuse_s5p6818/blob/master/README_cn.md

For the normal ATF chain, replace `fip-nonsecure.img` in `prebuilt/`
with your build and reflash, or - if the board already boots - use
https://github.com/celeron633/sd-fuse_s5p6818/blob/master/update-bl33.sh

For `SKIP_ATF` mode, see
[bl1-gec6818's BOOT_MODES.md](https://github.com/celeron633/bl1-gec6818/blob/artik/BOOT_MODES.md)
and `tools/write_sdcard.py` in that repo.

**If you're currently running the old 32-bit u-boot (2014.07), don't
flash straight over it - it'll brick the board.** You need to rebuild
the eMMC partition table first, then write bl2 (`bl1-emmcboot.bin`),
bl31 (`fip-loader.img`), bl32 (`fip-secure.img`) (ARM ATF's "bl1" here
means the on-chip boot ROM - the naming is a bit confusing), and this
repo's bl33 (`fip-nonsecure.img`), re-partition with fdisk, and restore
the rootfs (tar.gz + nc works, sftp too - needs some basic Linux
familiarity).

### Status

- [x] Boots, runs Linux
- [x] Network (10M/100M work; **1000M has a clock config issue** - GMAC's
      clock to the PHY isn't quite right, not usable yet)
- [x] Storage (boots from both eMMC and SD; eMMC boot needs BL1's NSIH
      changed first)
- [x] Screen (OK - uses S70 to configure the RGB controller; at070tn92
      also works but has issues once booted, needs a dts tweak)
- [x] Serial (115200 baud, UART0, the RS232 port on the back of the board)

### Credits

- NexellCorp: https://github.com/NexellCorp/u-boot-2016.01
- FriendlyARM: https://github.com/friendlyarm/u-boot/tree/nanopi2-v2016.01
- U-Boot

### Original README

The previous version of this README is kept at
[`README.orig.md`](README.orig.md). The stock upstream U-Boot README is
at [`README_raw`](README_raw).

---

## 中文

### 硬件信息

- SOC：三星/Nexell S5P6818（官方已经不维护）
- RAM：1G
- EMMC：8G
- 资料（电路图、官方手册、三星6818手册）：
  https://github.com/celeron633/docs_gec6818

粤嵌的这个开发板和友善的 NanoPi Fire3 基本一样，友善的 u-boot 甚至可以直接运行，
需要简单修改适配下网络、屏幕。

### 这是三个仓库中的一个

- [bl1-gec6818](https://github.com/celeron633/bl1-gec6818)（分支 `artik`）—— BL1
- **u-boot_gec6818**（本仓库）—— BL33 / u-boot
- [linux_kernel_gec6818](https://github.com/celeron633/linux_kernel_gec6818)
  （分支 `gec6818-v4.4.y`）—— 内核

bl1-gec6818 支持两种启动模式——正常的 ARM Trusted Firmware 链路，以及新加的
`SKIP_ATF` 模式（BL1 直接跳到 u-boot）。完整链路和取舍见
[bl1-gec6818 的 BOOT_MODES.md](https://github.com/celeron633/bl1-gec6818/blob/artik/BOOT_MODES.md)
——这决定了你下面实际需要哪个镜像。

### 编译方法

工具链——用 [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
13.2.rel1、`aarch64-none-elf` 版本测过（其实随便哪个 aarch64 GCC 都行，`CROSS_COMPILE`
跟你手头工具链的前缀对上就行——仓库里 `profile_env.sh` 用的是 `aarch64-linux-`，
`compile_gec6818.sh` 用的是 `aarch64-elf-`，下面用 `aarch64-none-elf-`，都一样能编）：

```sh
wget https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf.tar.xz
tar xf arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf.tar.xz -C ~/
export PATH=~/arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf/bin:$PATH
sudo apt-get install -y device-tree-compiler  # dtc，编译 .dtb 要用
```

`ARCH`/`CROSS_COMPILE` 得是**环境变量**，不能写成 `make ARCH=... CROSS_COMPILE=...`
命令行参数——`config.mk` 里 `ARCH := $(CONFIG_SYS_ARCH:...)` 是个普通赋值，只能覆盖
环境变量，覆盖不了命令行传的变量（不然会得到一个过时的 `arch/aarch64/...` 路径，
而这份代码实际需要的是 `arch/arm/...`，`CONFIG_ARM64=y` 是放在 `arch/arm` 底下的）：

```sh
export ARCH=aarch64
export CROSS_COMPILE=aarch64-none-elf-
make s5p6818_gec6818_defconfig
make -j"$(nproc)"          # -> u-boot.bin, fip-nonsecure.img
make u-boot-direct.img     # -> 给 bl1-gec6818 的 SKIP_ATF 模式用
```

每次 push，GitHub Actions 会把 `fip-nonsecure.img` 和 `u-boot-direct.img` 都编译好，
打包成 workflow artifact 上传——看上面的徽章，或者 `.github/workflows/build.yml`。

`fip-nonsecure.img` 和 `u-boot-direct.img` **不能混用**：前者是真正的 ARM TF-A FIP
容器（用 `tools/fip_create` 打包），给 BL31 加载用；后者是 `u-boot.bin` 直接套上
NSIH 头、带着真实入口地址，是给 `SKIP_ATF` 模式下 BL1 直接跳转用的。放错槽位启动
不了。

### 烧写方法

参考 FriendlyARM 的：
https://github.com/friendlyarm/sd-fuse_s5p6818/blob/master/README_cn.md

正常 ATF 链路的话，用编译产物的 `fip-nonsecure.img` 替换 `prebuilt` 目录下面原来
的再烧写即可；或者已经能进系统了，用下面的脚本烧写：
https://github.com/celeron633/sd-fuse_s5p6818/blob/master/update-bl33.sh

`SKIP_ATF` 模式见
[bl1-gec6818 的 BOOT_MODES.md](https://github.com/celeron633/bl1-gec6818/blob/artik/BOOT_MODES.md)
和该仓库里的 `tools/write_sdcard.py`。

**若现在使用的 uboot 2014.07 版本的 32 位请不要直接烧，会砖掉**，需要重建 emmc
上面的分区表，然后写入 bl2（`bl1-emmcboot.bin`）、bl31（`fip-loader.img`）、bl32
（`fip-secure.img`）（bl1 在 ARM ATF 里的概念是芯片上面的 bootrom，名字有点绕），
以及本仓库编译出来的 bl33（`fip-nonsecure.img`），再用 fdisk 重新分区，重新恢复
rootfs（我用的 targz+nc，也可以用 sftp 等等），需要一定 Linux 操作基础。

### 开发进度

- [x] 启动，运行 Linux
- [x] 网络（10M/100M 可用，**1000M 由于时钟配置有点问题**，GMAC 给到 PHY 的时钟
      不太对，暂时不能用）
- [x] 存储（支持从 EMMC、SD 启动操作系统，不过从 EMMC 启动需要先改下 BL1 的 NSIH）
- [x] 屏幕（OK，用 S70 配置 RGB 控制器，at070tn92 也可以，不过进系统有问题，需要
      改下 dts）
- [x] 串口（波特率 115200，用 UART0 口，板上后面的 RS232）

### 致谢

- NexellCorp：https://github.com/NexellCorp/u-boot-2016.01
- FriendlyARM：https://github.com/friendlyarm/u-boot/tree/nanopi2-v2016.01
- u-boot

### 原始 README

之前版本的 README 保留在 [`README.orig.md`](README.orig.md)，上游 U-Boot 自带的
README 在 [`README_raw`](README_raw)。
