#include "common.h"

v128 vmuh_du(v128 a, v128 b) {
  v128 dst;
#include "vmuh_du.h"
  return dst;
}

void test() { FUZZ2(vmuh_du); }
