#include "common.h"

v128 vmaddwev_d_wu(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmaddwev_d_wu.h"
  return dst;
}

void test() { FUZZ3(vmaddwev_d_wu); }
