#include "common.h"

v128 vssrln_wu_d(v128 a, v128 b) {
  v128 dst;
#include "vssrln_wu_d.h"
  return dst;
}

void test() { FUZZ2(vssrln_wu_d); }
