#include "common.h"

v256 xvneg_b(v256 a) {
  v256 dst;
#include "xvneg_b.h"
  return dst;
}

void test() { XFUZZ1(xvneg_b); }
