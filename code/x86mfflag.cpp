#include "common.h"

uint64_t x86mfflag(eflags &EFLAGS, uint64_t mask) {
  uint64_t dst = 0;
#include "x86mfflag.h"
  return dst;
}

#define ref_x86mfflag(eflags, mask)                                            \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst = 0;                                                          \
    asm volatile("x86mtflag %0, 0x3f\nx86mfflag %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags), "=r"(dst)                                      \
                 : "n"(mask)                                                   \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ0(x86mfflag, 0);
  IFUZZ0(x86mfflag, 7);
  IFUZZ0(x86mfflag, 14);
  IFUZZ0(x86mfflag, 15);
  IFUZZ0(x86mfflag, 63);
}
