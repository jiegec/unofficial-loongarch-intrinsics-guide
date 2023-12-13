#include "common.h"

v256 xvneg_d(v256 a) {
  v256 dst;
#include "xvneg_d.h"
  return dst;
}

void test() { XFUZZ1(xvneg_d); }
