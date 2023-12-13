#include "common.h"

v256 xvsubi_wu(v256 a, int imm) {
  v256 dst;
#include "xvsubi_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvsubi_wu, 0);
  XFUZZ1(xvsubi_wu, 31);
}
