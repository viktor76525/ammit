#!/usr/bin/env bash
#rsync -vuLr --size-only --exclude current* --exclude latest* --exclude *.iso* rsync://mirror.aarnet.edu.au/pub/gentoo/releases/{amd64,arm64,riscv} releases
for i in $(seq 0 15 | while read n; do printf "%01x " $n; done); do
    rsync -vuL --size-only "rsync://mirror.aarnet.edu.au/pub/gentoo/distfiles/$i?/*" distfiles/
done
