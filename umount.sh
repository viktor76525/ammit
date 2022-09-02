#!/bin/bash

if [ ! -d "$1" ]; then
	echo First parameter is not a directory.
	exit
fi

if [ -e "/dev/disk/by-label/omen-home" ]; then
    rm -rf "$1"/var/tmp/*
    umount "$1"/var/tmp
fi

umount "$1"/proc
umount -l "$1"/sys
umount -l "$1"/dev
umount -l "$1"/run
umount "$1"/var/cache/distfiles
umount "$1"/usr/src/initramfs

if [ -f "$1"/etc/resolv.conf ]; then
    rm "$1"/etc/resolv.conf
fi
