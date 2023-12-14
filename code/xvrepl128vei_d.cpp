#include "common.h"

v256 xvrepl128vei_d(v256 a, int idx) {
  v256 dst;
#include "xvrepl128vei_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvrepl128vei_d, 0);
  XFUZZ1(xvrepl128vei_d, 1);
}
