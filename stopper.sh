#!/bin/bash

if lsmod | grep -q "fireWall"; then
    sudo rmmod fireWall
    last_occurrence=$(grep -n '§ ¶ I am become death, the destroyer of packets' /var/log/kern.log | tail -n 1 | cut -d: -f1)
    tail -n +$last_occurrence /var/log/kern.log | grep '§ ¶' > packets.log
    cat ./packets.log | grep '§ ¶' | grep 'DROP'  > "dropped.log"
fi

