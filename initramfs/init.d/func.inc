
print_title() {
    echo -e "${WHITE}..:: WELCOME TO ${YELLOW}${SYSTEM_NAME}${WHITE}, A ${MAGENTA}GENTOO${WHITE} SYSTEM ::..${NORMAL}"
    echo -e "======================================================="
}

good_msg() {
    echo -e "${GOOD}>>${NORMAL}${BOLD} ${1} ${NORMAL}"
}

bad_msg() {
    echo -e "${BAD}!!${NORMAL}${BOLD} ${1} ${NORMAL}"
}

warn_msg() {
    echo -e "${WARN}**${NORMAL}${BOLD} ${1} ${NORMAL}"
}


proc_start() {
    echo -n "${WHITE}[${YELLOW}>>${WHITE}] ${1}... ${NORMAL}"
    processing=true
}

proc_update() {
    echo -ne "${BACK_UP}${WHITE}[${YELLOW}>>${WHITE}] ${1}... ${YELLOW}${2}${NORMAL}"
}

proc_good() {
    echo -e "${BACK_UP}${WHITE}[${GREEN}OK${WHITE}]${NORMAL} ${1}. ${WHITE}${2}${NORMAL}"
    processing=false
}

proc_bad() {
    echo -e "${BACK_UP}${WHITE}[${RED}!!${WHITE}]${NORMAL} ${1}. ${WHITE}${2}${NORMAL}"
    processing=false
}

ask() {
    read -p "${CYAN}<<${NORMAL} $1: " answer
    echo $answer
}

adduser2() {
    # manual version of adduser

    # try to check for updates from persistent storage
    created="n"
    if [ "$PERSIST" != "NO" ]; then
    	info=$(grep "^${STD_USER}:" "$NEW_ROOT/etc/passwd")
	if [ ! -z "$info" ]; then
	    created="y"
	    STD_HOME=$(echo $info | cut -d: -f6)
	fi
    fi

    # add data if not existing from persistent storage
    if [ ! -d "${NEW_ROOT}${STD_HOME}" ]; then
        mkdir -p "${NEW_ROOT}${STD_HOME}"
        cp -rT "${NEW_ROOT}/etc/skel" "${NEW_ROOT}${STD_HOME}"
        cp -a /home2/. "${NEW_ROOT}${STD_HOME}/"
	if [ -d "/init.d/users/${STD_USER}" ]; then
	    cp -a "/init.d/users/${STD_USER}/." "${NEW_ROOT}${STD_HOME}/"
	fi
        chown -R $STD_UID:$STD_GID "${NEW_ROOT}${STD_HOME}"
    fi

    # add account information in system
    if [ "$created" == "y" ]; then
        return
    fi
    echo "${STD_USER}:x:${STD_UID}:${STD_GID}:${STD_NAME}:${STD_HOME}:${STD_SHELL}" \
        >> "${NEW_ROOT}/etc/passwd"
    echo "${STD_GROUP}:x:${STD_GID}:" >> "${NEW_ROOT}/etc/group"
    echo "${STD_USER}:${STD_PASSWD}:::::::" >> "${NEW_ROOT}/etc/shadow"
    for g in $STD_GROUPS; do
        sed -i "/^$g:/ s/$/,$STD_USER/" "${NEW_ROOT}/etc/group"
    done
}

find_machine_config() {
    plabel="Loading Machine Configuration"
    proc_start "$plabel"
    prefix="/init.d/machines/"
    # motherboard UUID
    a=$prefix$(cat /sys/class/dmi/id/product_uuid 2>/dev/null)/setup.inc
    if [ -f "$a" ]; then
	MACHINE=$prefix$(cat /sys/class/dmi/id/product_uuid)
        proc_good "$plabel" "Found UUID Configuration"
        return
    fi
    # motherboard Serial
    a=$(cat "/sys/class/dmi/id/"*_serial 2>/dev/null)
    for i in $a; do
        if [ -f "$prefix$i/setup.inc" ]; then
	    MACHINE="$prefix$i"
            proc_good "$plabel" "Found Serial Configuration"
            return
        fi
    done
    # mac address based id requires kernel built in network driver
    a=$(cat "/sys/class/net/"*"/address" 2>/dev/null)
    for i in $a; do
        if [ -f "$prefix$i/setup.inc" ]; then
	    MACHINE="$prefix$i"
            proc_good "$plabel" "Found MAC Address Configuration"
            return
        fi
    done
    proc_good "$plabel" "None available"
}

