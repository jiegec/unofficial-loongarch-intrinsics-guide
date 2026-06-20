#include "common.h"

uint64_t armrrx_w(eflags &ARMFLAGS, uint64_t a, int cond) {
  uint64_t dst = 0;
#include "armrrx_w.h"
  return dst;
}

#define ref_armrrx_w(eflags, a, cond)                                          \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\narmrrx.w %1, %2\nx86mfflag %0, 0x3f"     \
                 : "+r"(flags)                                                 \
                 : "r"(a), "n"(cond)                                           \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ1(armrrx_w, 0);
  IFUZZ1(armrrx_w, 7);
  IFUZZ1(armrrx_w, 14);
  IFUZZ1(armrrx_w, 15);
}
