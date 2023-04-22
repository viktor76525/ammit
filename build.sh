#!/bin/bash

if [ ! -e "gentoo/usr/src/linux/arch/x86_64/boot/bzImage" ]; then
    echo EFI kernel stub not build yet.
    exit
fi

mkdir -p build
if [ -e "build/gentoo.sfs" ]; then
    rm build/gentoo.sfs
fi
# -comp zstd -Xcompression-level 22 # normal
# -comp xz # highest compression for optical if required
gentoo/usr/bin/mksquashfs gentoo build/gentoo.sfs -comp zstd -Xcompression-level 22 -mem 8G -wildcards -e \
proc/* sys/* tmp/* usr/share/gtk-doc/* \
usr/share/sgml/* var/log/emerge*.log var/log/portage/* var/tmp/portage/* \
var/db/repos/gentoo/.git \
mnt/* */.bash_history */.lesshst \
*/.viminfo usr/src/*
#usr/share/man/* \
#var/db/* 
#var/cache/* \
#var/tmp/*

mkdir -p build/EFI/Boot
mkdir -p build/iso/images
cp gentoo/usr/src/linux/arch/x86_64/boot/bzImage build/EFI/Boot/bootx64.efi
BOOT_SIZE=$(du -s build/EFI | cut -f 1)
dd if=/dev/zero of=build/iso/ammit-boot.img bs=1024 count=$(($BOOT_SIZE + 256))
mkfs.vfat -n "Ammit EFI" build/iso/ammit-boot.img
mcopy -v -i build/iso/ammit-boot.img -s build/EFI ::/
ln -sf ../../gentoo.sfs build/iso/images/
mkisofs -A "Ammit EFI Live Disc" -V "Ammit Live" -J -posix-L -r -v -iso-level 3 -o build/ammit.iso -eltorito-alt-boot -eltorito-platform efi -eltorito-boot ammit-boot.img -no-emul-boot ./build/iso
