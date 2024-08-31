deps = [ 
  libinput,
  math,
  wayland_client,
  wayland_cursor,
  wayland_protos,
  wayland_server,
  wlroots,
  xcb,
  xcb_icccm,
  xkbcommon,
]

sources = [ 'dwl.c', 'util.c' ]

executable(
  'dwl',
  sources + wl_protos_src,
  dependencies: deps,
)
