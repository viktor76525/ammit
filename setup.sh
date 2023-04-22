#!/usr/bin/env sh

mkdir -p distfiles
mkdir -p gentoo

mkdir -p initramfs/dev
mkdir -p initramfs/mnt/root
mkdir -p initramfs/proc
mkdir -p initramfs/sys

mknod -m 600 initramfs/dev/console c 5 1
mknod -m 666 initramfs/dev/null c 1 3
mknod -m 620 initramfs/dev/tty0 c 4 0
mknod -m 620 initramfs/dev/tty1 c 4 1
mknod -m 660 initramfs/dev/ttyS0 c 4 64
mknod -m 666 initramfs/dev/zero c 1 5

chown 0:5 initramfs/dev/tty1
chown 0:14 initramfs/dev/ttyS0
