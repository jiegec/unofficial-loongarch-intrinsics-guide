#include "common.h"

v128 vreplgr2vr_d(int val) {
  v128 dst;
#include "vreplgr2vr_d.h"
  return dst;
}

void test() {
  FUZZ0(vreplgr2vr_d, 0);
  FUZZ0(vreplgr2vr_d, 1);
  FUZZ0(vreplgr2vr_d, 256);
}
