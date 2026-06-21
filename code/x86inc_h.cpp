#include "common.h"

uint64_t x86inc_h(eflags &EFLAGS, uint64_t a) {
  uint64_t dst = 0;
#include "x86inc_h.h"
  return dst;
}

#define ref_x86inc_h(eflags, a)                                                \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst = 0;                                                          \
    asm volatile("x86mtflag %0, 0x3f\nx86inc.h %1\nx86mfflag %0, 0x3f"         \
                 : "+r"(flags)                                                 \
                 : "r"(a)                                                      \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ1(x86inc_h);
  IFUZZ1(x86inc_h);
  IFUZZ1(x86inc_h);
  IFUZZ1(x86inc_h);
  IFUZZ1(x86inc_h);
  IFUZZ1(x86inc_h);
}
