#include "common.h"

v256 xvffint_d_l(v256 a) {
  v256 dst;
#include "xvffint_d_l.h"
  return dst;
}

void test() { XFUZZ1(xvffint_d_l); }
