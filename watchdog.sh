#!/bin/bash

HOSTS="8.8.8.8"

# no ping request
COUNT=10


while true
do

if ps ax | grep disp | grep -vq grep
then
  echo ""
else
  /automedia/disp &
fi


for myHost in $HOSTS
do
  count=$(ping -c $COUNT $myHost | grep 'received' | awk -F',' '{ print $2 }' | awk '{ print $1 }')
  if [ $count -lt 8 ]; then
    # ie 70% failed
        poff internet
        sleep 2
        pon internet
	sleep 3
	killall mplayer
	killall mplayer.sh
	killall pingtest.sh
	/automedia/pingtest.sh
  fi
done
done