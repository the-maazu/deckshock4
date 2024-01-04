#!/bin/bash

cp deckshock4 ~/.local/bin/
sudo cp -r etc /
sudo udevadm control --reload
sudo udevadm trigger
