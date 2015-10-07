#!/bin/bash


while true
do

pon internet &

if ps ax | grep disp | grep -vq grep
then
  echo ""
else
  /automedia/disp &
fi

if ps ax | grep mikas | grep -vq grep
then
  echo ""
else
  /automedia/mikas/mikas &
fi


  sleep 1

  count=$(ping -c 8 8.8.8.8 | grep 'received' | awk -F',' '{ print $2 }' | awk '{ print $1 }')
  echo "Received (8):" $count
  if [ "$count" != "" ]; then

  if [ $count -lt 5 ]; then
    # ie 70% failed
        echo "Poor network. Restart"
        poff internet
        pon internet &
	sleep 30
	killall mplayer
	killall mplayer.sh
	killall pingtest.sh
	/automedia/pingtest.sh &
  fi
else
	echo "Not network. Restart"
        poff internet
        pon internet &
	sleep 30
	killall mplayer
	killall mplayer.sh
	killall pingtest.sh
	/automedia/pingtest.sh &

fi

done