#include "common.h"

v256 xvclo_w(v256 a) {
  v256 dst;
#include "xvclo_w.h"
  return dst;
}

void test() { XFUZZ1(xvclo_w); }
