// atomic_sm_cmpswap_eq_u64 -- Atomically swaps on compare-equal in the 64-bit shared-memory element: `new = (old == cmp) ? Swap : old`.
//

u64 updated = sm_cmpswap_eq_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), ((u64)src[4] | ((u64)src[4+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
