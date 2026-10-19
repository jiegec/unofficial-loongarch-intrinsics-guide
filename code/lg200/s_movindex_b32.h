// s_movindex_b32 -- Selects a 32-bit source value using the
// execution-environment index state and writes the result to the scalar
// destination.
// CPU model: the environment supplies the selected window value in
// src[0] (an SGPR-window read, not an instruction-address or timer read);
// only the value result is modeled here.
dst[0] = src[0];
