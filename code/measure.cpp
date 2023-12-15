#include <assert.h>
#include <linux/perf_event.h>
#include <map>
#include <sched.h>
#include <set>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>
#include <utility>
#include <vector>

// learned from lmbench lat_mem_rd
#define FIVE(X) X X X X X
#define TEN(X) FIVE(X) FIVE(X)
#define FIFTY(X) TEN(X) TEN(X) TEN(X) TEN(X) TEN(X)
#define HUNDRED(X) FIFTY(X) FIFTY(X)
#define THOUSAND(X) HUNDRED(TEN(X))

// get time or cycles
// unit: ns or cycle
uint64_t get_time();
// prioritize cycle over time
void setup_time_or_cycles();
uint64_t get_time_or_cycles();

void bind_to_core();

// perf related
void setup_perf_cycles();
uint64_t perf_read_cycles();
void setup_perf_instructions();
uint64_t perf_read_instructions();
void setup_perf_llc_misses();
uint64_t perf_read_llc_misses();
void setup_perf_llc_loads();
uint64_t perf_read_llc_loads();

int N = 50000;

#define INSTR_TEST(NAME, INST, ...)                                            \
  void test_##NAME(int n) {                                                    \
    for (int i = 0; i < n; i++) {                                              \
      asm volatile(".align 4\n" THOUSAND(INST) : : : __VA_ARGS__);             \
    }                                                                          \
  }
#define INSTR_TEST8(NAME, INST, ...)                                           \
  INSTR_TEST(NAME, INST __VA_OPT__(, ) __VA_ARGS__)

#include "measure.h"

#undef INSTR_TEST
#undef INSTR_TEST8

struct InstrTest {
  const char *name;
  const char *inst;
  void (*test)(int);
  int repeat;
};

