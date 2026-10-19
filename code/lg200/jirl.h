// jirl -- Writes the return position to the destination scalar-register pair
// and jumps to the source-register address plus the encoded displacement.
// CPU model: the execution environment supplies the link in src[0:1]; the
// link pair is mirrored to dst and the transfer itself is an
// execution-environment effect.
u64 link = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(link & 0xffffffffu);
dst[1] = (u32)(link >> 32);
