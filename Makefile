.POSIX:
.SUFFIXES:

out/debug:
	meson setup out/debug
debug: out/debug
	ninja -C out/debug
out/release:
	meson setup --buildtype=release out/release
release: out/release
	ninja -C out/release

dist: out/release
	ninja -C out/release dist