#define INSTR_TEST(NAME, INST, ...)                                            \
  InstrTest{.name = #NAME, .inst = #INST, .test = test_##NAME, .repeat = 1},
#define INSTR_TEST8(NAME, INST, ...)                                           \
  InstrTest{.name = #NAME, .inst = #INST, .test = test_##NAME, .repeat = 8},

std::vector<InstrTest> tests = {
#include "measure.h"
};

#undef INSTR_TEST

struct InstrInfo {
  std::set<double> latency;
  double throughput_cpi;
  double throughput_ipc;
};

uint64_t get_time() {
  struct timeval tv = {};
  gettimeofday(&tv, nullptr);
  return (uint64_t)tv.tv_sec * 1000000000 + (uint64_t)tv.tv_usec * 1000;
}

uint64_t perf_read_common(int fd) {
  uint64_t counter;
  int res = read(fd, &counter, sizeof(counter));
  assert(res == sizeof(counter));
  return counter;
}

int setup_perf_common(uint32_t type, uint64_t config) {
  struct perf_event_attr *attr =
      (struct perf_event_attr *)malloc(sizeof(struct perf_event_attr));
  memset(attr, 0, sizeof(struct perf_event_attr));
  attr->type = type;
  attr->size = sizeof(struct perf_event_attr);
  attr->config = config;
  attr->disabled = 0;
  attr->pinned = 1;
  attr->inherit = 1;
  attr->exclude_kernel = 1;
  int fd = syscall(SYS_perf_event_open, attr, 0, -1, -1, 0);
  if (fd < 0) {
    perror("perf_event_open");
    fprintf(stderr, "try: sudo sysctl kernel.perf_event_paranoid=2");
    exit(1);
  }
  return fd;
}

int perf_fd_cycles = -1;

uint64_t perf_read_cycles() { return perf_read_common(perf_fd_cycles); }

void setup_perf_cycles() {
  perf_fd_cycles =
      setup_perf_common(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
  if (perf_fd_cycles >= 0) {
    printf("Using PMU to count cycles\n");
  }
}

int perf_fd_instructions = -1;

uint64_t perf_read_instructions() {
  return perf_read_common(perf_fd_instructions);
}

void setup_perf_instructions() {
  perf_fd_instructions =
      setup_perf_common(PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
}

int perf_fd_llc_misses = -1;
int perf_fd_llc_loads = -1;

uint64_t perf_read_llc_misses() { return perf_read_common(perf_fd_llc_misses); }

uint64_t perf_read_llc_loads() { return perf_read_common(perf_fd_llc_loads); }

void setup_perf_llc_misses() {
  perf_fd_llc_misses = setup_perf_common(
      PERF_TYPE_HW_CACHE, (PERF_COUNT_HW_CACHE_LL) |
                              (PERF_COUNT_HW_CACHE_OP_READ << 8) |
                              (PERF_COUNT_HW_CACHE_RESULT_MISS << 16));
}

void setup_perf_llc_loads() {
  perf_fd_llc_loads = setup_perf_common(
      PERF_TYPE_HW_CACHE, (PERF_COUNT_HW_CACHE_LL) |
                              (PERF_COUNT_HW_CACHE_OP_READ << 8) |
                              (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16));
}

void setup_time_or_cycles() { setup_perf_cycles(); }

uint64_t get_time_or_cycles() {
  if (perf_fd_cycles >= 0) {
    // cycle
    return perf_read_cycles();
  } else {
    // time
    return get_time();
  }
}

// bind to core 0
void bind_to_core() {
  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(0, &set);
  int res = sched_setaffinity(0, sizeof(set), &set);
  if (res == 0) {
    printf("Pinned to cpu 0\n");
  }
}

int main(int argc, char *argv[]) {
  bool perf = false;

  int opt;
  while ((opt = getopt(argc, argv, "n:p")) != -1) {
    switch (opt) {
    case 'n':
      sscanf(optarg, "%d", &N);
      break;
    case 'p':
      perf = true;
      break;
    default:
      fprintf(stderr, "Usage: %s [-p]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  bind_to_core();
  if (perf) {
    setup_time_or_cycles();
  }

  // calibrate unit cycle time
  uint64_t begin = get_time_or_cycles();
  test_unit(N);
  uint64_t unit_elapsed = get_time_or_cycles() - begin;

  std::map<std::string, InstrInfo> info;

  for (auto it : tests) {
    std::string name = it.name;
    if (name == "unit") {
      continue;
    }

    begin = get_time_or_cycles();
    it.test(N);
    uint64_t elapsed = get_time_or_cycles() - begin;
    double cycles = (double)elapsed / unit_elapsed;
    cycles /= it.repeat; // in some tests, instructiosn are repeated

    std::string base_name;
    size_t tp_index = name.find("_tp");
    if (tp_index != std::string::npos) {
      base_name = name.substr(0, tp_index);
      printf("%s: throughput 1/%.2lf=%.2lf instructions\n", it.name, cycles,
             1.0 / cycles);
      info[base_name].throughput_cpi = 1.0 / cycles;
      info[base_name].throughput_ipc = cycles;
    } else {
      base_name = name;

      size_t last_underscore_index = name.find_last_of("_");
      if (last_underscore_index != std::string::npos) {
        char *end = NULL;
        const char *start = &name.c_str()[last_underscore_index + 1];
        strtol(start, &end, 10);

        if (start != end) {
          // strip suffix _2 etc
          base_name = name.substr(0, last_underscore_index);
        }
      }

      // round to 0.01
      cycles = (double)(long)(cycles * 100 + 0.5) / 100.0;
      printf("%s: latency %.2lf cycles\n", it.name, cycles);
      info[base_name].latency.insert(cycles);
    }
  }

#ifdef MACHINE_3C5000
  FILE *fp = fopen("measure-3C5000.csv", "w");
#else
  FILE *fp = fopen("measure-3A6000.csv", "w");
#endif
  assert(fp);
  fprintf(fp, "name,latency,throughput(cpi),throughput(ipc)\n");
  for (auto pair : info) {
    std::string latency;
    auto entry = pair.second;
    for (auto lat : entry.latency) {
      char buffer[32];
      std::sprintf(buffer, "%.2lf", lat);
      if (!latency.empty()) {
        latency += "/";
      }
      latency += buffer;
    }
    fprintf(fp, "%s,%s,%.2lf,%.2lf\n", pair.first.c_str(), latency.c_str(),
            entry.throughput_cpi, entry.throughput_ipc);
  }
  printf("Result written to measure.csv\n");
}
