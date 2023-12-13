for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u8 shift = (b.dword[i] & 63);
    if (shift == 0) {
      dst.word[i] = (s32)(s64)a.dword[i];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i] >> shift) +
                          (((s64)a.dword[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.word[i] = 0;
  }
}
