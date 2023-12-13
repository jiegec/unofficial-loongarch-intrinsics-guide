#include "common.h"

v256 xvclz_w(v256 a) {
  v256 dst;
#include "xvclz_w.h"
  return dst;
}

void test() { XFUZZ1(xvclz_w); }
