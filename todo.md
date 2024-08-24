# Clipboard
* Cannot paste into FFXIV or Rofi 

Fixed by `sleep 1; wl-paste | wl-copy`. So if something is copied while ffxiv is active, it works.

* Cannot paste if clipboard source is closed 

Same as with XOrg, but with XOrg pasting works if it was already pasted onced.
Likely this means that clipboard content is being sourced every time it is
accessed, and not just the first time. Figure out a way to buffer the clipboard
data immediately?

# Ickies

* Bar height resizes when certain characters are rendered: 🜏 Edmund McMillen 🜏
* Mouse stuck after exiting fullscreen (FFXIV) -- fixed by toggling fullscreen again
* Client borders restored after exiting fullscreen 

# Missing

* Keybind screenshot
* Toggle mouse focus
* "Game Mode": Put center window on top of bar, disable borders, force 2560x1440

# Features?

Ipc? More configurability without restarting compositor. -- Consider if the
benefits outweigh the added complexity? This will be used a lot early on, but
after landing on something nice it may not change for months. Still, the
inability to restart the compositor without losing clients is one of the main
advantages X WMs have over Wayland compositors.

IPC protocols to consider:
- Client placement
- Keybinds (grabkeys + rhkd?)
- What is the minimum set of features needed in the compositor?
- Waybar buttons for stuff. (gaps, setlayout, toggle mouse focus, mouse warp, mfact, ffact)

Auto-fire mode?
- Turn key repeats into release/tap

- wayland-scanner module to generate bash clients
-- events: subscribe and post to stdout
-- commands: simple bash functions with positionals
