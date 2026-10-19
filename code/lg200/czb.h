// czb -- Branches to the encoded 32-bit displacement when the scalar condition
// operand is zero.
// Conditional control-flow effect: taken iff the condition operand is zero;
// the target is the control-flow position plus the encoded displacement. No
// register result is produced.
return true;
