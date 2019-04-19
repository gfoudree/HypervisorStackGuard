#!/bin/bash

docker run --rm -v "$PWD":/tmp/myos -w /tmp/myos devenv make
