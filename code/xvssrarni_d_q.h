for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i];
    } else {
      temp = ((s128)b.qword[i] >> imm) + (((s128)b.qword[i] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 2];
    } else {
      temp = ((s128)a.qword[i - 2] >> imm) +
             (((s128)a.qword[i - 2] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  }
}
