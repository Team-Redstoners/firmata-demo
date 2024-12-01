#!/bin/bash

bash build.sh
./build/main build/FAKE_SERIAL_DEVICE.bin
hexdump -C build/FAKE_SERIAL_DEVICE.bin
