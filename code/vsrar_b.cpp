#include "common.h"

v128 vsrar_b(v128 a, v128 b) {
  v128 dst;
#include "vsrar_b.h"
  return dst;
}

void test() { FUZZ2(vsrar_b); }
