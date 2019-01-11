VERBOSE ?=
MXE_ROOT ?= /opt/devel/mxe/mxe

define build_cross
	-rm -fr build-$(2)
	mkdir build-$(2)
	cd build-$(2) && \
		cmake \
			-D CMAKE_TOOLCHAIN_FILE=cmake/$(1)-$(2).cmake \
			..
	make -j$$((nproc+1)) VERBOSE=$(VERBOSE) -C build-$(2)
endef

define build_mxe
	-rm -fr build-$(1)
	mkdir build-$(1)
	cd build-$(1) && $(MXE_ROOT)/usr/bin/$(1)-cmake ..
	make -j$$((nproc+1)) VERBOSE=$(VERBOSE) -C build-$(1)
endef

help:
	@echo -e '\n Try `make <all|x86|x86_64-w64-mingw32.shared|imx6|ar71xx>`\n'

all: x86 imx6 ar71xx x86_64-w64-mingw32.shared i686-w64-mingw32.shared

x86:
	-rm -fr build-x86
	mkdir build-x86
	cd build-x86 && CC=gcc-8 cmake ..
	make -j$$((nproc+1)) VERBOSE=$(VERBOSE) -C build-x86

x86_64-w64-mingw32.shared:
	$(call build_mxe,$@)

i686-w64-mingw32.shared:
	$(call build_mxe,$@)

imx6:
	$(call build_cross,openwrt-toolchain,$@)

ar71xx:
	$(call build_cross,openwrt-toolchain,$@)

clean:
	@-rm -fr build-*
