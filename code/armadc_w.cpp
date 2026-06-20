#include "common.h"

uint64_t armadc_w(eflags &ARMFLAGS, uint64_t a, uint64_t b, int cond) {
  uint64_t dst = 0;
#include "armadc_w.h"
  return dst;
}

#define ref_armadc_w(eflags, a, b, cond)                                       \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst;                                                              \
    asm volatile("x86mtflag %0, 0x3f\narmadc.w %1, %2, %3\nx86mfflag %0, 0x3f" \
                 : "+r"(flags)                                                 \
                 : "r"(a), "r"(b), "n"(cond)                                   \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ2(armadc_w, 0);
  IFUZZ2(armadc_w, 14);
  IFUZZ2(armadc_w, 15);
  IFUZZ2(armadc_w, 7);
}
