# !/bin/bash

if lsmod | grep -q "fireWall"; then
    echo "Firewall is already running."
else
    if ! [ -f fireWall.ko ]; then
	bash maker.sh
    fi
    if [ $# -eq 3 ]; then
        sudo insmod fireWall.ko param_var=$1,$2,$3
    else
        sudo insmod fireWall.ko param_var=0,0,0
    fi
fi
