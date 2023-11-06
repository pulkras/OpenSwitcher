#! /usr/bin/env bash

# Somewhat stolen from: <https://unix.stackexchange.com/a/507209>
# Problem: multiple devices with the same name
find_event() {
    local lnNo handlers
    local INFO_FILE='/proc/bus/input/devices'
    lnNo=$(grep -n 'N: Name=' $INFO_FILE | grep "$1" | head -n1 | cut -d: -f1)
    handlers="$(tail +"$lnNo" $INFO_FILE | grep 'H: Handlers=' | head -n1 | cut -d= -f2)"

    [ -z "$handlers" ] && return 1

    for handler in $handlers; do
        case $handler in event*)
            echo "/dev/input/$handler"
            return 0
        esac
    done

    return 1
}

find_event "keyboard"
