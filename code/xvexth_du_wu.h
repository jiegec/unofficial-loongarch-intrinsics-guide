int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 + i];
}
for (; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[4 + i];
}
