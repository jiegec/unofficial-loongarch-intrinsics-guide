for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? (s64)((s128)b.qword[i] >> imm)
                         : (s64)((s128)a.qword[i - 1] >> imm);
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? (s64)((s128)b.qword[i - 1] >> imm)
                         : (s64)((s128)a.qword[i - 2] >> imm);
}
