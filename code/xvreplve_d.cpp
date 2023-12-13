#include "common.h"

v256 xvreplve_d(v256 a, int idx) {
  v256 dst;
#include "xvreplve_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplve_d, 0);
  XFUZZ1(xvreplve_d, 1);
}
