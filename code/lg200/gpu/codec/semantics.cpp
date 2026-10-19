#include "semantics.hpp"

#include "generated/lg200_codec.inc"

#include <cstddef>
#include <cstring>
#include <vector>

namespace lgpu::isa {
namespace {

// Operand semantics per rule class. The class of each mnemonic and
// the per-mnemonic tuple overrides come from the generated table; the
// field-to-semantic mapping below is the codec contract.
constexpr FieldRule kImmAluRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"IK32", FieldSem::Immediate32, 1, nullptr, false, false},
};

constexpr FieldRule kImmMovRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"IJ32", FieldSem::Immediate32, 1, nullptr, false, false},
};

constexpr FieldRule kImmMovScalarRules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"IJ32", FieldSem::Immediate32, 1, nullptr, false, false},
};

constexpr FieldRule kAlu3Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"GJ11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"GK11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"GA11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"M.mod2", FieldSem::Modifier, 1, "mod2", false, true},
    {"M.clmp1", FieldSem::Modifier, 1, "clmp1", false, true},
};

constexpr FieldRule kAluCarryRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"GJ11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"GK11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"RD8", FieldSem::Vcc8, 1, nullptr, false, false},
    {"M.mod2", FieldSem::Modifier, 1, "mod2", false, true},
    {"M.clmp1", FieldSem::Modifier, 1, "clmp1", false, true},
};

// agentA: VOP compares. The result destination is a plain scalar register
// printed as 'sN' in canonical forms, not vcc(sN); FieldSem::Vcc8 is for
// the implicit carry output of the alu-carry class.
constexpr FieldRule kCmpRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"GJ11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"GK11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"M.mod2", FieldSem::Modifier, 1, "mod2", false, true},
    {"M.clmp1", FieldSem::Modifier, 1, "clmp1", false, true},
};

constexpr FieldRule kCmpImmRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"IK32", FieldSem::Immediate32, 1, nullptr, false, false},
};

// agentB: s_*_imm scalar ALU; the "(scc)" prefix marks the SCC side write.
constexpr FieldRule kSop2ImmRules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"RJ8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"IK32", FieldSem::Immediate32, 1, nullptr, false, false},
};

// s_cmp*: compare result written to RD8, encoding a scalar register or SCC
// (0x80). Same operand order as sop2.
constexpr FieldRule kSopCmpRules[] = {
    {"RD8", FieldSem::ScalarCmp8, 1, nullptr, true, false},
    {"GJ8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"RK8", FieldSem::GeneralSource8, 1, nullptr, false, false},
};

// s_cmp_imm_*: no destination field; the result is implicitly SCC, printed
// as the "SCC, " assembly prefix.
constexpr FieldRule kSopCmpImmRules[] = {
    {"RJ8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"IK32", FieldSem::Immediate32, 1, nullptr, false, false},
};

// s_cmp_*_u64: both sources are 64-bit register pairs.
constexpr FieldRule kSopCmp64Rules[] = {
    {"RD8", FieldSem::ScalarCmp8, 1, nullptr, true, false},
    {"GJ8", FieldSem::GeneralSource8, 2, nullptr, false, false},
    {"RK8", FieldSem::GeneralSource8, 2, nullptr, false, false},
};

constexpr FieldRule kAlu2Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"GJ11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"GK11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"M.mod2", FieldSem::Modifier, 1, "mod2", false, true},
    {"M.clmp1", FieldSem::Modifier, 1, "clmp1", false, true},
};

constexpr FieldRule kAlu1Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"GJ11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"M.mod2", FieldSem::Modifier, 1, "mod2", false, true},
    {"M.clmp1", FieldSem::Modifier, 1, "clmp1", false, true},
};

constexpr FieldRule kFirstV2sRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"GJ11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"M.mod2", FieldSem::Modifier, 1, "mod2", false, true},
    {"M.clmp1", FieldSem::Modifier, 1, "clmp1", false, true},
};

constexpr FieldRule kSop2Rules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"GJ8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"RK8", FieldSem::GeneralSource8, 1, nullptr, false, false},
};

constexpr FieldRule kSop1Rules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"GJ8", FieldSem::GeneralSource8, 1, nullptr, false, false},
};

