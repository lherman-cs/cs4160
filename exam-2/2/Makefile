PROGRAMS := $(patsubst %.cpp,%,$(wildcard *.cpp))

all: $(PROGRAMS)

%: %.cpp
	g++ -o $@ $?

clean:
	rm -f $(PROGRAMS)