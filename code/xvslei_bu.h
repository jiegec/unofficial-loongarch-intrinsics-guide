for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] <= imm) ? 0xFF : 0;
}
