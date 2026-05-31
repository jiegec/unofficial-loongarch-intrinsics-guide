#include "common.h"

v128 vfmaxa_d(v128 a, v128 b) {
  v128 dst;
#include "vfmaxa_d.h"
  return dst;
}

void test() { FUZZ2(vfmaxa_d); }
