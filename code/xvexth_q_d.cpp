#include "common.h"

v256 xvexth_q_d(v256 a) {
  v256 dst;
#include "xvexth_q_d.h"
  return dst;
}

void test() { XFUZZ1(xvexth_q_d); }
