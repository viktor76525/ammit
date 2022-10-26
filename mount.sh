#!/bin/bash

if [ ! -d "$1" ]; then
    echo First parameter is not a directory.
    exit
fi

if [ -e "/dev/disk/by-label/omen-sys" ]; then
    mkdir -p /home/tmp
    mount --bind /home/tmp "$1"/var/tmp
fi

mount -t proc proc "$1"/proc
mount --rbind /sys "$1"/sys
#mount --make-rslave "$1"/sys
mount --rbind /dev "$1"/dev
#mount --make-rslave "$1"/dev
mount --rbind /run "$1"/run
#mount --make-rslave "$1"/run
mkdir -p "$1"/var/cache/distfiles
mount --bind distfiles "$1"/var/cache/distfiles
mkdir -p "$1"/usr/src/initramfs
mount --bind initramfs "$1"/usr/src/initramfs
if [ -f /etc/resolv.conf ]; then
    cp --dereference /etc/resolv.conf "$1"/etc/resolv.conf
fi

chroot "$1" /bin/bash
