for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? (u64)((u128)b.qword[i] >> imm)
                         : (u64)((u128)a.qword[i - 1] >> imm);
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? (u64)((u128)b.qword[i - 1] >> imm)
                         : (u64)((u128)a.qword[i - 2] >> imm);
}
