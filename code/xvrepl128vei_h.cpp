#include "common.h"

v256 xvrepl128vei_h(v256 a, int idx) {
  v256 dst;
#include "xvrepl128vei_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvrepl128vei_h, 0);
  XFUZZ1(xvrepl128vei_h, 1);
  XFUZZ1(xvrepl128vei_h, 2);
  XFUZZ1(xvrepl128vei_h, 3);
}
