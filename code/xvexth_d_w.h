int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 + i];
}
for (; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[4 + i];
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)((s32)a.word[2]);
  dst.dword[1] = (s64)((s32)a.word[3]);
  dst.dword[2] = (s64)((s32)a.word[6]);
  dst.dword[3] = (s64)((s32)a.word[7]);
}
