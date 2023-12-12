#include "common.h"

v128 vsrar_w(v128 a, v128 b) {
  v128 dst;
#include "vsrar_w.h"
  return dst;
}

void test() { FUZZ2(vsrar_w); }
