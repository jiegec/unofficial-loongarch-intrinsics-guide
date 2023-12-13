#include "common.h"

v256 xvreplgr2vr_b(int val) {
  v256 dst;
#include "xvreplgr2vr_b.h"
  return dst;
}

void test() {
  XFUZZ0(xvreplgr2vr_b, 0);
  XFUZZ0(xvreplgr2vr_b, 1);
  XFUZZ0(xvreplgr2vr_b, 256);
}
