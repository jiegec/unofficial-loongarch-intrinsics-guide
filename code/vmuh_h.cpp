#include "common.h"

v128 vmuh_h(v128 a, v128 b) {
  v128 dst;
#include "vmuh_h.h"
  return dst;
}

void test() { FUZZ2(vmuh_h); }
