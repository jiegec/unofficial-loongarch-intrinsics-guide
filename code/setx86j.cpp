#include "common.h"

uint64_t setx86j(eflags &EFLAGS, uint64_t cond) {
  uint64_t dst = 0;
#include "setx86j.h"
  return dst;
}

#define ref_setx86j(eflags, cond)                                              \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t dst = 0;                                                          \
    asm volatile("x86mtflag %0, 0x3f\nsetx86j %1, %2\nx86mfflag %0, 0x3f"      \
                 : "+r"(flags), "=r"(dst)                                      \
                 : "n"(cond)                                                   \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    dst;                                                                       \
  })

void test() {
  IFUZZ0(setx86j, 0);
  IFUZZ0(setx86j, 3);
  IFUZZ0(setx86j, 7);
  IFUZZ0(setx86j, 14);
  IFUZZ0(setx86j, 15);
}
