for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u128 temp;
    if (imm == 0) {
      temp = (u128)b.qword[i];
    } else {
      temp = ((u128)b.qword[i] >> imm) + (((u128)b.qword[i] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  } else {
    u128 temp;
    if (imm == 0) {
      temp = (u128)a.qword[i - 2];
    } else {
      temp = ((u128)a.qword[i - 2] >> imm) +
             (((u128)a.qword[i - 2] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  }
}
