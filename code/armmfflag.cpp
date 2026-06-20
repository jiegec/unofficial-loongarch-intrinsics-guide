#include "common.h"

uint64_t armmfflag(eflags &ARMFLAGS, uint64_t mask) {
  uint64_t dst = 0;
#include "armmfflag.h"
  return dst;
}

#define ref_armmfflag(eflags, mask)                                            \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst = 0;                                                          \
    asm volatile("x86mtflag %0, 0x3f\narmmfflag %1, %2\nx86mfflag %0, 0x3f"    \
                 : "+r"(flags), "=r"(dst)                                      \
                 : "n"(mask)                                                   \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ0(armmfflag, 0);
  IFUZZ0(armmfflag, 7);
  IFUZZ0(armmfflag, 14);
  IFUZZ0(armmfflag, 15);
  IFUZZ0(armmfflag, 0x3f);
}
