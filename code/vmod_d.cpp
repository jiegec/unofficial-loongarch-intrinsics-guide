#include "common.h"

v128 vmod_d(v128 a, v128 b) {
  v128 dst;
#include "vmod_d.h"
  return dst;
}

void test() { FUZZ2(vmod_d); }
