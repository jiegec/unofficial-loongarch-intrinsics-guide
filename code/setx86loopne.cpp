#include "common.h"

uint64_t setx86loopne(eflags &EFLAGS, uint64_t a) {
  uint64_t dst = 0;
#include "setx86loopne.h"
  return dst;
}

#define ref_setx86loopne(eflags, a)                                             \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst;                                                              \
    asm volatile("x86mtflag %1, 0x3f\nsetx86loopne %0, %2\nx86mfflag %1, 0x3f"  \
                 : "=r"(dst), "+r"(flags)                                      \
                 : "r"(a)                                                      \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ1(setx86loopne);
  IFUZZ1(setx86loopne);
  IFUZZ1(setx86loopne);
  IFUZZ1(setx86loopne);
  IFUZZ1(setx86loopne);
  IFUZZ1(setx86loopne);
}
