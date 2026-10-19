u64 data = memory_load(64, addr + offset);
for (int i = 0; i < 2; i++) {
  dst.dword[i] = data;
}
