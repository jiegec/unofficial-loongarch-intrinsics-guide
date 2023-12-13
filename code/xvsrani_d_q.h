for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i < 2) ? (s64)((s128)b.qword[i] >> imm)
                         : (s64)((s128)a.qword[i - 2] >> imm);
}
