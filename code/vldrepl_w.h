u32 data = memory_load(32, addr + offset);
for (int i = 0; i < 4; i++) {
  dst.word[i] = data;
}