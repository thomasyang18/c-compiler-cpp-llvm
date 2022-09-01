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

# optional flags
#manually define `llvm-config --cxxflags` for c++17
CXXFLAGS = -I/usr/lib/llvm-10/include -std=c++17 -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LLVM = --ldflags --system-libs --libs core
CFLAGS = $(CORE) $(CXXFLAGS) `llvm-config $(LLVM)`


src = $(shell find $(SRCDIR) -name '*.$(SUF)')
obj = $(patsubst $(SRCDIR)/%.$(SUF), $(OBJDIR)/%.o, $(src))
dep = $(obj:.o=.d)
dirs = $(sort $(dir $(obj)))

all:
	make folders \

	make $(TAR_NAME)

folders:
	mkdir -p $(OBJDIR) \

	mkdir -p $(OBJDIR)/unit_tests \
	
	mkdir -p $(dirs) \

$(TAR_NAME):
	make $(TARGET)

$(TARGET): $(obj)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SUF)
	$(CC) $(CFLAGS) -c $< -o $@

# add scripts here (add target to phony list as needed)

bin/unit_tests/%.o: unit_tests/%.cpp
	g++ -g -Wall $(CORE) -std=c++17 -c $< -o $@

unit_test: bin/unit_tests/parser_driver.o	
	g++ -g -Wall $(CORE) -pthread -std=c++17 $^ -lgtest  -lgtest_main  -o bin/$@

# -------------------utils---------------------

-include $(dep) 

.PHONY: clean prints $(TAR_NAME) unit_test folders

clean:
	rm -rf $(OBJDIR) \
	
	rm -f $(obj) $(TARGET) $(dep)

#debugging
prints:
	echo '$(src)'