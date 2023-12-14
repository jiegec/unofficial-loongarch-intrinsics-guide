u8 data = memory_load(8, addr + offset);
for (int i = 0; i < 32; i++) {
  dst.byte[i] = data;
}
