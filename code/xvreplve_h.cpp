#include "common.h"

v256 xvreplve_h(v256 a, int idx) {
  v256 dst;
#include "xvreplve_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplve_h, 0);
  XFUZZ1(xvreplve_h, 1);
}