machine_config() {
    find_machine_config

    if [ ! -z "$MACHINE" ]; then
        read -t 1 -n 1 -p "press any key to cancel machine config..." && unset MACHINE || source "$MACHINE/setup.inc"
    fi
}

virtual_fs() {
    if [ "$1" == "mount" ]; then
        mount -t devtmpfs none /dev
        mount -t proc none /proc
        mount -t sysfs none /sys
	if ! mountpoint -q /sys/firmware/efi/efivars; then
            mount -t efivarfs none /sys/firmware/efi/efivars &>/dev/null
        fi
    else
        if mountpoint -q /sys/firmware/efi/efivars; then
	    umount /sys/firmware/efi/efivars
	fi
        umount /dev /proc /sys
    fi
}

kernel_messages() {
    echo "$1" > /proc/sys/kernel/printk
}

exec_shell() {
    warn_msg "Dropping into Interactive Shell. Exiting will Shutdown."
    exec /init.d/init.end
}

add_module() {
    sed -i "/\(^modules=\"[^\"]*\)\"/ s//\1 $1\"/" "$NEW_ROOT/etc/conf.d/modules"
}

auto_larger_font() {
    for size in 28 24 22 20 18 16; do
        if setfont /lib/fonts/ter-v${size}b.psf.gz -C /dev/console \
        && [ $(ttysize | awk '$0=$1') -gt 120 ]; then break; fi
    done
    FONT_SIZE=$size
}

set_larger_font() {
    sed -i "s/^consolefont=\".*\"$/consolefont=\"ter-v${FONT_SIZE}b\"/" "$NEW_ROOT/etc/conf.d/consolefont"
}

load_cmdline() {
    touch /tmp/cmdline
    for i in $(cat /proc/cmdline); do
        if echo "$i" | grep -qE '^[a-zA-Z_]+[a-zA-Z0-9_]*=[a-zA-Z0-9_~!@#$%^.,+]+$'; then
            echo "$i" >> /tmp/cmdline
        fi
    done
    source /tmp/cmdline
}

choose_keymap() {
    if [ -z "$KEYMAP" ]; then
        echo
        cat /lib/keymaps/list
        KEYMAP=$(ask "Load Keymap (Enter for Default)")
        case ${KEYMAP} in
            1|azerty) KEYMAP=azerty ;;
            2|be) KEYMAP=be ;;
            3|bg) KEYMAP=bg ;;
            4|br-a) KEYMAP=br-a ;;
            5|br-l) KEYMAP=br-l ;;
            6|by) KEYMAP=by ;;
            7|cf) KEYMAP=cf ;;
            8|croat) KEYMAP=croat ;;
            9|cz) KEYMAP=cz ;;
            10|de) KEYMAP=de ;;
            11|dk) KEYMAP=dk ;;
            12|dvorak) KEYMAP=dvorak ;;
            13|es) KEYMAP=es ;;
            14|et) KEYMAP=et ;;
            15|fi) KEYMAP=fi ;;
            16|fr) KEYMAP=fr ;;
            17|gr) KEYMAP=gr ;;
            18|hu) KEYMAP=hu ;;
            19|il) KEYMAP=il ;;
            20|is) KEYMAP=is ;;
            21|it) KEYMAP=it ;;
            22|jp) KEYMAP=jp ;;
            23|la) KEYMAP=la ;;
            24|lt) KEYMAP=lt ;;
            25|mk) KEYMAP=mk ;;
            26|nl) KEYMAP=nl ;;
            27|no) KEYMAP=no ;;
            28|pl) KEYMAP=pl ;;
            29|pt) KEYMAP=pt ;;
            30|ro) KEYMAP=ro ;;
            31|ru) KEYMAP=ru ;;
            32|se) KEYMAP=se ;;
            33|sg) KEYMAP=sg ;;
            34|sk-y) KEYMAP=sk-y ;;
            35|sk-z) KEYMAP=sk-z ;;
            36|slovene) KEYMAP=slovene ;;
            37|trf) KEYMAP=trf ;;
            38|trq) KEYMAP=trq ;;
            39|ua) KEYMAP=ua ;;
            40|uk) KEYMAP=uk ;;
            41|us) KEYMAP=us ;;
            42|wangbe) KEYMAP=wangbe ;;
            43|sf|ch*) KEYMAP=sf ;;
        esac
    fi

    if [ -e /lib/keymaps/${KEYMAP}.map ]; then
        good_msg "Loading the ''${KEYMAP}'' Keymap"
        loadkmap < /lib/keymaps/${KEYMAP}.map
    elif [ -z "${KEYMAP}" ]; then
        KEYMAP=us
        good_msg "Keeping US Keymap"
    else
        bad_msg "Invalid Choice: ''${KEYMAP}''"
        unset KEYMAP
        choose_keymap
    fi
}

