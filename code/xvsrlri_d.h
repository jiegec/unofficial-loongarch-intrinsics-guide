for (int i = 0; i < 4; i++) {
  if (imm == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] = (a.dword[i] >> imm) + ((a.dword[i] >> (imm - 1)) & 0x1);
  }
}
