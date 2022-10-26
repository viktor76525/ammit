#!/usr/bin/env bash
#rsync -vuLr --size-only --exclude current* --exclude latest* --exclude *.iso* rsync://mirror.aarnet.edu.au/pub/gentoo/releases/{amd64,arm64,riscv} releases
rsync -vuL --size-only rsync://mirror.aarnet.edu.au/pub/gentoo/distfiles/* distfiles/
