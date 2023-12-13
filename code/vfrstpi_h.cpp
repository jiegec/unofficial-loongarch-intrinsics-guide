#include "common.h"

v128 vfrstpi_h(v128 a, v128 b, int imm) {
  v128 dst;
#include "vfrstpi_h.h"
  return dst;
}

void test() {
  FUZZ2(vfrstpi_h, 0);
  FUZZ2(vfrstpi_h, 4);
  FUZZ2(vfrstpi_h, 31);
}
