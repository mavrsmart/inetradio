#!/bin/bash

nums=`cat /automedia/oldvol.db`

echo "nutm=$nums"

let "nums=5+$nums"
if [ "$nums" -ge 152 ];
 then
    nums=151
 fi
  amixer set PCM "$nums"
echo $nums>/automedia/oldvol.db

