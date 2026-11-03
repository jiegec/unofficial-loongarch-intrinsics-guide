// load_v_buff_u8 -- Reads one unsigned byte through a buffer descriptor and zero-extends it into one 32-bit vector register.
//
// buff-carrier load: the descriptor/address formation is part of the buffer contract; the model reads the element out of the low byte of the word the carrier stages at that address.
//
// [HW 2026-11-03] The identity body was wrong for the same reason the flat
// sibling's was: a byte load that returns a staged 0x12345678 unchanged is not a
// byte load.  Measured once the instruction word became decodable (see the bit-27
// repair in the catalog, which landed with this): 0x12345678 reads back 0x00000078 and 0x80 reads back 0x00000080.  The row's title
// says exactly this, and load_v_buff_i16 has modelled it since 2026-10-22.

dst[0] = src[0] & 0xffu;
