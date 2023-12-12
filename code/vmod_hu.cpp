#include "common.h"

v128 vmod_hu(v128 a, v128 b) {
  v128 dst;
#include "vmod_hu.h"
  return dst;
}

void test() { FUZZ2(vmod_hu); }
