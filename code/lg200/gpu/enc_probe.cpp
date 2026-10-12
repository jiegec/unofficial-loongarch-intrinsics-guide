// enc_probe: encode a fixed probe set through this tree's codec and print each
// word, so it can be diffed against the vendor's own assembler.
//
// Why this exists.  Two independent decoders/encoders can agree with each other
// and both be wrong: this tree's codec and Mesa's lg200.xml were synchronized
// from the same artifact, so comparing them proves transcription, not truth.
// The vendor's shipped LLVM (`llvm-mc -arch=lggpu -mcpu=lg200`) is the closest
// thing to an authority available without hardware, and this program is the
// other half of that comparison:
//
//     ./build/enc_probe            # one "<msid> <lo:08x> <hi:08x>" per line
//
// `code/lg200/gpu/enc_probe.py` drives both sides and reports differences.
//
// The probe set is the carrier SCAFFOLDING - the instructions a carrier emits
// around the instruction under test (register staging, flat/buffer stores,
// scalar loads, the program tail).  A wrong instruction-under-test fails a
// case; wrong scaffolding corrupts every case silently, which is why this set
// is the one worth pinning against an independent assembler.
#include "codec/codec.hpp"
#include "codec/operand.hpp"

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace {

struct Probe {
  const char* id;              // stable key shared with the Python driver
  const char* note;            // what this entry pins
  lgpu::isa::McInst inst;
};

std::vector<Probe> probes() {
  using namespace lgpu::isa;  // NOLINT
  std::vector<Probe> p;
  const auto add = [&p](const char* id, const char* note, McInst inst) {
    p.push_back({id, note, std::move(inst)});
  };

  // ---- register staging: how a vector source reaches the instruction under
  // test (s_mov_imm_b32 into a scalar temp, then v_mov_b32 into the VGPR) ----
  add("mov_imm_b32", "s_mov_imm_b32 s16, 0x0",
      McInst{"s_mov_imm_b32", "(scc)", {op_reg(sgpr(16)), op_imm(0x0u)}});
  add("mov_imm_b32_hi", "s_mov_imm_b32 s0, 0x000b0000",
      McInst{"s_mov_imm_b32", "(scc)",
             {op_reg(sgpr(0)), op_imm(0x000b0000u)}});
  add("mov_b32_v_v", "v_mov_b32 p0, v0, v1 (vector source)",
      McInst{"v_mov_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(0)), op_reg(vgpr(1))}});
  add("mov_b32_v_s", "v_mov_b32 p0, v0, s16 (scalar source)",
      McInst{"v_mov_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(0)), op_reg(sgpr(16))}});
  add("mov_b32_v_zero", "v_mov_b32 p0, v0, zero (the zero source)",
      McInst{"v_mov_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(0)), op_reg(zero_reg())}});
  add("mov_b32_s_s", "s_mov_b32 s0, s16",
      McInst{"s_mov_b32", "(scc)", {op_reg(sgpr(0)), op_reg(sgpr(16))}});
  add("mov_b32_s_v", "s_mov_b32 s16, v0 is not a form; pin s_mov_b32 s0, s1",
      McInst{"s_mov_b32", "(scc)", {op_reg(sgpr(0)), op_reg(sgpr(1))}});

  // ---- the kernarg read the compute prologue uses ----
  add("load_s_flat_w2", "load_s_flat_w2 s[0:1], s[6:7], zero, 0",
      McInst{"load_s_flat_w2", "",
             {op_reg(sgpr(0, 2)), op_reg(sgpr(6, 2)), op_reg(zero_reg()),
              op_imm(0u)}});
  add("load_s_flat_w8", "load_s_flat_w8 s[8:15], s[6:7], zero, 0",
      McInst{"load_s_flat_w8", "",
             {op_reg(sgpr(8, 8)), op_reg(sgpr(6, 2)), op_reg(zero_reg()),
              op_imm(0u)}});
  add("load_s_flat_w4_off", "load_s_flat_w4 s[4:7], s[0:1], zero, 0x200",
      McInst{"load_s_flat_w4", "",
             {op_reg(sgpr(4, 4)), op_reg(sgpr(0, 2)), op_reg(zero_reg()),
              op_imm(0x200u)}});

  // ---- the compute result store: the flat form with the absolute address in
  // the VGPR pair and the architectural zero base in a scalar pair ----
  add("store_v_flat_b32", "store_v_flat_b32 p0, v10, v[0:1], base s[20:21]",
      McInst{"store_v_flat_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(10)), op_reg(vgpr(0, 2)),
              op_reg(sgpr(20, 2))}});
  // The vendor's assembler has NO spelling for the base pair: its flat form
  // takes three operands and always encodes the architectural zero source,
  // s[20:21] (0x14 in the lo[15:8] field).  This probe passes exactly that pair,
  // which is what makes the two sides comparable - and it is also what the
  // carriers pass at run time.
  add("store_v_flat_b32_alt", "store_v_flat_b32 p0, v0, v[4:5], base s[20:21]",
      McInst{"store_v_flat_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(0)), op_reg(vgpr(4, 2)),
              op_reg(sgpr(20, 2))}});
  add("load_v_flat_b32", "load_v_flat_b32 p0, v10, v[0:1], base s[20:21]",
      McInst{"load_v_flat_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(10)), op_reg(vgpr(0, 2)),
              op_reg(sgpr(20, 2))}});

  // ---- the buffer descriptor store/load the graphics carriers use ----
  add("store_v_buff_b32", "store_v_buff_b32 p0, v10, v9, s[0:3], zero, 0x20, offen",
      McInst{"store_v_buff_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(10)), op_reg(vgpr(9)),
              op_reg(sgpr(0, 4)), op_reg(zero_reg()), op_imm(0x20u),
              op_modifier("offen", 1)}});
  add("load_v_buff_b32", "load_v_buff_b32 p0, v10, v0, s[4:7], zero, 0x0, offen",
      McInst{"load_v_buff_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(10)), op_reg(vgpr(0)),
              op_reg(sgpr(4, 4)), op_reg(zero_reg()), op_imm(0u),
              op_modifier("offen", 1)}});

  // ---- shared memory ----
  add("load_sm_b32", "load_sm_b32 p0, v10, v8",
      McInst{"load_sm_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(10)), op_reg(vgpr(8))}});
  add("store_sm_b32", "store_sm_b32 p0, v9, v8",
      McInst{"store_sm_b32", "",
             {op_reg(predicate(0)), op_reg(vgpr(9)), op_reg(vgpr(8))}});
  // The vendor's assembly operand order for the shared-memory atomics is
  // (pred, dst, addr, data); this tree's table lists them as
  // (pred, addr, data, dst).  The FIELDS are the same, so the encodings agree
  // once the vendor text spells the same field assignment - which is what this
  // probe's text does.  The order difference is a delta to document, not an
  // encoding disagreement.
  add("atomic_sm_add_u32", "atomic_sm_add_u32 p0, dst=v9, addr=v0, data=v8",
      McInst{"atomic_sm_add_u32", "",
             {op_reg(predicate(0)), op_reg(vgpr(0)), op_reg(vgpr(8)),
              op_reg(vgpr(9))}});

  // ---- the program tail ----
  add("check", "check k vm vs sm",
      McInst{"check", "", {op_raw("k vm vs sm")}});
  add("nop", "nop", McInst{"nop", "", {}});
  add("exit", "exit", McInst{"exit", "", {}});

  return p;
}

}  // namespace

int main() {
  for (const auto& probe : probes()) {
    lgpu::isa::EncodedInst words;
    std::string error;
    if (!lgpu::isa::encode_instruction(probe.inst.mnemonic, probe.inst.operands,
                                       words, error)) {
      std::printf("%s ENCODE-FAILED %s\n", probe.id, error.c_str());
      continue;
    }
    std::printf("%s %08x %08x\n", probe.id, words.lo, words.hi);
  }
  return 0;
}
