#include "common.h"

v256 xvreplve0_w(v256 a) {
  v256 dst;
#include "xvreplve0_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvreplve0_w);
}