constexpr FieldRule kSop3Rules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"GJ8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"RK8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"RA8", FieldSem::GeneralSource8, 1, nullptr, false, false},
};

constexpr FieldRule kSioEmitRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"GK11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"GA11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"M.mod2", FieldSem::Modifier, 1, "mod2", false, true},
    {"M.clmp1", FieldSem::Modifier, 1, "clmp1", false, true},
};

constexpr FieldRule kThreadIdRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
};

constexpr FieldRule kBuffLoadRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VQ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"RJ8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RK8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

constexpr FieldRule kBuffStoreRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VQ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"RK8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RA8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// Buffer atomics keep the VQ8 field unmodeled: canonical forms print no
// vaddr operand and the template preserves the field bits.
constexpr FieldRule kBuffAtomicRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VQ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"RJ8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RK8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

constexpr FieldRule kTexLoadRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8x4", FieldSem::VectorDest8, 4, nullptr, true, false},
    {"VJ8x8", FieldSem::VectorSource8, 3, nullptr, false, false},
    {"RK8x8", FieldSem::Scalar8, 8, nullptr, false, false},
    {"M.un1", FieldSem::Modifier, 1, "un", false, true},
    {"M.na1", FieldSem::Modifier, 1, "na", false, true},
    {"M.rsz1", FieldSem::Modifier, 1, "rsz", false, true},
    {"M.dmask4", FieldSem::Modifier, 1, "dmask", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

constexpr FieldRule kTexLoadRk9Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8x4", FieldSem::VectorDest8, 4, nullptr, true, false},
    {"VJ8x8", FieldSem::VectorSource8, 3, nullptr, false, false},
    {"RK9x8", FieldSem::Scalar8, 8, nullptr, false, false},
    {"M.un1", FieldSem::Modifier, 1, "un", false, true},
    {"M.na1", FieldSem::Modifier, 1, "na", false, true},
    {"M.dmask4", FieldSem::Modifier, 1, "dmask", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

constexpr FieldRule kInterpLoadRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"RJ8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"UO8", FieldSem::Immediate8, 1, nullptr, false, false},
};

// ================= agentC: memory-image =================
// tex-gather: pred, vdst-quad, coords-oct, desc-octet, sampler-quad, mods.
constexpr FieldRule kTexGatherRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8x4", FieldSem::VectorDest8, 4, nullptr, true, false},
    {"VJ8x8", FieldSem::VectorSource8, 3, nullptr, false, false},
    {"RK8x8", FieldSem::Scalar8, 8, nullptr, false, false},
    {"RA8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"M.un1", FieldSem::Modifier, 1, "un", false, true},
    {"M.na1", FieldSem::Modifier, 1, "na", false, true},
    {"M.rsz1", FieldSem::Modifier, 1, "rsz", false, true},
    {"M.dmask4", FieldSem::Modifier, 1, "dmask", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// vbuff-load without vaddr: pred, vdst, desc-quad, offset-src, imm12, mods.
constexpr FieldRule kVbuffLoadRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"RJ8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RK8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// vbuff-store without vaddr: pred, vdata, desc-quad, offset-src, imm12, mods.
constexpr FieldRule kVbuffStoreRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"RK8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RA8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// flat-load: pred, vdst, vaddr-pair, desc-pair, mods.
constexpr FieldRule kFlatLoadRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VJ8x2", FieldSem::VectorSource8, 2, nullptr, false, false},
    {"RK8x2", FieldSem::Scalar8, 2, nullptr, false, false},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// flat-store: pred, vdata, vaddr-pair, desc-pair, mods.
constexpr FieldRule kFlatStoreRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8x2", FieldSem::VectorSource8, 2, nullptr, false, false},
    {"RA8x2", FieldSem::Scalar8, 2, nullptr, false, false},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// tex-store: pred, vdata-quad, coords-oct, desc-octet, mods.
constexpr FieldRule kTexStoreRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8x4", FieldSem::VectorSource8, 4, nullptr, false, false},
    {"VK8x8", FieldSem::VectorSource8, 8, nullptr, false, false},
    {"RA8x8", FieldSem::Scalar8, 8, nullptr, false, false},
    {"M.un1", FieldSem::Modifier, 1, "un", false, true},
    {"M.na1", FieldSem::Modifier, 1, "na", false, true},
    {"M.rsz1", FieldSem::Modifier, 1, "rsz", false, true},
    {"M.dmask4", FieldSem::Modifier, 1, "dmask", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// sm-copy / sm-pred: predicate only, remaining bits are template.
constexpr FieldRule kSmPredRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
};

// sm-load: pred, vdst, vaddr; X0_16 reserved.
constexpr FieldRule kSmLoadRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
};

// sm-store: pred, vaddr, vdata; X0_16 reserved.
constexpr FieldRule kSmStoreRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8", FieldSem::VectorSource8, 1, nullptr, false, false},
};

constexpr FieldRule kSmPermuteRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8", FieldSem::VectorSource8, 1, nullptr, false, false},
};

