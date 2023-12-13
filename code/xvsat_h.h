for (int i = 0; i < 16; i++) {
  dst.half[i] = clamp<s16>(a.half[i], -(1 << imm), (1 << imm) - 1);
}
