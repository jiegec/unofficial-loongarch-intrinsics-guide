#include "common.h"

v256 xvclo_h(v256 a) {
  v256 dst;
#include "xvclo_h.h"
  return dst;
}

void test() { XFUZZ1(xvclo_h); }
