#include "common.h"

v128 vssub_wu(v128 a, v128 b) {
  v128 dst;
#include "vssub_wu.h"
  return dst;
}

void test() { FUZZ2(vssub_wu); }
