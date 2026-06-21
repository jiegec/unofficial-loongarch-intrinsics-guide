#include "common.h"

uint64_t x86srli_b(eflags &EFLAGS, uint64_t a, int imm) {
  uint64_t dst = 0;
#include "x86srli_b.h"
  return dst;
}

#define ref_x86srli_b(eflags, a, imm)                                          \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\nx86srli.b %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags)                                                 \
                 : "r"(a), "n"(imm)                                            \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ1(x86srli_b, 0);
  IFUZZ1(x86srli_b, 1);
  IFUZZ1(x86srli_b, 7);
}
