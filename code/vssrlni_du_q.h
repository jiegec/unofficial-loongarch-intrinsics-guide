for (int i = 0; i < 2; i++) {
  if (i < 1) {
    u128 temp = (u128)b.qword[i] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  } else {
    u128 temp = (u128)a.qword[i - 1] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  }
}
