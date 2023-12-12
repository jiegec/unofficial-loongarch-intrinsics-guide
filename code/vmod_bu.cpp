#include "common.h"

v128 vmod_bu(v128 a, v128 b) {
  v128 dst;
#include "vmod_bu.h"
  return dst;
}

void test() { FUZZ2(vmod_bu); }
