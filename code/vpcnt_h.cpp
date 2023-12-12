#include "common.h"

v128 vpcnt_h(v128 a) {
  v128 dst;
#include "vpcnt_h.h"
  return dst;
}

void test() { FUZZ1(vpcnt_h); }
