
SRCS=$(wildcard *.cpp)
OBJ=main.o

$(OBJ_DIR)/$(OBJ): $(SRCS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<