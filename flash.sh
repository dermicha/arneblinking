#!/bin/bash

particle compile photon --target 0.6.3 ./ --saveTo ./arne.bin
particle flash --usb ./arne.bin
