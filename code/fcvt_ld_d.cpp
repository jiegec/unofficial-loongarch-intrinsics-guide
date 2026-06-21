#include "common.h"

uint64_t fcvt_ld_d(eflags &EFLAGS, uint64_t a) {
  uint64_t dst = 0;
#include "fcvt_ld_d.h"
  return dst;
}

uint64_t ref_fcvt_ld_d(eflags &EFLAGS, uint64_t a) {
  uint16_t eflags = EFLAGS.raw;
  uint64_t dst = 0;
  asm volatile("x86mtflag %1, 0x3f\nmovgr2fr.d $f0, %2\nfcvt.ld.d "
               "$f1, $f0\nmovfr2gr.d %0, $f1\nx86mfflag %1, 0x3f"
               : "=r"(dst), "+r"(eflags)
               : "r"(a)
               : "memory", "f0", "f1", "f2");
  EFLAGS.raw = eflags;
  return dst;
}

void test() { IFUZZ1(fcvt_ld_d); }
