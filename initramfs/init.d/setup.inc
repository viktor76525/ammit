# distribution name
SYSTEM_NAME="Ammit"
# boot device containing main image, path
BOOT_LABEL="Hathor"
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
STD_GROUPS="users wheel video scanner plugdev uucp vboxusers rstudio-server"

# system setup
HOSTNAME="phoenix"
# services enabled by default (xdm for gui login)
SERVICES="xdm NetworkManager sshd rstudio-server ammit-hosts"
# blank to ask, examples: "us", "dvorak"
KEYMAP=""
# blank to ask, options: "FREE" (opensource) | "NVIDIA" (proprietary) | "KILL" (poweroff, for dual graphics)
GPU=""
# blank to ask, options: "YES" (setup if available), "REQUIRE" (fail if not found), will not ask if partition not available
PERSIST=""
# label of persistance drive
PERSIST_LABEL="persistent"
