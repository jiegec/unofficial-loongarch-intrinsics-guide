for (int i = 0; i < 4; i++) {
  dst.dword[i] = clamp<u64>(a.dword[i], 0, (1 << (imm + 1)) - 1);
}
