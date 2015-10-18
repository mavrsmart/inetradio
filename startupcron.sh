#!/bin/bash

service hostapd restart &

while true
do

sleep 5

if ps ax | grep watchdog.sh | grep -vq grep
then
  sleep 1
else
  /automedia/watchdog.sh &
fi

if ps ax | grep ard.sh | grep -vq grep
then
  sleep 1
else
  /automedia/arduino1/ard.sh &
fi

done