set_keymap() {
    # system
    sed -i "s/^keymap=\".*\"$/keymap=\"$KEYMAP\"/" "$NEW_ROOT/etc/conf.d/keymaps"
    # default user
    if [ "$KEYMAP" != "us" ]; then
	# display name for KDE tray (first 2 characters)
        dname=$( echo ${KEYMAP::2} | tr a-z A-Z )
        cat << EOF > "${NEW_ROOT}${STD_HOME}/.config/kxkbrc"
[Layout]
DisplayNames=$dname,US
LayoutList=$KEYMAP,us
LayoutLoopCount=-1
Model=pc101
ResetOldOptions=false
ShowFlag=false
ShowLabel=true
ShowLayoutIndicator=true
ShowSingle=false
SwitchMode=Global
Use=true
EOF
        chown -R $STD_UID:$STD_GID "${NEW_ROOT}${STD_HOME}/.config"
    fi
}

set_zram_swap() {
    plabel="Creating ZRAM SWAP"
    proc_start "$plabel"
    echo "lz4" > /sys/block/zram0/comp_algorithm
    echo $(cat /proc/meminfo | grep MemTotal | awk '{ print $2 }')K > /sys/block/zram0/disksize
    if mkswap -L zswap /dev/zram0 >/dev/null; then
        proc_good "$plabel"
    else
        proc_bad "$plabel"
    fi
}

set_fstab() {
    echo "/dev/zram0 none swap sw 0 0" >> "$NEW_ROOT/etc/fstab"
    for l in $FSTAB; do
        mp=$NEW_ROOT$(echo -e "$l" | cut -d$'\t' -f2)
        if [ ! -d "$mp" ]; then
            mkdir -p "$mp"
        fi
        echo -e "$l" >> "$NEW_ROOT/etc/fstab"
    done
    for s in $SYMLINKS; do
        src=$(echo -e "$s" | cut -d$'\t' -f1)
        dst=$(echo -e "$s" | cut -d$'\t' -f2)
        parent=$(dirname "$dst")
        mkdir -p "$NEW_ROOT$parent"
        if [ -e "$NEW_ROOT$dst" ]; then
            mv "$NEW_ROOT$dst" "$NEW_ROOT${dst}_presymlink"
        fi
        ln -sf "$src" "$NEW_ROOT$dst"
    done
}

finish_mounts() {
    # mount system
    if [ "$COPY2RAM" == "NO" ]; then
        mount -t squashfs /mnt/boot$BOOT_IMG /mnt/sfs
	mpoints="boot sfs rw"
    else
        umount /mnt/boot
	proc_good "Boot Device Safe to Remove"
        mount -t squashfs /mnt/ram/gentoo.sfs /mnt/sfs
	mpoints="sfs rw ram"
    fi

    # rw layer tmpfs can compete and evenually moved to zram or start with zram
    # mount -t tmpfs -o size=100% rootRW /mnt/rw
    echo "lz4" > /sys/block/zram1/comp_algorithm
    echo $(cat /proc/meminfo | grep MemTotal | awk '{ print $2 }')K > /sys/block/zram1/disksize
    mke2fs -L rootRW -m 1 /dev/zram1 >/dev/null
    mount -o discard /dev/zram1 /mnt/rw
    # construct overlayfs
    mkdir -p /mnt/rw/ofs_delta
    mkdir -p /mnt/rw/ofs_work
    mount -t overlay -o lowerdir=/mnt/sfs,upperdir=/mnt/rw/ofs_delta,workdir=/mnt/rw/ofs_work rootOFS $NEW_ROOT

    # rearrange
    for mpoint in $mpoints; do
        mkdir -p "$NEW_ROOT/mnt/$mpoint"
        mount --move /mnt/$mpoint ${NEW_ROOT}/mnt/$mpoint
    done
    # efi boot partition
    if [ ! -z "$PARTUUID" ]; then
        echo -e "PARTUUID=$PARTUUID\t/boot\tvfat\tnoauto,user\t0 0" >> "$NEW_ROOT/etc/fstab"
    fi

    proc_good "System Ready for Configuration"
}

