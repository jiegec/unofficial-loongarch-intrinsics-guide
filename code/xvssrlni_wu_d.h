for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u64 temp = (u64)b.dword[i] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    u64 temp = (u64)a.dword[i - 4] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  }
}
