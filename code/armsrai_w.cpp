#include "common.h"

uint64_t armsrai_w(eflags &ARMFLAGS, uint64_t a, int imm, int cond) {
  uint64_t dst = 0;
#include "armsrai_w.h"
  return dst;
}

#define ref_armsrai_w(eflags, a, imm, cond)                                    \
  ({                                                                           \
    uint16_t flags = eflags.raw;                                               \
    asm volatile(                                                              \
        "x86mtflag %0, 0x3f\narmsrai.w %1, %2, %3\nx86mfflag %0, 0x3f"         \
        : "+r"(flags)                                                          \
        : "r"(a), "n"(imm), "n"(cond)                                          \
        : "memory");                                                           \
    eflags.raw = flags;                                                        \
    0;                                                                         \
  })

void test() {
  IFUZZ1(armsrai_w, 0, 7);
  IFUZZ1(armsrai_w, 0, 7);
  IFUZZ1(armsrai_w, 15, 14);
  IFUZZ1(armsrai_w, 15, 15);
  IFUZZ1(armsrai_w, 31, 15);
  IFUZZ1(armsrai_w, 31, 15);
}
