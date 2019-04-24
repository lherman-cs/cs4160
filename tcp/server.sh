#!/bin/sh

msg="command:create\tname:lukas\t\n"

while true
do
    echo "$msg" | nc -l 8080
done