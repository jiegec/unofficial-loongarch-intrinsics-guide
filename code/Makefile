SRCS=$(wildcard *.cpp)
EXES=$(patsubst %.cpp,%,$(SRCS))

all: $(EXES)

run:
	./run.sh $(EXES) | tee run.log

clean:
	rm -rf $(EXES)

%: %.cpp common.h
	g++ $< -mlsx -mlasx -o $@
