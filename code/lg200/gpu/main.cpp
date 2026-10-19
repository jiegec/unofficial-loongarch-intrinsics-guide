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

int movidx(std::uint32_t idx, std::uint32_t magic) {
  return movidxb(0, idx, magic);
}

int movidxb(std::uint32_t base, std::uint32_t idx, std::uint32_t magic) {
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
  const auto report =
      lgpu::drm::submit_timestamp_probe(shape, 3000, fs_size);

      lgpu::drm::submit_timestamp_probe(shape, 3000, fs_size);
  if (report.status != lgpu::drm::PsStatus::Clean) {
    std::fprintf(stderr, "tsprobe: submit %s\n", report.detail.c_str());
    return 3;
  }
  std::uint32_t a = 0, b = 0;
  if (0x3000 + 8 <= report.result.size()) {
    std::memcpy(&a, report.result.data() + 0x3000, 4);
    std::memcpy(&b, report.result.data() + 0x3008, 4);
  }
  const std::uint64_t delta =
      static_cast<std::uint64_t>(b) | (static_cast<std::uint64_t>(a) << 32);
  std::printf("tsprobe shape %u: a=0x%08x b=0x%08x delta=%llu ticks (%.1f ns @32ns/tick)\n",
              shape, a, b,
              static_cast<unsigned long long>(delta),
              static_cast<double>(delta) * 32.0);
  return a == 0 && b == 0 ? 4 : 0;
}

}  // namespace

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
  if (argc >= 3 && std::strcmp(argv[1], "tsprobe") == 0) {
    const std::uint32_t fs_size =
        argc >= 4 ? static_cast<std::uint32_t>(std::strtoul(argv[3], nullptr, 0))
                  : 0u;
    return tsprobe(
        static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0)),
        fs_size);
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