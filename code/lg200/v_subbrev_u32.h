// v_subbrev_u32 -- Subtracts the first u32 source from the second under a borrow state held in the third source.
//

dst[0] = (src[2] & 0x3fffffffu) == 0x3fffffffu
             ? src[1] - src[0] - 1u
             : (src[2] & 0x3fffffffu) == 0u
                   ? src[1] - src[0]
                   : (src[1] - src[0]) | (src[1] - src[0] - 1u);
