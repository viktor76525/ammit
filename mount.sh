#!/bin/bash

if [ ! -d "$1" ]; then
	echo First parameter is not a directory.
	exit
fi

mount -t proc proc "$1"/proc
mount --rbind /dev "$1"/dev
mount --rbind /sys "$1"/sys
mkdir -p "$1"/var/cache/distfiles
mount --bind distfiles "$1"/var/cache/distfiles
mkdir -p "$1"/usr/src/initramfs
mount --bind initramfs "$1"/usr/src/initramfs
if [ -f /etc/resolv.conf ]; then
    cp /etc/resolv.conf "$1"/etc/resolv.conf
fi

chroot "$1" /bin/bash