// sbuff-load: sdst, desc, saddr, imm20; no predicate.
constexpr FieldRule kSbuffLoadRules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"RJ8", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"GK8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"UO20", FieldSem::Immediate20, 1, nullptr, false, false},
};

// ================= agentD: atomics =================
constexpr FieldRule kBuffAtomic64Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8x2", FieldSem::VectorDest8, 2, nullptr, true, false},
    {"VQ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"RJ8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RK8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// dec/inc/sub: 32-bit dst with vdata field (closed decoder shows v0).
constexpr FieldRule kBuffAtomicNoSrcRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VQ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"RJ8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RK8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// cmpswap_b32: VQ8 src plus 2-wide dst.
constexpr FieldRule kBuffAtomicCmpSwapRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8x2", FieldSem::VectorDest8, 2, nullptr, true, false},
    {"VQ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"RJ8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RK8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

constexpr FieldRule kBuffAtomic128Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8x4", FieldSem::VectorDest8, 4, nullptr, true, false},
    {"RJ8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"RK8", FieldSem::OffsetSource8, 1, nullptr, false, false},
    {"UO12", FieldSem::Immediate12, 1, nullptr, false, false},
    {"M.offen1", FieldSem::Modifier, 1, "offen", false, true},
    {"M.idxen1", FieldSem::Modifier, 1, "idxen", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

constexpr FieldRule kSmAtomic32Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
};

constexpr FieldRule kSmAtomic64Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8x2", FieldSem::VectorSource8, 2, nullptr, false, false},
    {"VD8x2", FieldSem::VectorDest8, 2, nullptr, true, false},
};

constexpr FieldRule kSmAtomicCmpswap32Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VA8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
};

constexpr FieldRule kSmAtomicCmpswap64Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VA8x2", FieldSem::VectorSource8, 2, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8x2", FieldSem::VectorSource8, 2, nullptr, false, false},
    {"VD8x2", FieldSem::VectorDest8, 2, nullptr, true, false},
};

constexpr FieldRule kFlatAtomic32Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VJ8x2", FieldSem::VectorSource8, 2, nullptr, false, false},
    {"RK8x2", FieldSem::Scalar8, 2, nullptr, false, false},
    {"VA8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

constexpr FieldRule kFlatAtomic64Rules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8x2", FieldSem::VectorDest8, 2, nullptr, true, false},
    {"VJ8x2", FieldSem::VectorSource8, 2, nullptr, false, false},
    {"RK8x2", FieldSem::Scalar8, 2, nullptr, false, false},
    {"VA8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

constexpr FieldRule kTexAtomicRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"VJ8x8", FieldSem::VectorSource8, 8, nullptr, false, false},
    {"RK8x8", FieldSem::Scalar8, 8, nullptr, false, false},
    {"M.un1", FieldSem::Modifier, 1, "un", false, true},
    {"M.na1", FieldSem::Modifier, 1, "na", false, true},
    {"M.rsz1", FieldSem::Modifier, 1, "rsz", false, true},
    {"M.dmask4", FieldSem::Modifier, 1, "dmask", false, true},
    {"M.bl11", FieldSem::Modifier, 1, "bl1", false, true},
    {"M.bl21", FieldSem::Modifier, 1, "bl2", false, true},
};

// ================= agentE: control/sio/sm =================
constexpr FieldRule kSopImmRules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"RJ8", FieldSem::Scalar8, 1, nullptr, false, false},
    {"IK32", FieldSem::Immediate32, 1, nullptr, false, false},
};

