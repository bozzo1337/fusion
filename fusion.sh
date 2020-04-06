#!/bin/bash
watch -n 10 'thermalCam/thermalCam ; base64 thermalCam/testIR.bmp > thermalCam/testIR.b64 ; mosquitto_pub -h 192.168.0.22 -p 1883 -t '"'"'fusion/ir'"'"' -f '"'"'thermalCam/testIR.b64'"'"