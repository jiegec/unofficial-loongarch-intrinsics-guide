// lgpu-gpu: minimal pure-GPU runner for the LG200 PS-generic carrier.
//
//   lgpu-gpu --matrix
//       print \"mnemonic\tmaterializable\\tform\" for every catalog instruction
//   lgpu-gpu run <mnemonic> [--dry-run]
//       read case rows on stdin (id\tfp_policy\tin-words...\texp-words...),
//       assemble the PS-generic envelope, and (hardware path) submit once,
//       wait for the fence, read back the argument BO, and compare the GPU
//       result dwords against the expected dwords. --dry-run only assembles
//       and lists the program without opening a DRM node.
//
// This is the bare GPU runner: no allowlist, no gate, no quarantine, no
// state ladder.  A case either passes, fails, or (if no carrier can stage
// its I/O model) is reported as cannot-run by the caller.
#include "carrier_map.hpp"
#include "envelope.hpp"
#if defined(LGPU_HAVE_DRM)
#include "drm.hpp"
#endif
#include "codec/codec.hpp"
#include "codec/semantics.hpp"
#include "codec/generated/lg200_codec.inc"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace {

struct Row {
  std::string id;
  std::string fp_policy;
  std::vector<std::uint32_t> inputs;
  std::vector<std::uint32_t> expected;
};

int movidxb(std::uint32_t base, std::uint32_t idx, std::uint32_t magic);

// Case comparison shared by both transports (defined after run()).
int compare_cases(const lgpu::envelope::EnvelopeProgram& program,
                  const std::vector<std::uint8_t>& result);

bool parse_hex_list(const std::string& text, std::vector<std::uint32_t>& out) {
  const char* p = text.c_str();
  while (*p) {
    while (*p == ' ') ++p;
    if (!*p) break;
    char* end = nullptr;
    const unsigned long v = std::strtoul(p, &end, 0);
    if (end == p) return false;
    out.push_back(static_cast<std::uint32_t>(v));
    p = end;
  }
  return true;
}

bool parse_rows(std::vector<Row>& rows) {
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty() || line[0] == '#') continue;
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (true) {
      const std::size_t tab = line.find('\t', start);
      if (tab == std::string::npos) {
        parts.push_back(line.substr(start));
        break;
      }
      parts.push_back(line.substr(start, tab - start));
      start = tab + 1;
    }
    if (parts.size() != 4) {
      std::fprintf(stderr, "run: expected 4 tab fields, got %zu\n",
                   parts.size());
      return false;
    }
    Row row;
    row.id = parts[0];
    row.fp_policy = parts[1];
    if (!parse_hex_list(parts[2], row.inputs) ||
        !parse_hex_list(parts[3], row.expected)) {
      std::fprintf(stderr, "run: bad hex list in row %s\n", row.id.c_str());
      return false;
    }
    rows.push_back(std::move(row));
  }
  return true;
}

int matrix() {
  std::map<std::string, bool> seen;
  for (const auto& v : lgpu::isa::codec_variants()) {
    if (seen.count(v.mnemonic)) continue;
    seen.emplace(v.mnemonic, true);
    const bool avail = lgpu::envelope::generic_available(v.mnemonic);
    const std::string form = lgpu::envelope::generic_form(v.mnemonic);
    std::printf("%s\t%d\t%s\n", v.mnemonic.c_str(), avail ? 1 : 0,
                form.c_str());
  }
  return 0;
}

// Dump the codec's own semantic rule set: mnemonic, field, effective tuple
// width, whether the canonical form may omit it, and whether the field is the
// destination.  This is what audit.py rule F checks the driver row's declared
// fields against, so the codec - not a second transcription of it - is the
// authority for what an emitter must cover.
//
//   lgpu-gpu --specs
int specs() {
  // kSemanticsRows, not codec_variants(): 241 mnemonics (every atomic_sm_*
  // among them) have a semantic rule class but no canonical variant, so
  // enumerating the variants would silently skip them and the coverage check
  // would never look at their fields.
  for (const auto& row : lgpu::isa::generated::kSemanticsRows) {
    const auto* spec = lgpu::isa::semantic_spec(row.mnemonic);
    if (spec == nullptr) continue;
    for (unsigned i = 0; i < spec->rule_count; ++i) {
      const auto& rule = spec->rules[i];
      const std::uint8_t tuple = lgpu::isa::semantic_tuple(
          row.mnemonic, rule.field, rule.tuple_width);
      std::printf("%s\t%s\t%d\t%d\t%d\n", row.mnemonic, rule.field,
                  static_cast<int>(tuple), rule.optional ? 1 : 0,
                  rule.is_output ? 1 : 0);
    }
  }
  return 0;
}

