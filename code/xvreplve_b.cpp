#include "common.h"

v256 xvreplve_b(v256 a, int idx) {
  v256 dst;
#include "xvreplve_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplve_b, 0);
  XFUZZ1(xvreplve_b, 1);
}
