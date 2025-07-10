#include "common.h"

v128 vmepatmsk_v(int mode, int uimm5) {
  v128 dst;
#include "vmepatmsk_v.h"
  return dst;
}

void test() {
  FUZZ0(vmepatmsk_v, 0, 0);
  FUZZ0(vmepatmsk_v, 0, 1);
  FUZZ0(vmepatmsk_v, 0, 2);
  FUZZ0(vmepatmsk_v, 0, 31);
  FUZZ0(vmepatmsk_v, 1, 0);
  FUZZ0(vmepatmsk_v, 1, 1);
  FUZZ0(vmepatmsk_v, 1, 2);
  FUZZ0(vmepatmsk_v, 1, 31);
  FUZZ0(vmepatmsk_v, 2, 0);
  FUZZ0(vmepatmsk_v, 2, 1);
  FUZZ0(vmepatmsk_v, 2, 2);
  FUZZ0(vmepatmsk_v, 2, 31);
  FUZZ0(vmepatmsk_v, 3, 0);
  FUZZ0(vmepatmsk_v, 3, 1);
  FUZZ0(vmepatmsk_v, 3, 2);
  FUZZ0(vmepatmsk_v, 3, 31);
}
