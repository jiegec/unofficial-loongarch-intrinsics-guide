for (int i = 0; i < 2; i++) {
  if (i < 1) {
    if (imm == 0) {
      dst.dword[i] = (s64)(s128)b.qword[i];
    } else {
      dst.dword[i] = (s64)(((s128)b.qword[i] >> imm) +
                           (((s128)b.qword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (s64)(s128)a.qword[i - 1];
    } else {
      dst.dword[i] = (s64)(((s128)a.qword[i - 1] >> imm) +
                           (((s128)a.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  }
}
