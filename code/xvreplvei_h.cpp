#include "common.h"

v256 xvreplvei_h(v256 a, int idx) {
  v256 dst;
#include "xvreplvei_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplvei_h, 0);
  XFUZZ1(xvreplvei_h, 1);
}
