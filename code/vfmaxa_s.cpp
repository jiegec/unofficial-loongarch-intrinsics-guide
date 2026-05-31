#include "common.h"

v128 vfmaxa_s(v128 a, v128 b) {
  v128 dst;
#include "vfmaxa_s.h"
  return dst;
}

void test() { FUZZ2(vfmaxa_s); }
