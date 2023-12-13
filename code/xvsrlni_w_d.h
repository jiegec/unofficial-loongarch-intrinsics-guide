for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (u32)((u64)b.dword[i] >> imm)
                        : (u32)((u64)a.dword[i - 2] >> imm);
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? (u32)((u64)b.dword[i - 2] >> imm)
                        : (u32)((u64)a.dword[i - 4] >> imm);
}
