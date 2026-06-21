#include "common.h"

uint64_t x86rcli_d(eflags &EFLAGS, uint64_t a, int imm) {
  uint64_t dst = 0;
#include "x86rcli_d.h"
  return dst;
}

#define ref_x86rcli_d(eflags, a, imm)                                          \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\nx86rcli.d %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags)                                                 \
                 : "r"(a), "n"(imm)                                            \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ1(x86rcli_d, 0);
  IFUZZ1(x86rcli_d, 1);
  IFUZZ1(x86rcli_d, 7);
  IFUZZ1(x86rcli_d, 31);
}
