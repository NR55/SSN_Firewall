#!/bin/bash

if lsmod | grep -q "fireWall"; then
    sudo rmmod fireWall
fi
if [ -f fireWall.ko ]; then
    rm -f fireWall.ko
fi
cd Builder
if [ -f fireWall.ko ]; then
    rm -f fireWall.ko
fi
make
cp fireWall.ko ../fireWall.ko
