u64 data = memory_load(64, addr + (offset << 3));
for (int i = 0;i < 2;i++) {
    dst.dword[i] = data;
}
