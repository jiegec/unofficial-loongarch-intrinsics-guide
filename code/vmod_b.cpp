#include "common.h"

v128 vmod_b(v128 a, v128 b) {
  v128 dst;
#include "vmod_b.h"
  return dst;
}

void test() { FUZZ2(vmod_b); }
