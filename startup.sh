#!/bin/env bash

start_sway_idle() {
  session_lock=600
  suspend=1800
  swayidle -w \
    timeout $session_lock 'swaylock -f' \
    timeout $suspend 'systemctl suspend' \
    before-sleep 'pgrep -x swaylock || swaylock -f'
}

# We are doing things in a round-about way here because
# these applications will fail to start if the Wayland compositor is not
# already running when we start them.
do_startup() {
  dex /etc/xdg/autostart/xfce-polkit.desktop &
  waybar &
  swaybg -i ~/.wallpapers/current.png -m fill &
  pgrep -x mako || mako &
  start_sway_idle &
  wmname LG3D & # java fix v2 -- needs to run after the compository has started
  kwalletd6 &
  # clipboard data is synchronized directly between wayland clients.
  # To persist the clipboard when an application is closed, we use a clipboard manager.
  wl-paste -t text --watch clipman store & 
  "$HOME/repos/dwl/release/dwl_yoink/yoink" &
}

do_launch() {
  # Fix various java issues. Taken from https://wiki.gentoo.org/wiki/Dwm#Fix_Java_application_misbehaving
  # Primarily for ghidra and jebtrains IDEs
  export _JAVA_AWT_WM_NONREPARENTING=1
  export AWT_TOOLKIT=MToolkit
  export XDG_CURRENT_DESKTOP=dwl
  # Create a dbus session. This is primarily needed for kwallet
  export $(dbus-launch)
  [ -x "$HOME/repos/dwl/release/dwl/dwl" ] || cd ~/repos/dwl && make release
  "$HOME/repos/dwl/release/dwl/dwl" -s "$0 startup"
}

# {
#   dex /etc/xdg/autostart/xfce-polkit.desktop
#   waybar &
#   swaybg -i ~/.wallpapers/current.png -m fill &
#   pgrep -x mako || mako &
#   start_sway_idle &
#   wmname LG3D # java fix v2 -- needs to run after the compository has started
#   kwalletd6 &
# } &

case "$1" in
  startup) 
    do_startup &
    ;;
  *) 
    do_launch
    ;;
esac
