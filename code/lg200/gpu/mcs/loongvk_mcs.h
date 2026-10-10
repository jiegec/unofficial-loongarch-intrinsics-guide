/*
 * Copyright 2026 LoongGPU contributors
 * SPDX-License-Identifier: MIT
 *
 * LG200 MCS (Micro-Code Stream) dispatch builder.
 *
 * The CPIPE user ring does NOT carry HSA AQL packets.  For a kernel dispatch the
 * vendor runtime translates the AQL packet into MCS commands and memcpy's those
 * into the ring (analysis/lacm/vdd-decompiled/SubmitAqlPacketLg200.c, the
 * write into the ring being AqlQueue::SubmitMCS).  A perfectly formed AQL packet
 * at the right write pointer is therefore ignored: the firmware is waiting for
 * MCS.  This file builds that stream.
 *
 * Everything here is measured, never inferred from the AQL spec or from AMD:
 *
 *   - the vendor ships full DWARF, so the per-command header structs come from
 *     its own vdd/src/core/inc/command_micro_codes.h (gdb ptype /o of
 *     lacr::LS::MC_ITEM_*_TAG);
 *   - AqlQueue::SubmitAqlPacketLg200 case 2 gives the command sequence and each
 *     argument, with the argument registers read from objdump (the decompiler
 *     misprints several offsets);
 *   - the decoded encoding was replayed through the vendor's own emitters in a
 *     host harness (analysis/lacm/mcs_ref.c) and matched byte for byte against a
 *     live 144-byte capture taken at AqlQueue::SubmitMCS during a real kernel
 *     launch.
 *
 * The full evidence is analysis/lacm/lggpu-abi/MCS-ENCODING.md.
 *
 * This builder is deliberately pure: it writes bytes into a caller buffer and
 * allocates nothing, so a unit test can pin the stream against the captured
 * reference before anything reaches the CPIPE.
 */

#ifndef LOONGVK_MCS_H
#define LOONGVK_MCS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * One command is a little-endian dword with the opcode in the low byte,
 * followed by an opcode-specific payload of further dwords.  The vendor's
 * MC_ITEM_*_TAG structs name the header bitfields.
 */
#define LOONGVK_MCS_OP_NOP 0x00u
#define LOONGVK_MCS_OP_WB32 0x10u
#define LOONGVK_MCS_OP_WB64 0x11u
#define LOONGVK_MCS_OP_WBDBAR 0x16u
#define LOONGVK_MCS_OP_POLL 0x83u
#define LOONGVK_MCS_OP_SIGNAL 0x86u
#define LOONGVK_MCS_OP_GROUP 0x87u
#define LOONGVK_MCS_OP_BAR_VAL 0x88u
#define LOONGVK_MCS_OP_STSCFG 0x89u
#define LOONGVK_MCS_OP_SBMT 0x8au
#define LOONGVK_MCS_OP_CINV 0x8bu
#define LOONGVK_MCS_OP_SYNC 0x8cu
#define LOONGVK_MCS_OP_WAIT 0x8du
#define LOONGVK_MCS_OP_DISPATCH 0x8eu

/*
 * The vendor's GetMcsMaxLenPerPacket() returns 256 and allocates exactly that,
 * so no single dispatch may exceed it.  The concrete stream for a normal
 * shader is far shorter (140 bytes for the captured add.cl dispatch).
 */
#define LOONGVK_MCS_MAX_LEN 256u

/*
 * The .kd +0x00 flag dword.  These are the kernel_descriptor_t bits that gate
 * the configuration stscfg commands, taken from the vendor's own bitfield names
 * (en_sgpr_private_buffer ... en_sgpr_group_id_z).  The measured value for a
 * normal compute shader - the captured .kd and every one the offline producer
 * emits - is 0x0fac, i.e. bits 2, 3, 5, 7 and 8.
 */
