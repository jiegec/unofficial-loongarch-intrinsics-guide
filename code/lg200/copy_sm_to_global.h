// copy_sm_to_global -- Copies the workgroup shared-memory region bound for
// the active workgroup to global memory.
// Workgroup transfer effect: the region, extent, and completion are part of
// the workgroup launch contract ([HW-observed] on the shared-memory
// carrier); no register value is produced.
return true;
