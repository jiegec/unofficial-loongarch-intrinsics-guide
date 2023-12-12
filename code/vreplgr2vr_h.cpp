#include "common.h"

v128 vreplgr2vr_h(int val) {
  v128 dst;
#include "vreplgr2vr_h.h"
  return dst;
}

void test() {
  FUZZ0(vreplgr2vr_h, 0);
  FUZZ0(vreplgr2vr_h, 1);
  FUZZ0(vreplgr2vr_h, 256);
}
