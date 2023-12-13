for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[i] << imm;
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[i + 2] << imm;
}
