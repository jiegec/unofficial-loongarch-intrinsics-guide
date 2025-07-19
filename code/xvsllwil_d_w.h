for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[i] << imm;
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[i + 2] << imm;
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[0])) << imm;
  dst.dword[1] = ((s64)((s32)a.word[1])) << imm;
  dst.dword[2] = ((s64)((s32)a.word[4])) << imm;
  dst.dword[3] = ((s64)((s32)a.word[5])) << imm;
}
