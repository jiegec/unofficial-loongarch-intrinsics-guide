#include "common.h"

v256 xvfrstpi_h(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvfrstpi_h.h"
  return dst;
}

void test() {
  XFUZZ2(xvfrstpi_h, 0);
  XFUZZ2(xvfrstpi_h, 4);
  XFUZZ2(xvfrstpi_h, 31);
}
