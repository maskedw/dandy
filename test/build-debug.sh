#!/bin/bash -e
##################################################
# Generated by xmbedinit(https://github.com/maskedw/xmbedinit)
##################################################
thisdir="$(dirname "${BASH_SOURCE:-$0}")"
cd "$thisdir"
BUILD_TYPE=Debug ./build.sh "$@"