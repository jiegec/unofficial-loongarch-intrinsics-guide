#include "common.h"

uint64_t adc_b(uint64_t a, uint64_t b, eflags &EFLAGS) {
  uint64_t dst;
#include "adc_b.h"
  return dst;
}

uint64_t ref_adc_b(uint64_t a, uint64_t b, eflags &EFLAGS) {
  uint16_t eflags = EFLAGS.raw;
  uint64_t dst;
  asm volatile("x86mtflag %1, 0x3f\nadc.b %0, %2, %3\nx86mfflag %1, 0x3f"
               : "=r"(dst), "+r"(eflags)
               : "r"(a), "r"(b)
               : "memory");
  EFLAGS.raw = eflags;
  return dst;
}

void test() { IFUZZ2(adc_b); }
