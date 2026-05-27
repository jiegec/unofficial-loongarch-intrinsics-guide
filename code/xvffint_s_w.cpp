#include "common.h"

v256 xvffint_s_w(v256 a) {
  v256 dst;
#include "xvffint_s_w.h"
  return dst;
}

void test() { XFUZZ1(xvffint_s_w); }
