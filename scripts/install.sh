#!/bin/bash

mkdir -p ~/.local/bin/ && cp deckshock4 ~/.local/bin/
sudo cp -r etc /
sudo udevadm control --reload
sudo udevadm trigger
sudo modprobe uhid
