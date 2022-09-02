#name
TAR_NAME = c-compiler

SRCDIR = src
INCDIR = include #local include directory
OBJDIR = bin
TARGET = $(OBJDIR)/$(TAR_NAME)

#core
SUF = cpp
CC = clang++
CORE = -MMD -MP -I$(INCDIR)

# pthread core because PegLib needs it

# optional flags
#manually define `llvm-config --cxxflags` for c++17
CXXFLAGS = -I/usr/lib/llvm-10/include -std=c++17 -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LLVM = --ldflags --system-libs --libs core
CFLAGS = $(CORE) $(CXXFLAGS) -fPIE `llvm-config $(LLVM)`


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

$(TARGET): $(obj) $(OBJDIR)/main.o
	$(CC) $(CFLAGS) $^ -o $@

# Test link

# todo: clean this up but probably fine

bin/%.o: unit_tests/%.cpp
	g++ -g -Wall -fPIE $(CORE) -pthread -std=c++17 -c $< -o $@

unit_test: bin/parser_driver.o $(obj)
	g++ -g -Wall -fPIE $(CORE) -pthread -std=c++17 $^ -lgtest  -lgtest_main  -o bin/$@ && ./bin/$@

# -------------------utils---------------------

-include $(dep) 

.PHONY: clean prints $(TAR_NAME) unit_test folders grammar

clean:
	rm -rf $(OBJDIR) \
	
	rm -f $(obj) $(TARGET) $(dep)

#debugging
prints:
	echo '$(src)'
