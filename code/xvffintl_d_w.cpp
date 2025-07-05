#include "common.h"

v256 xvffintl_d_w(v256 a) {
  v256 dst;
#include "xvffintl_d_w.h"
  return dst;
}

void test() { XFUZZ1(xvffintl_d_w); }
