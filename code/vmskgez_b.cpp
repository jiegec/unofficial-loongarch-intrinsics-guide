#include "common.h"

v128 vmskgez_b(v128 a) {
  v128 dst;
#include "vmskgez_b.h"
  return dst;
}

void test() { FUZZ1(vmskgez_b); }
