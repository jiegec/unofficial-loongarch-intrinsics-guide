#include "common.h"

v128 vmuh_hu(v128 a, v128 b) {
  v128 dst;
#include "vmuh_hu.h"
  return dst;
}

void test() { FUZZ2(vmuh_hu); }
