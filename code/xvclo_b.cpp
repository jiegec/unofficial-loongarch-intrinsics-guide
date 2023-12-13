#include "common.h"

v256 xvclo_b(v256 a) {
  v256 dst;
#include "xvclo_b.h"
  return dst;
}

void test() { XFUZZ1(xvclo_b); }
