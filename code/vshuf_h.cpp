#include "common.h"

v128 vshuf_h(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vshuf_h.h"
  return dst;
}

void test() {
  FUZZ3(vshuf_h);
}
