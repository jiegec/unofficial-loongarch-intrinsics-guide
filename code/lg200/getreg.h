// getreg -- Reads the execution-environment register selected by the encoded
// immediate and writes its 32-bit value to the scalar destination.
// CPU model: src[1] is the encoded selector and src[0] carries the value
// observed from the execution-environment register (the PS carrier gates
// getreg to fixed/constant state); the register namespace is part of the
// execution-environment contract.
dst[0] = src[0];
