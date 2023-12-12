#include "common.h"

v128 vsrar_h(v128 a, v128 b) {
  v128 dst;
#include "vsrar_h.h"
  return dst;
}

void test() { FUZZ2(vsrar_h); }
