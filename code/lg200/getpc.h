// getpc -- Writes the current control-flow position to the destination scalar-register pair; no other effect.
//
// The pair holds the position of this instruction (or of the next one - the row
// leaves that open).  That is launch-assembled state: no case input reaches it,
// so this reference names no value instead of copying one.  The GPU row's
// evidence is the control carrier's sentinel - the launch reaches getpc and
// continues past it - not a comparison of the pair.

dst[0] = 0u;
dst[1] = 0u;
