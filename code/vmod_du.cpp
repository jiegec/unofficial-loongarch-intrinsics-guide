#include "common.h"

v128 vmod_du(v128 a, v128 b) {
  v128 dst;
#include "vmod_du.h"
  return dst;
}

void test() { FUZZ2(vmod_du); }
