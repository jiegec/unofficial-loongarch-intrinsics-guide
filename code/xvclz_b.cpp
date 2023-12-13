#include "common.h"

v256 xvclz_b(v256 a) {
  v256 dst;
#include "xvclz_b.h"
  return dst;
}

void test() { XFUZZ1(xvclz_b); }
