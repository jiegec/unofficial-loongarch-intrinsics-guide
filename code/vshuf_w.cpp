#include "common.h"

v128 vshuf_w(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vshuf_w.h"
  return dst;
}

void test() { FUZZ3(vshuf_w); }
