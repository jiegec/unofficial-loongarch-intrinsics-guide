#include "common.h"

v128 vssrln_h_w(v128 a, v128 b) {
  v128 dst;
#include "vssrln_h_w.h"
  return dst;
}

void test() { FUZZ2(vssrln_h_w); }
