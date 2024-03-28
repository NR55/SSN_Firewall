# !/bin/bash

if lsmod | grep -q "fireWall"; then
    echo "Firewall is already running."
else
    if ! [ -f fireWall.ko ]; then
	    bash maker.sh
    fi
    if [ $# -eq 1 ]; then
        sudo insmod fireWall.ko param_var=$1
    else
        sudo insmod fireWall.ko param_var="255."
    fi
fi