constexpr FieldRule kSmemLoadRules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"RJ8x4", FieldSem::ScalarQuad8, 4, nullptr, false, false},
    {"GK8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"UO20", FieldSem::Immediate20, 1, nullptr, false, false},
};

constexpr FieldRule kPrefixImmRules[] = {
    {"IJ32", FieldSem::Immediate32, 1, nullptr, false, false},
};

// flag: k/vm/vs flag bits (closed decoder: flag k / flag vm / flag vs).
constexpr FieldRule kFlagRules[] = {
    {"X0_1", FieldSem::Immediate8, 1, nullptr, false, false},
    {"M.vm1", FieldSem::Modifier, 1, "vm", false, true},
    {"X1_2", FieldSem::Immediate8, 1, nullptr, false, false},
};

// jirl: link-and-jump; dst pair + src pair + 40-bit offset
// (closed decoder: dst@HI[23:16], src@HI[15:8]).
constexpr FieldRule kCtrlJirlRules[] = {
    {"RD8", FieldSem::Scalar8, 2, nullptr, true, false},
    {"RJ8", FieldSem::Scalar8, 2, nullptr, false, false},
    {"X0_32", FieldSem::Immediate32, 1, nullptr, false, false},
};

constexpr FieldRule kCtrlPairRules[] = {
    {"RD8", FieldSem::Scalar8, 2, nullptr, true, false},
};

constexpr FieldRule kCtrlPairSrcRules[] = {
    {"RJ8", FieldSem::Scalar8, 2, nullptr, false, false},
};

constexpr FieldRule kCtrlBranchCondRules[] = {
    {"GJ8", FieldSem::Scalar8, 1, nullptr, false, false},
    {"X0_32", FieldSem::Immediate32, 1, nullptr, false, false},
};

constexpr FieldRule kV2sRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"GJ11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"GK11", FieldSem::GeneralSource11, 1, nullptr, false, false},
    {"M.mod2", FieldSem::Modifier, 1, "mod2", false, true},
    {"M.clmp1", FieldSem::Modifier, 1, "clmp1", false, true},
};

constexpr FieldRule kJumpStackRules[] = {
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"RA8", FieldSem::Scalar8, 1, nullptr, false, false},
    {"X1_8", FieldSem::Scalar8, 1, nullptr, false, false},
    {"X2_23", FieldSem::Immediate23, 1, nullptr, false, false},
};

// s_select_b32: RA8 accepts the "SCC" selector encoding 0x80.
constexpr FieldRule kSop3SccRules[] = {
    {"RD8", FieldSem::Scalar8, 1, nullptr, true, false},
    {"GJ8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"RK8", FieldSem::GeneralSource8, 1, nullptr, false, false},
    {"RA8", FieldSem::SccSource8, 1, nullptr, false, false},
};

// SIO family slot roles used by siowrrot_imm and siowrrot_imm_c4.
constexpr FieldRule kSioRotRules[] = {
    {"X1_1", FieldSem::SioId, 1, nullptr, false, false},
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"X0_12", FieldSem::SioChannum, 1, nullptr, false, false},
    {"IA16", FieldSem::Immediate16, 1, nullptr, false, false},
};

constexpr FieldRule kSioRotIdRules[] = {
    {"X1_1", FieldSem::SioId, 1, nullptr, false, false},
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VA8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VK8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"X0_12", FieldSem::SioChannum, 1, nullptr, false, false},
    {"X2_16", FieldSem::Immediate16, 1, nullptr, false, false},
};

constexpr FieldRule kSioReadRules[] = {
    {"X1_1", FieldSem::SioId, 1, nullptr, false, false},
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"X0_12", FieldSem::SioChannum, 1, nullptr, false, false},
    {"IK16", FieldSem::Immediate16, 1, nullptr, false, false},
};

constexpr FieldRule kSioImmRules[] = {
    {"X1_1", FieldSem::SioId, 1, nullptr, false, false},
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"X0_12", FieldSem::SioChannum, 1, nullptr, false, false},
    {"IK16", FieldSem::Immediate16, 1, nullptr, false, false},
    {"X2_12", FieldSem::SioChan, 1, nullptr, false, false},
};

