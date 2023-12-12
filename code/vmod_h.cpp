#include "common.h"

v128 vmod_h(v128 a, v128 b) {
  v128 dst;
#include "vmod_h.h"
  return dst;
}

void test() { FUZZ2(vmod_h); }