#define LOONGVK_MCS_KD_EN_SGPR_PRIVATE_BUFFER (1u << 2)
#define LOONGVK_MCS_KD_EN_SGPR_DISPATCH_PTR (1u << 3)
#define LOONGVK_MCS_KD_EN_SGPR_QUEUE_PTR (1u << 4)
#define LOONGVK_MCS_KD_EN_SGPR_KERNARG_PTR (1u << 5)
#define LOONGVK_MCS_KD_EN_SGPR_DISPATCH_ID (1u << 6)
#define LOONGVK_MCS_KD_EN_SGPR_FLAT_PRIVATE_INIT (1u << 7)
#define LOONGVK_MCS_KD_EN_SGPR_PRIVATE_SIZE (1u << 8)
#define LOONGVK_MCS_KD_EN_SGPR_GROUP_ID_X (1u << 9)
#define LOONGVK_MCS_KD_EN_SGPR_GROUP_ID_Y (1u << 10)
#define LOONGVK_MCS_KD_EN_SGPR_GROUP_ID_Z (1u << 11)

/* The id the config block starts from, and the step each command advances it by.
 * These are the vendor's own constants, not sizes: the first command advances
 * with OR 0x10 and every later one adds 8. */
#define LOONGVK_MCS_CFG_FIRST_ID 0x40u

struct loongvk_mcs_dispatch {
   /* .kd +0x00.  Gates which configuration commands are emitted, and supplies
    * the group command's three enable bits (bits 9/10/11). */
   uint32_t kd_flags;
   /* GPU VA of the 96-byte .kd record (the AQL kernel_object). */
   uint64_t kernel_object;
   /* GPU VA of the first instruction - the .kd's own +0x20 word.  This must be
    * the same value the descriptor carries so the implicit block and the .kd
    * cannot disagree. */
   uint64_t entry;
   /* GPU VA of the implicit-argument block. */
   uint64_t kernarg_address;
   /* GPU VA of the 64-bit signal the device increments once per completed
    * packet; the AQL completion_signal. */
   uint64_t completion_signal;
   /* Register counts from the compiler.  They are packed exactly as the .kd's
    * +0x30 word, by loongvk_kd_granulated(), so the two cannot drift. */
   uint32_t num_sgprs;
   uint32_t num_vgprs;
   /* Declared shared-memory bytes, which land in granulated_shm_size. */
   uint32_t shared_segment_size;
   /* Local size (the shader's workgroup size).  The aperture command's first
    * payload dword is the PACKED workgroup size derived from this - see
    * shape_workgroup_size() in loongvk_mcs.c - so it is deliberately not a
    * caller field: every dispatch must carry it or the hardware runs one lane. */
   uint16_t group_size[3];
   /* AQL grid_size: the GLOBAL size in work-items, NOT the workgroup count.
    * The group command divides this by group_size to obtain the count, so a
    * caller holding a workgroup count must multiply by group_size first. */
   uint32_t grid_size[3];
   /* Configuration words the vendor takes from the queue and scratch state.
    * They stay zero when a dispatch has no private segment. */
   uint32_t group_aperture_hi;
   uint32_t private_aperture_hi;
   uint32_t scratch_resource_descriptor[4];
   uint32_t scratch_backing_lo;
   uint32_t scratch_backing_hi;
   /* The .kd +0x18 private_segment_size, used when bit 0x100 selects it. */
   uint32_t private_segment_size;
   /* Mirrors the vendor's "queue has a scratch region" branch: when true, bit
    * 0x100 of kd_flags carries a slot-size log2 instead of the private size.
    * This driver uses one whole segment per dispatch, so this remains false. */
   bool has_scratch;
   uint32_t scratch_slot_size_log2;
   /*
    * rsrc1 bits 14:21 - the dispatch's floating-point execution mode.  Taken
    * from the shader's SPIR-V float-control execution modes (see
    * loongvk_denorm_mode_for_nir); zero means "hardware default", which the
    * measurement identifies as FLUSH_NONE.
    */
   uint32_t float_mode;
};

