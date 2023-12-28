SRCS=$(wildcard *.cpp)
HDRS=$(wildcard *.h)
EXES=$(patsubst %.cpp,%,$(SRCS))
CXX?=g++

all: $(EXES)

run: $(patsubst %,run-%,$(EXES))

run-%: %
	echo "Running $<"
	./$<

clean:
	rm -rf $(EXES)

%: %.cpp %.h common.h
	$(CXX) -Wno-narrowing $(CFLAGS) $< -mlsx -mlasx -o $@

measure: measure.cpp measure.h
	$(CXX) $(CFLAGS) -O2 $< -o $@

run-measure: measure
	nice -n 19 ./measure -p

.SUFFIXES:
