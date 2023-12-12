#include "common.h"

v128 vreplgr2vr_w(int val) {
  v128 dst;
#include "vreplgr2vr_w.h"
  return dst;
}

void test() {
  FUZZ0(vreplgr2vr_w, 0);
  FUZZ0(vreplgr2vr_w, 1);
  FUZZ0(vreplgr2vr_w, 256);
}
