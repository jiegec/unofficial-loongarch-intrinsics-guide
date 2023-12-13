#include "common.h"

v128 vfrstpi_b(v128 a, v128 b, int imm) {
  v128 dst;
#include "vfrstpi_b.h"
  return dst;
}

void test() {
  FUZZ2(vfrstpi_b, 0);
  FUZZ2(vfrstpi_b, 4);
  FUZZ2(vfrstpi_b, 31);
}
