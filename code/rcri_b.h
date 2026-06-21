uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm % (8 + 1);
// Rotate [CF, v] right by c, write low 8 bits to rd
uint64_t ring_hi = EFLAGS.CF;
uint64_t ring_lo = v;
uint64_t r;
if (c == 0) {
  r = v;
} else {
  uint64_t tmp_hi = ring_hi, tmp_lo = ring_lo;
  for (unsigned i = 0; i < c; i++) {
    uint64_t new_lo = (tmp_lo >> 1) | (tmp_hi << (8 - 1));
    uint64_t new_hi = tmp_lo & 1;
    tmp_lo = new_lo;
    tmp_hi = new_hi;
  }
  r = tmp_lo & ((1ULL << 8) - 1);
}
dst = sext(r, 8);