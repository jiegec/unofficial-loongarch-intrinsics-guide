#include "common.h"

v128 vmuh_wu(v128 a, v128 b) {
  v128 dst;
#include "vmuh_wu.h"
  return dst;
}

void test() { FUZZ2(vmuh_wu); }
