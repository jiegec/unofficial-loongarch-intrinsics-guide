# Minimal pure-GPU runner for the LG200 PS-generic carrier.
#
#   make -j build/lgpu-gpu   build the runner (uses libdrm_loonggpu if found)
#   make -j matrix           dump mnemonic/materializable/form for all 768
#   make -j verify-gpu       run the PS-generic hardware matrix (run_gpu.py)
#
# Builds with libdrm_loonggpu when found.  Point LIBDRMDIR at an uninstalled
# libdrm build in the LoongGPU tree (host convenience), or leave it empty to
# use a system-installed libdrm_loonggpu.  A build without the DRM headers
# still compiles; the hardware path then reports "drm submit layer not built".
CXX ?= g++
CXXFLAGS ?= -std=c++20 -O1 -Wall -Wextra -Wno-unused-parameter -Wno-type-limits
INC := -I.

LIBDRMDIR ?= /home/lin/work/src/loonggpu/src/libdrm/build/meson-uninstalled

ifneq ($(LIBDRMDIR),)
LIBDRM_PC := $(wildcard $(LIBDRMDIR)/libdrm_loonggpu-uninstalled.pc)
ifneq ($(LIBDRM_PC),)
LGPU_CFLAGS := $(shell PKG_CONFIG_PATH="$(LIBDRMDIR)" pkg-config --cflags libdrm_loonggpu)
LGPU_LIBS  := $(shell PKG_CONFIG_PATH="$(LIBDRMDIR)" pkg-config --libs libdrm_loonggpu)
LGPU_RPATH := $(shell PKG_CONFIG_PATH="$(LIBDRMDIR)" pkg-config --libs-only-L libdrm_loonggpu | sed 's/^-L//' | sed 's|^|-Wl,-rpath,|')
LGPU_DEF   := -DLGPU_HAVE_LIBDRM -DLGPU_HAVE_DRM
endif
else
LGPU_CFLAGS := $(shell pkg-config --cflags libdrm_loonggpu 2>/dev/null)
ifneq ($(LGPU_CFLAGS),)
LGPU_LIBS  := $(shell pkg-config --libs libdrm_loonggpu 2>/dev/null)
LGPU_DEF   := -DLGPU_HAVE_LIBDRM -DLGPU_HAVE_DRM
endif
endif

SRCS := main.cpp envelope.cpp drm.cpp ps_packet.cpp codec/codec.cpp codec/operand.cpp codec/semantics.cpp
OBJS := $(patsubst %.cpp,build/%.o,$(SRCS))

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC) $(LGPU_CFLAGS) $(LGPU_DEF) -MMD -MP -c $< -o $@

build/lgpu-gpu: $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $(LGPU_CFLAGS) $(LGPU_DEF) $(OBJS) $(LGPU_LIBS) $(LGPU_RPATH) -o $@

matrix: build/lgpu-gpu
	./build/lgpu-gpu --matrix

verify-gpu: build/lgpu-gpu
	python3 run_gpu.py

clean:
	rm -rf build

-include $(OBJS:.o=.d)

.PHONY: matrix verify-gpu clean
