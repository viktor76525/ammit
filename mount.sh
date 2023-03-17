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
mount --rbind /dev "$1"/dev
mount --rbind /run "$1"/run
mkdir -p "$1"/var/cache/distfiles
mount --bind distfiles "$1"/var/cache/distfiles
# nodejs doesnt like being compiled in the chroot
mkdir -p "$1"/var/cache/binpkgs
if [ -d /var/cache/binpkgs ]; then
    mount --bind /var/cache/binpkgs "$1"/var/cache/binpkgs
fi
mkdir -p "$1"/usr/src/initramfs
mount --bind initramfs "$1"/usr/src/initramfs
if [ -f /etc/resolv.conf ]; then
    cp --dereference /etc/resolv.conf "$1"/etc/resolv.conf
fi

chroot "$1" /bin/bash
