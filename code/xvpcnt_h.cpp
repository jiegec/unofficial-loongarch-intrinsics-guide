#include "common.h"

v256 xvpcnt_h(v256 a) {
  v256 dst;
#include "xvpcnt_h.h"
  return dst;
}

void test() { XFUZZ1(xvpcnt_h); }
