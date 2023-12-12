SRCS=$(wildcard *.cpp)
HDRS=$(wildcard *.h)
EXES=$(patsubst %.cpp,%,$(SRCS))

all: $(EXES)

run: all
	./run.sh $(EXES)

clean:
	rm -rf $(EXES)

%: %.cpp $(HDRS)
	g++ $< -mlsx -mlasx -o $@
