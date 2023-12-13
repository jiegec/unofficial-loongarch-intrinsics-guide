#include "common.h"

v256 xvpcnt_b(v256 a) {
  v256 dst;
#include "xvpcnt_b.h"
  return dst;
}

void test() { XFUZZ1(xvpcnt_b); }
