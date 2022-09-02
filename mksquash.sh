#!/bin/bash

if [ -e "gentoo.sfs" ]; then
    rm gentoo.sfs
fi
gentoo/usr/bin/mksquashfs gentoo gentoo.sfs -comp zstd -Xcompression-level 22 -mem 4G -wildcards -e \
proc/* sys/* tmp/* usr/share/gtk-doc/* \
usr/share/sgml/* var/log/emerge*.log var/log/portage/* var/tmp/portage/* \
var/db/repos/gentoo/.git \
mnt/* */.bash_history */.lesshst \
*/.viminfo usr/src/*
#usr/share/man/* \
#var/db/* 
#var/cache/* \
#var/tmp/*
