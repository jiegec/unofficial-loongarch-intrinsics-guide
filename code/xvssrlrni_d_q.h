for (int i = 0; i < 2; i++) {
  if (i < 1) {
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
      temp = (u128)a.qword[i - 1];
    } else {
      temp = ((u128)a.qword[i - 1] >> imm) +
             (((u128)a.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  }
}
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    u128 temp;
    if (imm == 0) {
      temp = (u128)b.qword[i - 1];
    } else {
      temp = ((u128)b.qword[i - 1] >> imm) +
             (((u128)b.qword[i - 1] >> (imm - 1)) & 1);
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
