#include "common.h"

v256 xvclo_d(v256 a) {
  v256 dst;
#include "xvclo_d.h"
  return dst;
}

void test() { XFUZZ1(xvclo_d); }
