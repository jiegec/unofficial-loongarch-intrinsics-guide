#include "common.h"

v128 vreplgr2vr_b(int val) {
  v128 dst;
#include "vreplgr2vr_b.h"
  return dst;
}

void test() {
  FUZZ0(vreplgr2vr_b, 0);
  FUZZ0(vreplgr2vr_b, 1);
  FUZZ0(vreplgr2vr_b, 256);
}
