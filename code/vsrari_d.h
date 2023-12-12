for (int i = 0; i < 2; i++) {
  if (imm == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] =
        ((s64)a.dword[i] >> imm) + (((s64)a.dword[i] >> (imm - 1)) & 0x1);
  }
}
