PROJECT_DIR = $(PWD)

SRC_DIR = $(PROJECT_DIR)/src
OBJ_DIR = $(PROJECT_DIR)/obj

EXEC = asg4

# Be sure to use g++ to get effc++ warnings
CXX = g++

# SDL2 specifics
# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl2-config --cflags` -std=c++11 -I `sdl2-config --prefix`/include/
ifeq "$(ENV)" "prod"
	CXXFLAGS += -Ofast
else
	CXXFLAGS += -g -O0 -W -Wall -Weffc++ -Wextra -pedantic
endif
LDFLAGS = `sdl2-config --libs` -lm -lSDL2_ttf -lSDL2_image -lexpat
# User space
CXXFLAGS += -I$(PROJECT_DIR)/include

MODULES = $(notdir $(wildcard $(SRC_DIR)/*))
MODULES_OBJS = $(MODULES:%=$(OBJ_DIR)/%.o)

$(EXEC): $(MODULES_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MODULES_OBJS) $(MAIN_OBJS) $(LDFLAGS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%/unity.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# this rule is to clean up a specific module obj
clean-%: $(OBJ_DIR)/%.o
	$(RM) $<

archive: clean
	tar -zcf $(EXEC).tar.gz *

clean:
	$(RM) $(MODULES_OBJS)
	$(RM) $(EXEC)