poweroff_gpu() {
	proc_start "Powering off GPU"
    insmod /lib/modules/acpi_call.ko
	if lsmod | grep -q acpi_call; then
		methods="
		\_SB.PCI0.P0P1.VGA._OFF
		\_SB.PCI0.P0P2.VGA._OFF
		\_SB_.PCI0.OVGA.ATPX
		\_SB_.PCI0.OVGA.XTPX
		\_SB.PCI0.P0P3.PEGP._OFF
		\_SB.PCI0.P0P2.PEGP._OFF
		\_SB.PCI0.P0P1.PEGP._OFF
		\_SB.PCI0.MXR0.MXM0._OFF
		\_SB.PCI0.PEG1.GFX0._OFF
		\_SB.PCI0.PEG0.GFX0.DOFF
		\_SB.PCI0.PEG1.GFX0.DOFF
		\_SB.PCI0.PEG0.PEGP._OFF
		\_SB.PCI0.XVR0.Z01I.DGOF
		\_SB.PCI0.PEGR.GFX0._OFF
		\_SB.PCI0.PEG.VID._OFF
		\_SB.PCI0.PEG0.VID._OFF
		\_SB.PCI0.P0P2.DGPU._OFF
		\_SB.PCI0.P0P4.DGPU.DOFF
		\_SB.PCI0.IXVE.IGPU.DGOF
		\_SB.PCI0.RP00.VGA._PS3
		\_SB.PCI0.RP00.VGA.P3MO
		\_SB.PCI0.GFX0.DSM._T_0
		\_SB.PCI0.LPC.EC.PUBS._OFF
		\_SB.PCI0.P0P2.NVID._OFF
		\_SB.PCI0.P0P2.VGA.PX02
		\_SB_.PCI0.PEGP.DGFX._OFF
		\_SB_.PCI0.VGA.PX02
		\_SB.PCI0.PEG0.PEGP.SGOF
		\_SB.PCI0.AGP.VGA.PX02
		"
		
		for m in $methods; do
			echo $m > /proc/acpi/call
			result=$(cat /proc/acpi/call)
			case "$result" in
				Error*)
					;;
				*)
				proc_good "Powering off GPU ($m)"
				# break # try other methods too
				;;
			esac
		done
	
	else
		proc_bad "Powering off GPU" "(no kernel module)"
	fi
	
}

ask_timezone() {
    if [ -z "$TIMEZONE" ]; then
        ls /usr/share/zoneinfo
        TIMEZONE=$(ask "Enter timezone (Enter for NZ)")
    fi

    if [ -z "$TIMEZONE" ]; then
        TIMEZONE="NZ"
        good_msg "Using timezone NZ"
	return
    fi

    while [ -d /usr/share/zoneinfo/$TIMEZONE ]; do
        ls /usr/share/zoneinfo/$TIMEZONE
	TIMEZONE=$TIMEZONE/$(ask "Enter timezone")
    done

    if [ ! -f /usr/share/zoneinfo/$TIMEZONE ]; then
        bad_msg "Invalid Choice: ''${TIMEZONE}''"
	unset TIMEZONE
    	ask_timezone
    else
        good_msg "Using timezone $TIMEZONE"
    fi
}

set_timezone() {
    if [ -f "$NEW_ROOT/etc/localtime" ]; then
        rm "$NEW_ROOT/etc/localtime"
    fi
    echo "$TIMEZONE" > "$NEW_ROOT/etc/timezone"
    ln -s "/usr/share/zoneinfo/$TIMEZONE" "$NEW_ROOT/etc/localtime"
}

