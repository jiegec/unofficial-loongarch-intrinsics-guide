for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp;
    if (imm == 0) {
      temp = (u64)b.dword[i];
    } else {
      temp = ((u64)b.dword[i] >> imm) + (((u64)b.dword[i] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    u64 temp;
    if (imm == 0) {
      temp = (u64)a.dword[i - 2];
    } else {
      temp = ((u64)a.dword[i - 2] >> imm) +
             (((u64)a.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  }
}
