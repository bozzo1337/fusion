#!/bin/bash
watch -n 10 'thermalCam/thermalCam ; mosquitto_pub -h 192.168.0.22 -p 1883 -t '"'"'fusion/ir'"'"' -f '"'"'thermalCam/testIR.bmp'"'"