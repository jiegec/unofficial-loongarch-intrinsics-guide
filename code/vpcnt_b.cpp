#include "common.h"

v128 vpcnt_b(v128 a) {
  v128 dst;
#include "vpcnt_b.h"
  return dst;
}

void test() { FUZZ1(vpcnt_b); }
