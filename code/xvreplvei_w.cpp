#include "common.h"

v256 xvreplvei_w(v256 a, int idx) {
  v256 dst;
#include "xvreplvei_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplvei_w, 0);
  XFUZZ1(xvreplvei_w, 1);
}
