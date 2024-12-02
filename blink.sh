#!/bin/bash
echo f40201 | xxd -r -p > /dev/ttyUSB0
while true; do echo f50200 | xxd -r -p > /dev/ttyUSB0 ; sleep 1; echo f50201 | xxd -r -p > /dev/ttyUSB0 ; sleep 1; done

