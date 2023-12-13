#include "common.h"

v256 xvreplve_w(v256 a, int idx) {
  v256 dst;
#include "xvreplve_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplve_w, 0);
  XFUZZ1(xvreplve_w, 1);
}
