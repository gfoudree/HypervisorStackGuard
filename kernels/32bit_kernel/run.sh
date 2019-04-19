#!/bin/bash

docker run --rm -v "$PWD":/tmp/myos -w /tmp/myos devenv make

qemu-system-x86_64 -enable-kvm -kernel kernel.bin
