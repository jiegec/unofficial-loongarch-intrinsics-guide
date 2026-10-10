// jirl -- Register-indirect call: writes the return pair and jumps to the source pair plus displacement.
//

u64 link = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(link & 0xffffffffu);
dst[1] = (u32)(link >> 32);
