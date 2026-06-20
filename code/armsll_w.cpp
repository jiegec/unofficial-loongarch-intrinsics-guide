#include "common.h"

uint64_t armsll_w(eflags &ARMFLAGS, uint64_t a, uint64_t b, int cond) {
  uint64_t dst = 0;
#include "armsll_w.h"
  return dst;
}

#define ref_armsll_w(eflags, a, b, cond)                                       \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\narmsll.w %1, %2, %3\nx86mfflag %0, 0x3f" \
                 : "+r"(flags)                                                 \
                 : "r"(a), "r"(b), "n"(cond)                                   \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ2(armsll_w, 0);
  IFUZZ2(armsll_w, 7);
  IFUZZ2(armsll_w, 14);
  IFUZZ2(armsll_w, 15);
}
