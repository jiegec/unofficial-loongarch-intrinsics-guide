for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i];
    } else {
      temp = ((s64)b.dword[i] >> imm) + (((s64)b.dword[i] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 2];
    } else {
      temp = ((s64)a.dword[i - 2] >> imm) +
             (((s64)a.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i - 2];
    } else {
      temp = ((s64)b.dword[i - 2] >> imm) +
             (((s64)b.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 4];
    } else {
      temp = ((s64)a.dword[i - 4] >> imm) +
             (((s64)a.dword[i - 4] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  }
}
