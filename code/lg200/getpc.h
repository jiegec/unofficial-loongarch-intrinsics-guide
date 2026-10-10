// getpc -- Writes the current control-flow position to the destination scalar-register pair; no other effect.
//

u64 link = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(link & 0xffffffffu);
dst[1] = (u32)(link >> 32);
