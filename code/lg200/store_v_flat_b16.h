// store_v_flat_b16 -- Stores the low 16 bits of a vector register to a per-lane flat address.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract.
//
// [HW 2026-11-03] only the low halfword is written: staged 0x7fffffff reads back 0x0000ffff on both pipes, in every case whose source word does not already fit. The buffer form has carried the same rule since 2026-10-22 (store_v_buff_b16.h); the flat row was simply not propagated.

dst[0] = src[0] & 0xffffu;
