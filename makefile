#name
TAR_NAME = c-compiler

SRCDIR = src
INCDIR = include #local include directory
OBJDIR = bin
TARGET = $(OBJDIR)/$(TAR_NAME)

#core
SUF = cpp
CC = g++
CORE = -MMD -MP -I$(INCDIR)

# pthread core because PegLib needs it

# optional flags
#manually define `llvm-config --cxxflags` for c++17, and allow exceptions
#CXXFLAGS = -I/usr/lib/llvm-10/include -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LLVM = --ldflags --system-libs --libs core
LLVM_FLAGS = `llvm-config $(LLVM)`
CFLAGS = $(CORE) -g -std=c++17 -pthread 

src = $(shell find $(SRCDIR) -name '*.$(SUF)')
obj = $(patsubst $(SRCDIR)/%.$(SUF), $(OBJDIR)/%.o, $(src))
dep = $(obj:.o=.d)
dirs = $(sort $(dir $(obj)))

all:
	make folders
	make $(TAR_NAME)

folders:
	mkdir -p $(OBJDIR)
	mkdir -p $(dirs)

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SUF)
	$(CC) $(CFLAGS) -c $< -o $@

# Main link

$(OBJDIR)/main.o: drivers/main.$(SUF)
	$(CC) $(CFLAGS) -c $< -o $@

$(TAR_NAME):
	make $(TARGET)

$(TARGET): $(OBJDIR)/main.o $(obj) 
	$(CC) $(CFLAGS) $^ -o $@

# Test link

# todo: add dependencies and clean up a little maybe

UNIT_SRC = $(wildcard unit_tests/*.cpp)
UNIT_OBJ = $(patsubst unit_tests/%.cpp, $(OBJDIR)/%.o, $(UNIT_SRC))

bin/%.o: unit_tests/%.cpp
	g++ -g -Wall -fPIE $(CORE) -pthread -std=c++17 -c $< -o $@

unit_test: $(UNIT_OBJ) $(obj)
	g++ -g -Wall -fPIE $(CORE) -pthread -std=c++17 $^ -lgtest  -lgtest_main  -o bin/$@ && ./bin/$@

# Graph debugging

graph_debug:
	c-compiler | python3 graph_viz.py | dot -Tpng -o out.png

# -------------------utils---------------------

-include $(dep) 

.PHONY: clean prints $(TAR_NAME) unit_test folders grammar graph_debug

clean:
	rm -rf $(OBJDIR) \
	
	rm -f $(obj) $(TARGET) $(dep)

#debugging
prints:
	echo '$(src)'
