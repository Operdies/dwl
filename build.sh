#!/bin/env sh

case "$1" in 
  debug)
    [ -d build ] || meson setup build
    ninja -C build
    ;;
  release)
    [ -d release ] || meson setup --buildtype=release release
    ninja -C release
    ;;
  dist)
    [ -d release ] || meson setup --buildtype=release release
    ninja -C release dist
    ;;
  *)
    echo "Unknown build target $@"
    ;;
esac
