for (int i = 0; i < 8; i++) {
  dst.word[i] = clamp<s32>(a.word[i], -(1 << imm), (1 << imm) - 1);
}
