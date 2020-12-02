#!/bin/busybox bb
#
# passwd, group, shadow updater
#

while inotifywait -e attrib -qq /etc/passwd /etc/shadow /etc/group; do
    # user account information changed, update
    # avoid race conditions, try 3 times
    for i in 1 2 3; do
        for usr in "passwd" "shadow" "group"; do
            if [ "/etc/${usr}" -nt "/home/.etc/${usr}" ] || [ ! -s "/home/.etc/${usr}" ]; then
                cp "/etc/${usr}" "/home/.etc/${usr}"
            fi
        done
	if [ $i -lt 3 ]; then
            sleep 1
	fi
    done
done
