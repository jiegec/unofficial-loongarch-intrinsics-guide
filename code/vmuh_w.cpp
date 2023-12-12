#include "common.h"

v128 vmuh_w(v128 a, v128 b) {
  v128 dst;
#include "vmuh_w.h"
  return dst;
}

void test() { FUZZ2(vmuh_w); }
