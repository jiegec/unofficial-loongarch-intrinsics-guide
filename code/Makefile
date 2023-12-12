SRCS=$(wildcard *.cpp)
HDRS=$(wildcard *.h)
EXES=$(patsubst %.cpp,%,$(SRCS))
CXX?=g++

all: $(EXES)

run: all
	./run.sh $(EXES)

clean:
	rm -rf $(EXES)

%: %.cpp %.h
	$(CXX) $< -mlsx -mlasx -o $@

.SUFFIXES: