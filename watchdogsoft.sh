#!/bin/bash


while true
do

if ps ax | grep disp1602 | grep -vq grep
then
  echo ""
else
  /automedia/disp1602 &
fi

if ps ax | grep mikas | grep -vq grep
then
  echo ""
else
  /automedia/mikas/mikas &
fi


  sleep 1
done