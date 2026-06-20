int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;
dst = a + (uint64_t)offset;
