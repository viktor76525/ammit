#!/bin/busybox bb
#
# bad hosts updater
#

status=1

while [ $status -gt 0 ]; do
    status=0

    while ! ping -c 1 -q raw.githubusercontent.com >&/dev/null; do
        sleep 20
    done

    if [ ! -f "/etc/hosts.backup" ]; then
        cp "/etc/hosts" "/etc/hosts.backup"
    fi

    wget -q https://raw.githubusercontent.com/Ultimate-Hosts-Blacklist/Ultimate.Hosts.Blacklist/master/hosts/hosts1 -O /etc/hosts.block
    status=$(($status+$?))
    wget -q https://raw.githubusercontent.com/Ultimate-Hosts-Blacklist/Ultimate.Hosts.Blacklist/master/hosts.deny/hosts0.deny -O /etc/hosts.deny
    status=$(($status+$?))

    if [ $status -gt 0 ]; then
        sleep 20
        continue
    fi

    cp "/etc/hosts.backup" "/etc/hosts"
    cat "/etc/hosts.block" >> "/etc/hosts"

    sed -i 's/^multi on$/multi off/' "/etc/host.conf"
done
