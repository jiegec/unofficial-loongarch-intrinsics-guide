dst = 0;
if (mask & (1 << 4))
  dst |= (uint64_t)ARMFLAGS.N << 31;
if (mask & (1 << 3))
  dst |= (uint64_t)ARMFLAGS.Z << 30;
if (mask & (1 << 0))
  dst |= (uint64_t)ARMFLAGS.C << 29;
if (mask & (1 << 5))
  dst |= (uint64_t)ARMFLAGS.V << 28;
dst = sext(dst, 32);
