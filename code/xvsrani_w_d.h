for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)b.dword[i] >> imm)
                        : (s32)((s64)a.dword[i - 2] >> imm);
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? (s32)((s64)b.dword[i - 2] >> imm)
                        : (s32)((s64)a.dword[i - 4] >> imm);
}
