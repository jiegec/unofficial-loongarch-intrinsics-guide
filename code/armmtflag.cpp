#include "common.h"

uint64_t armmtflag(eflags &ARMFLAGS, uint64_t a, uint64_t mask) {
  uint64_t dst = 0;
#include "armmtflag.h"
  return dst;
}

#define ref_armmtflag(eflags, a, mask)                                         \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst = 0;                                                          \
    asm volatile("x86mtflag %0, 0x3f\narmmtflag %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags)                                                 \
                 : "r"(a), "n"(mask)                                           \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ1(armmtflag, 0);
  IFUZZ1(armmtflag, 7);
  IFUZZ1(armmtflag, 14);
  IFUZZ1(armmtflag, 15);
  IFUZZ1(armmtflag, 0x3f);
}
