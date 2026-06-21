#include "common.h"

uint64_t addu12i_d(eflags &EFLAGS, uint64_t a, int imm) {
  uint64_t dst;
#include "addu12i_d.h"
  return dst;
}

#define ref_addu12i_d(eflags, a, imm)                                          \
  ({                                                                           \
    uint64_t _result;                                                          \
    asm volatile("addu12i.d %0, %1, %2"                                        \
                 : "=r"(_result)                                               \
                 : "r"(a), "n"(imm)                                            \
                 : "memory");                                                  \
    _result;                                                                   \
  })

void test() {
  IFUZZ1(addu12i_d, 0);
  IFUZZ1(addu12i_d, 1);
  IFUZZ1(addu12i_d, 15);
}
