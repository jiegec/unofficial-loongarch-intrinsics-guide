/*
 * Copyright 2026 LoongGPU contributors
 * SPDX-License-Identifier: MIT
 *
 * LG200 MCS dispatch builder.  See loongvk_mcs.h for the encoding's provenance
 * and analysis/lacm/lggpu-abi/MCS-ENCODING.md for the full evidence.
 *
 * The command order below is the vendor's, from
 * AqlQueue::SubmitAqlPacketLg200 case 2, and must stay in this order.
 */

#include "loongvk_mcs.h"

#include "mcs_kd.h"

#include <string.h>

/*
 * Every emitter ORs its bitfields into the dword already at the write cursor
 * instead of storing a fresh value (the vendor reads, masks, inserts).  Since
 * each command is emitted at a different offset the two are equivalent for a
 * zeroed buffer, but the ops below reproduce the masking exactly so a stream
 * built into a dirty buffer is still correct.
 */
struct writer {
   uint8_t *buf;
   size_t len;
   size_t cap;
   bool overflow;
};

static void
w_u32(struct writer *w, size_t off, uint32_t v)
{
   w->buf[off + 0] = (uint8_t)(v & 0xffu);
   w->buf[off + 1] = (uint8_t)((v >> 8) & 0xffu);
   w->buf[off + 2] = (uint8_t)((v >> 16) & 0xffu);
   w->buf[off + 3] = (uint8_t)((v >> 24) & 0xffu);
}

static void
w_u32_direct(uint8_t *p, uint32_t v)
{
   p[0] = (uint8_t)(v & 0xffu);
   p[1] = (uint8_t)((v >> 8) & 0xffu);
   p[2] = (uint8_t)((v >> 16) & 0xffu);
   p[3] = (uint8_t)((v >> 24) & 0xffu);
}

static uint32_t
r_u32(const uint8_t *p)
{
   return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) |
          ((uint32_t)p[3] << 24);
}

static bool
reserve(struct writer *w, size_t n)
{
   if (w->len + n > w->cap) {
      w->overflow = true;
      return false;
   }
   return true;
}

/* MC_ITEM_NOP_TAG { op:8, reserved:24 } - the vendor only clears the low byte. */
static void
emit_nop(struct writer *w)
{
   if (!reserve(w, 4))
      return;
   const uint32_t old = r_u32(w->buf + w->len);
   w_u32(w, w->len, (old & 0xffffff00u) | LOONGVK_MCS_OP_NOP);
   w->len += 4;
}

/*
 * AppendStscfgCommand(id, num, ...) - MC_ITEM_STSCFG_TAG
 * { op:8, offset:12, num:12 }.
 *
 * The header is 0x89 | ((id >> 2) << 8) | (num << 20): the id is a dword index,
 * so its low two bits are dropped, and the payload is num dwords.  The vendor
 * emits the header and the payload in one call and the payload length is exactly
 * 4 * num; the captured stream shows this for num = 1, 2, 3 and 4.
 */
static void
emit_stscfg(struct writer *w, uint32_t id, const uint32_t *dwords, uint32_t num)
{
   if (!reserve(w, 4u + 4u * num))
      return;
   const uint32_t old = r_u32(w->buf + w->len);
   uint32_t header = (old & ~((0xfffu << 8) | (0xfffu << 20))) |
                     (((id >> 2) & 0xfffu) << 8) | ((num & 0xfffu) << 20) |
                     LOONGVK_MCS_OP_STSCFG;
   w_u32(w, w->len, header);
   for (uint32_t i = 0; i < num; ++i)
      w_u32(w, w->len + 4u + 4u * i, dwords[i]);
   w->len += 4u + 4u * num;
}

