#include "common.h"

v256 xvrepl128vei_w(v256 a, int idx) {
  v256 dst;
#include "xvrepl128vei_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvrepl128vei_w, 0);
  XFUZZ1(xvrepl128vei_w, 1);
  XFUZZ1(xvrepl128vei_w, 2);
  XFUZZ1(xvrepl128vei_w, 3);
}
