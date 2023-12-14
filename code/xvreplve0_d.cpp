#include "common.h"

v256 xvreplve0_d(v256 a) {
  v256 dst;
#include "xvreplve0_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplve0_d);
}
