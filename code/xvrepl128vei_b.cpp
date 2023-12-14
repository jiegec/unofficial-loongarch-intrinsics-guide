#include "common.h"

v256 xvrepl128vei_b(v256 a, int idx) {
  v256 dst;
#include "xvrepl128vei_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvrepl128vei_b, 0);
  XFUZZ1(xvrepl128vei_b, 1);
  XFUZZ1(xvrepl128vei_b, 2);
  XFUZZ1(xvrepl128vei_b, 3);
}
