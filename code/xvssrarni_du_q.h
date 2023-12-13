for (int i = 0; i < 2; i++) {
  if (i < 1) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i];
    } else {
      temp = ((s128)b.qword[i] >> imm) + (((s128)b.qword[i] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 1];
    } else {
      temp = ((s128)a.qword[i - 1] >> imm) +
             (((s128)a.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  }
}
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i - 1];
    } else {
      temp = ((s128)b.qword[i - 1] >> imm) +
             (((s128)b.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 2];
    } else {
      temp = ((s128)a.qword[i - 2] >> imm) +
             (((s128)a.qword[i - 2] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  }
}
