#include "common.h"

v128 vmod_w(v128 a, v128 b) {
  v128 dst;
#include "vmod_w.h"
  return dst;
}

void test() { FUZZ2(vmod_w); }
