#include "common.h"

v256 xvfrstpi_b(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvfrstpi_b.h"
  return dst;
}

void test() {
  XFUZZ2(xvfrstpi_b, 0);
  XFUZZ2(xvfrstpi_b, 4);
  XFUZZ2(xvfrstpi_b, 31);
}
