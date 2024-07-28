#!/bin/env bash

# Fix various java issues. Taken from https://wiki.gentoo.org/wiki/Dwm#Fix_Java_application_misbehaving
# Primarily for ghidra and jebtrains IDEs
export _JAVA_AWT_WM_NONREPARENTING=1
export AWT_TOOLKIT=MToolkit

# Create a dbus session. This is primarily needed for kwallet
export $(dbus-launch)

start_sway_idle() {
  session_lock=120
  suspend=300
  swayidle -w \
    timeout $session_lock 'swaylock -f' \
    timeout $suspend 'systemctl suspend' \
    before-sleep 'pgrep -x swaylock || swaylock -f'
}

# We are doing things in a round-about way here because
# these applications will fail to start if the Wayland compositor is not
# already running when we start them.
do_startup() {
  wmname LG3D # java fix v2 -- needs to run after the compository has started
  waybar &
  swaybg -i ~/.wallpapers/current.png -m fill &
  pgrep -x mako || mako
  kwalletd6 &
  start_sway_idle &
  dex /etc/xdg/autostart/xfce-polkit.desktop
}

case "$1" in
  startup) 
    do_startup
    ;;
  *) 
    "$HOME/repos/dwl/dwl" -s "$0 startup"
    ;;
esac
