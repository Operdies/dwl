project(
	'dwl',
	'c',
	version: '0.8-dev',
	license: 'GNU GPL',
	meson_version: '>=1.3',
	default_options: [
		'c_std=c11',
		'warning_level=2',
		'werror=true',
	],
)

add_project_arguments(
	[
		'-DWLR_USE_UNSTABLE',
		'-D_POSIX_C_SOURCE=200809L',
		'-DXWAYLAND',

		'-Wno-unused-parameter',
		'-Wno-unused-result',
		'-Wno-missing-braces',
		'-Wno-format-zero-length',
		'-Wundef',
		'-Wvla',
	],
	language: 'c',
)

if get_option('buildtype').startswith('release')
    add_project_arguments('-DMODKEY=WLR_MODIFIER_LOGO', language : ['c'])
else
    add_project_arguments('-DMODKEY=WLR_MODIFIER_ALT', language : ['c'])
endif

cc = meson.get_compiler('c')

# Execute the wlroots subproject, if any
wlroots_version = ['>=0.18.0', '<=0.19.0-dev']
subproject(
	'wlroots',
	default_options: ['examples=false'],
	required: false,
	version: wlroots_version,
)
wlroots = dependency('wlroots-0.19', version: wlroots_version, fallback: 'wlroots')
wlroots_features = {
	'xwayland': false,
	'libinput_backend': false,
	'session': false,
}
foreach name, _ : wlroots_features
	var_name = 'have_' + name.underscorify()
	have = wlroots.get_variable(pkgconfig: var_name, internal: var_name) == 'true'
	wlroots_features += { name: have }
endforeach

null_dep = dependency('', required: false)

wayland_server = dependency('wayland-server', version: '>=1.21.0')
wayland_client = dependency('wayland-client')
wayland_cursor = dependency('wayland-cursor')
wayland_protos = dependency('wayland-protocols', version: '>=1.24', default_options: ['tests=false'])
xkbcommon = dependency('xkbcommon', version: '>=1.5.0')
libinput = wlroots_features['libinput_backend'] ? dependency('libinput', version: '>=1.26.0') : null_dep
xcb = wlroots_features['xwayland'] ? dependency('xcb') : null_dep
xcb_icccm = wlroots_features['xwayland'] ? dependency('xcb-icccm') : null_dep
math = cc.find_library('m')

version = '"@0@"'.format(meson.project_version())
add_project_arguments('-DVERSION=@0@'.format(version), language: 'c')

subdir('protocols')
subdir('dwl')

