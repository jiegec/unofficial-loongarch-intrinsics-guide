# Host Interface

This chapter describes where a command comes from and who owns which layer.  The
payload a graphics submission carries is in [Graphics Submission](submission.md);
this page covers the layers around it.

## Ownership

Two very different interfaces carry the same name:

| Layer | Owned by | A user-mode driver |
|-------|----------|--------------------|
| Command interface (registers, command words, queue setup, MMU) | kernel | never touches it |
| Submission interface (buffer addresses, submission, fences) | user-mode driver through the kernel | this is the driver's contract |

Everything below the submission interface — page tables, queue provisioning,
virtual-memory domains, interrupt routing — is kernel business.  A user-mode
driver submits work and waits for completion; it does not program registers.

## Layer model

| Layer | Format | Producer | Role |
|------:|--------|----------|------|
| 0 | kernel command word | kernel | configure a pipe, a queue, the MMU, an interrupt or a VRAM window |
| 1 | stream command word | outer ring | select a virtual-memory domain, execute an indirect buffer, poll or write memory |
| 2 | pipe command stream | a pipe front end | graphics, blit, transfer or compute records |
| 3 | two-word shader instruction | shader front end | execute instructions from the kernel entry |

The same numeric opcode can appear in two layers with different meanings, so a
decoder must always carry the layer and the pipe with a command word.

## Kernel interface

The command interface is a small register file plus a command word.  The
registers a driver may observe indirectly, and that a kernel driver programs:

| Offset | Register |
|-------:|----------|
| `0x00` | command word |
| `0x04` | command status |
| `0x08`, `0x0c` | command arguments, low and high |
| `0x10`, `0x14` | command results, low and high |
| `0x18`, `0x1c` | graphics pipe ring write and read cursors |
| `0x20`, `0x24` | transfer pipe ring cursors |
| `0x30`, `0x34` | blit pipe ring cursors |
| `0x38`, `0x3c` | compute pipe ring cursors |
| `0x40` | firmware version |
| `0x44` | command and pipe state |
| `0x90`, `0x94` | 64-bit time counter, low and high |

A ring is a circular buffer of command words in GPU memory with a cursor pair:
the producer writes words and advances the write cursor, and the command
processor consumes up to the read cursor.  Each pipe has its own cursor pair and
its own command vocabulary.

Pipes are provisioned by the kernel; their ownership model is:

| Pipe | Typical owner |
|------|---------------|
| Graphics | the graphics ring |
| Blit | inner graphics commands and blits |
| Compute | queues created by the kernel compute driver, submitted through a doorbell on the management ring |
| Transfer | its own ring, linear copies and fills |

Virtual-memory domains are handed out by the kernel; a submission names one
implicitly through the context it belongs to.

### Command words

The kernel-side command word selects a block and an operation inside it:

| Block | Operations |
|-------|------------|
| graphics pipe | queue, queue size, unqueue, reset |
| blit pipe | queue, queue size, unqueue |
| compute pipe | queue, queue size, unqueue, doorbell |
| transfer pipe | queue, queue size, unqueue |
| synchronisation | graphics, blit and compute sync |
| MMU | enable, directory, safe mode, page group, expand, flush, address, size, retry |
| compression tag | enable, disable, tag address, tag mask, count |
| frequency | update, power level |
| exception queue | queue, size, unqueue, history |
| doorbell | enable, disable |
| wave context save/restore | enable, disable |
| video memory window | bind address, read, write |

A command word is a block selector plus an operation selector, optionally with a
configuration field.  The names above are the kernel-visible vocabulary: a
user-mode driver never emits them.

## Submission interface

A user-mode driver's contract is:

1. Allocate a buffer; the kernel returns a handle.
2. Obtain a GPU virtual address for a range and map the buffer into it, with the
   access flags the consumer needs — an indirect buffer must be executable.
3. Build the indirect buffer's contents (see [Graphics Submission](submission.md)).
4. Submit the buffer address and its length, together with a completion fence and
   the buffers that must be resident.
5. Wait on the fence, or export it as a sync object.

The submission carries an ip type that selects the engine class: graphics,
compute or transfer.  A graphics submission additionally uses instance and ring
selectors which are zero in the established path.

Buffers enter the GPU as imported handles; there is no user-pointer path.  A
submission lists the buffers it needs, duplicate entries are collapsed, and a
submission whose list exceeds the limit is rejected rather than split, so a
driver must split its own work.

## Completion

Completion is a fence buffer whose word receives the kernel's sequence number,
plus a sync object that can be waited on or exported.  Waiting checks the fence
word first and falls back to the sync object, so a finished submission costs no
system call.  Sync objects follow the usual binary and timeline rules; a
submission appends its own fence as the last signal in the chain and is
serialised against the previous submission unless the dependency is overridden.

## UNRESOLVED

* The encoding and vocabulary of the stream layer above a submission: the
  command words that select a virtual-memory domain, execute an indirect buffer
  or poll memory are kernel-internal here, and the payload opcodes used by the
  envelope are emitted as fixed constants without documented names.
* The kernel's device interface: ioctl numbers, argument structures, interrupt
  numbers and queue provisioning are outside this documentation.
* The legal range of the submission's instance and ring selectors.
* The compute path's queue record layout as seen by a user-mode driver.
