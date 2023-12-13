#include "common.h"

v256 xvreplgr2vr_w(int val) {
  v256 dst;
#include "xvreplgr2vr_w.h"
  return dst;
}

void test() {
  XFUZZ0(xvreplgr2vr_w, 0);
  XFUZZ0(xvreplgr2vr_w, 1);
  XFUZZ0(xvreplgr2vr_w, 256);
}