ask_localtime() {
    if [ -z "$LOCALTIME" ]; then
        LOCALTIME=$(ask "System Clock in Local Time? [y/N]")
    fi

    if [ "$LOCALTIME" == "" ] || [ "$LOCALTIME" == "n" ] || [ "$LOCALTIME" == "N" ] || [ "$LOCALTIME" == "NO" ]; then
        LOCALTIME="NO"
        good_msg "System clock in UTC"
    elif [ "$LOCALTIME" == "y" ] || [ "$LOCALTIME" == "Y" ] || [ "$LOCALTIME" == "YES" ]; then
        LOCALTIME="YES"
        good_msg "System clock in local time"
    else
        bad_msg "Invalid Choice: Enter an option or nothing for the default."
        unset LOCALTIME
        ask_localtime
    fi
}

set_localtime() {
    if [ "$LOCALTIME" == "YES" ]; then
        sed -i "s/^clock=\".*\"$/clock=\"local\"/" "$NEW_ROOT/etc/conf.d/hwclock"
    fi
}

ask_gpu_select() {
    if [ -z "$GPU" ]; then
        GPU=$(ask "Graphics Selection (Free/Nvidia/Poweroff)? [F/n/p]")
    fi

    if [ "$GPU" == "" ] || [ "$GPU" == "f" ] || [ "$GPU" == "F" ] || [ "$GPU" == "FREE" ]; then
        GPU="FREE"
        good_msg "Using free drivers"
    elif [ "$GPU" == "p" ] || [ "$GPU" == "P" ] || [ "$GPU" == "KILL" ]; then
        GPU="KILL"
        good_msg "Hybrid GPU will be switched off"
    elif [ "$GPU" == "n" ] || [ "$GPU" == "N" ] || [ "$GPU" == "NVIDIA" ]; then
        good_msg "Using NVIDIA proprietary drivers"
        GPU="NVIDIA"
    else
        bad_msg "Invalid Choice: Enter an option or nothing for the default."
        unset GPU
        ask_gpu_select
    fi
}

set_gpu_select() {
    if [ "$GPU" == "FREE" ]; then
        # replace nvidia setup with blacklist
        echo "blacklist nvidia" > "$NEW_ROOT/etc/modprobe.d/nvidia.conf"

    elif [ "$GPU" == "KILL" ]; then
        # sleep hook will power down GPU on every resume
        #cp "/etc/gpu_kill" "$NEW_ROOT/etc/pm/sleep.d/50_gpu_kill"
        #chmod +x "$NEW_ROOT/etc/pm/sleep.d/50_gpu_kill"
        # poweroff discreet graphics
        poweroff_gpu
        # also part of nvidia drivers
        if [ -e "$NEW_ROOT/etc/modprobe.d/nvidia.conf" ]; then
            rm "$NEW_ROOT/etc/modprobe.d/nvidia.conf"
        fi

    elif [ "$GPU" == "NVIDIA" ]; then
        # blacklist free drivers
        echo "blacklist nouveau" > "$NEW_ROOT/etc/modprobe.d/nouveau-noauto.conf"
        # eselect nvidia (outdated?)
        cp "/etc2/local.d/nvidia.start" "$NEW_ROOT/etc/local.d/nvidia.start"

    fi
}

set_modules() {
    # init modules file
    if ! grep -q '^modules=".*"' "$NEW_ROOT/etc/conf.d/modules"; then
        echo 'modules=""' >> "$NEW_ROOT/etc/conf.d/modules"
    fi

    # 80ee:beef = vboxvideo
    # 80ee:cafe = vboxservice
    #add_module "vboxdrv vboxnetadp vboxnetflt"
    proc_start "Testing if VirtualBox Guest"
    if busybox lspci | grep -q ': 80ee:'; then
        add_module "vboxguest"
        add_module "vboxsf"
        add_module "vboxvideo"
        # TODO! must set service to start
        # TODO? add shortcut to share locations
        proc_good "Testing if VirtualBox Guest" "guest drivers set to load"
    else
        proc_good "Testing if VirtualBox Guest (false)"
    fi

	# 14e4: = broadcom
	proc_start "Testing for Broadcom WiFi"
	for i in $(busybox lspci | grep -o '14e4:....'); do
		if cat /lib/modules/broadcom.list | grep -q $i; then
			# blacklist conflicting modules
			echo blacklist b43 > "$NEW_ROOT/etc/modprobe.d/bcm-blacklist.conf"
			echo blacklist bcma >> "$NEW_ROOT/etc/modprobe.d/bcm-blacklist.conf"
			echo blacklist ssb >> "$NEW_ROOT/etc/modprobe.d/bcm-blacklist.conf"
			echo blacklist mac80211 >> "$NEW_ROOT/etc/modprobe.d/bcm-blacklist.conf"

			rm "$NEW_ROOT/etc/modprobe.d/wl-noauto.conf"
			proc_good "Using Broadcom WiFi Driver"
			break
		fi
	done
	if $processing; then
		proc_good "Not Using Broadcom Driver"
	fi
}