constexpr FieldRule kSioImmReadRules[] = {
    {"X1_1", FieldSem::SioId, 1, nullptr, false, false},
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VD8", FieldSem::VectorDest8, 1, nullptr, true, false},
    {"X0_12", FieldSem::SioChannum, 1, nullptr, false, false},
    {"IJ16", FieldSem::Immediate16, 1, nullptr, false, false},
    {"X2_12", FieldSem::SioChan, 1, nullptr, false, false},
};

constexpr FieldRule kSioImmMRules[] = {
    {"M.sioid1", FieldSem::SioId, 1, nullptr, false, false},
    {"PJ2", FieldSem::Predicate2, 1, nullptr, false, false},
    {"VJ8", FieldSem::VectorSource8, 1, nullptr, false, false},
    {"M.channum12", FieldSem::SioChannum, 1, nullptr, false, false},
    {"IK16", FieldSem::Immediate16, 1, nullptr, false, false},
    {"M.chan12", FieldSem::SioChan, 1, nullptr, false, false},
};

struct ClassSpec {
  const char* name;
  const FieldRule* rules;
  unsigned rule_count;
};

constexpr ClassSpec kClasses[] = {
    {"imm-alu", kImmAluRules, 4},
    {"imm-mov", kImmMovRules, 3},
    {"imm-mov-scalar", kImmMovScalarRules, 2},
    {"alu3", kAlu3Rules, 7},
    {"alu-carry", kAluCarryRules, 7},
    {"alu2", kAlu2Rules, 6},
    {"alu2-64b", kAlu2Rules, 6},
    {"alu3-64", kAlu3Rules, 7},
    {"alu2-64", kAlu2Rules, 6},
    {"first-v2s", kFirstV2sRules, 5},
    {"alu1", kAlu1Rules, 5},
    {"alu1-64", kAlu1Rules, 5},
    {"sop1", kSop1Rules, 2},
    {"sop3", kSop3Rules, 4},
    {"sop2", kSop2Rules, 3},
    {"sio-emit", kSioEmitRules, 6},
    {"thread-id", kThreadIdRules, 2},
    {"buff-load", kBuffLoadRules, 10},
    {"buff-store", kBuffStoreRules, 10},
    {"buff-atomic", kBuffAtomicRules, 10},
    {"buff-atomic-nosrc", kBuffAtomicNoSrcRules, 10},
    {"buff-atomic-cmpswap", kBuffAtomicCmpSwapRules, 10},
    {"tex-load", kTexLoadRules, 10},
    {"tex-load-rk9", kTexLoadRk9Rules, 9},
    {"interp-load", kInterpLoadRules, 4},
    // agentA: VOP compares
    {"vop-cmp", kCmpRules, 6},
    {"vop-cmp-64", kCmpRules, 6},
    {"vop-cmp-imm", kCmpImmRules, 4},
    // agentB: scalar imm/cmp
    {"sop2-imm", kSop2ImmRules, 3},
    {"sop-cmp", kSopCmpRules, 3},
    {"sop-cmp-imm", kSopCmpImmRules, 2},
    {"sop-cmp-64", kSopCmp64Rules, 3},
    // agentC: memory-image
    {"tex-gather", kTexGatherRules, 11},
    {"vbuff-load", kVbuffLoadRules, 9},
    {"vbuff-store", kVbuffStoreRules, 9},
    {"flat-load", kFlatLoadRules, 6},
    {"flat-store", kFlatStoreRules, 6},
    {"tex-store", kTexStoreRules, 10},
    {"sm-pred", kSmPredRules, 1},
    {"sm-load", kSmLoadRules, 3},
    {"sm-store", kSmStoreRules, 3},
    {"sm-permute", kSmPermuteRules, 4},
    {"sbuff-load", kSbuffLoadRules, 4},
    // agentD: atomics
    {"buff-atomic-64", kBuffAtomic64Rules, 10},
    {"buff-atomic-128", kBuffAtomic128Rules, 9},
    {"tex-atomic", kTexAtomicRules, 10},
    {"sm-atomic32", kSmAtomic32Rules, 4},
    {"sm-atomic64", kSmAtomic64Rules, 4},
    {"sm-atomic-cmpswap32", kSmAtomicCmpswap32Rules, 5},
    {"sm-atomic-cmpswap64", kSmAtomicCmpswap64Rules, 5},
    {"flat-atomic32", kFlatAtomic32Rules, 7},
    {"flat-atomic64", kFlatAtomic64Rules, 7},
    // agentE: control/sio/sm
    {"sop-imm", kSopImmRules, 3},
    {"smem-load", kSmemLoadRules, 4},
    {"prefix-imm", kPrefixImmRules, 1},
    {"ctrl-pair", kCtrlPairRules, 1},
    {"ctrl-pair-src", kCtrlPairSrcRules, 1},
    {"ctrl-jirl", kCtrlJirlRules, 3},
    {"ctrl-jirl32", kCtrlJirlRules, 3},
    {"ctrl-trap32", kCtrlJirlRules, 3},
    {"ctrl-flag", kFlagRules, 3},
    {"ctrl-none", nullptr, 0},
    {"ctrl-branch-cond", kCtrlBranchCondRules, 2},
    {"v2s", kV2sRules, 6},
    {"jump-stack", kJumpStackRules, 4},
    {"sop3-scc", kSop3SccRules, 4},
    {"sio-rot", kSioRotRules, 6},
    {"sio-rot-id", kSioRotIdRules, 7},
    {"sio-read", kSioReadRules, 6},
    {"sio-imm", kSioImmRules, 6},
    {"sio-imm-read", kSioImmReadRules, 6},
    {"sio-imm-m", kSioImmMRules, 6},
};

