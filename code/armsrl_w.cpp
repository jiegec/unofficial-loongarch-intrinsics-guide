#include "common.h"

uint64_t armsrl_w(eflags &ARMFLAGS, uint64_t a, uint64_t b, int cond) {
  uint64_t dst = 0;
#include "armsrl_w.h"
  return dst;
}

#define ref_armsrl_w(eflags, a, b, cond)                                       \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\narmsrl.w %1, %2, %3\nx86mfflag %0, 0x3f" \
                 : "+r"(flags)                                                 \
                 : "r"(a), "r"(b), "n"(cond)                                   \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ2(armsrl_w, 0);
  IFUZZ2(armsrl_w, 7);
  IFUZZ2(armsrl_w, 14);
  IFUZZ2(armsrl_w, 15);
}
