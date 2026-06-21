#include "common.h"

uint64_t x86rotri_w(eflags &EFLAGS, uint64_t a, int imm) {
  uint64_t dst = 0;
#include "x86rotri_w.h"
  return dst;
}

#define ref_x86rotri_w(eflags, a, imm)                                         \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\nx86rotri.w %1, %2\nx86mfflag %0, 0x3f"   \
                 : "+r"(flags)                                                 \
                 : "r"(a), "n"(imm)                                            \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ1(x86rotri_w, 0);
  IFUZZ1(x86rotri_w, 1);
  IFUZZ1(x86rotri_w, 7);
  IFUZZ1(x86rotri_w, 31);
}
