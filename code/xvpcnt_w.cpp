#include "common.h"

v256 xvpcnt_w(v256 a) {
  v256 dst;
#include "xvpcnt_w.h"
  return dst;
}

void test() { XFUZZ1(xvpcnt_w); }
