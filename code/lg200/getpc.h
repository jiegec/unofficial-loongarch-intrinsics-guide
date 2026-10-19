// getpc -- Writes the current control-flow position to the destination
// scalar-register pair.
// CPU model: the execution environment supplies the position in src[0:1]
// and the pair is mirrored to dst; a shader-readable architectural PC
// readback is not part of this model.
u64 link = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(link & 0xffffffffu);
dst[1] = (u32)(link >> 32);