/* MC_ITEM_GROUP_TAG { op:8, xid_enable:1, yid_enable:1, zid_enable:1 }. */
static void
emit_group(struct writer *w, uint32_t enable_bits, const uint32_t counts[3])
{
   if (!reserve(w, 16))
      return;
   const uint32_t old = r_u32(w->buf + w->len);
   const uint32_t header = (old & ~((1u << 8) | (1u << 9) | (1u << 10))) |
                           (enable_bits & ((1u << 8) | (1u << 9) | (1u << 10))) |
                           LOONGVK_MCS_OP_GROUP;
   w_u32(w, w->len + 0, header);
   w_u32(w, w->len + 4, counts[0]);
   w_u32(w, w->len + 8, counts[1]);
   w_u32(w, w->len + 12, counts[2]);
   w->len += 16;
}

/* MC_ITEM_WAIT_TAG { op:8, reserved1:8, sid:4, reserved2:12 } - sid 0 here. */
static void
emit_wait(struct writer *w)
{
   if (!reserve(w, 4))
      return;
   w_u32(w, w->len, LOONGVK_MCS_OP_WAIT);
   w->len += 4;
}

/* MC_ITEM_CINV_TAG { op:8, cache_inv_tag:4, reserved1:4, eop:1, reserved2:15 }. */
static void
emit_cinv(struct writer *w, uint32_t tag, uint32_t eop)
{
   if (!reserve(w, 4))
      return;
   uint32_t v = LOONGVK_MCS_OP_CINV | ((tag & 0xfu) << 8) | ((eop & 1u) << 16);
   w_u32(w, w->len, v);
   w->len += 4;
}

/* MC_ITEM_SBMT_TAG { op:8, skip:1, reserved:23 }.  The vendor always passes
 * skip = 0 for a dispatch and then writes the 64-bit signal after the header. */
static void
emit_sbmt(struct writer *w, uint32_t skip, uint64_t signal)
{
   if (!reserve(w, 12))
      return;
   const uint32_t old = r_u32(w->buf + w->len);
   const uint32_t header = (old & ~(1u << 8)) | ((skip & 1u) << 8) |
                           LOONGVK_MCS_OP_SBMT;
   w_u32(w, w->len + 0, header);
   w_u32(w, w->len + 4, (uint32_t)(signal & 0xffffffffu));
   w_u32(w, w->len + 8, (uint32_t)(signal >> 32));
   w->len += 12;
}

/* MC_ITEM_WBDBAR_TAG { op:8, reserved:24 } - emitted by AppendWbdbarCommand,
 * which is a vtable slot of its own (not AppendWaitCommand). */
static void
emit_wbdbar(struct writer *w)
{
   if (!reserve(w, 4))
      return;
   w_u32(w, w->len, LOONGVK_MCS_OP_WBDBAR);
   w->len += 4;
}

/*
 * The workgroup-size word the aperture command carries in its first payload
 * dword.  MEASURED, not inferred: the vendor OpenCL runtime was captured live
 * (AppendStscfgCommand, id 4, num 3) on this same GPU for a working multi-lane
 * launch and passes
 *
 *    local 1x1x1 -> 0x00000000     local 4x4x1 -> 0x00000c03
 *    local 8x1x1 -> 0x00000007     local 3x2x1 -> 0x00000402
 *    local 32x1x1-> 0x0000001f     local 2x2x2 -> 0x00100401
 *    local 64x1x1-> 0x0000003f     local 4x4x4 -> 0x00300c03
 *    local 512x1x1 -> 0x000001ff   local 1x512x1 -> 0x0007fc00
 *    local 32x32x1 -> 0x00007c1f
 *
 * i.e. one 10-bit field per dimension, each holding (size - 1).  The second and
 * third payload dwords are the group/private apertures, confirmed with the
 * vendor's own emitter (analysis/lacm: a distinctive first vararg appears at
 * +4 and the apertures at +8/+0c).
 *
 * This word is the ONLY place the local size reaches hardware: without it the
 * firmware derives one wave per workgroup, so exactly one lane runs (measured:
 * an atomicAdd counter returns 1 for local 8, 32 and 64 alike).  The earlier
 * 1x1x1-only capture could not reveal it because the packing is 0 there, which
 * is why the byte-identical differential harness never caught the omission.
 */
