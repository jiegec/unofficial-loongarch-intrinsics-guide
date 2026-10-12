// store_v_buff_fmtinst_c2 -- Converts two components into the element format carried by the instruction and stores both.
//
// buff-carrier formatted store: the format space is established (loonggpu, from mesa's LG200_BUFFER_DESCRIPTOR contract): `data_format` is 4 bits at descriptor [114:111] (R8=1, R16=2, RG8=3, R32=4, RG16=5, RGBA8=10, RG32=11, RGBA16=12, RGB32=13, RGBA32=14) and `number_format` 3 bits at [110:108] (UNORM 0, SNORM 1, USCALED 2, SSCALED 3, UINT 4, SINT 5, FLOAT 7), with the destination selects at [107:96]; the `_cN` suffix is the component count the data format delivers.  The CPU model passes the stored words through because the encoding side is launch-staged, not because the decode is unknown.

u64 r = ((u64)src[0] | ((u64)src[0+1] << 32));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
