// store_v_flat_b8 -- Stores the low byte of a vector register to a per-lane flat address.
//
// flat-carrier memory access: address formation and the bl1/bl2 behavior are part of the flat contract.
//
// [HW 2026-11-03] only the low byte is written: staged 0x12345678 reads back 0x00000078 on both pipes, in every case whose source word does not already fit. The buffer form has carried the same rule since 2026-10-22 (store_v_buff_b16.h); the flat row was simply not propagated.

dst[0] = src[0] & 0xffu;
