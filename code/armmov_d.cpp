#include "common.h"

uint64_t armmov_d(eflags &ARMFLAGS, uint64_t a, int cond) {
  uint64_t dst = 0;
#include "armmov_d.h"
  return dst;
}

#define ref_armmov_d(eflags, a, cond)                                       \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\narmmov.d %1, %2\nx86mfflag %0, 0x3f" \
                 : "+r"(flags)                                                 \
                 : "r"(a), "n"(cond)                                   \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ1(armmov_d, 0);
  IFUZZ1(armmov_d, 7);
  IFUZZ1(armmov_d, 14);
  IFUZZ1(armmov_d, 15);
}
