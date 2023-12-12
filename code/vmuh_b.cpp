#include "common.h"

v128 vmuh_b(v128 a, v128 b) {
  v128 dst;
#include "vmuh_b.h"
  return dst;
}

void test() { FUZZ2(vmuh_b); }
