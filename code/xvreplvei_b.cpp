#include "common.h"

v256 xvreplvei_b(v256 a, int idx) {
  v256 dst;
#include "xvreplvei_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplvei_b, 0);
  XFUZZ1(xvreplvei_b, 1);
}
