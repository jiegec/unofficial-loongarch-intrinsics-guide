uint64_t counter_after_dec = a;
dst = (counter_after_dec != 0 && !EFLAGS.ZF) ? 1 : 0;