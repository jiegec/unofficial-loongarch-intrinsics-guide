CC ?= gcc
CFLAGS ?= -O0 -g -Wall -Wextra
PROBE_SRCS := $(wildcard gen_probe_lbt_chunks/*.c)
PROBE_OBJS := $(PROBE_SRCS:.c=.o)
VERIFY_DEPS := lbt_verify.c gen_verify.c lbt_model.h

.PHONY: all lbt_verify lbt_probe lbt_verify_x86 run run-verify run-probe clean _lbt_verify_build _lbt_probe_build _lbt_verify_x86_build

all: lbt_verify

lbt_verify:
	python3 gen_lbt_verify.py
	$(MAKE) _lbt_verify_build

_lbt_verify_build: $(VERIFY_DEPS)
	$(CC) -std=gnu11 $(CFLAGS) lbt_verify.c -I. -o lbt_verify

lbt_verify_x86:
	python3 gen_lbt_verify.py --x86
	$(MAKE) _lbt_verify_x86_build

_lbt_verify_x86_build: gen_verify_x86.c
	$(CC) -std=gnu11 $(CFLAGS) gen_verify_x86.c -o lbt_verify_x86

lbt_verify_probe.o: $(VERIFY_DEPS) gen_probe_lbt.c
	$(CC) -std=gnu11 $(CFLAGS) -DLBT_WITH_PROBE -I. -c lbt_verify.c -o $@

gen_probe_lbt_chunks/%.o: gen_probe_lbt_chunks/%.c lbt_model.h
	$(CC) -std=gnu11 $(CFLAGS) -DLBT_WITH_PROBE -I. -c $< -o $@

lbt_probe:
	python3 gen_lbt_verify.py
	$(MAKE) _lbt_probe_build

_lbt_probe_build: lbt_verify_probe.o $(PROBE_OBJS)
	$(CC) $(CFLAGS) $^ -o lbt_probe

run: run-verify

run-verify: lbt_verify
	./lbt_verify

run-probe: lbt_probe
	./lbt_probe --probe

clean:
	rm -f lbt_verify lbt_probe lbt_verify_x86 _lbt_probe_build lbt_verify_probe.o $(PROBE_OBJS)
	rm -f gen_verify.c gen_probe_lbt.c gen_verify_x86.c
	rm -rf gen_probe_lbt_chunks
