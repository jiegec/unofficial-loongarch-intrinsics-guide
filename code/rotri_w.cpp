#include "common.h"

uint64_t rotri_w(eflags &EFLAGS, uint64_t a, int imm) {
  uint64_t dst;
#include "rotri_w.h"
  return dst;
}

#define ref_rotri_w(eflags, a, imm)                                            \
  ({                                                                           \
    uint16_t _efl = (eflags).raw;                                              \
    uint64_t _result;                                                          \
    asm volatile("x86mtflag %1, 0x3f\nrotri.w %0, %2, %3\nx86mfflag %1, 0x3f"  \
                 : "=r"(_result), "+r"(_efl)                                   \
                 : "r"(a), "n"(imm)                                            \
                 : "memory");                                                  \
    (eflags).raw = _efl;                                                       \
    _result;                                                                   \
  })

void test() {
  IFUZZ1(rotri_w, 0);
  IFUZZ1(rotri_w, 1);
  IFUZZ1(rotri_w, 7);
  IFUZZ1(rotri_w, 15);
  IFUZZ1(rotri_w, 31);
}
