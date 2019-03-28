# example makefile
#
# Notes:
# - the indents must be tabs
# - if you need to specify a non-standard compiler, then set CXX
#   (it defaults to g++)
# - if you need to pass flags to the linker, set LDFLAGS rather
#   than alter the ${TARGET}: ${OBJECTS} rule
# - to see make's built-in rules use make -p

# target executable name
EXE = klsm

CXX := /usr/local/bin/g++-8

# source files

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS += -Iinclude
CFLAGS += -Wall
LDLIBS += -lm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

