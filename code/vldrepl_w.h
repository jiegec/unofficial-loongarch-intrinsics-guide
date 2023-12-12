u32 data = memory_load(32, addr + (offset << 2));
for (int i = 0;i < 4;i++) {
    dst.word[i] = data;
}