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

%: %.cpp %.h
	$(CXX) $< -mlsx -mlasx -o $@

.SUFFIXES: