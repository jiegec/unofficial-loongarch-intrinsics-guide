#include "common.h"

v128 vabsd_wu(v128 a, v128 b) {
  v128 dst;
#include "vabsd_wu.h"
  return dst;
}

void test() { FUZZ2(vabsd_wu); }
