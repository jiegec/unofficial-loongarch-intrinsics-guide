#include "common.h"

v128 vfmina_d(v128 a, v128 b) {
  v128 dst;
#include "vfmina_d.h"
  return dst;
}

void test() { FUZZ2(vfmina_d); }
