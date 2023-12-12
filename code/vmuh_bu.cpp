#include "common.h"

v128 vmuh_bu(v128 a, v128 b) {
  v128 dst;
#include "vmuh_bu.h"
  return dst;
}

void test() { FUZZ2(vmuh_bu); }
