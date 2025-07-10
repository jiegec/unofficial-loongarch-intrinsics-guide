#include "common.h"

v256 xvmepatmsk_v(int mode, int uimm5) {
  v256 dst;
#include "xvmepatmsk_v.h"
  return dst;
}

void test() {
  XFUZZ0(xvmepatmsk_v, 0, 0);
  XFUZZ0(xvmepatmsk_v, 0, 1);
  XFUZZ0(xvmepatmsk_v, 0, 2);
  XFUZZ0(xvmepatmsk_v, 0, 31);
  XFUZZ0(xvmepatmsk_v, 1, 0);
  XFUZZ0(xvmepatmsk_v, 1, 1);
  XFUZZ0(xvmepatmsk_v, 1, 2);
  XFUZZ0(xvmepatmsk_v, 1, 31);
  XFUZZ0(xvmepatmsk_v, 2, 0);
  XFUZZ0(xvmepatmsk_v, 2, 1);
  XFUZZ0(xvmepatmsk_v, 2, 2);
  XFUZZ0(xvmepatmsk_v, 2, 31);
  XFUZZ0(xvmepatmsk_v, 3, 0);
  XFUZZ0(xvmepatmsk_v, 3, 1);
  XFUZZ0(xvmepatmsk_v, 3, 2);
  XFUZZ0(xvmepatmsk_v, 3, 31);
}
