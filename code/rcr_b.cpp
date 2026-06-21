#include "common.h"

uint64_t rcr_b(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint64_t dst;
#include "rcr_b.h"
  return dst;
}

uint64_t ref_rcr_b(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint16_t eflags = EFLAGS.raw;
  uint64_t dst;
  asm volatile("x86mtflag %1, 0x3f\nrcr.b %0, %2, %3\nx86mfflag %1, 0x3f"
               : "=r"(dst), "+r"(eflags)
               : "r"(a), "r"(b)
               : "memory");
  EFLAGS.raw = eflags;
  return dst;
}

void test() { IFUZZ2(rcr_b); }