find_device() {
    if [ -z "$BOOT_LABEL" ]; then
        ask_device
        return
    fi
    plabel="Waiting for Boot Device"
    proc_start "$plabel"
    TRIES=50
    while [ $TRIES -gt 0 ] && sleep 0.2 && ! BOOT_PARTITION=$(findfs LABEL=$BOOT_LABEL); do
        TRIES=$(($TRIES-1))
    done
    if [ -e "$BOOT_PARTITION" ]; then
        proc_good "$plabel"
    else
        proc_bad "$plabel"
        ask_device
    fi
}

ask_device() {
    n=0
    echo
    for dev in $(cat /proc/partitions | tail -n +3 | awk '{ print $4 }'); do
        details=$(blkid /dev/$dev | cut -c6-)
	if [ ! -z "$details" ]; then
	    echo $details
	    n=$(($n + 1))
	fi
    done
    echo
    echo "$n partitions found (press enter to refresh, shell to exit to shell)"
    echo

    BOOT_PARTITION=$(ask "System Partition")
    if [ "$BOOT_PARTITION" == "" ]; then
        ask_device
    elif [ "$BOOT_PARTITION" == "shell" ]; then
        exec_shell
    elif [ -b "/dev/$BOOT_PARTITION" ]; then
       BOOT_PARTITION="/dev/$BOOT_PARTITION"
       good_msg "System Partition $BOOT_PARTITION" 
    else
        bad_msg "Invalid Choice: ''/dev/${BOOT_PARTITION}''"
	ask_device
    fi
}

open_device() {
    plabel="Accessing Device"
    proc_start "$plabel"
    mkdir /mnt/boot
    mkdir /mnt/ram
    mkdir /mnt/sfs
    mkdir /mnt/rw
    mount -o ro $BOOT_PARTITION /mnt/boot
    if [ -e /mnt/boot$BOOT_IMG ]; then
        proc_good "$plabel"
    else
        proc_bad "$plabel"
        choose_keymap
        exec_shell
    fi
}

find_efi() {
    plabel="EFI Info"
    proc_start "$plabel"

    vars="/sys/firmware/efi/efivars"
    guid="8be4df61-93ca-11d2-aa0d-00e098032b8c"
    if [ ! -f $vars/BootCurrent-$guid ]; then
        proc_bad "$plabel" "efivars not found"
	return
    fi
    current=$vars/Boot$(hexdump -v -e '1/2 "%04x\n"' $vars/BootCurrent-$guid | tail -n 1)-$guid
    if [ ! -f $current ]; then
        proc_bad "$plabel" "efivars not found"
        return
    fi

    boot=$(hexdump -v -e '1/2 "%04x\n"' $current | awk '''
    BEGIN {
        section=1; subsection=0; skip=0
        label=""; partuuid=""
    }
    # first 2 are info about var, next 2 are EFI_LOAD_OPTION Attributes
    NR <= 4 { next }
    NR == 5 { fpllength = int("0x" $1) - 0x0002; next }
    {
        if (section == 1) {
            if ($1 == "0000") {
                section += 1
            } else {
                label = label sprintf("%c", int("0x" $1))
            }
        }
        else if (section == 2) {
            if (skip > 0) {
                skip -= 2
            } else if (subsection == 0) {
                type = $1
                if (type == "ff7f") exit
                subsection += 1
            } else if (subsection == 1) {
                if (type != "0104") { skip = int("0x" $1) - 0x004; subsection = 0 }
                else { subsection += 1; fpllength = int("0x" $1) - 0x002; skip = 20 }
            } else {
                len = length( partuuid )
                if (len == 0) partuuid = $1 "-"
                else if (len == 5) partuuid = $1 partuuid
                else if (len <= 14) partuuid = partuuid $1 "-"
                else if (len == 19) partuuid = partuuid substr($1, 3, 2) substr($1, 1, 2) "-"
                else if (len <= 32) partuuid = partuuid substr($1, 3, 2) substr($1, 1, 2)
                else if ($1 != "0202") partuuid = ""
            }
            fpllength -= 2
            if (fpllength == 0 ) exit
        }
    }
    END { print label; print partuuid }
    ''')

    EFI_LOAD_OPTION_DESC=$(echo -e "$boot" | cut -d$'\n' -f1)
    PARTUUID=$(echo -e "$boot" | cut -d$'\n' -f2)
    if [ ! -z "$EFI_LOAD_OPTION_DESC" ]; then
        if [ ! -z "$PARTUUID" ]; then
            proc_good "$plabel" "$EFI_LOAD_OPTION_DESC"
        else
            proc_good "$plabel (no guid)" "$EFI_LOAD_OPTION_DESC"
        fi
    else
        proc_bad "$plabel" "EFI Description Not Read (non fatal)"
    fi
}

