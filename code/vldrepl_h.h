u16 data = memory_load(16, addr + (offset << 1));
for (int i = 0; i < 8; i++) {
  dst.half[i] = data;
}
