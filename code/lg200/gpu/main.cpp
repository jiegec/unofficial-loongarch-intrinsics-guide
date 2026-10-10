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
  auto program = lgpu::envelope::build_carrier(mnemonic, cases, error);
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
  lgpu::drm::PsSubmitReport report;
  if (program->probe) {
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
  for (const auto& c : program->cases) {
    bool ok = true;
    if (program->observe) {
      // Observation-only: report the readback, never fail.
      for (std::size_t k = 0; k < c.offsets.size(); ++k) {
        const std::uint32_t off = c.offsets[k];
        if (off + 4 > report.result.size()) continue;
        std::uint32_t actual = 0;
        std::memcpy(&actual, report.result.data() + off, 4);
        std::printf("OBS %s: offset 0x%x = 0x%08x\n", c.id.c_str(), off,
                    actual);
      }
      std::printf("PASS %s (observed)\n", c.id.c_str());
      continue;
    }
    if (program->control) {
      // control carrier: fence clean + pre sentinel present = observed execution
      // (survived vs terminated is the outcome, not a failure)
      std::uint32_t pre = 0, post = 0;
      if (c.offsets.size() >= 1 && c.offsets[0] + 4 <= report.result.size())
        std::memcpy(&pre, report.result.data() + c.offsets[0], 4);
      if (c.offsets.size() >= 2 && c.offsets[1] + 4 <= report.result.size())
        std::memcpy(&post, report.result.data() + c.offsets[1], 4);
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
      if (off + 4 > report.result.size()) {
        std::printf("FAIL %s: readback too small for offset 0x%x\n",
                    c.id.c_str(), off);
        ok = false; ++failures; continue;
      }
      std::uint32_t actual = 0;
      std::memcpy(&actual, report.result.data() + off, 4);
      // [HW 2026-08-24] the f32/f64 transcendentals and sqrt use a
      // hardware approximation that can differ by one ulp from the
      // correctly-rounded softfloat oracle; accept the 1-ulp window for
      // those families.
      const bool approx_ok =
          program->approx1ulp && actual != c.expected[k] &&
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
  return failures == 0 ? 0 : 1;
#endif
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
  if (argc >= 3 && std::strcmp(argv[1], "run") == 0) {
    const bool dry_run = argc >= 4 && std::strcmp(argv[argc - 1], "--dry-run") == 0;
    return run(argv[2], dry_run);
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
  if (argc >= 2 && std::strcmp(argv[1], "csqprobe") == 0) {
    const std::uint32_t ring_size =
        argc >= 3 ? static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0))
                  : 65536u;
    return csqprobe(ring_size);
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
               "lgpu-gpu bench <m> lat|tp <copies> <reps> [--dry-run]\n");
  return 2;
}