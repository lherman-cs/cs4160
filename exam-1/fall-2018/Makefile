CXX= clang++
CXXFLAGS= -std=c++11
SRC=$(wildcard *.cpp)
EXE=$(SRC:%.cpp=%) 

.PHONY: all clean

all: $(EXE)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(EXE)