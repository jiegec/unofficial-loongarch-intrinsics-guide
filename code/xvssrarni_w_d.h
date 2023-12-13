for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i];
    } else {
      temp = ((s64)b.dword[i] >> imm) + (((s64)b.dword[i] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 4];
    } else {
      temp = ((s64)a.dword[i - 4] >> imm) +
             (((s64)a.dword[i - 4] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  }
}
