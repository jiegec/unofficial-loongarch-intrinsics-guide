#include "common.h"

uint64_t addu12i_w(eflags &EFLAGS, uint64_t a, int imm) {
  uint64_t dst;
#include "addu12i_w.h"
  return dst;
}

#define ref_addu12i_w(eflags, a, imm)                                          \
  ({                                                                           \
    uint64_t _result;                                                          \
    asm volatile("addu12i.w %0, %1, %2"                                        \
                 : "=r"(_result)                                               \
                 : "r"(a), "n"((((imm) & 0x1f) ^ 16) - 16)                     \
                 : "memory");                                                  \
    _result;                                                                   \
  })

void test() {
  IFUZZ1(addu12i_w, 0);
  IFUZZ1(addu12i_w, 1);
  IFUZZ1(addu12i_w, 31);
}
