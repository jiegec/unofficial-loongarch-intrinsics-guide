for (int i = 0; i < 2; i++) {
  if (i < 1) {
    u128 temp = (u128)a.qword[i] >> (b.qword[i] & 127);
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  } else {
    dst.dword[i] = 0;
  }
}
