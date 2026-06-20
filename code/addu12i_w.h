int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;
dst = sext((uint32_t)a + (uint32_t)offset, 32);
