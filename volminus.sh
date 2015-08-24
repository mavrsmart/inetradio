#!/bin/bash

nums=`cat /automedia/oldvol.db`

echo "nutm=$nums"

let "nums=$nums-5"
if [ "$nums" -le 1 ];
 then
    nums=1
 fi
  amixer set PCM "$nums"
echo $nums>/automedia/oldvol.db

