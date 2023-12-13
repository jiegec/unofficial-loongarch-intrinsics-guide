#include "common.h"

v256 xvsllwil_du_wu(v256 a, int imm) {
  v256 dst;
#include "xvsllwil_du_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvsllwil_du_wu, 0);
  XFUZZ1(xvsllwil_du_wu, 7);
}
