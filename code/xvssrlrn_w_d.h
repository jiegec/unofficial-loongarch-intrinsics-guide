for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp;
    if ((b.dword[i] & 63) == 0) {
      temp = (u64)a.dword[i];
    } else {
      temp = ((u64)a.dword[i] >> (b.dword[i] & 63)) +
             (((u64)a.dword[i] >> ((b.dword[i] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp;
    if ((b.dword[i - 2] & 63) == 0) {
      temp = (u64)a.dword[i - 2];
    } else {
      temp = ((u64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) +
             (((u64)a.dword[i - 2] >> ((b.dword[i - 2] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
