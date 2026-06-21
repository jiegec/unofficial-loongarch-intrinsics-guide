#include "common.h"

uint64_t setx86loope(eflags &EFLAGS, uint64_t a) {
  uint64_t dst = 0;
#include "setx86loope.h"
  return dst;
}

#define ref_setx86loope(eflags, a)                                             \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst;                                                              \
    asm volatile("x86mtflag %1, 0x3f\nsetx86loope %0, %2\nx86mfflag %1, 0x3f"  \
                 : "=r"(dst), "+r"(flags)                                      \
                 : "r"(a)                                                      \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ1(setx86loope);
  IFUZZ1(setx86loope);
  IFUZZ1(setx86loope);
  IFUZZ1(setx86loope);
  IFUZZ1(setx86loope);
  IFUZZ1(setx86loope);
}
