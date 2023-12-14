#include "common.h"

v256 vext2xv_wu_bu(v256 a) {
  v256 dst;
#include "vext2xv_wu_bu.h"
  return dst;
}

void test() { XFUZZ1(vext2xv_wu_bu); }
