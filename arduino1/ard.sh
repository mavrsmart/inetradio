#!/bin/bash

#setserial /dev/ttyUSB0 baud_base 38400
#stty 9600 -F /dev/ttyUSB0 

do_log() {

 while true
 do
  IFS=';' read -a arr
  if [ ! -z "$arr" ]; then
   
   echo "${arr[@]}";
#   echo "${arr[1]}";

#    IFS=';' read -a array <<< "$REPLY"

    echo ${arr[0]} > /tmp/ard1/gps_lat
    echo ${arr[1]} > /tmp/ard1/gps_lon
    echo ${arr[2]} > /tmp/ard1/gps_time
    echo ${arr[3]} > /tmp/ard1/gps_date
    echo ${arr[4]} > /tmp/ard1/gps_qty
    echo ${arr[5]} > /tmp/ard1/gps_sat
    echo ${arr[6]} > /tmp/ard1/gps_speed
    echo ${arr[7]} > /tmp/ard1/inp_speed
    echo ${arr[8]} > /tmp/ard1/temp
    echo ${arr[9]} > /tmp/ard1/klap_temp
    echo ${arr[10]} > /tmp/ard1/max_klap_temp
    echo ${arr[11]} > /tmp/ard1/pwm_klap_temp

   fi
 done
}


mkdir /tmp/ard1


stty -F /dev/ttyARD raw ispeed 9600 ospeed 9600 -echo
#stty 19200 -F /dev/ttyARD
#setserial /dev/ttyARD baud_base 19200

while true; do

 cat /dev/ttyARD | do_log

done