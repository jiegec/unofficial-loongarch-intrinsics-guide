// copy_sm_from_global -- Copies the global-memory region bound for the active
// workgroup into workgroup shared memory.
// Workgroup transfer effect: the region, extent, and completion are part of
// the workgroup launch contract ([HW-observed] on the shared-memory
// carrier); no register value is produced.
return true;
