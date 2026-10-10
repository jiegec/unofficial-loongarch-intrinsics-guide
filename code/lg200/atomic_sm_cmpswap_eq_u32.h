// atomic_sm_cmpswap_eq_u32 -- Atomically swaps on compare-equal in the 32-bit shared-memory element: `new = (old == cmp) ? Swap : old`.
//

u32 updated = sm_cmpswap_eq_u32(src[0], src[1], src[2]);
dst[0] = src[0];
dst[1] = updated;
