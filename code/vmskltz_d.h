u64 m = 0x8000000000000000;
u64 c = m & a.dword[0];
c >>= 63;
dst.dword[0] = c;
c = m & a.dword[1];
c >>= 63;
dst.dword[0] |= c << 1;
dst.dword[1] = 0;
