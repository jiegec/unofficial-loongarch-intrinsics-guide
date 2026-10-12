// atomic_sm_add_u32 -- Atomically adds the lane's source to the 32-bit shared-memory element: `new = old + src` modulo 2^32.
//

u32 old = src[0];
dst[0] = old;
dst[1] = old + src[1];      // wraps at 32 bits
