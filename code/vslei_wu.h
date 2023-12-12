for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] <= imm) ? 0xFFFFFFFF : 0;
}
