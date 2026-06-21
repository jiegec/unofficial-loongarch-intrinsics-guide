dst = 0;
if (mask & (1 << 0))
  dst |= (uint64_t)EFLAGS.CF << 0;
if (mask & (1 << 1))
  dst |= (uint64_t)EFLAGS.PF << 2;
if (mask & (1 << 2))
  dst |= (uint64_t)EFLAGS.AF << 4;
if (mask & (1 << 3))
  dst |= (uint64_t)EFLAGS.ZF << 6;
if (mask & (1 << 4))
  dst |= (uint64_t)EFLAGS.SF << 7;
if (mask & (1 << 5))
  dst |= (uint64_t)EFLAGS.OF << 11;