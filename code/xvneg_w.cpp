#include "common.h"

v256 xvneg_w(v256 a) {
  v256 dst;
#include "xvneg_w.h"
  return dst;
}

void test() { XFUZZ1(xvneg_w); }
