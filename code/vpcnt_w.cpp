#include "common.h"

v128 vpcnt_w(v128 a) {
  v128 dst;
#include "vpcnt_w.h"
  return dst;
}

void test() { FUZZ1(vpcnt_w); }