/*
 * Build the MCS stream for one kernel dispatch into @out.
 *
 * @out must have room for LOONGVK_MCS_MAX_LEN bytes.  On success @out_len
 * receives the emitted length, already padded to a 64-byte multiple: the vendor
 * pads to 16 bytes, and this driver pads further to 64 so the byte write pointer
 * it publishes stays 64-byte aligned, which is what the kernel's DOORBELL
 * validation (IS_ALIGNED(wptr, 64)) requires.  MCS is a self-delimiting dword
 * stream, so extra NOP padding is always legal.
 *
 * Returns false on a missing argument, a short buffer, a bad kd_flags word, a
 * zero group dimension, or a dispatch whose natural length would exceed
 * LOONGVK_MCS_MAX_LEN - i.e. anything that would otherwise reach the ring as a
 * silently malformed stream.
 */
bool loongvk_mcs_build_dispatch(uint8_t *out, size_t out_size,
                                const struct loongvk_mcs_dispatch *d,
                                size_t *out_len);


/*
 * Patch the per-dispatch fields of the 96-byte .kd record at @kd.
 *
 * The vendor does NOT leave the descriptor frozen at its creation values:
 * AqlQueue::SubmitAqlPacketLg200 copies the AQL packet's workgroup size, grid
 * size, shared size and kernarg address into the record, and recomputes and
 * writes the two resource words back into it, on every dispatch
 * (analysis/lacm/vdd-full.c:64780-64809; @kd is the packet's kernel_object).
 * The firmware reads those words when it launches the waves, so a record that
 * keeps creation-time zeros describes a different launch than the one asked
 * for - and in particular an unshaped rsrc1 does not carry the wave geometry.
 *
 * The resource words are byte-identical to the ones
 * loongvk_mcs_build_dispatch() puts in the id-0x20 configuration command,
 * because both call the same two shaping helpers, so the descriptor and the
 * stream cannot drift apart.
 *
 * Returns false for a missing argument, a short buffer, or a zero group
 * dimension (which the group command would divide by).
 */
bool loongvk_mcs_patch_kd(uint8_t *kd, size_t kd_size,
                          const struct loongvk_mcs_dispatch *d);

/*
 * Choose the ring byte offset for the next MCS stream.
 *
 * The ring is circular and the driver publishes the END of the stream as the
 * byte write pointer, which the kernel's DOORBELL path then rejects with
 * -EINVAL unless it is 64-byte aligned AND strictly below the ring size
 * (loonggpu_drm_userq.c:272-280).  A stream may occupy up to
 * LOONGVK_MCS_MAX_LEN bytes, so a candidate offset @slot is usable only when
 *
 *     slot + LOONGVK_MCS_MAX_LEN < LOONGVK_CS_RING_SIZE
 *
 * i.e. the WORST-CASE stream has to fit, not just the current one.  Testing
 * `slot + MAX_LEN <= ring_size` (or, as an earlier revision did, comparing
 * with `>`) leaves the exact boundary slot ring_size - MAX_LEN admissible: a
 * maximal stream started there would end at ring_size and the doorbell would be
 * refused.  This driver does not wrap by copying across the end (the vendor's
 * SubmitMCS does, but the firmware's read index is a byte counter, so a
 * straddling stream is not expressible here); it starts a fresh stream at
 * @first instead.  Because MCS is a self-delimiting dword stream, skipping the
 * tail is always legal.
 *
 * Returns @slot unchanged when it fits, else @first.  @slot itself must already
 * be 64-byte aligned and below the ring size, which the caller maintains.
 */
static inline uint32_t
loongvk_mcs_ring_offset(uint32_t slot, uint32_t ring_size, uint32_t first)
{
   if (slot >= ring_size ||
       (uint64_t)slot + LOONGVK_MCS_MAX_LEN >= ring_size)
      return first;
   return slot;
}

/*
 * The byte offset just past the 64-byte-aligned span a stream of @len bytes (as
 * returned by loongvk_mcs_build_dispatch) occupies.  @len must be non-zero and
 * no greater than LOONGVK_MCS_MAX_LEN.
 */
static inline uint32_t
loongvk_mcs_next_offset(uint32_t slot, size_t len)
{
   return slot + (uint32_t)((len + 63u) & ~(size_t)63u);
}

#endif /* LOONGVK_MCS_H */
