SRCS=$(wildcard *.cpp)
HDRS=$(wildcard *.h)
EXES=$(patsubst %.cpp,%,$(SRCS))
CXX?=g++

all: $(EXES)

run: all
	./run.sh $(EXES)

clean:
	rm -rf $(EXES)

%: %.cpp $(HDRS)
	$(CXX) $< -mlsx -mlasx -o $@
