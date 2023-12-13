#include "common.h"

v256 xvextl_q_d(v256 a) {
  v256 dst;
#include "xvextl_q_d.h"
  return dst;
}

void test() { XFUZZ1(xvextl_q_d); }
