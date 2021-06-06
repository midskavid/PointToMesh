#
# Compiler flags
#
CC     = g++
CFLAGS = -std=c++17 -Wall -Werror -I../src/

#
# Project files
#
SRCS = test.cpp ../src/mesh.cpp ../src/spheretree.cpp ../src/utility.cpp
EXE  = TestPoint2Mesh

RELCFLAGS = -O3 -DFLOAT_AS_DOUBLE

.PHONY: all clean release remake

# Default build
all: $(EXE)

$(EXE):
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(EXE) $(SRCS)

remake: clean all

clean:
	rm -f $(EXE)
