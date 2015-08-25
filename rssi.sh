#!/bin/bash

  mk /tmp/rssi
    while true
    do
	/automedia/rssi.py signal > /tmp/rssi
	sleep 5
    done