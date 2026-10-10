// v_bfi_b32 -- Inserts bits from one value into another under a mask.
//

dst[0] = (src[0] & src[1]) | (~src[0] & src[2]);