static uint32_t
shape_workgroup_size(const uint16_t group_size[3])
{
   return ((uint32_t)(group_size[0] - 1) & 0x3ffu) |
          (((uint32_t)(group_size[1] - 1) & 0x3ffu) << 10) |
          (((uint32_t)(group_size[2] - 1) & 0x3ffu) << 20);
}

/*
 * The launch-shape word, exactly as SubmitAqlPacketLg200 builds it:
 *
 *   rsrc1' = (((local_x*local_y*local_z) * 0x400000 + 0x7C00000) & 0xF8000000)
 *            | ((rsrc1 | (2 << 12)) & 0x7C02FFF)
 *            | 0x3C0000
 *
 * Verified against the live stream: rsrc1 0x76 with local 1x1x1 -> 0x083C2076
 * and with local 64x1x1 -> 0x103C2076.
 *
 * FLOAT MODE (the 0x3C0000 term).  This is not a reserved constant: those four
 * bits are rsrc1's floating-point execution mode, named by the vendor's own
 * reference runtime as
 *
 *   bit 14:15  float_round_mode_32        (LS_FLOAT_ROUND_MODE_*)
 *   bit 16:17  float_round_mode_16_64
 *   bit 18:19  float_denorm_mode_32       (LS_FLOAT_DENORM_MODE_*)
 *   bit 20:21  float_denorm_mode_16_64
 *
 * (analysis/lacm/lggpu-abi/KERNEL-DESCRIPTOR.md offset 52, and the symbol names
 * LS_FLOAT_DENORM_MODE_* / setFloatDenormMode32 are present in the vendor's
 * libvdd-runtime64.so.0.1.0 and libLLVMLGPUCodeGen.a).  The vendor backend even
 * carries needsDenormHandlingF32, i.e. the *compiler* is expected to account
 * for denormals, with this field selecting the hardware mode.
 *
 * The mask above (0x7C02FFF) deliberately CLEARS bits 14..21, so these fields
 * are entirely supplied by the term OR-ed in below; the mode is therefore not
 * inherited from rsrc1 and cannot be left to a default.
 *
 * Measured value (round 65).  The compute pipe's mode is observable: with
 * v_abs_f32 (whose semantics are a pure bit-AND, so only OUTPUT flushing can
 * change its result) on input 0x000003f0, sweeping the field gives
 *
 *   mode 0 -> 0x00000000   mode 1 -> 0x00000000
 *   mode 2 -> 0x000003f0   mode 3 -> 0x000003f0
 *
 * Combined with the vendor string order (FLUSH_SOURCE_OUTPUT, FLUSH_OUTPUT,
 * FLUSH_SOURCE, FLUSH_NONE) that identifies 0=FLUSH_SOURCE_OUTPUT,
 * 1=FLUSH_OUTPUT, 2=FLUSH_SOURCE, 3=FLUSH_NONE -- and the graphics pipe, which
 * has no such field, already flushes this same instruction correctly.
 *
 * LOONGVK_KD_FLOAT_MODE selects FLUSH_SOURCE_OUTPUT for both widths, which is
 * what Vulkan's DenormFlushToZero execution mode requires (flush denormals both
 * on input and on output).  The previous value, 0x3C0000, meant mode 3 =
 * FLUSH_NONE for both widths: that is why every fp32/fp64 denorm case failed
 * with the unflushed input reappearing in the result.  With mode 0 the fp32
 * input_args set goes from 36 Pass / 23 Fail to 58 Pass / 1 Fail.
 */
