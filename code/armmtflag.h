uint64_t v = a;
if (mask & (1 << 4))
  ARMFLAGS.N = (v >> 31) & 1;
if (mask & (1 << 3))
  ARMFLAGS.Z = (v >> 30) & 1;
if (mask & (1 << 0))
  ARMFLAGS.C = (v >> 29) & 1;
if (mask & (1 << 5))
  ARMFLAGS.V = (v >> 28) & 1;
