
SRCS=$(wildcard *.cpp)
OBJ=frameGen.o

$(OBJ_DIR)/$(OBJ): $(SRCS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<