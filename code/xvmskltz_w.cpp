#include "common.h"

v256 xvmskltz_w(v256 a) {
  v256 dst;
#include "xvmskltz_w.h"
  return dst;
}

void test() { XFUZZ1(xvmskltz_w); }
