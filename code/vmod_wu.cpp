#include "common.h"

v128 vmod_wu(v128 a, v128 b) {
  v128 dst;
#include "vmod_wu.h"
  return dst;
}

void test() { FUZZ2(vmod_wu); }
