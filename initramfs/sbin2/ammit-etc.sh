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
    while inotifywait -qqr -e modify,attrib,create,delete,move_self,delete_self "/etc/$1"; do
        # something changed, update
        delayed_update "$1" &
        sleep 0.1
    done
}

nproc() {
    jobs -p | wc -l
}

for item in /home/.etc/*; do
    watch_target "$(basename "$item")" &
done

while [ $(nproc) -gt 1 ]; do
    sleep 3600
done
