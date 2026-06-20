#include "common.h"

uint64_t armmove(eflags &ARMFLAGS, uint64_t dst, uint64_t a, int cond) {
#include "armmove.h"
  return dst;
}

#define ref_armmove(eflags, dst, a, cond)                                      \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    uint64_t temp_dst = dst;                                                   \
    asm volatile("x86mtflag %0, 0x3f\narmmove %1, %2, %3\nx86mfflag %0, 0x3f"  \
                 : "+r"(flags), "+r"(temp_dst)                                 \
                 : "r"(a), "n"(cond)                                           \
                 : "memory");                                                  \
    eflags.raw = flags;                                                        \
    temp_dst;                                                                  \
  })

void test() {
  IFUZZ2(armmove, 0);
  IFUZZ2(armmove, 7);
  IFUZZ2(armmove, 14);
  IFUZZ2(armmove, 15);
}
