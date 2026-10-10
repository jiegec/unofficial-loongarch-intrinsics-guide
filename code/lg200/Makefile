# KISS build + verify for the LG200 oracle suite in this directory.
#
#   make -j verify      compile every oracle and run every case vector
#   make clean
#
# Compilation unit layout:
#   <mnemonic>.cpp   oracle (LG200_ORACLE_*) + case vectors (LG200_CASE_*)
#   <mnemonic>.h     executable reference pseudocode, included by the oracle
#   ops/*            the oracle library (oracle, pseudo vocabulary, softfloat,
#                    and the committed registry mapping mnemonic -> oracle)
#   main.cpp         KISS stdin runner used by run_cases.py
CC ?= gcc
CXX ?= g++
CFLAGS ?= -O2 -Wall -Wno-unused-function
CXXFLAGS ?= -std=c++20 -O1 -Wall -Wextra -Wno-unused-parameter -Wno-type-limits
INC := -I. -I ops/berkeley-softfloat-3/source \
      -I ops/berkeley-softfloat-3/source/include -I ops/berkeley-softfloat-3/source/8086-SSE
LIBM := -lm

BERK_DIR := ops/berkeley-softfloat-3/source

CASE_SRCS := $(filter-out main.cpp,$(wildcard *.cpp))
# Berkeley SoftFloat: only the f16/f32/f64 families are needed; skip the
# bf16, extF80 and f128 extensions (and their memory-form variants).
BERK_ALL  := $(wildcard ops/berkeley-softfloat-3/source/*.c)
# f16/f32/f64 only: drop bf16, extF80, f128 and the multiword (M) forms
# (case-insensitive, since the library mixes bf16/BF16 extF80/ExtF80 cases)
BERK_KEEP := $(shell printf '%s\n' $(BERK_ALL) | grep -viE 'bf16|extf80|f128|M\.c$$')
# platform NaN-propagation/exception sources (single-threaded x86-64 flavor)
BERK_PLAT := $(shell printf '%s\n' $(wildcard ops/berkeley-softfloat-3/source/8086-SSE/*.c) | grep -viE 'bf16|extf80|f128')
OPS_SRCS  := $(wildcard ops/*.cpp) $(BERK_KEEP) $(BERK_PLAT)
OBJS      := $(patsubst %,build/%.o,$(CASE_SRCS) $(OPS_SRCS))

build/%.o: %
	@mkdir -p $(dir $@)
	$(if $(filter %.c,$<),$(CC) $(CFLAGS) $(INC),$(CXX) $(CXXFLAGS) $(INC)) -MMD -MP -c $< -o $@

build/lgpu-cases: $(OBJS) main.cpp
	$(CXX) $(CXXFLAGS) $(INC) main.cpp $(OBJS) $(LIBM) -o $@

verify: build/lgpu-cases
	python3 run_cases.py

# Regenerate the exhaustive boundary rows (gen_cases.py -> oracle --fill).
gen-cases: build/lgpu-cases
	python3 gen_cases.py | ./build/lgpu-cases --fill > generated_cases.tsv


clean:
	rm -rf build

-include $(OBJS:.o=.d)

.PHONY: verify gen-cases clean
