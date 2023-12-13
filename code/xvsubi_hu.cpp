#include "common.h"

v256 xvsubi_hu(v256 a, int imm) {
  v256 dst;
#include "xvsubi_hu.h"
  return dst;
}

void test() {
  XFUZZ1(xvsubi_hu, 0);
  XFUZZ1(xvsubi_hu, 31);
}
