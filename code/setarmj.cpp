#include "common.h"

uint64_t setarmj(eflags &ARMFLAGS, uint64_t cond) {
  uint64_t dst = 0;
#include "setarmj.h"
  return dst;
}

#define ref_setarmj(eflags, cond)                                              \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst = 0;                                                          \
    asm volatile("x86mtflag %0, 0x3f\nsetarmj %1, %2\nx86mfflag %0, 0x3f"      \
                 : "+r"(flags), "=r"(dst)                                      \
                 : "n"(cond)                                                   \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ0(setarmj, 0);
  IFUZZ0(setarmj, 3);
  IFUZZ0(setarmj, 7);
  IFUZZ0(setarmj, 14);
  IFUZZ0(setarmj, 15);
}
