#!/usr/bin/env bash
rsync -vuL --size-only rsync://mirror.aarnet.edu.au/pub/gentoo/distfiles/* distfiles/
