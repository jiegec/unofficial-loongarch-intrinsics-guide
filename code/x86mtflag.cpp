#include "common.h"

uint64_t x86mtflag(eflags &EFLAGS, uint64_t a, uint64_t mask) {
  uint64_t dst = 0;
#include "x86mtflag.h"
  return dst;
}

#define ref_x86mtflag(eflags, a, mask)                                         \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst = 0;                                                          \
    asm volatile("x86mtflag %0, 0x3f\nx86mtflag %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags)                                                 \
                 : "r"(a), "n"(mask)                                           \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ1(x86mtflag, 0);
  IFUZZ1(x86mtflag, 7);
  IFUZZ1(x86mtflag, 14);
  IFUZZ1(x86mtflag, 15);
  IFUZZ1(x86mtflag, 63);
}
