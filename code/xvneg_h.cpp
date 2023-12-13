#include "common.h"

v256 xvneg_h(v256 a) {
  v256 dst;
#include "xvneg_h.h"
  return dst;
}

void test() { XFUZZ1(xvneg_h); }
