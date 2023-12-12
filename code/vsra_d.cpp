#include "common.h"

v128 vsra_d(v128 a, v128 b) {
  v128 dst;
#include "vsra_d.h"
  return dst;
}

void test() { FUZZ2(vsra_d); }
