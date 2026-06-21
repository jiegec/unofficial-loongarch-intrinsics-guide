#include "common.h"

uint64_t rotri_h(eflags &EFLAGS, uint64_t a, int imm) {
  uint64_t dst;
#include "rotri_h.h"
  return dst;
}

#define ref_rotri_h(eflags, a, imm)                                            \
  ({                                                                           \
    uint16_t _efl = (eflags).raw;                                              \
    uint64_t _result;                                                          \
    asm volatile("x86mtflag %1, 0x3f\nrotri.h %0, %2, %3\nx86mfflag %1, 0x3f"  \
                 : "=r"(_result), "+r"(_efl)                                   \
                 : "r"(a), "n"(imm)                                            \
                 : "memory");                                                  \
    (eflags).raw = _efl;                                                       \
    _result;                                                                   \
  })

void test() {
  IFUZZ1(rotri_h, 0);
  IFUZZ1(rotri_h, 1);
  IFUZZ1(rotri_h, 7);
  IFUZZ1(rotri_h, 15);
}
