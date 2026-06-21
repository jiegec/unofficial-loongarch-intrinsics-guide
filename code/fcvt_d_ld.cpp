#include "common.h"

uint64_t fcvt_d_ld(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint64_t dst = 0;
#include "fcvt_d_ld.h"
  return dst;
}

uint64_t ref_fcvt_d_ld(eflags &EFLAGS, uint64_t a, uint64_t b) {
  uint16_t eflags = EFLAGS.raw;
  uint64_t dst = 0;
  asm volatile(
      "x86mtflag %1, 0x3f\nmovgr2fr.d $f0, %2\nmovgr2fr.d $f1, %3\nfcvt.d.ld "
      "$f2, $f0, $f1\nmovfr2gr.d %0, $f2\nx86mfflag %1, 0x3f"
      : "=r"(dst), "+r"(eflags)
      : "r"(a), "r"(b)
      : "memory", "f0", "f1", "f2");
  EFLAGS.raw = eflags;
  return dst;
}

void test() { IFUZZ2(fcvt_d_ld); }
