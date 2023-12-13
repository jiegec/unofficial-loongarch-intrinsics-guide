for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i < 2) ? (u64)((u128)b.qword[i] >> imm)
                         : (u64)((u128)a.qword[i - 2] >> imm);
}
