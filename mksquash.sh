#!/bin/bash

mkdir -p build
if [ -e "build/gentoo.sfs" ]; then
    rm build/gentoo.sfs
fi
# -comp zstd -Xcompression-level 22 # normal
# -comp xz # highest compression for optical if required
gentoo/usr/bin/mksquashfs gentoo build/gentoo.sfs -comp xz -mem 8G -wildcards -e \
proc/* sys/* tmp/* usr/share/gtk-doc/* \
usr/share/sgml/* var/log/emerge*.log var/log/portage/* var/tmp/portage/* \
var/db/repos/gentoo/.git \
mnt/* */.bash_history */.lesshst \
*/.viminfo usr/src/*
#usr/share/man/* \
#var/db/* 
#var/cache/* \
#var/tmp/*
