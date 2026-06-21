uint64_t v = a;
if (mask & (1 << 0))
  EFLAGS.CF = (v >> 0) & 1;
if (mask & (1 << 1))
  EFLAGS.PF = (v >> 2) & 1;
if (mask & (1 << 2))
  EFLAGS.AF = (v >> 4) & 1;
if (mask & (1 << 3))
  EFLAGS.ZF = (v >> 6) & 1;
if (mask & (1 << 4))
  EFLAGS.SF = (v >> 7) & 1;
if (mask & (1 << 5))
  EFLAGS.OF = (v >> 11) & 1;