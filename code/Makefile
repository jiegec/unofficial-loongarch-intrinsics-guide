SRCS=$(wildcard *.cpp)
EXES=$(patsubst %.cpp,%,$(SRCS))

all: $(EXES)

clean:
	rm -rf $(EXES)

%: %.cpp common.h
	g++ $< -mlsx -mlasx -o $@
