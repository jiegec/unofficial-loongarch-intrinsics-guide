#include "common.h"

v256 xvreplgr2vr_d(int val) {
  v256 dst;
#include "xvreplgr2vr_d.h"
  return dst;
}

void test() {
  XFUZZ0(xvreplgr2vr_d, 0);
  XFUZZ0(xvreplgr2vr_d, 1);
  XFUZZ0(xvreplgr2vr_d, 256);
}
