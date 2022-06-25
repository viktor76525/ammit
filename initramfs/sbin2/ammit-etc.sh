#!/bin/busybox bb
#
# etc subset updater
#

lock=/dev/shm/$(basename "$0").lock

delayed_update() {
    while [ -e "$lock" ]; do
        sleep 1
    done
    touch "$lock"
    sleep 1
    echo updating "$1"
    rsync -a --delete "/etc/$1" "/home/.etc/"
    rm "$lock"
}

watch_target() {
    while inotifywait -qq -e modify,attrib,create,delete,move_self,delete_self "/etc/$1"; do
        # something changed, update
        delayed_update "$1" &
    done
}

for item in /home/.etc/*; do
    watch_target "$(basename "$item")" &
done
