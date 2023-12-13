#include "common.h"

v128 vssrln_w_d(v128 a, v128 b) {
  v128 dst;
#include "vssrln_w_d.h"
  return dst;
}

void test() { FUZZ2(vssrln_w_d); }
