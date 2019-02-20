PROJECT_DIR = $(PWD)

SRC_DIR = $(PROJECT_DIR)/src
OBJ_DIR = $(PROJECT_DIR)/obj

EXEC = asg2

# Be sure to use g++ to get effc++ warnings
CXX = clang++

# SDL2 specifics
# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl2-config --cflags` -g -W -Wall -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/
LDFLAGS = `sdl2-config --libs` -lm -lSDL2_ttf
# User space
CXXFLAGS += -I$(PROJECT_DIR)/include

MODULES = $(notdir $(wildcard $(SRC_DIR)/*))
MODULES_OBJS = $(MODULES:%=$(OBJ_DIR)/%.o)

export PORJECT_DIR CXX CXXFLAGS OBJ_DIR

$(EXEC): $(MODULES_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MODULES_OBJS) $(MAIN_OBJS) $(LDFLAGS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%/unity.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# this rule is to clean up a specific module obj
clean-%: $(OBJ_DIR)/%.o
	$(RM) $<

archive: clean
	tar -zcf asg2.tar.gz *

clean:
	$(RM) $(MODULES_OBJS)
	$(RM) $(EXEC)