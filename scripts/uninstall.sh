#!/bin/bash

rm ~/.local/bin/deckshock4
sudo rm /etc/udev/rules.d/{99-ds4tm.rules,99-uhidmod.rules}
sudo rm /etc/modules-load.d/ds4.conf