#include "common.h"

v256 xvreplvei_d(v256 a, int idx) {
  v256 dst;
#include "xvreplvei_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplvei_d, 0);
  XFUZZ1(xvreplvei_d, 1);
}
