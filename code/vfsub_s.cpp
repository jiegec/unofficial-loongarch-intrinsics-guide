#include "common.h"

v128 vfsub_s(v128 a, v128 b) {
  v128 dst;
#include "vfsub_s.h"
  return dst;
}

void test() { FUZZ2(vfsub_s); }