ask_copy2ram() {
    if [ -z "$COPY2RAM" ]; then
        COPY2RAM=$(ask "Copy system to RAM? [y/N]")
    fi

    if [ "$COPY2RAM" == "y" ] || [ "$COPY2RAM" == "Y" ] || [ "$COPY2RAM" == "YES" ]; then
        COPY2RAM="YES"
        good_msg "System will be copied to RAM"
    elif [ "$COPY2RAM" == "" ] || [ "$COPY2RAM" == "n" ] || [ "$COPY2RAM" == "N" ] || [ "$COPY2RAM" == "NO" ]; then
        COPY2RAM="NO"
        good_msg "Not copying system to RAM"
    else
        bad_msg "Invalid Choice: Enter an option or nothing for the default."
        unset COPY2RAM
        ask_copy2ram
    fi
}

ask_persistent() {
    if [ -z "$PERSIST" ]; then
        PERSIST=$(ask "Use Persistent Storage? [y/N]")
    fi

    if [ "$PERSIST" == "y" ] || [ "$PERSIST" == "Y" ] || [ "$PERSIST" == "YES" ]; then
        PERSIST="YES"
        good_msg "Using Persistent Storage"
    elif [ "$PERSIST" == "" ] || [ "$PERSIST" == "n" ] || [ "$PERSIST" == "N" ] || [ "$PERSIST" == "NO" ]; then
        PERSIST="NO"
        good_msg "Not using Persistent Storage"
    else
        bad_msg "Invalid Choice: Enter an option or nothing for the default."
        unset PERSIST
        ask_persistent
    fi
}


set_persistent() {
    if [ "$PERSIST" == "NO" ]; then
        return
    fi
    plabel="Setting up Persistent Storage"
    proc_start "$plabel"

    # find persistent partition
    TRIES=50
    if [ $(echo ${PERSIST_LABEL:0:4}) == "lvm:" ]; then
        LVM_VG=$(echo $PERSIST_LABEL | cut -d: -f2)
        LVM_LV=$(echo $PERSIST_LABEL | cut -d: -f3)
        while [ $TRIES -gt 0 ] && sleep 0.1 && ! lvm.static vgchange -a y $LVM_VG &>/dev/null; do
            TRIES=$(($TRIES-1))
	done
        lvm.static vgscan --mknodes &>/dev/null
        PERSIST_PART="/dev/$LVM_VG/$LVM_LV"
    else
        # wait for device to show up
        while [ $TRIES -gt 0 ] && sleep 0.1 && ! PERSIST_PART=$(findfs LABEL=$PERSIST_LABEL); do
            TRIES=$(($TRIES-1))
        done
    fi
    if [ ! -e "$PERSIST_PART" ]; then
        if [ "$PERSIST" == "REQUIRE" ]; then
            bad_msg "Persistant Storage Device Required, Not Found"
            exec_shell
        fi
        proc_bad "$plabel" "Persistant Storage Not Found"
        PERSIST="NO"
        return
    fi

    # mount read-only to load configurations into system
    mkdir /mnt/ptmp
    mount -o ro $PERSIST_PART /mnt/ptmp
    if [ -d "/mnt/ptmp/.etc" ]; then
        cp -a "/mnt/ptmp/.etc/." "$NEW_ROOT/etc"
    fi
    umount /mnt/ptmp
    rm -rf /mnt/ptmp

    # add device into fstab and install update configuration services
    if [ -z $LVM_LV ]; then
        echo "LABEL=$PERSIST_LABEL /home ext4 noatime 0 2" >> "$NEW_ROOT/etc/fstab"
    else
        echo "$PERSIST_PART /home ext4 noatime 0 2" >> "$NEW_ROOT/etc/fstab"
        enable_service lvm
    fi

    # service to store account information to persistent storage
    enable_service ammit-etc

    proc_good "$plabel"
}


