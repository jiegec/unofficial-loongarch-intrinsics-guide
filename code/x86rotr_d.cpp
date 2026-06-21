#include "common.h"

uint64_t x86rotr_d(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint64_t dst = 0;
#include "x86rotr_d.h"
  return dst;
}

#define ref_x86rotr_d(eflags, a, b)                                            \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\nx86rotr.d %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags)                                                 \
                 : "r"(a), "r"(b)                                              \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ2(x86rotr_d);
  IFUZZ2(x86rotr_d);
  IFUZZ2(x86rotr_d);
  IFUZZ2(x86rotr_d);
}
