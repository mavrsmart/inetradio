#!/bin/bash

while true; do
result=$(ping -c 2 8.8.8.8 | tail -2 | head -1 | awk '{print $4}')

if [ "$result" != "" ]
then
if [ $result -eq 0 ]
 then
#echo "0"
echo "0" > /tmp/pingtest
#break
else
#echo "1"
echo "1" > /tmp/pingtest
fi

else
echo "0" > /tmp/pingtest

fi

#sleep 2
done

exit 0