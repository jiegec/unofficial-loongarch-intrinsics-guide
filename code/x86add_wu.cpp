#include "common.h"

uint64_t x86add_wu(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint64_t dst = 0;
#include "x86add_wu.h"
  return dst;
}

#define ref_x86add_wu(eflags, a, b)                                            \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\nx86add.wu %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags)                                                 \
                 : "r"(a), "r"(b)                                              \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ2(x86add_wu);
  IFUZZ2(x86add_wu);
  IFUZZ2(x86add_wu);
  IFUZZ2(x86add_wu);
}
