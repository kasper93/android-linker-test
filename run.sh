#!/bin/sh -e

adb shell "mkdir -p /data/local/tmp/armeabi-v7a"
adb push libs/armeabi-v7a/* /data/local/tmp/armeabi-v7a

adb exec-out "cd /data/local/tmp/armeabi-v7a; LD_LIBRARY_PATH=. ./linker-test;"
