for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)b.dword[i] >> imm)
                        : (s32)((s64)a.dword[i - 2] >> imm);
}
