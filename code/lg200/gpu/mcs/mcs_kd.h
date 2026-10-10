#pragma once

/*
 * The kernel-descriptor constants the MCS builder needs.
 *
 * `loongvk_mcs.c` was written against LoongVK's full `loongvk_kd.h`, but it
 * uses only five things from it.  Vendoring that whole header would pull in
 * `lco_abi.h` and with it a Mesa build dependency, so this file carries just
 * those five and nothing else.
 *
 * Values copied verbatim from
 *   src/mesa/src/loonggpu/vulkan/loongvk_kd.h
 * which is where their provenance is documented.  Do not change them here
 * without changing them there.
 */

#include <stdint.h>

/* loongvk_kd.h:40 - the kernel descriptor is a fixed 96 bytes.  The builder
 * itself does not need this (it defines its own KD_OFF_* offsets), but a caller
 * sizing a descriptor buffer does. */
#define LOONGVK_KD_SIZE 96u

/* loongvk_kd.h:63 - the .kd flag word.  Gates which configuration commands the
 * stream emits and supplies the group command's enable bits. */
#define LOONGVK_KD_FLAGS 0x0facu

/* loongvk_kd.h:70 - rsrc1 bits 1..8, kernel and group SGPR counts. */
#define LOONGVK_KD_KERNEL_SGPR_CNT 11u
#define LOONGVK_KD_GROUP_SGPR_CNT 3u
#define LOONGVK_KD_RSRC1                                                    \
   ((LOONGVK_KD_KERNEL_SGPR_CNT << 1) | (LOONGVK_KD_GROUP_SGPR_CNT << 5))

/* loongvk_kd.h:90-94 - the denorm/round encodings, and :99/:114/:124 the
 * packed rsrc1 float-mode words built from them. */
#define LOONGVK_KD_DENORM_FLUSH_SOURCE_OUTPUT 0u
#define LOONGVK_KD_DENORM_NO_FLUSH 3u
#define LOONGVK_KD_ROUND_NEAREST_EVEN 0u

#define LOONGVK_KD_FLOAT_MODE_DEFAULT                                       \
   ((LOONGVK_KD_ROUND_NEAREST_EVEN << 14) |                                 \
    (LOONGVK_KD_ROUND_NEAREST_EVEN << 16) |                                 \
    (LOONGVK_KD_DENORM_NO_FLUSH << 18) |                                    \
    (LOONGVK_KD_DENORM_NO_FLUSH << 20))

/* "the caller said nothing", kept distinct from DEFAULT on purpose. */
#define LOONGVK_KD_FLOAT_MODE_UNSET 0x80000000u

/*
 * loongvk_kd.c:41 - pack the SGPR/VGPR counts into the .kd +0x30 word.  VGPRs
 * are granulated in fours, SGPRs in eights, and the two land in different
 * halves of the byte.
 */
static inline uint32_t
loongvk_kd_granulated(uint32_t num_sgprs, uint32_t num_vgprs)
{
   const uint32_t gran_vgpr = (num_vgprs + 3u) / 4u;
   const uint32_t gran_sgpr = (num_sgprs + 7u) / 8u;
   return (gran_vgpr & 0x3fu) | ((gran_sgpr & 0xfu) << 6);
}
