// atomic_sm_cmpswap_gt_f32 -- Atomically swaps in the source when the 32-bit float contents are greater: `new = (old > src) ? Src : old`.
//

u32 updated = sm_cmpswap_gt_f32(src[0], src[1]);
dst[0] = src[0];
dst[1] = updated;
