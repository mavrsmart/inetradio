#!/bin/bash

service hostapd restart &

while true
do

sleep 5

if ps ax | grep watchdog.sh | grep -vq grep
then
  sleep 5
else
  /automedia/watchdog.sh &
fi

done