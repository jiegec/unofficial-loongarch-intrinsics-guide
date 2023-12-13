for (int i = 0; i < 32; i++) {
  dst.byte[i] = clamp<s8>(a.byte[i], -(1 << imm), (1 << imm) - 1);
}