static uint32_t
shape_rsrc1(uint32_t rsrc1, const uint16_t group_size[3], uint32_t float_mode)
{
   const uint64_t waves = (uint64_t)group_size[0] * group_size[1] * group_size[2];
   const uint32_t high = (uint32_t)((waves * 0x400000ull + 0x7C00000ull) &
                                    0xF8000000ull);
   const uint32_t keep = (rsrc1 | (2u << 12)) & 0x7C02FFFu;
   /* A caller that did not specify a mode gets the hardware default, so a
    * hand-built descriptor still matches the vendor word.  UNSET is a bit
    * outside the mode fields precisely so it cannot collide with mode 0. */
   const uint32_t mode = (float_mode == LOONGVK_KD_FLOAT_MODE_UNSET)
                            ? LOONGVK_KD_FLOAT_MODE_DEFAULT
                            : float_mode;
   return high | keep | (mode & 0x003FC000u);
}

/*
 * The resource word, exactly as the vendor folds the shared size in:
 *
 *   rsrc0' = (rsrc0 & ~0x1FFC00) | ((shared * 8 + 0x3F8) & 0x1FFC00)
 *
 * The shared size field is granulated_shm_size (bits 23:13; 4-byte units of an
 * 8-byte granule plus a 0x3f8 bias), matching the .kd's own bitfield.  With no
 * shared memory this is the identity, which the live 0x42 confirms.
 */
static uint32_t
shape_rsrc0(uint32_t rsrc0, uint32_t shared_bytes)
{
   const uint32_t shm = (uint32_t)(((uint64_t)shared_bytes * 8u + 0x3F8u) &
                                   0x1FFC00u);
   return (rsrc0 & ~0x1FFC00u) | shm;
}

/*
 * .kd byte offsets this patch touches.  They are the ones the vendor writes at
 * dispatch time (analysis/lacm/vdd-full.c:64780-64788) and they agree with the
 * record's own named offsets in loongvk_kd.h.
 */
#define KD_OFF_GROUP_SIZE_X 4u
#define KD_OFF_GROUP_SIZE_Y 6u
#define KD_OFF_GROUP_SIZE_Z 8u
#define KD_OFF_GRID_SIZE_X 12u
#define KD_OFF_GRID_SIZE_Y 16u
#define KD_OFF_GRID_SIZE_Z 20u
#define KD_OFF_SHARED_SIZE 28u
#define KD_OFF_ENTRY 32u
#define KD_OFF_KERNARG 40u
#define KD_OFF_GRANULATED 48u
#define KD_OFF_RSRC1 52u

static void
put_u16(uint8_t *p, uint16_t v)
{
   p[0] = (uint8_t)(v & 0xffu);
   p[1] = (uint8_t)((v >> 8) & 0xffu);
}

static void
put_u64(uint8_t *p, uint64_t v)
{
   w_u32_direct(p, (uint32_t)(v & 0xffffffffu));
   w_u32_direct(p + 4, (uint32_t)(v >> 32));
}

bool
loongvk_mcs_patch_kd(uint8_t *kd, size_t kd_size,
                     const struct loongvk_mcs_dispatch *d)
{
   if (!kd || !d || kd_size < 96u)
      return false;
   for (unsigned i = 0; i < 3; ++i) {
      if (d->group_size[i] == 0)
         return false;
   }

   const uint32_t rsrc0 = shape_rsrc0(
      loongvk_kd_granulated(d->num_sgprs, d->num_vgprs), d->shared_segment_size);
   const uint32_t rsrc1 = shape_rsrc1(LOONGVK_KD_RSRC1, d->group_size, d->float_mode);

   /* Geometry.  grid_size is the GLOBAL size, the same value the AQL packet
    * carries and the group command divides by the local size. */
   put_u16(kd + KD_OFF_GROUP_SIZE_X, d->group_size[0]);
   put_u16(kd + KD_OFF_GROUP_SIZE_Y, d->group_size[1]);
   put_u16(kd + KD_OFF_GROUP_SIZE_Z, d->group_size[2]);
   w_u32_direct(kd + KD_OFF_GRID_SIZE_X, d->grid_size[0]);
   w_u32_direct(kd + KD_OFF_GRID_SIZE_Y, d->grid_size[1]);
   w_u32_direct(kd + KD_OFF_GRID_SIZE_Z, d->grid_size[2]);

   /* Shared size and the kernarg address, which moves per dispatch once the
    * caller stages its arguments. */
   w_u32_direct(kd + KD_OFF_SHARED_SIZE, d->shared_segment_size);
   put_u64(kd + KD_OFF_KERNARG, d->kernarg_address);
   put_u64(kd + KD_OFF_ENTRY, d->entry);

   /* The two resource words, shaped exactly as the stream's id-0x20 copy. */
   w_u32_direct(kd + KD_OFF_GRANULATED, rsrc0);
   w_u32_direct(kd + KD_OFF_RSRC1, rsrc1);

   return true;
}

