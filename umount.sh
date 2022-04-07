#!/bin/bash

if [ ! -d "$1" ]; then
	echo First parameter is not a directory.
	exit
fi

umount "$1"/proc
umount -l "$1"/dev
umount -l "$1"/sys
umount "$1"/var/cache/distfiles
umount "$1"/usr/src/initramfs

if [ -f "$1"/etc/resolv.conf ]; then
    rm "$1"/etc/resolv.conf
fi