int run(const std::string& mnemonic, bool dry_run) {
  std::vector<Row> rows;
  if (!parse_rows(rows)) return 2;

  std::vector<lgpu::envelope::EnvelopeCase> cases;
  for (const auto& row : rows) {
    lgpu::envelope::EnvelopeCase c;
    c.id = row.id;
    c.inputs = row.inputs;
    c.expected = row.expected;
    c.fp_policy = row.fp_policy;
    cases.push_back(std::move(c));
  }
  std::string error;
  auto program = lgpu::envelope::build_carrier(
      mnemonic, cases, lgpu::envelope::LaunchEnvironment::Graphics, error);
  if (!program) {
    std::fprintf(stderr, "run: %s: %s\n", mnemonic.c_str(), error.c_str());
    return 2;
  }

  if (dry_run) {
    std::printf("-- envelope listing (%zu bytes, %zu instructions) --\n%s",
                program->bytes.size(), program->words.size(),
                program->listing.c_str());
    if (std::getenv("LGPU_DUMP_WORDS")) {
      for (std::size_t i = 0; i < program->words.size(); ++i) {
        std::printf("word %3zu: lo=0x%08x hi=0x%08x\n", i,
                    program->words[i].lo, program->words[i].hi);
      }
    }
    if (std::getenv("LGPU_DUMP_EXPECT")) {
      // The expected value is not part of the emitted stream, so this is the
      // only way to see a host-side expectation change without a device.
      for (const auto& c : program->cases) {
        std::printf("case %-24s", c.id.c_str());
        for (std::uint32_t value : c.expected) {
          std::printf(" 0x%08x", value);
        }
        std::printf("\n");
      }
    }
    return 0;
  }

#if !defined(LGPU_HAVE_DRM)
  (void)program;
  std::fprintf(stderr, "run: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  lgpu::drm::PsSubmitRequest request;
  request.shader_words.resize(program->words.size() * 2);
  for (std::size_t i = 0; i < program->words.size(); ++i) {
    request.shader_words[2 * i] = program->words[i].lo;
    request.shader_words[2 * i + 1] = program->words[i].hi;
  }
  std::uint32_t max_offset = 0;
  for (const auto& c : program->cases) {
    for (const auto off : c.offsets) max_offset = max_offset > off ? max_offset : off;
  }
  request.result_bytes = max_offset + 4096;
  // Exactly one submission per program.  (An earlier revision submitted the
  // program once here and then again in the stage dispatch below, so every PS
  // case ran twice; the two submits were identical, which is why it was never
  // visible in the results - only in the wall clock.)
  lgpu::drm::PsSubmitReport report;
  // GS-family programs carry the GS stage: submit the PS+GS pair.
  if (!program->fs_bytes.empty()) {
    request.gs_words.assign(program->words.size() * 2, 0u);
    for (std::size_t i = 0; i < program->words.size(); ++i) {
      request.gs_words[2 * i] = program->words[i].lo;
      request.gs_words[2 * i + 1] = program->words[i].hi;
    }
    std::vector<std::uint32_t> fs_words(program->fs_bytes.size() / 4);
    for (std::size_t i = 0; i < fs_words.size(); ++i) {
      fs_words[i] = static_cast<std::uint32_t>(program->fs_bytes[4 * i]) |
                    (static_cast<std::uint32_t>(program->fs_bytes[4 * i + 1]) << 8) |
                    (static_cast<std::uint32_t>(program->fs_bytes[4 * i + 2]) << 16) |
                    (static_cast<std::uint32_t>(program->fs_bytes[4 * i + 3]) << 24);
    }
    request.shader_words = std::move(fs_words);
    request.argument_dwords.assign(request.result_bytes / 4, 0u);
    report = lgpu::drm::submit_gs(request);
  } else if (program->probe) {
    // Memory carriers submit through the conformance-proven probe packet:
    // PS roots const_buffer=arg+0x600, constant=arg+0x300 (hydrated load
    // descriptor base), rsd=arg+0x200 (size/flags 0x40/0x27fac); the
    // carrier's own arg_preloads stage the load-window data.
    request.result_bytes = 0x2000;
    const auto roots = lgpu::carrier::kProbeRoots;
    report = lgpu::drm::submit_probe(
        request.shader_words, program->arg_preloads, roots.const_buffer,
        roots.constant, roots.rsd, request.result_bytes, request.timeout_ms);
  } else {
    request.argument_dwords.assign(request.result_bytes / 4, 0u);
    report = lgpu::drm::submit_ps(request);
  }
  if (report.status != lgpu::drm::PsStatus::Clean) {
    std::fprintf(stderr, "run: submit %s\n", report.detail.c_str());
    return 3;
  }
  int failures = 0;
  if (std::getenv("LGPU_DUMP_ARG")) {
    for (std::uint32_t off = 0; off + 4 <= report.result.size(); off += 4) {
      std::uint32_t v = 0;
      std::memcpy(&v, report.result.data() + off, 4);
      if (v != 0)
        std::printf("arg 0x%04x: 0x%08x\n", off, v);
    }
  }
  failures += compare_cases(*program, report.result);
  return failures == 0 ? 0 : 1;
#endif
}

// Case comparison shared by the graphics and compute transports: identical
// semantics on purpose, so a row that passes through one path and fails
// through the other is a statement about the two launch environments, not
// about two different comparators.
int compare_cases(const lgpu::envelope::EnvelopeProgram& program,
                  const std::vector<std::uint8_t>& result) {
  int failures = 0;
  for (const auto& c : program.cases) {
    bool ok = true;
    if (program.observe) {
      // Observation-only: report the readback, never fail.
      for (std::size_t k = 0; k < c.offsets.size(); ++k) {
        const std::uint32_t off = c.offsets[k];
        if (off + 4 > result.size()) continue;
        std::uint32_t actual = 0;
        std::memcpy(&actual, result.data() + off, 4);
        std::printf("OBS %s: offset 0x%x = 0x%08x\n", c.id.c_str(), off,
                    actual);
      }
      if (program.observe_reason[0])
        std::printf("PASS %s (observed: %s)\n", c.id.c_str(),
                    program.observe_reason);
      else
        std::printf("PASS %s (observed)\n", c.id.c_str());
      continue;
    }
    if (program.control) {
      // control carrier: fence clean + pre sentinel present = observed
      // execution (survived vs terminated is the outcome, not a failure)
      std::uint32_t pre = 0, post = 0;
      if (c.offsets.size() >= 1 && c.offsets[0] + 4 <= result.size())
        std::memcpy(&pre, result.data() + c.offsets[0], 4);
      if (c.offsets.size() >= 2 && c.offsets[1] + 4 <= result.size())
        std::memcpy(&post, result.data() + c.offsets[1], 4);
      const bool survived = pre == 0x11111111u && post == 0x22222222u;
      const bool terminated = pre == 0x11111111u && post == 0x00000000u;
      if (pre == 0x11111111u && (survived || terminated)) {
        std::printf("PASS %s (%s)\n", c.id.c_str(),
                    survived ? "survived" : "terminated");
        continue;
      }
      std::printf("FAIL %s: pre=0x%08x post=0x%08x (no clean observation)\n",
                  c.id.c_str(), pre, post);
      ++failures;
      continue;
    }
    for (std::size_t k = 0; k < c.offsets.size(); ++k) {
      const std::uint32_t off = c.offsets[k];
      if (off + 4 > result.size()) {
        std::printf("FAIL %s: readback too small for offset 0x%x\n",
                    c.id.c_str(), off);
        ok = false; ++failures; continue;
      }
      std::uint32_t actual = 0;
      std::memcpy(&actual, result.data() + off, 4);
      // [HW 2026-08-24] the f32/f64 transcendentals and sqrt use a
      // hardware approximation that can differ by one ulp from the
      // correctly-rounded softfloat oracle; accept the 1-ulp window for
      // those families.
      const bool approx_ok =
          program.approx1ulp && actual != c.expected[k] &&
          ((actual >> 1) == (c.expected[k] >> 1) ||  // f32/f64 mag close
           actual - c.expected[k] == 1u ||
           c.expected[k] - actual == 1u);
      if (actual != c.expected[k] && !approx_ok) {
        std::printf("FAIL %s: offset 0x%x expected 0x%08x got 0x%08x\n",
                    c.id.c_str(), off, c.expected[k], actual);
        ok = false; ++failures;
      }
    }
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", c.id.c_str());
  }
  return failures;
}

// Workgroup size for the compute path, from LGPU_CS_GROUP ("x" or "x,y,z").
// Default 4x1x1: an instruction is defined per lane, so a workgroup that runs
// one lane is the shape whose oracle comparison is unambiguous, and more than
// one lane in x is what makes a stale-lane-id read visible.  Larger groups
// exist for the launch-state probe and the bench template.
void cs_group(std::uint32_t group[3]) {
  group[0] = 4;
  group[1] = group[2] = 1;
  const char* text = std::getenv("LGPU_CS_GROUP");
  if (text == nullptr) return;
  unsigned long v[3] = {0, 0, 0};
  const int got = std::sscanf(text, "%lu,%lu,%lu", &v[0], &v[1], &v[2]);
  for (int i = 0; i < got && i < 3; ++i) {
    if (v[i] > 0) group[i] = static_cast<std::uint32_t>(v[i]);
  }
}

// The implicit-argument block the producer's kernels read through the kernarg
// pointer.  It follows the user arguments - here the single eight-byte output
// pointer submit_compute writes at kernarg + 0 - and its layout is the one
// analysis/lacm/lggpu-abi/README.md section 7 recovered and section 10
// confirmed against a live kernel:
//
//   +0  u32 block_count_x/y/z      (grid size, in WORKGROUPS)
//   +12 u16 group_size_x/y/z       (local size)
//   +18 u16 remainder_x/y/z
//   +24 u16 grid_dims
//   +32 i64 global_offset_x/y/z
//
// This carrier's programs do not read it (they assemble their own addresses),
// but a dispatch that stages it exercises the same ABI a compiled kernel uses,
// and get_num_groups()/get_local_size() would return the truth.
std::vector<std::uint8_t> cs_implicit_block(const std::uint32_t group[3],
                                            const std::uint32_t blocks[3],
                                            std::uint16_t grid_dims) {
  std::vector<std::uint8_t> out(56, 0);  // kernarg + 8 .. + 64
  const auto put_u16 = [&out](std::size_t off, std::uint16_t v) {
    out[off] = static_cast<std::uint8_t>(v);
    out[off + 1] = static_cast<std::uint8_t>(v >> 8);
  };
  const auto put_u32 = [&out](std::size_t off, std::uint32_t v) {
    for (int i = 0; i < 4; ++i)
      out[off + i] = static_cast<std::uint8_t>(v >> (8 * i));
  };
  for (int i = 0; i < 3; ++i)
    put_u32(static_cast<std::size_t>(i) * 4, blocks[i]);
  for (int i = 0; i < 3; ++i)
    put_u16(12 + 2 * static_cast<std::size_t>(i),
            static_cast<std::uint16_t>(group[i]));
  put_u16(24, grid_dims);
  // global_offset stays zero; the dispatch has no base group.
  return out;
}

// The number of SGPRs the hardware itself writes for a compute dispatch.  The
// .kd flag word enables the group ids, and the ABI places them at
// s[kernel_sgpr_cnt + 0..2] with kernel_sgpr_cnt pinned to 11 by
// LOONGVK_KD_RSRC1 - so s11/s12/s13 exist whether or not the program reads
// them, and the allocation must cover them.  A descriptor that declares fewer
// SGPRs than the hardware writes is the failure mode this function exists to
// prevent.
constexpr std::uint32_t kComputeLaunchSgprs = 14;

// A program that executes a shared-memory instruction must declare an LDS
// window.
//
// This preflight exists because its absence cost three pipe resets: the request
// filler below never copied program.shared_bytes into the submit request, so
// every shared-memory row this carrier has ever built ran with a zero-byte
// window and faulted inside the compute pipe (measured: store_sm_b32 lat 4 and
// atomic_sm_add_u32 lat 256 both at 0x100084000).  A windowless shared-memory
// launch is not a measurable configuration, it is a malformed one, and no
// carrier may submit without a preflight that the launch state is legal.
//
// The check is "present", not "large enough": the LDS offsets live in VGPRs, so
// the program's real footprint is not statically knowable here.  What is
// knowable is the pair that was actually wrong - a shared-memory instruction
// with no window declared at all.
bool cs_lds_preflight(const lgpu::envelope::EnvelopeProgram& program,
                      std::uint32_t declared, std::string& why) {
  const bool uses_lds = program.listing.find("_sm_") != std::string::npos;
  if (uses_lds && declared == 0) {
    why = "the program executes a shared-memory instruction but the launch "
          "declares a zero-byte LDS window; set program.shared_bytes in the "
          "builder (the row cannot be measured without one)";
    return false;
  }
  return true;
}

void cs_fill_request(lgpu::drm::ComputeSubmitRequest& request,
                     const lgpu::envelope::EnvelopeProgram& program,
                     std::uint32_t output_bytes) {
  request.text = program.bytes;
  request.output_size = output_bytes;
  // The declared LDS window.  This is the ONLY place it is copied from the
  // program into the submit request, and it used to be nowhere: it was set for
  // the pj2 probe only, so every csbench row that used shared memory ran with
  // shared_segment_size = 0 while its own builder had declared bytes.  The
  // descriptor then described a launch with no LDS region, and the row faulted
  // inside the compute pipe - which read as "LDS does not work on this pipe"
  // when it was really "this runner never told the descriptor about the window".
  // Whatever a builder declares now reaches .kd +0x1c and rsrc0's
  // granulated_shm_size without any per-runner plumbing.
  request.shared_segment_size = program.shared_bytes;
  request.num_sgprs = std::max<std::uint32_t>(program.sgpr_high_water,
                                             kComputeLaunchSgprs);
  // A VGPR count of zero would declare an allocation the wave cannot have.
  request.num_vgprs = std::max<std::uint32_t>(program.vgpr_high_water, 1u);
  std::uint32_t group[3];
  cs_group(group);
  const std::uint32_t blocks[3] = {1, 1, 1};
  const std::uint16_t dims =
      (group[2] > 1) ? 3 : (group[1] > 1) ? 2 : 1;
  request.kernarg_tail = cs_implicit_block(group, blocks, dims);
  for (int i = 0; i < 3; ++i) {
    request.group[i] = group[i];
    // grid_size is the GLOBAL size in work-items and the stream's group command
    // divides it by the local size, so passing the local size through is what
    // makes the workgroup count exactly one.
    request.grid[i] = group[i];
  }
  const char* tmo = std::getenv("LGPU_CS_TIMEOUT");
  request.timeout_ms = tmo ? static_cast<std::uint32_t>(std::strtoul(tmo, nullptr, 0))
                           : 8000u;
}

// Print everything needed to reason about a submission that did not come back
// clean - and, on a real wedge, everything needed to reason about it after the
// fact without another power cycle.
void cs_print_wedge(const lgpu::drm::ComputeSubmitReport& report,
                    const char* who) {
  std::fprintf(stderr, "%s: signal=0x%016llx %s\n", who,
               static_cast<unsigned long long>(report.signal_value),
               report.detail.empty() ? "" : report.detail.c_str());
  if (!report.wedge_fields.empty())
    std::fprintf(stderr, "%s: submitted %s\n", who,
                 report.wedge_fields.c_str());
  if (std::getenv("LGPU_DUMP_WEDGE")) {
    const auto dump = [who](const char* name,
                            const std::vector<std::uint8_t>& bytes) {
      std::fprintf(stderr, "%s: %s[%zu]:", who, name, bytes.size());
      for (std::size_t i = 0; i < bytes.size(); ++i) {
        if (i % 16 == 0) std::fprintf(stderr, "\n%s:   %04zx:", who, i);
        std::fprintf(stderr, " %02x", bytes[i]);
      }
      std::fprintf(stderr, "\n");
    };
    dump("mcs", report.wedge_stream);
    dump("kd", report.wedge_kd);
    dump("kernarg", report.wedge_kernarg);
  }
}

// The preflight every compute submission must pass: prove the MCS stream and
// kernel descriptor are the vendor's own bytes before the doorbell.  The pipe
// has no BO validation list, so a stream the firmware rejects does not return
// an error - it wedges the device and costs a power cycle.  LGPU_CS_NO_PREFLIGHT
// exists only to demonstrate that a bad stream is not caught later.
bool cs_preflight(const char* who) {
  if (std::getenv("LGPU_CS_NO_PREFLIGHT")) return true;
  std::string detail;
  if (!lgpu::drm::compute_selfcheck(detail)) {
    std::fprintf(stderr, "%s: PREFLIGHT FAILED, refusing to submit: %s\n", who,
                 detail.c_str());
    return false;
  }
  if (std::getenv("LGPU_CS_VERBOSE"))
    std::fprintf(stderr, "%s: preflight OK: %s\n", who, detail.c_str());
  return true;
}

// Compute (CPIPE) transport for the same case rows the graphics path consumes:
// assemble the program for the compute launch environment, dispatch it through
// the MCS user queue with one workgroup, and compare the output BO against the
// same expected dwords.
int csrun(const std::string& mnemonic, bool dry_run) {
  std::vector<Row> rows;
  if (!parse_rows(rows)) return 2;

  std::vector<lgpu::envelope::EnvelopeCase> cases;
  for (const auto& row : rows) {
    lgpu::envelope::EnvelopeCase c;
    c.id = row.id;
    c.inputs = row.inputs;
    c.expected = row.expected;
    c.fp_policy = row.fp_policy;
    cases.push_back(std::move(c));
  }
  // A per-lane atomic's expectation depends on how many lanes the dispatch
  // executes, so the builder is handed the same group the launcher will use.
  // Both read cs_group(), which is why they cannot drift apart.
  std::uint32_t group[3];
  cs_group(group);
  const unsigned lanes = group[0] * group[1] * group[2];
  std::string error;
  auto program = lgpu::envelope::build_cs(mnemonic, cases, error, lanes);
  if (!program) {
    std::fprintf(stderr, "csrun: %s: %s\n", mnemonic.c_str(), error.c_str());
    return 2;
  }
  if (dry_run) {
    std::printf("-- compute envelope listing (%zu bytes, %zu instructions; "
                "sgpr<=%u vgpr<=%u) --\n%s",
                program->bytes.size(), program->words.size(),
                program->sgpr_high_water, program->vgpr_high_water,
                program->listing.c_str());
    if (std::getenv("LGPU_DUMP_WORDS")) {
      for (std::size_t i = 0; i < program->words.size(); ++i) {
        std::printf("word %3zu: lo=0x%08x hi=0x%08x\n", i,
                    program->words[i].lo, program->words[i].hi);
      }
    }
    if (std::getenv("LGPU_DUMP_EXPECT")) {
      for (const auto& c : program->cases) {
        std::printf("case %-24s", c.id.c_str());
        for (std::uint32_t value : c.expected) {
          std::printf(" 0x%08x", value);
        }
        std::printf("\n");
      }
    }
    return 0;
  }

#if !defined(LGPU_HAVE_DRM)
  std::fprintf(stderr, "csrun: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  if (!cs_preflight("csrun")) return 5;
  {
    // Same preflight as the bench path: a shared-memory instruction with no
    // declared window is a malformed launch, not a case whose result can be
    // read.  program->shared_bytes is the value cs_fill_request copies below.
    std::string why;
    if (!cs_lds_preflight(*program, program->shared_bytes, why)) {
      std::fprintf(stderr, "csrun: %s: %s\n", mnemonic.c_str(), why.c_str());
      return 4;
    }
  }
  std::uint32_t max_offset = 0;
  for (const auto& c : program->cases) {
    for (const auto off : c.offsets)
      max_offset = max_offset > off ? max_offset : off;
  }
  lgpu::drm::ComputeSubmitRequest request;
  cs_fill_request(request, *program, max_offset + 4096);
  request.output_preload = program->arg_preloads;

  const auto report = lgpu::drm::submit_compute(request);
  if (report.status != lgpu::drm::ComputeSubmitReport::Status::Clean) {
    cs_print_wedge(report, "csrun");
    return 3;
  }
  if (std::getenv("LGPU_DUMP_ARG")) {
    for (std::uint32_t off = 0; off + 4 <= report.output.size(); off += 4) {
      std::uint32_t v = 0;
      std::memcpy(&v, report.output.data() + off, 4);
      if (v != 0) std::printf("out 0x%04x: 0x%08x\n", off, v);
    }
  }
  const int failures = compare_cases(*program, report.output);
  return failures == 0 ? 0 : 1;
#endif
}

// Compute batch entry point: one process, one DRM session, many mnemonics.
//
// Stdin is a sequence of blocks, each introduced by a line "@<mnemonic>" and
// followed by the same four-tab case rows the single-mnemonic path reads.  The
// reason this exists is the session rather than the parsing: a submission
// otherwise opens a DRM file and creates a user queue, and a few hundred of
// those create/destroy cycles wedge the pipe (see README, "the churn hazard"),
// so a bulk sweep pays one session for the whole batch.
//
// Per case the output is the single-mnemonic shape, so the same readers work,
// and each block ends with one summary line the driver attributes by mnemonic:
//
//   BATCH <mnemonic> <STATUS> pass=<n> fail=<n> rows=<n>
//
// A submission that does not come back clean ends the batch: the pipe is
// quarantined from that point and every later submission would report the same
// thing, so the remaining blocks are reported as NOT-RUN rather than submitted.
int csrun_batch(bool dry_run) {
  struct Block {
    std::string mnemonic;
    std::vector<Row> rows;
  };
  std::vector<Block> blocks;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (!line.empty() && line[0] == '#') continue;
    if (!line.empty() && line[0] == '@') {
      blocks.push_back(Block{line.substr(1), {}});
      continue;
    }
    if (line.empty()) continue;
    if (blocks.empty()) {
      std::fprintf(stderr, "csrun-batch: row before any @mnemonic block\n");
      return 2;
    }
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (true) {
      const std::size_t tab = line.find('\t', start);
      if (tab == std::string::npos) {
        parts.push_back(line.substr(start));
        break;
      }
      parts.push_back(line.substr(start, tab - start));
      start = tab + 1;
    }
    if (parts.size() != 4) {
      std::fprintf(stderr, "csrun-batch: expected 4 tab fields, got %zu\n",
                   parts.size());
      return 2;
    }
    Row row;
    row.id = parts[0];
    row.fp_policy = parts[1];
    if (!parse_hex_list(parts[2], row.inputs) ||
        !parse_hex_list(parts[3], row.expected)) {
      std::fprintf(stderr, "csrun-batch: bad hex list in row %s\n",
                   row.id.c_str());
      return 2;
    }
    blocks.back().rows.push_back(std::move(row));
  }
  if (blocks.empty()) {
    std::fprintf(stderr, "csrun-batch: no blocks on stdin\n");
    return 2;
  }

#if !defined(LGPU_HAVE_DRM)
  (void)dry_run;
  std::fprintf(stderr, "csrun-batch: drm layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  // One preflight for the batch: it checks the stream/descriptor shape this
  // build produces, which does not vary per mnemonic.
  if (!dry_run && !cs_preflight("csrun-batch")) return 5;

  int failures = 0;
  bool halted = false;
  for (const auto& block : blocks) {
    std::vector<lgpu::envelope::EnvelopeCase> cases;
    for (const auto& row : block.rows) {
      lgpu::envelope::EnvelopeCase c;
      c.id = row.id;
      c.inputs = row.inputs;
      c.expected = row.expected;
      c.fp_policy = row.fp_policy;
      cases.push_back(std::move(c));
    }
    if (halted) {
      std::printf("BATCH %s NOT-RUN pass=0 fail=0 rows=%zu\n",
                  block.mnemonic.c_str(), block.rows.size());
      continue;
    }
    std::uint32_t group[3];
    cs_group(group);
    const unsigned lanes = group[0] * group[1] * group[2];
    std::string error;
    auto program = lgpu::envelope::build_cs(block.mnemonic, cases, error,
                                            lanes);
    if (!program) {
      // The host refused the stream; nothing was submitted.  Same meaning as
      // the single-mnemonic path's exit code 2.  The BATCH-BLOCK markers carry
      // the refusal into that mnemonic's own log, so a batch dry run leaves the
      // same per-mnemonic listing set a non-batch run does instead of silently
      // leaving the previous run's programs for audit.py to read.
      std::printf("BATCH-BLOCK %s\n", block.mnemonic.c_str());
      std::printf("csrun-batch: %s: compute carrier has no %s: %s\n",
                  block.mnemonic.c_str(), block.mnemonic.c_str(), error.c_str());
      std::printf("BATCH-BLOCK-END %s\n", block.mnemonic.c_str());
      std::printf("BATCH %s SKIP-ENVELOPE pass=0 fail=0 rows=%zu -- %s\n",
                  block.mnemonic.c_str(), block.rows.size(), error.c_str());
      continue;
    }
    if (dry_run) {
      // Same status AND the same listing the single-mnemonic dry path emits, so
      // a batch dry run regenerates the per-mnemonic logs instead of leaving the
      // previous run's listings in place.  The BATCH-BLOCK markers let the
      // Python driver attribute each block to its mnemonic.
      std::printf("BATCH-BLOCK %s\n", block.mnemonic.c_str());
      std::printf("-- compute envelope listing (%zu bytes, %zu instructions; "
                  "sgpr<=%u vgpr<=%u) --\n%s",
                  program->bytes.size(), program->words.size(),
                  program->sgpr_high_water, program->vgpr_high_water,
                  program->listing.c_str());
      std::printf("BATCH-BLOCK-END %s\n", block.mnemonic.c_str());
      std::printf("BATCH %s PASS pass=0 fail=0 rows=%zu -- assembled only\n",
                  block.mnemonic.c_str(), block.rows.size());
      continue;
    }
    std::uint32_t max_offset = 0;
    for (const auto& c : program->cases)
      for (const auto off : c.offsets)
        max_offset = max_offset > off ? max_offset : off;
    lgpu::drm::ComputeSubmitRequest request;
    cs_fill_request(request, *program, max_offset + 4096);
    request.output_preload = program->arg_preloads;
    request.persistent_session = true;   // the point of this entry point
    const auto report = lgpu::drm::submit_compute(request);
    if (report.status != lgpu::drm::ComputeSubmitReport::Status::Clean) {
      // Same attribution markers as the dry path: the wedge record becomes this
      // mnemonic's own log instead of leaving the previous run's listing behind.
      std::printf("BATCH-BLOCK %s\n", block.mnemonic.c_str());
      cs_print_wedge(report, "csrun-batch");
      std::printf("BATCH-BLOCK-END %s\n", block.mnemonic.c_str());
      std::printf("BATCH %s HANG/FAULT pass=0 fail=0 rows=%zu\n",
                  block.mnemonic.c_str(), block.rows.size());
      halted = true;
      continue;
    }
    if (std::getenv("LGPU_DUMP_ARG")) {
      for (std::uint32_t off = 0; off + 4 <= report.output.size(); off += 4) {
        std::uint32_t v = 0;
        std::memcpy(&v, report.output.data() + off, 4);
        if (v != 0)
          std::printf("out 0x%04x: 0x%08x\t%s\n", off, v,
                      block.mnemonic.c_str());
      }
    }
    // Wrap the per-case pass/fail lines in the same markers the dry path uses,
    // so a hardware batch run also leaves a per-mnemonic log.
    std::printf("BATCH-BLOCK %s\n", block.mnemonic.c_str());
    const int f = compare_cases(*program, report.output);
    std::printf("BATCH-BLOCK-END %s\n", block.mnemonic.c_str());
    failures += f;
    std::printf("BATCH %s %s pass=%zu fail=%d rows=%zu\n",
                block.mnemonic.c_str(), f == 0 ? "PASS" : "FAIL",
                program->cases.size() - static_cast<std::size_t>(f), f,
                block.rows.size());
  }
  // Tear the persistent session down explicitly: drain, free the one queue,
  // then close.  Leaving it to DRM file close is the path whose failure retains
  // the VM.
  lgpu::drm::close_cached_session();
  return halted ? 3 : (failures == 0 ? 0 : 1);
#endif
}

// The compute launch-state self-test.  This is the first thing to run on
// hardware: it proves the transport and the launch state on a program whose
// semantics are eight register reads, so a failure is attributable.
//
// Deterministic (the value is wave-uniform, so every lane writes the same
// thing and the readback cannot race):
//   out[0..1]  the kernarg block's first user argument = the output BO's VA
//   out[2..4]  group id x/y/z  (0 for a single workgroup at base 0)
// Observed (the value is per-lane and every lane stores to the same offset, so
// the readback is one lane's value and which one is not established):
//   out[5..7]  local invocation id x/y/z
// Submit the launch-state probe N times through ONE persistent session and ONE
// queue.  This exercises exactly the path the queue-lifetime fix changed
// (drm.cpp CachedSession): the queue and its ring are created on the first
// submission and reused by the rest, where before every submission created and
// destroyed both.  It is the first thing to run on hardware after a reboot:
// N=1 proves the queue, and N=100/1000 prove that reusing it does not churn the
// ring's VA mapping, the PASID-filtered UNMAP_QUEUE, or the all-VMID TLB flush.
int cspersist(unsigned n, std::uint32_t timeout_ms) {
#if !defined(LGPU_HAVE_DRM)
  (void)n;
  (void)timeout_ms;
  std::fprintf(stderr, "cspersist: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  if (!cs_preflight("cspersist")) return 5;
  if (n == 0) return 2;
  std::string error;
  std::uint32_t group[3];
  cs_group(group);
  const unsigned lanes = group[0] * group[1] * group[2];
  auto program = lgpu::envelope::build_cs_probe(error, lanes);
  if (!program) {
    std::fprintf(stderr, "cspersist: %s\n", error.c_str());
    return 2;
  }
  static const std::uint32_t kOff[4] = {0x20u, 0x28u, 0x30u, 0x38u};
  unsigned ok = 0;
  for (unsigned i = 0; i < n; ++i) {
    lgpu::drm::ComputeSubmitRequest request;
    cs_fill_request(request, *program, 256);
    request.timeout_ms = timeout_ms;
    request.persistent_session = true;
    request.dump_signal = true;
    const auto report = lgpu::drm::submit_compute(request);
    if (report.status != lgpu::drm::ComputeSubmitReport::Status::Clean) {
      cs_print_wedge(report, "cspersist");
      std::printf("cspersist: FAILED at submission %u/%u\n", i + 1, n);
      break;
    }
    ++ok;
    if (i < 4 || (i + 1) % 100 == 0 || i + 1 == n) {
      std::uint64_t v[4] = {0, 0, 0, 0};
      for (int k = 0; k < 4; ++k) {
        if (kOff[k] + 8 <= report.signal_raw.size())
          std::memcpy(&v[k], report.signal_raw.data() + kOff[k], 8);
      }
      std::printf("cspersist %u/%u signal=0x%016llx ts20=0x%016llx ts30=0x%016llx "
                  "ts38=0x%016llx win=%llu\n",
                  i + 1, n, static_cast<unsigned long long>(report.signal_value),
                  (unsigned long long)v[0], (unsigned long long)v[2],
                  (unsigned long long)v[3],
                  (unsigned long long)(v[3] - v[2]));
    }
  }
  // Exactly one queue teardown for the whole run, through the explicit path.
  lgpu::drm::close_cached_session();
  std::printf("cspersist: %u/%u submissions completed on one queue\n", ok, n);
  return ok == n ? 0 : 3;
#endif
}

int csprobe(std::uint32_t timeout_ms) {
#if !defined(LGPU_HAVE_DRM)
  (void)timeout_ms;
  std::fprintf(stderr, "csprobe: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  if (!cs_preflight("csprobe")) return 5;
  std::string error;
  std::uint32_t group[3];
  cs_group(group);
  const unsigned lanes = group[0] * group[1] * group[2];
  auto program = lgpu::envelope::build_cs_probe(error, lanes);
  if (!program) {
    std::fprintf(stderr, "csprobe: %s\n", error.c_str());
    return 2;
  }
  lgpu::drm::ComputeSubmitRequest request;
  cs_fill_request(request, *program, 256);
  request.timeout_ms = timeout_ms;
  // The vendor's own OpenCL profiling gets START/END timestamps for compute
  // dispatches from this queue, and the signal allocation is sized for a
  // firmware block; dump it on request so the offsets can be located rather
  // than guessed.  This is also the enabling step for the CS microbench.
  request.dump_signal = std::getenv("LGPU_CS_DUMP_SIGNAL") != nullptr;
  const auto report = lgpu::drm::submit_compute(request);
  if (report.status != lgpu::drm::ComputeSubmitReport::Status::Clean) {
    cs_print_wedge(report, "csprobe");
    return 3;
  }
  std::uint32_t got[8] = {0};
  for (unsigned i = 0; i < 8; ++i) {
    if (4u * i + 4 <= report.output.size())
      std::memcpy(&got[i], report.output.data() + 4u * i, 4);
  }
  const lgpu::drm::ComputeVaLayout& va = lgpu::drm::compute_va_layout();
  const std::uint32_t want_lo = static_cast<std::uint32_t>(va.output);
  const std::uint32_t want_hi = static_cast<std::uint32_t>(va.output >> 32);
  std::printf("csprobe signal=0x%016llx\n",
              static_cast<unsigned long long>(report.signal_value));
  std::printf("kernarg+0 arg0 = 0x%08x%08x (expect the output BO VA "
              "0x%08x%08x)\n",
              got[1], got[0], want_hi, want_lo);
  std::printf("group id x/y/z = %u/%u/%u (expect 0/0/0 for one workgroup)\n",
              got[2], got[3], got[4]);
  std::printf("local id x/y/z = %u/%u/%u (observed: per-lane, one lane's "
              "value)\n",
              got[5], got[6], got[7]);
  int failures = 0;
  if (got[0] != want_lo || got[1] != want_hi) {
    std::printf("FAIL cs-launch-state: the kernarg pointer did not deliver "
                "the output VA\n");
    ++failures;
  }
  if (got[2] != 0 || got[3] != 0 || got[4] != 0) {
    std::printf("FAIL cs-launch-state: group id is not zero for a single "
                "workgroup at base 0\n");
    ++failures;
  }
  if (request.dump_signal) {
    std::printf("-- signal BO (%zu bytes), non-zero 16-byte rows --\n",
                report.signal_raw.size());
    for (std::size_t i = 0; i + 16 <= report.signal_raw.size(); i += 16) {
      bool zero = true;
      for (std::size_t j = i; j < i + 16; ++j)
        if (report.signal_raw[j]) { zero = false; break; }
      if (zero) continue;
      std::printf("  +0x%04zx:", i);
      for (std::size_t j = i; j < i + 16; ++j)
        std::printf(" %02x", report.signal_raw[j]);
      std::printf("\n");
    }
  }
  if (failures == 0) {
    std::printf("PASS cs-launch-state (kernarg, group ids, flat store, "
                "completion signal)\n");
  }
  return failures == 0 ? 0 : 1;
#endif
}

// Host-only: run the compute preflight and print what it checked.  No device
// is opened, so this is the check to run before ever touching hardware.
// Graphics-stage counterpart of csmask: the same p0..p3 write/read-back
// through the PS carrier (4x4 = 16 lanes).
int psmask(unsigned variant, bool dry_run) {
  std::string error;
  auto program = lgpu::envelope::build_ps_mask_probe(variant, error);
  if (!program) {
    std::fprintf(stderr, "psmask: %s\n", error.c_str());
    return 2;
  }
  if (dry_run) {
    std::printf("-- PS mask probe variant %u (%zu bytes, %zu instructions; "
                "sgpr<=%u vgpr<=%u) --\n%s",
                variant, program->bytes.size(), program->words.size(),
                program->sgpr_high_water, program->vgpr_high_water,
                program->listing.c_str());
    return 0;
  }
#if !defined(LGPU_HAVE_DRM)
  std::fprintf(stderr, "psmask: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  lgpu::drm::PsSubmitRequest request;
  request.shader_words.resize(program->words.size() * 2);
  for (std::size_t i = 0; i < program->words.size(); ++i) {
    request.shader_words[2 * i] = program->words[i].lo;
    request.shader_words[2 * i + 1] = program->words[i].hi;
  }
  request.result_bytes = 0x1000;
  request.argument_dwords.assign(request.result_bytes / 4, 0u);
  const auto report = lgpu::drm::submit_ps(request);
  if (report.status != lgpu::drm::PsStatus::Clean) {
    std::fprintf(stderr, "psmask: submit %s\n", report.detail.c_str());
    return 3;
  }
  std::uint32_t got[4] = {0, 0, 0, 0};
  for (unsigned i = 0; i < 4; ++i)
    if (4u * i + 4 <= report.result.size())
      std::memcpy(&got[i], report.result.data() + 4u * i, 4);
  std::printf("psmask variant=%u p0=0x%08x p1=0x%08x p2=0x%08x p3=0x%08x",
              variant, got[0], got[1], got[2], got[3]);
  if (variant >= 10) {
    // 0x10 carries the individual test's readback; 0x40 is where variant 14's
    // gated vector store landed.
    std::uint32_t extra[2] = {0, 0};
    if (16u + 4 <= report.result.size())
      std::memcpy(&extra[0], report.result.data() + 16u, 4);
    if (0x40u + 4 <= report.result.size())
      std::memcpy(&extra[1], report.result.data() + 0x40u, 4);
    std::printf(" r10=0x%08x st=0x%08x\n", extra[0], extra[1]);
    return 0;
  }
  std::printf("\n");
  if (variant == 6) {
    std::printf("OBSERVE psmask: p1 was not written\n");
    return 0;
  }
  if (got[1] != 0x5a5aa5a5u) {
    std::printf("FAIL ps-mask-probe: p1 read back 0x%08x\n", got[1]);
    return 1;
  }
  std::printf("PASS ps-mask-probe: p1 survived variant %u\n", variant);
  return 0;
#endif
}

// X-1 raw-shader sequence probe: p1/p0/p2/p3 write-read-back around a chosen
// exec-mask disturbance.  One program, one submission, fixed output offsets.
int csmask(unsigned variant, std::uint32_t timeout_ms, bool dry_run) {
  std::string error;
  std::uint32_t group[3];
  cs_group(group);
  const unsigned lanes = group[0] * group[1] * group[2];
  auto program = lgpu::envelope::build_cs_mask_probe(variant, error, lanes);
  if (!program) {
    std::fprintf(stderr, "csmask: %s\n", error.c_str());
    return 2;
  }
  if (dry_run) {
    std::printf("-- mask probe variant %u (%zu bytes, %zu instructions; "
                "sgpr<=%u vgpr<=%u) --\n%s",
                variant, program->bytes.size(), program->words.size(),
                program->sgpr_high_water, program->vgpr_high_water,
                program->listing.c_str());
    if (std::getenv("LGPU_DUMP_WORDS")) {
      for (std::size_t i = 0; i < program->words.size(); ++i)
        std::printf("word %3zu: lo=0x%08x hi=0x%08x\n", i,
                    program->words[i].lo, program->words[i].hi);
    }
    return 0;
  }
#if !defined(LGPU_HAVE_DRM)
  std::fprintf(stderr, "csmask: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  if (!cs_preflight("csmask")) return 5;
  lgpu::drm::ComputeSubmitRequest request;
  cs_fill_request(request, *program, 256);
  request.timeout_ms = timeout_ms;
  const auto report = lgpu::drm::submit_compute(request);
  if (report.status != lgpu::drm::ComputeSubmitReport::Status::Clean) {
    cs_print_wedge(report, "csmask");
    return 3;
  }
  std::uint32_t got[4] = {0, 0, 0, 0};
  for (unsigned i = 0; i < 4; ++i)
    if (4u * i + 4 <= report.output.size())
      std::memcpy(&got[i], report.output.data() + 4u * i, 4);
  // Output offsets: 0 = p0, 4 = p1, 8 = p2, 12 = p3.
  std::printf("csmask variant=%u p0=0x%08x p1=0x%08x p2=0x%08x p3=0x%08x "
              "signal=0x%016llx\n",
              variant, got[0], got[1], got[2], got[3],
              static_cast<unsigned long long>(report.signal_value));
  // Variant 6 does not write p1: it reports whatever the launch left there.
  if (variant == 6) {
    std::printf("OBSERVE mask-probe: p1 was not written (launch value)\n");
    return 0;
  }
  const std::uint32_t want = variant == 8 ? 0xdeadbeefu : 0x5a5aa5a5u;
  if (got[1] != want) {
    std::printf("FAIL mask-probe: p1 read back 0x%08x, not the written "
                "0x%08x\n",
                got[1], want);
    return 1;
  }
  std::printf("PASS mask-probe: p1 survived variant %u\n", variant);
  return 0;
#endif
}

// U-4 / U-6 / U-7: the three probes the LCO review left blocked.  One command,
// one submission, a fixed output layout per probe - and a printer that names
// the offsets, so a reader never has to reconstruct them from the builder.
int csp(const char* probe, unsigned variant, std::uint32_t timeout_ms,
        bool dry_run) {
  std::string error;
  std::uint32_t group[3];
  cs_group(group);
  const unsigned lanes = group[0] * group[1] * group[2];
  std::optional<lgpu::envelope::EnvelopeProgram> program;
  if (std::strcmp(probe, "divchk") == 0)
    program = lgpu::envelope::build_cs_divergent_check_probe(variant, error,
                                                             lanes);
  else if (std::strcmp(probe, "matrix") == 0)
    program = lgpu::envelope::build_cs_matrix_probe(variant, error, lanes);
  else if (std::strcmp(probe, "masklife") == 0)
    program = lgpu::envelope::build_cs_mask_lifetime_probe(variant, error,
                                                           lanes);
  else {
    std::fprintf(stderr, "csp: unknown probe '%s' (divchk|matrix|masklife)\n",
                 probe);
    return 2;
  }
  if (!program) {
    std::fprintf(stderr, "csp: %s\n", error.c_str());
    return 2;
  }
  std::printf("csp probe=%s variant=%u lanes=%u lds=%u bytes=%zu "
              "instructions=%zu\n",
              probe, variant, lanes, program->shared_bytes,
              program->bytes.size(), program->words.size());
  if (dry_run) {
    std::printf("%s", program->listing.c_str());
    if (std::getenv("LGPU_DUMP_WORDS"))
      for (std::size_t i = 0; i < program->words.size(); ++i)
        std::printf("word %3zu: lo=0x%08x hi=0x%08x\n", i,
                    program->words[i].lo, program->words[i].hi);
    return 0;
  }
#if !defined(LGPU_HAVE_DRM)
  std::fprintf(stderr, "csp: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  if (!cs_preflight("csp")) return 5;
  lgpu::drm::ComputeSubmitRequest request;
  cs_fill_request(request, *program, 0x1000);
  // The carriers stage their own sentinel cells (and the matrix probe stages
  // none: it fills LDS from the shader); dropping the list here would make
  // every "did the lane write the sentinel" readback a zero.
  request.output_preload = program->arg_preloads;
  request.timeout_ms = timeout_ms;
  const auto report = lgpu::drm::submit_compute(request);
  if (report.status != lgpu::drm::ComputeSubmitReport::Status::Clean) {
    cs_print_wedge(report, "csp");
    return 3;
  }
  auto dword = [&](std::size_t i) -> std::uint32_t {
    std::uint32_t v = 0;
    if (4 * i + 4 <= report.output.size())
      std::memcpy(&v, report.output.data() + 4 * i, 4);
    return v;
  };
  auto record = [&](const char* label, std::uint32_t base) {
    std::printf("  %s:", label);
    for (unsigned l = 0; l < lanes && l < 32; ++l)
      std::printf(" %08x", dword(base / 4 + l));
    std::printf("\n");
  };
  if (std::strcmp(probe, "divchk") == 0) {
    std::printf("armA store=0x%08x read=0x%08x | armB store=0x%08x "
                "read=0x%08x | round2 read=0x%08x | p0=0x%08x\n",
                dword(0x40 / 4), dword(0x44 / 4), dword(0x48 / 4),
                dword(0x4c / 4), dword(0x54 / 4), dword(0x50 / 4));
    record("armA lanes", 0x200);
    record("armB lanes", 0x300);
  } else if (std::strcmp(probe, "matrix") == 0) {
    record("c0", 0x100);
    record("c1", 0x200);
    record("c2", 0x300);
    record("c3", 0x400);
    // The x4 form is declared as four registers, but u-l-i-g records that it
    // writes more than four, so four more are recorded as well.
    record("c4", 0x500);
    record("c5", 0x600);
    record("c6", 0x700);
    record("c7", 0x800);
  } else {
    std::printf("p0=0x%08x p1=0x%08x p2=0x%08x p3=0x%08x\n",
                dword(4), dword(5), dword(6), dword(7));
  }
  std::printf("  signal=0x%016llx\n",
              static_cast<unsigned long long>(report.signal_value));
  return 0;
#endif
}

// E-1: PJ2 is the per-instruction mask selector.  Each variant stages known
// p0/p1 contents and reports, per lane, whether the instruction under test
// wrote.  Output: p0..p3 at 0..12, the post-test p1 at 0x10, the branch
// marker at 0x18, the two gated-store dwords at 0x120/0x124, and the 32
// per-lane results at 0x100.
int pj2(unsigned variant, std::uint32_t timeout_ms, bool dry_run) {
  std::string error;
  std::uint32_t group[3];
  cs_group(group);
  const unsigned lanes = group[0] * group[1] * group[2];
  auto program = lgpu::envelope::build_cs_pj2_probe(variant, error, lanes);
  if (!program) {
    std::fprintf(stderr, "pj2: %s\n", error.c_str());
    return 2;
  }
  if (dry_run) {
    std::printf("-- PJ2 probe variant %u (%zu bytes, %zu instructions; "
                "sgpr<=%u vgpr<=%u) --\n%s",
                variant, program->bytes.size(), program->words.size(),
                program->sgpr_high_water, program->vgpr_high_water,
                program->listing.c_str());
    // LGPU_DUMP_WORDS turns the listing into a word-per-instruction dump, so
    // a control can be checked against the encoded field rather than the
    // mnemonic: the emitter writes PJ2 at INST_HI[17:16] for these layouts,
    // and only the selector bits may differ between two otherwise equal
    // probes.
    if (std::getenv("LGPU_DUMP_WORDS"))
      for (std::size_t i = 0; i < program->words.size(); ++i)
        std::printf("word %3zu: lo=0x%08x hi=0x%08x\n", i,
                    program->words[i].lo, program->words[i].hi);
    return 0;
  }
#if !defined(LGPU_HAVE_DRM)
  std::fprintf(stderr, "pj2: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  if (!cs_preflight("pj2")) return 5;
  lgpu::drm::ComputeSubmitRequest request;
  // 1024 bytes: the per-lane record lives at 0x100..0x17c.
  cs_fill_request(request, *program, 1024);
  // Only the shared-memory variants declare LDS; zero everywhere else keeps
  // the descriptor of the E-1 variants unchanged.
  request.shared_segment_size = program->shared_bytes;
  request.timeout_ms = timeout_ms;
  const auto report = lgpu::drm::submit_compute(request);
  if (report.status != lgpu::drm::ComputeSubmitReport::Status::Clean) {
    cs_print_wedge(report, "pj2");
    return 3;
  }
  auto dword = [&](std::size_t i) -> std::uint32_t {
    std::uint32_t v = 0;
    if (4 * i + 4 <= report.output.size())
      std::memcpy(&v, report.output.data() + 4 * i, 4);
    return v;
  };
  std::printf("pj2 variant=%u p0=0x%08x p1=0x%08x p2=0x%08x p3=0x%08x "
              "after=0x%08x branch=0x%08x st0=0x%08x st1=0x%08x\n",
              variant, dword(0), dword(1), dword(2), dword(3), dword(4),
              dword(6), dword(128), dword(129));
  if (variant >= 40) {
    // The variants past 40 report up to eight dwords of scalar readback at
    // 0x10..0x2c, in addition to the p0..p3 window above.
    std::printf("probe=");
    for (unsigned i = 4; i < 12; ++i)
      std::printf("%08x%s", dword(i), i == 11 ? "\n" : ",");
  }
  std::printf("lanes=");
  for (unsigned i = 0; i < 32; ++i)
    std::printf("%08x%s", dword(64 + i), i == 31 ? "\n" : ",");
  return 0;
#endif
}

int csverify() {
  std::string detail;
  if (!lgpu::drm::compute_selfcheck(detail)) {
    std::fprintf(stderr, "csverify: FAILED: %s\n", detail.c_str());
    return 1;
  }
  std::printf("csverify: %s\n", detail.c_str());
  return 0;
}

int bench(const std::string& mnemonic, const std::string& kind,
           unsigned copies, unsigned reps, bool dry_run) {
  // Differential wall-clock bench (same shader shape both kinds): lat = one
  // destination-to-source dependency chain per lane, tp = independent 8-way
  // register streams.  Relative, not architectural-cycle numbers.
  const bool chained = kind == "lat";
  std::string error;
  auto program = lgpu::envelope::build_bench(mnemonic, copies, chained, error);
  if (!program) {
    std::fprintf(stderr, "bench: %s: %s\n", mnemonic.c_str(), error.c_str());
    return 2;
  }
  if (dry_run) {
    std::printf("-- bench %s %s (%zu bytes, %zu instructions) --\n%s",
                mnemonic.c_str(), kind.c_str(), program->bytes.size(),
                program->words.size(), program->listing.c_str());
    return 0;
  }
#if !defined(LGPU_HAVE_DRM)
  (void)program;
  std::fprintf(stderr, "bench: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  lgpu::drm::PsSubmitRequest request;
  request.shader_words.resize(program->words.size() * 2);
  for (std::size_t i = 0; i < program->words.size(); ++i) {
    request.shader_words[2 * i] = program->words[i].lo;
    request.shader_words[2 * i + 1] = program->words[i].hi;
  }
  request.result_bytes = 0x4000;  // full page: covers ts words at 0x3000
  request.argument_dwords.assign(0x4000 / 4, 0u);
  request.timestamped = true;
  std::vector<double> samples;
  std::vector<std::uint64_t> deltas;
  samples.reserve(reps);
  deltas.reserve(reps);
  for (unsigned rep = 0; rep < reps; ++rep) {
    const auto t0 = std::chrono::steady_clock::now();
    const auto report = lgpu::drm::submit_ps(request);
    const auto t1 = std::chrono::steady_clock::now();
    if (report.status != lgpu::drm::PsStatus::Clean) {
      std::fprintf(stderr, "bench: submit %s\n", report.detail.c_str());
      return 3;
    }
    samples.push_back(
        std::chrono::duration<double, std::milli>(t1 - t0).count());
    if (std::getenv("LGPU_BENCH_TRACE"))
      std::fprintf(stderr, "  rep %u: ts_begin=0x%016llx ts_end=0x%016llx\n",
                   rep, static_cast<unsigned long long>(report.ts_begin),
                   static_cast<unsigned long long>(report.ts_end));
    if (report.ts_end > report.ts_begin)
      deltas.push_back(report.ts_end - report.ts_begin);
  }
  std::sort(samples.begin(), samples.end());
  const double med = samples[samples.size() / 2];
  std::sort(deltas.begin(), deltas.end());
  std::uint64_t med_delta = 0;
  if (!deltas.empty()) {
    std::nth_element(deltas.begin(), deltas.begin() + deltas.size() / 2,
                     deltas.end());
    med_delta = deltas[deltas.size() / 2];
  }
  // columns: name kind copies reps wall_ms_med wall_ms_min wall_ms_max
  //          gpu_ticks_med gpu_ticks_per_op
  std::printf("%s\t%s\t%u\t%u\t%.4f\t%.4f\t%.4f\t%llu\t%.2f\n",
              mnemonic.c_str(), kind.c_str(), copies, reps, med,
              samples.front(), samples.back(),
              static_cast<unsigned long long>(med_delta),
              static_cast<double>(med_delta) / static_cast<double>(copies));
  return 0;
#endif
}

// Compute-pipe microbench.  Same instruction template as bench(), emitted
// through the compute carrier, and the time comes from the firmware's timestamp
// block in the SIGNAL allocation (four 64-bit counters at +0x20/+0x28/+0x30/
// +0x38, located 2026-10-12).  Which pair is the wave window is not assumed:
// the four per-rep offsets from the earliest counter are printed, and the
// K-copy sweep is what identifies them (the execution window grows with K, the
// enqueue/retire counters do not).
int csbench_run(const std::string& mnemonic, const std::string& kind,
                unsigned copies, unsigned reps, bool persistent) {
  const bool chained = kind == "lat";
  std::string error;
  std::uint32_t group[3];
  cs_group(group);
  const unsigned lanes = group[0] * group[1] * group[2];
  auto program =
      lgpu::envelope::build_cs_bench(mnemonic, copies, chained, lanes, error);
  if (!program) {
    std::fprintf(stderr, "csbench: %s: %s\n", mnemonic.c_str(), error.c_str());
    return 2;
  }
  if (reps == 0) return 2;
  // The declared window is resolved ONCE, so the preflight and every submission
  // below carry the same number.  CSBENCH_SHARED is the diagnostic override
  // described at its use site: it can force a window onto a program with no
  // shared-memory instruction (the discriminator experiment), which is why the
  // preflight runs after it.
  std::uint32_t declared_shared = program->shared_bytes;
  if (const char *forced = std::getenv("CSBENCH_SHARED"))
    declared_shared =
        static_cast<std::uint32_t>(std::strtoul(forced, nullptr, 0));
  {
    std::string why;
    if (!cs_lds_preflight(*program, declared_shared, why)) {
      std::fprintf(stderr, "csbench: %s: %s\n", mnemonic.c_str(), why.c_str());
      return 4;
    }
    if (program->listing.find("_sm_") != std::string::npos)
      std::fprintf(stderr,
                   "csbench: %s: LDS window %u bytes, program executes "
                   "shared-memory instructions\n",
                   mnemonic.c_str(), declared_shared);
  }
#if !defined(LGPU_HAVE_DRM)
  std::fprintf(stderr, "csbench: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  static const std::uint32_t kOff[4] = {0x20u, 0x28u, 0x30u, 0x38u};
  std::vector<std::uint64_t> d[4];
  std::vector<double> wall;
  for (unsigned rep = 0; rep < reps; ++rep) {
    lgpu::drm::ComputeSubmitRequest request;
    cs_fill_request(request, *program, 256);
    // Already resolved and preflighted above.  CSBENCH_SHARED exists to separate
    // the two halves of an LDS failure with one submission each - a declared
    // window carrying no shared-memory instruction (CSBENCH_SHARED=512 on a flat
    // row) versus a shared-memory instruction with no window - and is never a
    // production setting.
    request.shared_segment_size = declared_shared;
    request.dump_signal = true;
    request.persistent_session = persistent;
    const auto t0 = std::chrono::steady_clock::now();
    const auto report = lgpu::drm::submit_compute(request);
    const auto t1 = std::chrono::steady_clock::now();
    if (report.status != lgpu::drm::ComputeSubmitReport::Status::Clean) {
      std::fprintf(stderr, "csbench: %s\n", report.detail.c_str());
      return 3;
    }
    wall.push_back(
        std::chrono::duration<double, std::milli>(t1 - t0).count());
    std::uint64_t v[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; ++i) {
      if (kOff[i] + 8 <= report.signal_raw.size())
        std::memcpy(&v[i], report.signal_raw.data() + kOff[i], 8);
    }
    const std::uint64_t base = std::min(std::min(v[0], v[1]),
                                        std::min(v[2], v[3]));
    if (std::getenv("LGPU_BENCH_TRACE"))
      std::fprintf(stderr,
                   "  rep %u: +20=0x%016llx +28=0x%016llx +30=0x%016llx "
                   "+38=0x%016llx\n",
                   rep, (unsigned long long)v[0], (unsigned long long)v[1],
                   (unsigned long long)v[2], (unsigned long long)v[3]);
    for (int i = 0; i < 4; ++i) d[i].push_back(v[i] - base);
  }
  auto med = [](std::vector<std::uint64_t> v) {
    std::sort(v.begin(), v.end());
    return v[v.size() / 2];
  };
  std::sort(wall.begin(), wall.end());
  // The K sweep (2026-10-12) fixes which pair is the execution window: with a
  // chain of dependent v_add_f32, d28 and d38 both grow with copies while d30
  // stays at a fixed 2081 after d20, and the ordering is always
  // +0x20 (enqueue) <= +0x30 (wave start) <= +0x38 (wave end) <= +0x28
  // (retire).  The window is therefore +0x38 - +0x30; +0x28 - +0x38 is the
  // fixed retire cost and is not part of it.
  std::vector<std::uint64_t> win;
  for (std::size_t i = 0; i < d[2].size(); ++i)
    win.push_back(d[3][i] - d[2][i]);
  // columns: mnemonic kind copies reps wall_ms_med win_ticks ticks_per_op
  //          d20 d28 d30 d38
  std::printf("CSBENCH\t%s\t%s\t%u\t%u\t%.4f\t%llu\t%.3f\t%llu\t%llu\t"
              "%llu\t%llu\t%s\n",
              mnemonic.c_str(), kind.c_str(), copies, reps, wall[wall.size() / 2],
              (unsigned long long)med(win),
              static_cast<double>(med(win)) / static_cast<double>(copies),
              (unsigned long long)med(d[0]), (unsigned long long)med(d[1]),
              (unsigned long long)med(d[2]), (unsigned long long)med(d[3]),
              lgpu::envelope::bench_note(mnemonic));
  return 0;
#endif
}

// Single shot: print the listing in dry mode, otherwise run one configuration.
int csbench(const std::string& mnemonic, const std::string& kind,
            unsigned copies, unsigned reps, bool dry_run) {
  if (dry_run) {
    std::string error;
    std::uint32_t group[3];
    cs_group(group);
    auto program = lgpu::envelope::build_cs_bench(
        mnemonic, copies, kind == "lat", group[0] * group[1] * group[2], error);
    if (!program) {
      std::fprintf(stderr, "csbench: %s: %s\n", mnemonic.c_str(),
                   error.c_str());
      return 2;
    }
    std::printf("-- csbench %s %s copies=%u (%zu bytes, %zu instructions) "
                "--\n%s",
                mnemonic.c_str(), kind.c_str(), copies, program->bytes.size(),
                program->words.size(), program->listing.c_str());
    return 0;
  }
  return csbench_run(mnemonic, kind, copies, reps, false);
}

// Stdin: one "mnemonic kind copies reps" per line.  Every submission shares one
// process-lifetime DRM file and user queue, the way the validation batch does,
// because a few thousand create/destroy cycles is what wedges the pipe.
int csbench_batch() {
  std::string line;
  // Bounded by construction.  The 2026-10-12 full sweep created ~5,400 user
  // queues in one run and CPIPE faulted at the ring's own address; the kernel's
  // queue lifetime is a known Axis-3 defect, so this process publishes at most
  // LGPU_CS_BENCH_MAX streams (default 100, well under the ~330 cycles the
  // submit layer documents as the wedge threshold) and stops at the first
  // failure instead of feeding a queue that may already be in a bad state.
  unsigned long long submissions = 0;
  unsigned long long max_submissions = 100;
  if (const char* e = std::getenv("LGPU_CS_BENCH_MAX"))
    max_submissions = std::strtoull(e, nullptr, 0);
  std::printf("CSBENCH-HEADER\tmnemonic\tkind\tcopies\treps\twall_ms\t"
              "win_ticks\tticks_per_op\td20\td28\td30\td38\n");
  while (std::getline(std::cin, line)) {
    if (line.empty() || line[0] == '#') continue;
    std::istringstream in(line);
    std::string m, kind;
    unsigned copies = 1, reps = 1;
    in >> m >> kind >> copies >> reps;
    if (m.empty() || kind.empty()) continue;
    if (submissions + reps > max_submissions) {
      std::printf("CSBENCH-LIMIT\t%llu\n", submissions);
      break;
    }
    // rc == 2 is "this mnemonic has no benchable form" (no canonical variant or
    // not materializable) - a property of the instruction, not a failed
    // submission, so it is a SKIP and the process keeps going.  Any other
    // non-zero code means a submission did not come back clean, and those are
    // the ones that compound, so they stop the process.
    const int rc = csbench_run(m, kind, copies, reps, true);
    if (rc == 2) {
      std::printf("CSBENCH-SKIP\t%s\t%s\n", m.c_str(), kind.c_str());
      continue;
    }
    if (rc != 0) {
      std::printf("CSBENCH-FAULT\t%s\t%s\t%u\t%d\n", m.c_str(),
                  kind.c_str(), copies, rc);
      break;
    }
    submissions += reps;
  }
  std::printf("CSBENCH-END\t%llu\n", submissions);
  // Same explicit teardown as csrun_batch: one queue, freed once.
  lgpu::drm::close_cached_session();
  return 0;
}

int getregval(std::uint32_t lo, std::uint32_t hi) {
  // One program: N getreg s8,<id> probes, values at 0x40 + 4*i.  Prints the
  // raw readback dwords (no expected values; id semantics are the question).
  std::vector<std::uint32_t> ids;
  for (std::uint32_t id = lo; id <= hi && ids.size() < 200; ++id) {
    ids.push_back(id);
  }
  if (ids.empty()) return 2;
  std::string error;
  auto program = lgpu::envelope::build_getreg_sweep(ids, error);
  if (!program) {
    std::fprintf(stderr, "getregval: %s\n", error.c_str());
    return 2;
  }
#if !defined(LGPU_HAVE_DRM)
  (void)program;
  std::fprintf(stderr, "getregval: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  lgpu::drm::PsSubmitRequest request;
  request.shader_words.resize(program->words.size() * 2);
  for (std::size_t i = 0; i < program->words.size(); ++i) {
    request.shader_words[2 * i] = program->words[i].lo;
    request.shader_words[2 * i + 1] = program->words[i].hi;
  }
  request.result_bytes = 0x2000;
  request.argument_dwords.assign(0x2000 / 4, 0u);
  const auto report = lgpu::drm::submit_ps(request);
  if (report.status != lgpu::drm::PsStatus::Clean) {
    std::fprintf(stderr, "getregval: submit %s\n", report.detail.c_str());
    return 3;
  }
  std::printf("getregval %u..%u (count=%zu):\n", lo, hi, ids.size());
  for (std::size_t i = 0; i < ids.size(); ++i) {
    const std::uint32_t off = 0x40 + 4u * i;
    std::uint32_t v = 0;
    if (off + 4 <= report.result.size()) {
      std::memcpy(&v, report.result.data() + off, 4);
    }
    std::printf("id=0x%08x  value=0x%08x\n", ids[i], v);
  }
  return 0;
#endif
}

int selectorval_impl(std::uint32_t lo, std::uint32_t hi, bool sop);

// VOP-format sweep (v_mov.b32, field GJ11 low byte).
int selectorval(std::uint32_t lo, std::uint32_t hi) {
  return selectorval_impl(lo, hi, false);
}

// SOP-format sweep (s_mov.b32, field GJ8).  Same GeneralSource8 encoding, a
// different machine format: running both separates "the selector space has
// this value" from "this one instruction's decoder does".
int selectorval_sop(std::uint32_t lo, std::uint32_t hi) {
  return selectorval_impl(lo, hi, true);
}

int selectorval_impl(std::uint32_t lo, std::uint32_t hi, bool sop) {
  // Sweep literal selector bytes through the general-source field of one
  // instruction and store each result at 0x40 + 4*i.  Two controls are emitted
  // FIRST so a broken probe can never be mistaken for a hardware result:
  //   slot 0: selector 0x66 (zero)   must read 0x00000000
  //   slot 1: selector 0xa0 (i32(1)) must read 0x00000001
  // The controls also prove that the byte actually reaches the general-source
  // field, which is the whole question for the unmodelled encodings.
  std::vector<std::uint32_t> selectors{0x66u, 0xa0u};
  for (std::uint32_t b = lo; b <= hi && selectors.size() < 200; ++b) {
    selectors.push_back(b);
  }
  if (selectors.size() <= 2) return 2;
  std::string error;
  auto program = sop ? lgpu::envelope::build_selector_sweep_sop(selectors, error)
                     : lgpu::envelope::build_selector_sweep(selectors, error);
  if (!program) {
    std::fprintf(stderr, "selectorval: %s\n", error.c_str());
    return 2;
  }
#if !defined(LGPU_HAVE_DRM)
  (void)program;
  std::fprintf(stderr, "selectorval: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  lgpu::drm::PsSubmitRequest request;
  request.shader_words.resize(program->words.size() * 2);
  for (std::size_t i = 0; i < program->words.size(); ++i) {
    request.shader_words[2 * i] = program->words[i].lo;
    request.shader_words[2 * i + 1] = program->words[i].hi;
  }
  request.result_bytes = 0x2000;
  request.argument_dwords.assign(0x2000 / 4, 0u);
  const auto report = lgpu::drm::submit_ps(request);
  if (report.status != lgpu::drm::PsStatus::Clean) {
    std::fprintf(stderr, "selectorval: submit %s\n", report.detail.c_str());
    return 3;
  }
  auto readback = [&](std::size_t i) {
    const std::uint32_t off = 0x40 + 4u * i;
    std::uint32_t v = 0;
    if (off + 4 <= report.result.size()) {
      std::memcpy(&v, report.result.data() + off, 4);
    }
    return v;
  };
  const std::uint32_t zero_val = readback(0);
  const std::uint32_t one_val = readback(1);
  const char* form = sop ? "s_mov.b32 s8,<byte> (SOP, GJ8)" : "v_mov.b32 p0,v8,<byte> (VOP, GJ11)";
  std::printf("selectorval %s  %u..%u (count=%zu):\n", form, lo, hi,
              selectors.size() - 2);
  std::printf("control zero   (0x66) = 0x%08x expect 0x00000000\n", zero_val);
  std::printf("control i32(1) (0xa0) = 0x%08x expect 0x00000001\n", one_val);
  for (std::size_t i = 2; i < selectors.size(); ++i) {
    std::printf("b=0x%02x  value=0x%08x\n", selectors[i], readback(i));
  }
  // A failed control invalidates the whole run; say so in the exit status.
  if (zero_val != 0u || one_val != 1u) {
    std::fprintf(stderr,
                 "selectorval: CONTROL FAILED - probe is not reading the "
                 "general-source field; the sweep below is meaningless\n");
    return 4;
  }
  return 0;
#endif
}

int selectorhi(std::uint32_t base) {
  // Probe the three high bits of the 11-bit general source for one base:
  // 0x100 vector domain, 0x200 clear sign, 0x400 flip sign.  v8 is preloaded
  // with -1.0f and the destination is v9, so a base that names v8 exercises
  // the vector domain against a value whose sign bit is known to be set.
  std::string error;
  auto program = lgpu::envelope::build_selector_hi_sweep(base, error);
  if (!program) {
    std::fprintf(stderr, "selectorhi: %s\n", error.c_str());
    return 2;
  }
#if !defined(LGPU_HAVE_DRM)
  (void)program;
  std::fprintf(stderr, "selectorhi: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  lgpu::drm::PsSubmitRequest request;
  request.shader_words.resize(program->words.size() * 2);
  for (std::size_t i = 0; i < program->words.size(); ++i) {
    request.shader_words[2 * i] = program->words[i].lo;
    request.shader_words[2 * i + 1] = program->words[i].hi;
  }
  request.result_bytes = 0x2000;
  request.argument_dwords.assign(0x2000 / 4, 0u);
  const auto report = lgpu::drm::submit_ps(request);
  if (report.status != lgpu::drm::PsStatus::Clean) {
    std::fprintf(stderr, "selectorhi: submit %s\n", report.detail.c_str());
    return 3;
  }
  std::printf("selectorhi base=0x%02x   (v8 preloaded with 0xf3 = -1.0f = 0xbf800000)\n",
              base & 0xffu);
  for (std::uint32_t hi = 0; hi < 8; ++hi) {
    const std::uint32_t off = 0x40 + 4u * hi;
    std::uint32_t v = 0;
    if (off + 4 <= report.result.size()) {
      std::memcpy(&v, report.result.data() + off, 4);
    }
    std::printf("  sel=0x%03x  value=0x%08x\n", (hi << 8) | (base & 0xffu), v);
  }
  return 0;
#endif
}

int movidx(std::uint32_t idx, std::uint32_t magic) {
  return movidxb(0, idx, magic);
}int movidxb(std::uint32_t base, std::uint32_t idx, std::uint32_t magic) {
  std::string error;
  auto program = lgpu::envelope::build_movidx_base_probe(base, idx, magic, error);
  if (!program) {
    std::fprintf(stderr, "movidx: %s\n", error.c_str());
    return 2;
  }
  // Dry-run listing when asked; otherwise submit and print the value.
  if (std::getenv("LGPU_MOVIDX_DRYRUN")) {
    std::printf("%s", program->listing.c_str());
    return 0;
  }
#if !defined(LGPU_HAVE_DRM)
  (void)program;
  std::fprintf(stderr, "movidx: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  lgpu::drm::PsSubmitRequest request;
  request.shader_words.resize(program->words.size() * 2);
  for (std::size_t i = 0; i < program->words.size(); ++i) {
    request.shader_words[2 * i] = program->words[i].lo;
    request.shader_words[2 * i + 1] = program->words[i].hi;
  }
  request.result_bytes = 0x2000;
  request.argument_dwords.assign(0x2000 / 4, 0u);
  const auto report = lgpu::drm::submit_ps(request);
  if (report.status != lgpu::drm::PsStatus::Clean) {
    std::fprintf(stderr, "movidx: submit %s\n", report.detail.c_str());
    return 3;
  }
  std::uint32_t v = 0;
  if (0x40 + 4 <= report.result.size()) {
    std::memcpy(&v, report.result.data() + 0x40, 4);
  }
  std::printf("movidx base=%u idx=%u magic=0x%08x -> 0x%08x\n", base, idx,
              magic, v);
  return 0;
#endif
}

int tsprobe(std::uint32_t shape, std::uint32_t fs_size) {
#if !defined(LGPU_HAVE_DRM)
  (void)shape;
  (void)fs_size;
  std::fprintf(stderr, "tsprobe: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#else
  const auto report =
      lgpu::drm::submit_timestamp_probe(shape, 3000, fs_size);
  if (report.status != lgpu::drm::PsStatus::Clean) {
    std::fprintf(stderr, "tsprobe: submit %s\n", report.detail.c_str());
    return 3;
  }
  // The IB wrapper writes a full 64-bit timestamp at arg+0x3000 (begin) and a
  // second at arg+0x3008 (end).  Reading one dword of each and OR-ing them
  // together produced a number that was neither a timestamp nor a difference:
  // read both words in full and subtract.
  std::uint64_t begin = 0, end = 0;
  if (0x3010 <= report.result.size()) {
    std::memcpy(&begin, report.result.data() + 0x3000, 8);
    std::memcpy(&end, report.result.data() + 0x3008, 8);
  }
  const std::uint64_t delta = end - begin;
  // The counter advances at the GPU clock, not at the ~32 ns/tick this tool
  // used to assume.  Measured by reading the absolute stamp twice around a
  // known wall-clock interval: 748.5-749.8 MHz over five 10 s runs, i.e.
  // ~1.334 ns/tick.  The old constant was wrong by about 24x, so every ns
  // figure this probe printed was too large by that factor.  The rate is a
  // clock, so it is only valid while DVFS is off (this part reports "GPU DPM
  // function is disabled"); re-measure rather than assume on another part.
  constexpr double kGpuTickNanos = 1.334;
  std::printf("tsprobe shape %u: begin=0x%016llx end=0x%016llx delta=%llu ticks "
              "(%.1f ns @%.3f ns/tick)\n",
              shape,
              static_cast<unsigned long long>(begin),
              static_cast<unsigned long long>(end),
              static_cast<unsigned long long>(delta),
              static_cast<double>(delta) * kGpuTickNanos, kGpuTickNanos);
  return begin == 0 && end == 0 ? 4 : 0;
#endif
}

// Compute (CPIPE) user-queue probe.  Allocates the ring, maps it, creates a
// COMPUTE queue over it and frees it again; no dispatch is sent.  Answers
// whether this session may create a compute queue and whether the kernel takes
// a ring we mapped ourselves, which are prerequisites for the MCS carrier and
// are independent of its encoding.
int csqprobe(std::uint32_t ring_size) {
#ifdef LGPU_HAVE_DRM
  const auto report = lgpu::drm::compute_queue_probe(ring_size);
  std::printf("csqprobe ring_size=%u ring_va=0x%llx ok=%d queue_id=%u\n",
              report.ring_size,
              static_cast<unsigned long long>(report.ring_va),
              report.ok ? 1 : 0, report.queue_id);
  if (!report.ok) {
    std::fprintf(stderr, "csqprobe: %s\n", report.detail.c_str());
    return 3;
  }
  return 0;
#else
  (void)ring_size;
  std::fprintf(stderr, "csqprobe: drm submit layer not built (LGPU_HAVE_DRM)\n");
  return 3;
#endif
}

}  // namespace

// Host-side plan for one compute dispatch: builds the kernel descriptor and the
// MCS stream with the vendored builder and prints them.  Nothing is submitted,
// so this is the step that separates "our stream is wrong" from "the firmware
// would not take it" without spending a debug cycle on each.
int csplan(std::int64_t text_size, std::uint32_t gx, std::uint32_t gy,
           std::uint32_t gz) {
  const auto report = lgpu::drm::compute_plan(
      static_cast<std::uint64_t>(text_size), gx, gy, gz, gx, gy, gz);
  if (!report.ok) {
    std::fprintf(stderr, "csplan: %s\n", report.detail.c_str());
    return 3;
  }
  std::printf("csplan text_va=0x%llx kd_va=0x%llx kernarg_va=0x%llx "
              "signal_va=0x%llx sgprs=%u vgprs=%u\n",
              static_cast<unsigned long long>(report.text_va),
              static_cast<unsigned long long>(report.kd_va),
              static_cast<unsigned long long>(report.kernarg_va),
              static_cast<unsigned long long>(report.signal_va),
              report.num_sgprs, report.num_vgprs);
  std::printf("kd[%zu]:\n", report.kd.size());
  for (std::size_t i = 0; i < report.kd.size(); i += 16) {
    std::printf("  %04zx:", i);
    for (std::size_t j = i; j < i + 16 && j < report.kd.size(); ++j) {
      std::printf(" %02x", report.kd[j]);
    }
    std::printf("\n");
  }
  std::printf("stream[%zu]:\n", report.stream.size());
  for (std::size_t i = 0; i < report.stream.size(); i += 16) {
    std::printf("  %04zx:", i);
    for (std::size_t j = i; j < i + 16 && j < report.stream.size(); ++j) {
      std::printf(" %02x", report.stream[j]);
    }
    std::printf("\n");
  }
  return 0;
}

// One compute dispatch: read the kernel's .text from a file and run it through
// the CPIPE carrier.  The kernel must take a single __global uint* as its first
// argument; the carrier puts that buffer's GPU VA at kernarg offset 0.
int cstest(const char* path, std::uint32_t group, std::uint32_t timeout_ms) {
  std::FILE* f = std::fopen(path, "rb");
  if (f == nullptr) {
    std::fprintf(stderr, "cstest: cannot open %s\n", path);
    return 3;
  }
  std::fseek(f, 0, SEEK_END);
  const long size = std::ftell(f);
  std::fseek(f, 0, SEEK_SET);
  if (size <= 0) {
    std::fclose(f);
    std::fprintf(stderr, "cstest: %s is empty\n", path);
    return 3;
  }
  std::vector<std::uint8_t> text(static_cast<std::size_t>(size));
  const std::size_t got = std::fread(text.data(), 1, text.size(), f);
  std::fclose(f);
  if (got != text.size()) {
    std::fprintf(stderr, "cstest: short read on %s\n", path);
    return 3;
  }

  lgpu::drm::ComputeSubmitRequest request;
  request.text = text;
  request.output_size = 64;
  request.group[0] = request.group[1] = request.group[2] = group;
  request.grid[0] = request.grid[1] = request.grid[2] = group;
  request.timeout_ms = timeout_ms;

  const auto report = lgpu::drm::submit_compute(request);
  const char* status = "?";
  switch (report.status) {
    case lgpu::drm::ComputeSubmitReport::Status::Clean: status = "clean"; break;
    case lgpu::drm::ComputeSubmitReport::Status::Fault: status = "fault"; break;
    case lgpu::drm::ComputeSubmitReport::Status::Timeout: status = "timeout"; break;
    case lgpu::drm::ComputeSubmitReport::Status::Rejected: status = "rejected"; break;
  }
  std::printf("cstest text=%zu bytes group=%u status=%s signal=0x%016llx\n",
              text.size(), group, status,
              static_cast<unsigned long long>(report.signal_value));
  if (!report.output.empty()) {
    std::printf("output[%zu]:", report.output.size());
    for (std::size_t i = 0; i < report.output.size(); ++i) {
      std::printf(" %02x", report.output[i]);
      if (i % 16 == 15) std::printf("\n         ");
    }
    std::printf("\n");
  }
  if (!report.detail.empty()) {
    std::fprintf(stderr, "cstest: %s\n", report.detail.c_str());
  }
  return report.status == lgpu::drm::ComputeSubmitReport::Status::Clean ? 0 : 3;
}

// Run the baseline compute kernel and dump the whole signal allocation.  The
// vendor's own OpenCL profiling obtains START/END for compute dispatches from
// this firmware queue, so if the hardware writes a timestamp block it is here.
int cssig(const char* path) {
  std::FILE* f = std::fopen(path, "rb");
  if (!f) { std::fprintf(stderr, "cssig: cannot open %s\n", path); return 3; }
  std::fseek(f, 0, SEEK_END); const long n = std::ftell(f); std::fseek(f, 0, SEEK_SET);
  std::vector<std::uint8_t> text(static_cast<std::size_t>(n));
  if (std::fread(text.data(), 1, text.size(), f) != text.size()) { std::fclose(f); return 3; }
  std::fclose(f);
  lgpu::drm::ComputeSubmitRequest req;
  req.text = text;
  req.output_size = 64;
  req.dump_signal = true;
  const auto rep = lgpu::drm::submit_compute(req);
  std::printf("status=%d signal=0x%016llx raw=%zu bytes\n",
              static_cast<int>(rep.status),
              static_cast<unsigned long long>(rep.signal_value),
              rep.signal_raw.size());
  // Only non-zero rows: the region is 4 KiB and almost all of it is zero, so
  // printing everything buries whatever the firmware actually wrote.
  for (std::size_t i = 0; i + 16 <= rep.signal_raw.size(); i += 16) {
    bool zero = true;
    for (std::size_t j = i; j < i + 16; ++j)
      if (rep.signal_raw[j]) { zero = false; break; }
    if (zero) continue;
    std::printf("  %04zx:", i);
    for (std::size_t j = i; j < i + 16; ++j)
      std::printf(" %02x", rep.signal_raw[j]);
    std::printf("\n");
  }
  if (!rep.detail.empty()) std::fprintf(stderr, "cssig: %s\n", rep.detail.c_str());
  return 0;
}

int main(int argc, char** argv) {
  if (argc >= 2 && std::strcmp(argv[1], "--matrix") == 0) return matrix();
  if (argc >= 2 && std::strcmp(argv[1], "--specs") == 0) return specs();
  if (argc >= 3 && std::strcmp(argv[1], "run") == 0) {
    const bool dry_run = argc >= 4 && std::strcmp(argv[argc - 1], "--dry-run") == 0;
    return run(argv[2], dry_run);
  }
  if (argc >= 2 && std::strcmp(argv[1], "csrun-batch") == 0) {
    const bool dry_run = argc >= 3 && std::strcmp(argv[argc - 1], "--dry-run") == 0;
    return csrun_batch(dry_run);
  }
  if (argc >= 3 && std::strcmp(argv[1], "csrun") == 0) {
    const bool dry_run = argc >= 4 && std::strcmp(argv[argc - 1], "--dry-run") == 0;
    return csrun(argv[2], dry_run);
  }
  if (argc >= 5 && std::strcmp(argv[1], "movidxb") == 0) {
    return movidxb(static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0)),
                   static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0)),
                   static_cast<std::uint32_t>(std::strtoul(argv[4], nullptr, 0)));
  }
  if (argc >= 4 && std::strcmp(argv[1], "movidx") == 0) {
    return movidx(static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0)),
                  static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0)));
  }
  if (argc >= 4 && std::strcmp(argv[1], "getregval") == 0) {
    return getregval(static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0)),
                     static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0)));
  }
  if (argc >= 4 && std::strcmp(argv[1], "selectorval") == 0) {
    return selectorval(static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0)),
                       static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0)));
  }
  if (argc >= 4 && std::strcmp(argv[1], "selectorvalsop") == 0) {
    return selectorval_sop(
        static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0)),
        static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0)));
  }
  if (argc >= 3 && std::strcmp(argv[1], "selectorhi") == 0) {
    return selectorhi(
        static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0)));
  }
  if (argc >= 3 && std::strcmp(argv[1], "tsprobe") == 0) {
    const std::uint32_t fs_size =
        argc >= 4 ? static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0))
                  : 0u;
    return tsprobe(
        static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0)),
        fs_size);
  }
  if (argc >= 3 && std::strcmp(argv[1], "cssig") == 0) return cssig(argv[2]);
  if (argc >= 3 && std::strcmp(argv[1], "cstest") == 0) {
    const std::uint32_t g = argc >= 4
                                ? static_cast<std::uint32_t>(
                                      std::strtoul(argv[3], nullptr, 0))
                                : 1u;
    const std::uint32_t tmo = argc >= 5
                                  ? static_cast<std::uint32_t>(
                                        std::strtoul(argv[4], nullptr, 0))
                                  : 3000u;
    return cstest(argv[2], g, tmo);
  }
  if (argc >= 3 && std::strcmp(argv[1], "csplan") == 0) {
    const std::int64_t text_size =
        static_cast<std::int64_t>(std::strtoll(argv[2], nullptr, 0));
    const std::uint32_t g = argc >= 4
                                ? static_cast<std::uint32_t>(
                                      std::strtoul(argv[3], nullptr, 0))
                                : 1u;
    return csplan(text_size, g, g, g);
  }
  if (argc >= 2 && std::strcmp(argv[1], "csverify") == 0) {
    return csverify();
  }
  if (argc >= 2 && std::strcmp(argv[1], "csprobe") == 0) {
    const std::uint32_t tmo =
        argc >= 3 ? static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0))
                  : 8000u;
    return csprobe(tmo);
  }
  if (argc >= 3 && std::strcmp(argv[1], "psmask") == 0) {
    const unsigned variant =
        static_cast<unsigned>(std::strtoul(argv[2], nullptr, 0));
    const bool dry_run =
        std::strcmp(argv[argc - 1], "--dry-run") == 0;
    return psmask(variant, dry_run);
  }
  if (argc >= 3 && std::strcmp(argv[1], "csmask") == 0) {
    const unsigned variant =
        static_cast<unsigned>(std::strtoul(argv[2], nullptr, 0));
    const bool dry_run =
        std::strcmp(argv[argc - 1], "--dry-run") == 0;
    const std::uint32_t tmo =
        argc >= 4 && !dry_run
            ? static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0))
            : 8000u;
    return csmask(variant, tmo, dry_run);
  }
  if (argc >= 4 && std::strcmp(argv[1], "csp") == 0) {
    const bool dry_run = std::strcmp(argv[argc - 1], "--dry-run") == 0;
    const unsigned variant =
        static_cast<unsigned>(std::strtoul(argv[3], nullptr, 0));
    const std::uint32_t tmo =
        argc >= 5 && !dry_run
            ? static_cast<std::uint32_t>(std::strtoul(argv[4], nullptr, 0))
            : 8000u;
    return csp(argv[2], variant, tmo, dry_run);
  }
  if (argc >= 3 && std::strcmp(argv[1], "pj2") == 0) {
    const unsigned variant =
        static_cast<unsigned>(std::strtoul(argv[2], nullptr, 0));
    const bool dry_run =
        std::strcmp(argv[argc - 1], "--dry-run") == 0;
    const std::uint32_t tmo =
        argc >= 4 && !dry_run
            ? static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0))
            : 8000u;
    return pj2(variant, tmo, dry_run);
  }
  if (argc >= 3 && std::strcmp(argv[1], "cspersist") == 0) {
    const unsigned n =
        static_cast<unsigned>(std::strtoul(argv[2], nullptr, 0));
    const std::uint32_t tmo =
        argc >= 4 ? static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0))
                  : 8000u;
    return cspersist(n, tmo);
  }
  if (argc >= 2 && std::strcmp(argv[1], "csqprobe") == 0) {
    const std::uint32_t ring_size =
        argc >= 3 ? static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0))
                  : 65536u;
    return csqprobe(ring_size);
  }
  if (argc >= 2 && std::strcmp(argv[1], "csbench-batch") == 0) {
    return csbench_batch();
  }
  if (argc >= 4 && std::strcmp(argv[1], "csbench") == 0) {
    const bool dry_run =
        argc >= 2 && std::strcmp(argv[argc - 1], "--dry-run") == 0;
    return csbench(argv[2], argv[3],
                   argc >= 5 ? static_cast<unsigned>(
                                   std::strtoul(argv[4], nullptr, 0))
                             : 1,
                   argc >= 6 ? static_cast<unsigned>(
                                   std::strtoul(argv[5], nullptr, 0))
                             : 1,
                   dry_run);
  }
  if (argc >= 6 && std::strcmp(argv[1], "bench") == 0) {
    const bool dry_run = argc >= 7 && std::strcmp(argv[argc - 1], "--dry-run") == 0;
    return bench(argv[2], argv[3],
                 static_cast<unsigned>(std::strtoul(argv[4], nullptr, 0)),
                 static_cast<unsigned>(std::strtoul(argv[5], nullptr, 0)),
                 dry_run);
  }
  std::fprintf(stderr,
               "usage: lgpu-gpu --matrix | lgpu-gpu run <m> [--dry-run] | "
               "lgpu-gpu bench <m> lat|tp <copies> <reps> [--dry-run] | "
               "lgpu-gpu csbench <m> lat|tp [copies] [reps] [--dry-run] | "
               "lgpu-gpu csp <divchk|matrix|masklife> <variant> [timeout] "
               "[--dry-run]\n");
  return 2;
}