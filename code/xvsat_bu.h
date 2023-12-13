for (int i = 0; i < 32; i++) {
  dst.byte[i] = clamp<u8>(a.byte[i], 0, (1 << (imm + 1)) - 1);
}
