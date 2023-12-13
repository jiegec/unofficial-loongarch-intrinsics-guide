for (int i = 0; i < 2; i++) {
  dst.dword[i] = clamp<s64>(a.dword[i], -(1 << imm), (1 << imm) - 1);
}
