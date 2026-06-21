#include "common.h"

uint64_t rcri_b(eflags &EFLAGS, uint64_t a, int imm) {
  uint64_t dst;
#include "rcri_b.h"
  return dst;
}

#define ref_rcri_b(eflags, a, imm)                                             \
  ({                                                                           \
    uint16_t _efl = (eflags).raw;                                              \
    uint64_t _result;                                                          \
    asm volatile("x86mtflag %1, 0x3f\nrcri.b %0, %2, %3\nx86mfflag %1, 0x3f"  \
                 : "=r"(_result), "+r"(_efl)                                   \
                 : "r"(a), "n"(imm)                                            \
                 : "memory");                                                  \
    (eflags).raw = _efl;                                                       \
    _result;                                                                   \
  })

void test() {
  IFUZZ1(rcri_b, 0);
  IFUZZ1(rcri_b, 1);
  IFUZZ1(rcri_b, 7);
}
