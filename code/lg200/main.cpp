// KISS oracle runner for the Loongson GPU (LG200) instruction set.
//
// Reads case vectors on stdin, one per line, tab-separated:
//
//   <mnemonic>\t<fp-policy>\t<input hex dwords>\t<expected hex dwords>
//
// Oracles and their case vectors live in this directory (one .cpp per
// instruction); the committed ops/registry.cpp maps mnemonic -> oracle.
// Exit code 0 only if every case passed.
#include "ops/registry.hpp"

#include "ops/oracle.hpp"

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool parse_policy(const std::string& tokens, lgpu::test::FpPolicy& policy,
                  std::string& error) {
  std::size_t pos = 0;
  while (pos < tokens.size()) {
    const std::size_t end = tokens.find_first_of("|+", pos);
    const std::string token = tokens.substr(
        pos, end == std::string::npos ? std::string::npos : end - pos);
    if (token == "rtne") {
    } else if (token == "rtz") {
      policy.rounding = lgpu::test::FpPolicy::Rounding::TowardZero;
    } else if (token == "rtup") {
      policy.rounding = lgpu::test::FpPolicy::Rounding::TowardPositive;
    } else if (token == "rtdn") {
      policy.rounding = lgpu::test::FpPolicy::Rounding::TowardNegative;
    } else if (token == "ftz") {
      policy.ftz = true;
    } else if (token == "daz") {
      policy.daz = true;
    } else if (token != "") {
      error = "unknown fp policy token: " + token;
      return false;
    }
    if (end == std::string::npos) break;
    pos = end + 1;
  }
  return true;
}

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

std::vector<std::string> split_tabs(const std::string& line) {
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
  return parts;
}

int failures = 0;

}  // namespace

int main(int argc, char** argv) {
  // --fill: read 3-tab rows (mnemonic, policy, inputs) and print the same
  // rows with the oracle-computed expected dwords appended.  Used by
  // gen_cases.py to build generated_cases.tsv.
  const bool fill = argc > 1 && std::string(argv[1]) == "--fill";
  std::string line;
  std::size_t lineno = 0;
  while (std::getline(std::cin, line)) {
    ++lineno;
    if (line.empty() || line[0] == '#') continue;
    const std::vector<std::string> parts = split_tabs(line);
    const std::size_t wanted = fill ? 3 : 4;
    if (parts.size() != wanted) {
      std::printf("FAIL %zu: expected %zu tab fields\n", lineno, wanted);
      ++failures;
      continue;
    }
    const std::string& mnemonic = parts[0];
    lgpu::ops::OpsFn op = lgpu::ops::find_op(mnemonic);
    if (op == nullptr) {
      std::printf("FAIL %zu: no oracle for %s\n", lineno, mnemonic.c_str());
      ++failures;
      continue;
    }
    lgpu::test::FpPolicy policy;
    std::string error;
    if (!parse_policy(parts[1], policy, error)) {
      std::printf("FAIL %zu: %s (%s)\n", lineno, error.c_str(), mnemonic.c_str());
      ++failures;
      continue;
    }
    std::vector<std::uint32_t> inputs, results;
    if (!parse_hex_list(parts[2], inputs)) {
      std::printf("FAIL %zu: bad hex list (%s)\n", lineno, mnemonic.c_str());
      ++failures;
      continue;
    }
    if (!op(inputs, policy, results)) {
      std::printf("FAIL %zu: %s oracle rejected inputs\n", lineno,
                  mnemonic.c_str());
      ++failures;
      continue;
    }
    if (fill) {
      std::printf("%s\t%s\t%s\t", mnemonic.c_str(), parts[1].c_str(),
                  parts[2].c_str());
      for (std::size_t i = 0; i < results.size(); ++i) {
        std::printf("%s0x%08x", i ? " " : "", results[i]);
      }
      std::printf("\n");
      continue;
    }
    std::vector<std::uint32_t> expected;
    if (!parse_hex_list(parts[3], expected)) {
      std::printf("FAIL %zu: bad hex list (%s)\n", lineno, mnemonic.c_str());
      ++failures;
      continue;
    }
    if (results.size() != expected.size()) {
      std::printf("FAIL %zu: %s result count %zu != expected %zu\n", lineno,
                  mnemonic.c_str(), results.size(), expected.size());
      ++failures;
      continue;
    }
    bool ok = true;
    for (std::size_t i = 0; i < results.size() && ok; ++i) {
      if (results[i] != expected[i]) {
        std::printf("FAIL %zu: %s result[%zu] expected 0x%08x got 0x%08x\n",
                    lineno, mnemonic.c_str(), i, expected[i], results[i]);
        ++failures;
        ok = false;
      }
    }
    if (ok) std::printf("OK %zu\n", lineno);
  }
  return failures == 0 ? 0 : 1;
}
