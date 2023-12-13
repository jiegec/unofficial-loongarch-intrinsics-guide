for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u8 shift = (b.dword[i] & 63);
    if (shift == 0) {
      dst.word[i] = (u32)(u64)a.dword[i];
    } else {
      dst.word[i] = (u32)(((u64)a.dword[i] >> shift) +
                          (((u64)a.dword[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.word[i] = 0;
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u8 shift = (b.dword[i - 2] & 63);
    if (shift == 0) {
      dst.word[i] = (u32)(u64)a.dword[i - 2];
    } else {
      dst.word[i] = (u32)(((u64)a.dword[i - 2] >> shift) +
                          (((u64)a.dword[i - 2] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.word[i] = 0;
  }
}
