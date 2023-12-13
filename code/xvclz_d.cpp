#include "common.h"

v256 xvclz_d(v256 a) {
  v256 dst;
#include "xvclz_d.h"
  return dst;
}

void test() { XFUZZ1(xvclz_d); }
