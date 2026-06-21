#include "common.h"

uint64_t sbc_w(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint64_t dst;
#include "sbc_w.h"
  return dst;
}

uint64_t ref_sbc_w(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint16_t eflags = EFLAGS.raw;
  uint64_t dst;
  asm volatile("x86mtflag %1, 0x3f\nsbc.w %0, %2, %3\nx86mfflag %1, 0x3f"
               : "=r"(dst), "+r"(eflags)
               : "r"(a), "r"(b)
               : "memory");
  EFLAGS.raw = eflags;
  return dst;
}

void test() { IFUZZ2(sbc_w); }