start_copy_system() {
    if [ "$COPY2RAM" == "NO" ]; then
        return
    fi

    plabel="Starting Copy to RAM"
    proc_start "$plabel"
    mount -t ramfs rootRO /mnt/ram
    cp /mnt/boot$BOOT_IMG /mnt/ram/ > /dev/null &
    while [ ! -e /mnt/ram/gentoo.sfs ]; do
        sleep 0.1
    done
    proc_good "$plabel"
}

wait_copy_system() {
    if [ "$COPY2RAM" == "NO" ]; then
        return
    fi

    plabel="Waiting for Copy to RAM"
    while [ $(stat -c %s /mnt/ram/gentoo.sfs) -lt $(stat -c %s /mnt/boot$BOOT_IMG) ]; do
        diff=$(( $(stat -c %s /mnt/ram/gentoo.sfs) * 100 / $(stat -c %s /mnt/boot$BOOT_IMG) ))
        proc_update "$plabel ${diff}%"
        sleep 0.25
    done
    sync; echo 3 > /proc/sys/vm/drop_caches
    proc_good "$plabel"
}

enable_service() {
    ln -sf "/etc/init.d/$1" "$NEW_ROOT/etc/runlevels/default/$1"
}

add_path() {
    if [ -f "$NEW_ROOT/etc/env.d/99ammit" ]; then
    	sed -i "\%^PATH=%s%$%:$1%" "$NEW_ROOT/etc/env.d/99ammit"
    else
        echo "PATH=$1" > "$NEW_ROOT/etc/env.d/99ammit"
    fi
    sed -i "/^export PATH='/s%'$%:$1'%" "$NEW_ROOT/etc/profile.env"
}

etc_setup() {
    sed -i "s/^hostname=\".*\"$/hostname=\"${HOSTNAME}\"/" "$NEW_ROOT/etc/conf.d/hostname"
    mkdir -p "$NEW_ROOT/etc/zsh"
    cp "/etc2/zsh/zshrc" "$NEW_ROOT/etc/zsh/zshrc"
    cp "/etc2/tmux.conf" "$NEW_ROOT/etc/tmux.conf"
    cp "/etc2/init.d/"* "$NEW_ROOT/etc/init.d/"
    cp "/sbin2/"* "$NEW_ROOT/sbin/"

    # enable rstudio server to start
    if [ -f "$NEW_ROOT/etc/conf.d/rstudio-server.conf" ]; then
        sed -i 's/^#www-port=80$/www-port=80/' "$NEW_ROOT/etc/conf.d/rstudio-server.conf"
    fi
    for s in $SERVICES; do
        if [ "${s:0:4}" == "net." ]; then
	    # create network service
	    ln -sf net.lo "$NEW_ROOT/etc/init.d/$s"
	fi
        enable_service $s
    done

    sed -i "s/^DISPLAYMANAGER=\".*\"$/DISPLAYMANAGER=\"sddm\"/" "$NEW_ROOT/etc/conf.d/display-manager"
    cat << EOF > "$NEW_ROOT/etc/sddm.conf"
[Autologin]
Relogin=false
Session=plasma
User=$STD_USER

[General]
HaltCommand=/sbin/shutdown -h -P now
RebootCommand=/sbin/shutdown -r now

[Theme]
Current=breeze
CursorTheme=breeze_cursors

[Users]
MaximumUid=60000
MinimumUid=1000
EOF

    # allow X11 forwarding by default, also optional machine specific ssh keys
    sed -i 's/^#X11Forwarding no$/X11Forwarding yes/' "$NEW_ROOT/etc/ssh/sshd_config"
    if [ ! -z "$MACHINE" ] && [ -d "${MACHINE}/etc" ]; then
        cp -a "${MACHINE}/etc/." "$NEW_ROOT/etc"
    fi

    # GMT manual install does not install PATH
    if [ -d "$NEW_ROOT/opt/gmt/bin" ]; then
	add_path "/opt/gmt/bin"
    fi
}
