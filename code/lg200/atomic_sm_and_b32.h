// atomic_sm_and_b32 -- Atomically ANDs the lane's source into the 32-bit shared-memory element: `new = old & src`.
//

u32 updated = sm_and_b32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
