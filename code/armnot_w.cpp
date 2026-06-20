#include "common.h"

uint64_t armnot_w(eflags &ARMFLAGS, uint64_t a, int cond) {
  uint64_t dst = 0;
#include "armnot_w.h"
  return dst;
}

#define ref_armnot_w(eflags, a, cond)                                          \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\narmnot.w %1, %2\nx86mfflag %0, 0x3f"     \
                 : "+r"(flags)                                                 \
                 : "r"(a), "n"(cond)                                           \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ1(armnot_w, 0);
  IFUZZ1(armnot_w, 7);
  IFUZZ1(armnot_w, 14);
  IFUZZ1(armnot_w, 15);
}
