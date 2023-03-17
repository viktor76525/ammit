#!/usr/bin/env sh

mkdir -p initramfs/dev
mkdir -p initramfs/mnt/root
mkdir -p initramfs/proc
mkdir -p initramfs/sys

mknod initramfs/dev/console c 5 1
mknod initramfs/dev/null c 1 3
mknod initramfs/dev/tty0 c 4 0
mknod initramfs/dev/tty1 c 4 1
mknod initramfs/dev/ttyS0 c 4 64
mknod initramfs/dev/zero c 1 5

chmod 600 initramfs/dev/console
chmod 666 initramfs/dev/null
chmod 620 initramfs/dev/tty0
chmod 620 initramfs/dev/tty1
chmod 660 initramfs/dev/ttyS0
chmod 666 initramfs/dev/zero

chown 0:5 initramfs/dev/tty1
chown 0:14 initramfs/dev/ttyS0