bool
loongvk_mcs_build_dispatch(uint8_t *out, size_t out_size,
                           const struct loongvk_mcs_dispatch *d, size_t *out_len)
{
   if (!out || !d || !out_len || out_size < LOONGVK_MCS_MAX_LEN)
      return false;

   for (unsigned i = 0; i < 3; ++i) {
      if (d->group_size[i] == 0)
         return false;
   }

   /* The group command divides the grid by the local size, so a local size of
    * zero would fault.  The .kd flag word must be a subset of the defined bits:
    * an unknown bit would silently change which commands are emitted. */
   const uint32_t kd_known =
      LOONGVK_MCS_KD_EN_SGPR_PRIVATE_BUFFER | LOONGVK_MCS_KD_EN_SGPR_DISPATCH_PTR |
      LOONGVK_MCS_KD_EN_SGPR_QUEUE_PTR | LOONGVK_MCS_KD_EN_SGPR_KERNARG_PTR |
      LOONGVK_MCS_KD_EN_SGPR_DISPATCH_ID | LOONGVK_MCS_KD_EN_SGPR_FLAT_PRIVATE_INIT |
      LOONGVK_MCS_KD_EN_SGPR_PRIVATE_SIZE | LOONGVK_MCS_KD_EN_SGPR_GROUP_ID_X |
      LOONGVK_MCS_KD_EN_SGPR_GROUP_ID_Y | LOONGVK_MCS_KD_EN_SGPR_GROUP_ID_Z;
   if (d->kd_flags & ~kd_known)
      return false;

   memset(out, 0, LOONGVK_MCS_MAX_LEN);
   struct writer w = { .buf = out, .len = 0, .cap = LOONGVK_MCS_MAX_LEN,
                       .overflow = false };

   const uint32_t rsrc0 = shape_rsrc0(
      loongvk_kd_granulated(d->num_sgprs, d->num_vgprs), d->shared_segment_size);
   const uint32_t rsrc1 = shape_rsrc1(LOONGVK_KD_RSRC1, d->group_size, d->float_mode);

   /*
    * 1. The segment apertures.  num = 3 is the vendor's constant and, as the
    *    first command, its id is always 4 regardless of the flags.
    */
   {
      const uint32_t ap[3] = { shape_workgroup_size(d->group_size),
                               d->group_aperture_hi,
                               d->private_aperture_hi };
      emit_stscfg(&w, 4, ap, 3);
   }

   /*
    * 2. The implicit block: the kernel entry (as a lo/hi dword pair, not a
    *    64-bit store) and the two resource words.  The id 0x20 is fixed.
    */
   {
      const uint32_t blk[4] = {
         (uint32_t)(d->entry & 0xffffffffu), (uint32_t)(d->entry >> 32),
         rsrc0, rsrc1,
      };
      emit_stscfg(&w, 0x20, blk, 4);
   }

   /*
    * 3. The .kd flag-driven configuration commands.  cid starts at 0x40; the
    *    first command advances it with OR 0x10 and every later one with + 8,
    *    exactly as the vendor does.  The order is the vendor's, and the id of
    *    each command therefore depends on which earlier ones fired.
    */
   {
      uint32_t cid = LOONGVK_MCS_CFG_FIRST_ID;

      if (d->kd_flags & LOONGVK_MCS_KD_EN_SGPR_PRIVATE_BUFFER) {
         emit_stscfg(&w, cid, d->scratch_resource_descriptor, 4);
         cid |= 0x10;
      }
      if (d->kd_flags & LOONGVK_MCS_KD_EN_SGPR_DISPATCH_PTR) {
         const uint32_t kd[2] = { (uint32_t)(d->kernel_object & 0xffffffffu),
                                  (uint32_t)(d->kernel_object >> 32) };
         emit_stscfg(&w, cid, kd, 2);
         cid += 8;
      }
      if (d->kd_flags & LOONGVK_MCS_KD_EN_SGPR_QUEUE_PTR) {
         const uint32_t ap[2] = { d->group_aperture_hi, d->group_aperture_hi };
         emit_stscfg(&w, cid, ap, 2);
         cid += 8;
      }
      if (d->kd_flags & LOONGVK_MCS_KD_EN_SGPR_KERNARG_PTR) {
         const uint32_t ka[2] = { (uint32_t)(d->kernarg_address & 0xffffffffu),
                                  (uint32_t)(d->kernarg_address >> 32) };
         emit_stscfg(&w, cid, ka, 2);
         cid += 8;
      }
      if (d->kd_flags & LOONGVK_MCS_KD_EN_SGPR_DISPATCH_ID) {
         const uint32_t zero[2] = { 0, 0 };
         emit_stscfg(&w, cid, zero, 2);
         cid += 8;
      }
      if (d->kd_flags & LOONGVK_MCS_KD_EN_SGPR_FLAT_PRIVATE_INIT) {
         const uint32_t sb[2] = { d->scratch_backing_lo, d->scratch_backing_hi };
         emit_stscfg(&w, cid, sb, 2);
         cid += 8;
      }
      if (d->kd_flags & LOONGVK_MCS_KD_EN_SGPR_PRIVATE_SIZE) {
         const uint32_t v =
            d->has_scratch ? d->scratch_slot_size_log2 : d->private_segment_size;
         emit_stscfg(&w, cid, &v, 1);
      }
   }

   /*
    * 4. AppendDispatchCommand.  On LG200 this emits nothing (the LG210 variant
    *    writes 0x8e + a 64-bit operand and must not be used here), so the step
    *    is present only as this comment.
    */

   /*
    * 5. The group command.  The three dwords are a WORKGROUP count: writing the
    *    GLOBAL size instead (round-28 experiment) made the GPU launch one
    *    workgroup per work item - all invocations ran, but every one reported
    *    gl_LocalInvocationID = 0 and a different gl_WorkGroupID, so the grouping
    *    was destroyed.  Dividing by the local size, as the vendor's emitter
    *    does, is correct.
    *
    *    The per-workgroup thread count does NOT come from here: it is the
    *    packed workgroup size in the aperture command's first payload dword
    *    (see shape_workgroup_size).  The three enable bits come from kd_flags
    *    bits 9/10/11.
    */
   {
      const uint32_t counts[3] = {
         d->grid_size[0] / d->group_size[0],
         d->grid_size[1] / d->group_size[1],
         d->grid_size[2] / d->group_size[2],
      };
      const uint32_t enable = ((d->kd_flags >> 9) & 7u) << 8;
      emit_group(&w, enable, counts);
   }

   /* 6. wait, 7. cinv(0xf, 0), 8. the completion signal, 9. wbdbar. */
   emit_wait(&w);
   emit_cinv(&w, 0xf, 0);
   emit_sbmt(&w, 0, d->completion_signal);
   emit_wbdbar(&w);

   if (w.overflow)
      return false;

   /*
    * 10. Pad.  The vendor pads to 16 bytes; this driver additionally pads to 64
    *     so the byte write pointer it publishes stays 64-byte aligned, which is
    *     what the kernel's DOORBELL validation demands.  MCS is self-delimiting,
    *     so NOP padding is always safe.
    */
   while ((w.len & 0x3fu) != 0)
      emit_nop(&w);
   if (w.overflow || w.len > LOONGVK_MCS_MAX_LEN)
      return false;

   *out_len = w.len;
   return true;
}




