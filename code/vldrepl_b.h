u8 data = memory_load(8, addr + offset);
for (int i = 0; i < 16; i++) {
  dst.byte[i] = data;
}
