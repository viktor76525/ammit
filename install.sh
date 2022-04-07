#!/bin/bash

if ! mountpoint -q /boot; then
	echo not mounted
	exit
fi

mv /boot/EFI/Boot/bootx64.efi /boot/EFI/Boot/bootx64_prev.efi
cp gentoo/usr/src/linux/arch/x86/boot/bzImage /boot/EFI/Boot/bootx64.efi
