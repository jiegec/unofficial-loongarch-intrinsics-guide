for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp;
    if ((b.dword[i] & 63) == 0) {
      temp = (s64)a.dword[i];
    } else {
      temp = ((s64)a.dword[i] >> (b.dword[i] & 63)) +
             (((s64)a.dword[i] >> ((b.dword[i] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp;
    if ((b.dword[i - 2] & 63) == 0) {
      temp = (s64)a.dword[i - 2];
    } else {
      temp = ((s64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) +
             (((s64)a.dword[i - 2] >> ((b.dword[i - 2] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
