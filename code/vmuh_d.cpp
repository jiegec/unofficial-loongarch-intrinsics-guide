#include "common.h"

v128 vmuh_d(v128 a, v128 b) {
  v128 dst;
#include "vmuh_d.h"
  return dst;
}

void test() { FUZZ2(vmuh_d); }
