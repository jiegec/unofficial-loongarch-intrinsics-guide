#include "common.h"

v128 vshuf_d(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vshuf_d.h"
  return dst;
}

void test() { FUZZ3(vshuf_d); }
