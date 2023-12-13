#include "common.h"

v128 vssran_hu_w(v128 a, v128 b) {
  v128 dst;
#include "vssran_hu_w.h"
  return dst;
}

void test() { FUZZ2(vssran_hu_w); }
