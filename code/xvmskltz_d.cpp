#include "common.h"

v256 xvmskltz_d(v256 a) {
  v256 dst;
#include "xvmskltz_d.h"
  return dst;
}

void test() { XFUZZ1(xvmskltz_d); }
