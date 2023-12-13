#include "common.h"

v256 xvpcnt_d(v256 a) {
  v256 dst;
#include "xvpcnt_d.h"
  return dst;
}

void test() { XFUZZ1(xvpcnt_d); }
