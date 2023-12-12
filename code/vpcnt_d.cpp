#include "common.h"

v128 vpcnt_d(v128 a) {
  v128 dst;
#include "vpcnt_d.h"
  return dst;
}

void test() { FUZZ1(vpcnt_d); }
