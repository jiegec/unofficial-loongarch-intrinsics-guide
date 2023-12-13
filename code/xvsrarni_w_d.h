for (int i = 0; i < 4; i++) {
  if (i < 2) {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)b.dword[i];
    } else {
      dst.word[i] = (s32)(((s64)b.dword[i] >> imm) +
                          (((s64)b.dword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)a.dword[i - 2];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i - 2] >> imm) +
                          (((s64)a.dword[i - 2] >> (imm - 1)) & 0x1));
    }
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)b.dword[i - 2];
    } else {
      dst.word[i] = (s32)(((s64)b.dword[i - 2] >> imm) +
                          (((s64)b.dword[i - 2] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)a.dword[i - 4];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i - 4] >> imm) +
                          (((s64)a.dword[i - 4] >> (imm - 1)) & 0x1));
    }
  }
}