const ClassSpec* find_class(const char* name) {
  for (const auto& cls : kClasses) {
    if (std::strcmp(cls.name, name) == 0) {
      return &cls;
    }
  }
  return nullptr;
}

// Unconditional branch: b X0_32 offset.
constexpr FieldRule kCtrlBranchRules[] = {
    {"X0_32", FieldSem::Immediate32, 1, nullptr, false, false},
};

SemanticSpec spec_from_row(const generated::SemanticsRow& row) {
  const auto is = [&row](const char* mnemonic) {
    return std::strcmp(row.mnemonic, mnemonic) == 0;
  };
  if (is("b")) {
    return {row.mnemonic, row.asm_prefix, kCtrlBranchRules, 1};
  }
  if (is("jirl")) {
    return {row.mnemonic, row.asm_prefix, kCtrlJirlRules, 3};
  }
  if (is("atomic_v_buff_dec_u32") || is("atomic_v_buff_dec_u64") ||
      is("atomic_v_buff_inc_u32") || is("atomic_v_buff_inc_u64") ||
      is("atomic_v_buff_sub_i32") || is("atomic_v_buff_sub_i64")) {
    return {row.mnemonic, row.asm_prefix, kBuffAtomicNoSrcRules, 9};
  }
  if (is("atomic_v_buff_cmpswap_b32")) {
    return {row.mnemonic, row.asm_prefix, kBuffAtomicCmpSwapRules, 10};
  }
  const ClassSpec* cls = find_class(row.rule_class);
  if (cls == nullptr) {
    return {row.mnemonic, row.asm_prefix, nullptr, 0};
  }
  return {row.mnemonic, row.asm_prefix, cls->rules, cls->rule_count};
}

}  // namespace

std::uint8_t semantic_tuple(const char* mnemonic, const char* field,
                            std::uint8_t fallback) {
  for (const auto& override : generated::kSemanticsTuples) {
    if (std::strcmp(override.mnemonic, mnemonic) == 0 &&
        std::strcmp(override.field, field) == 0) {
      return override.tuple_width;
    }
  }
  return fallback;
}

const SemanticSpec* semantic_spec(const std::string& mnemonic) {
  // Build the per-mnemonic spec once from the generated table; pointers stay
  // valid for the process lifetime.
  static std::vector<SemanticSpec> specs = [] {
    std::vector<SemanticSpec> out;
    out.reserve(std::size(generated::kSemanticsRows));
    for (const auto& row : generated::kSemanticsRows) {
      out.push_back(spec_from_row(row));
    }
    return out;
  }();
  for (const auto& spec : specs) {
    if (mnemonic == spec.mnemonic) {
      return &spec;
    }
  }
  return nullptr;
}

}  // namespace lgpu::isa
