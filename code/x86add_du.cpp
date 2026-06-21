#include "common.h"

uint64_t x86add_du(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint64_t dst = 0;
#include "x86add_du.h"
  return dst;
}

#define ref_x86add_du(eflags, a, b)                                            \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile("x86mtflag %0, 0x3f\nx86add.du %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags)                                                 \
                 : "r"(a), "r"(b)                                              \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ2(x86add_du);
  IFUZZ2(x86add_du);
  IFUZZ2(x86add_du);
  IFUZZ2(x86add_du);
}
