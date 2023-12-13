#include "common.h"

v256 xvreplgr2vr_h(int val) {
  v256 dst;
#include "xvreplgr2vr_h.h"
  return dst;
}

void test() {
  XFUZZ0(xvreplgr2vr_h, 0);
  XFUZZ0(xvreplgr2vr_h, 1);
  XFUZZ0(xvreplgr2vr_h, 256);
}
