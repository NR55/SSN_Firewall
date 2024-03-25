#!/bin/bash

if lsmod | grep -q "fireWall"; then
    echo "Firewall is ACTIVE."
else
    echo "Firewall is INACTIVE."
fi

