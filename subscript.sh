#!/bin/bash
x=1
while true;
do
	mosquitto_sub -t fusion/ir -C 1 > output.b64
	sleep 1
	base64 -d output.b64 > images/image${x}.bmp
	x=$(( $x + 1 ))
done