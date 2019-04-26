#!/bin/sh

msg="command:create\tname:lukas\t\n"

while true
do
    while true; do echo "$msg"; sleep 1; done | nc -l 8081
done