wl_protocol_dir = wayland_protos.get_variable('pkgdatadir')

wayland_scanner_dep = dependency('wayland-scanner', native: true)
wayland_scanner = find_program(
	wayland_scanner_dep.get_variable('wayland_scanner'),
	native: true,
)

client_header_protocols = [
  'dwl-ipc-unstable-v2.xml',
]

server_header_protocols = [
	wl_protocol_dir / 'stable/xdg-shell/xdg-shell.xml',
  'dwl-ipc-unstable-v2.xml',
  'wlr-output-power-management-unstable-v1.xml',
]

enum_header_protocols = [
	wl_protocol_dir / 'staging/cursor-shape/cursor-shape-v1.xml',
	wl_protocol_dir / 'unstable/pointer-constraints/pointer-constraints-unstable-v1.xml',
  'wlr-layer-shell-unstable-v1.xml',
]

private_code_protocols = [
  'dwl-ipc-unstable-v2.xml',
]

wl_protos_src = []

foreach xml : client_header_protocols
	wl_protos_src += custom_target(
		xml.underscorify() + '_client_h',
		input: xml,
		output: '@BASENAME@-client-protocol.h',
		command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'],
	)
endforeach

foreach xml : server_header_protocols
	wl_protos_src += custom_target(
		xml.underscorify() + '_server_h',
		input: xml,
		output: '@BASENAME@-protocol.h',
		command: [wayland_scanner, 'server-header', '@INPUT@', '@OUTPUT@'],
	)
endforeach

foreach xml : enum_header_protocols
	wl_protos_src += custom_target(
		xml.underscorify() + '_h',
		input: xml,
		output: '@BASENAME@-protocol.h',
		command: [wayland_scanner, 'enum-header', '@INPUT@', '@OUTPUT@'],
	)
endforeach

foreach xml : private_code_protocols
	wl_protos_src += custom_target(
		xml.underscorify() + '_c',
		input: xml,
		output: '@BASENAME@-protocol.c',
		command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@'],
	)
endforeach
