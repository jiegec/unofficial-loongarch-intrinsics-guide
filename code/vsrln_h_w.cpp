#include "common.h"

v128 vsrln_h_w(v128 a, v128 b) {
  v128 dst;
#include "vsrln_h_w.h"
  return dst;
}

void test() { FUZZ2(vsrln_h_w); }
