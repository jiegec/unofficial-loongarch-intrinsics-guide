// atomic_sm_and_b64 -- Atomically ANDs the lane's source into the 64-bit shared-memory element: `new = old & src`.
//

u64 updated = sm_and_b64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = src[0];
dst[1] = src[1];
dst[2] = (u32)(updated & 0xffffffffu);
dst[3] = (u32)(updated >> 32);
