#!/bin/bash

xkb-switch -n
sleep 1
xsel -o | sudo openswitcher --device "$(input-device-info.sh)" --input
