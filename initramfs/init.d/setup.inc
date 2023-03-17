# distribution name
SYSTEM_NAME="AMMIT"
# boot device LABEL= or UUID= containing main image, path
BOOT_DEVICE="LABEL=Hesat"
# mount options for boot device
BOOT_OPTIONS="ro"
BOOT_IMG="/images/gentoo.sfs"

# superuser password
SU_PASSWD=''
# default user setup
STD_USER="live"
STD_GROUP=$STD_USER
STD_NAME="Live User"
STD_UID=1000
STD_GID=1000
STD_HOME="/home/$STD_USER"
STD_SHELL="/bin/zsh"
STD_PASSWD='$6$0cn6F1BR3y4wT64v$slbEp74yk0V5aoY/.1wpohsCnQVvL6jgbMHmmIotO6s.jKWwK7KcVM.hEKD.Y9dwOv9BNGy59H/YlT2qIOJ5//'
STD_GROUPS="users wheel video scanner plugdev docker dialout uucp vboxusers lpadmin kvm"

# system setup
HOSTNAME="phoenix"
# services enabled by default (display-manager for gui login) prefix with non-default runlevel eg: boot:zfs-import
SERVICES="display-manager NetworkManager sshd ammit-hosts bluetooth"
# blank to ask, examples: "us", "dvorak"
KEYMAP=""
# blank to ask, options: "FREE" (opensource) | "NVIDIA" (proprietary) | "KILL" (poweroff, for dual graphics)
GPU=""
# blank to ask, options: "YES" (setup if available), "REQUIRE" (fail if not found), will not ask if partition not available
PERSIST=""
# persistance drive LABEL= or UUID= or if lvm: "lvm:vg_name:lv_name"
PERSIST_DEVICE="LABEL=persistent"
# file system on persistance drive eg: ext4, btrfs
PERSIST_FS="ext4"
# mount options for persistance drive
PERSIST_OPTIONS="noatime,errors=remount-ro,discard"
# blank to ask, options: "YES", "NO"
COPY2RAM=""
# blank to ask, timezone spec eg: "NZ", "US/Pacific"
TIMEZONE=""
# blank to ask, "YES": system clock is in localtime (Windows default), "NO": system clock in UTC
LOCALTIME=""
# extra mounts, use escape sequences (\t) to separate columns
FSTAB=""
# symlink pairs separated by space, tab (\t) within pairs
SYMLINKS=""
