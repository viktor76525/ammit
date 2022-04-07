# Ammit
meta meta initramfs linux distribution

Created from a personal need, provides genuine features different from other distributions. No warranty provided, user is responsible for checking fitness for purpose including security. Named Ammit because Phoenix is overused but it is a very good description of what this distribution is. Free for anyone to use.

A linux distribution based on the Gentoo "meta" distribution for those without time to waste. Ammit goes a step further to abstract the system from the data and specific wanted configurations.
It achieves this by modifying what is basically a Gentoo stage 3 tarball which has had extra packages and emerge configurations installed. Everything else (software configuration) is handled by the initramfs. This allows some cool things that save lots of time for the admin.
 * multiple software configurations can be paired with the same system configurations
 * the system is a fixed image so any issues can be solved by using a previous version
 * initramfs can detect machine IDs so the same system can be deployed on many machines with slight changes in configuration such as ssh keys and hostname
 * system images can be tested on a test machine before being deployed to others
 * based on Gentoo so no painful version upgrades, system can be completely changed using a few emerege configuration files
 * can run from a USB key, can run from RAM
 * allows no mess from being saved in your home directory

# Install

## Initial Setup
```
mkdir distfiles
mkdir gentoo
cp <stage-3-tarball> gentoo/
cd gentoo
tar xvvf <stage-3-tarball>
cd ..
```

## System Maintenance
Check the gentoo install guide for ideas on things to do. Most of it is irrelevant so initial setup is quick.
```
./mount.sh gentoo
<do-stuff-in-chroot>
exit
./umount.sh gentoo
```

## Deploying
You need only 2 partitions.
1. EFI boot partition (gpt partition table on disk, fat32 file system, boot/esp flag on).
2. Partition for gentoo.sfs image. Any partition format which has built in support in the kernel.
```
# make sure you have run the umount script
./mksquash.sh
cp gentoo.sfs /mnt/boot-partition/images/gentoo.sfs
cp gentoo/usr/src/linux/arch/x86_64/boot/bzImage /mnt/efi-partition/EFI/Boot/bootx64.efi
```
