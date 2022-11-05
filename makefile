#
# Directories
#
SRC_DIR = src/c
INC_DIR = src/header

#
# Compiler flags
#
CC     = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC

#
# Project files
#
SRCS=$(notdir $(shell find $(SRC_DIR) -name '*.c'))
OBJS = $(SRCS:.c=.o)
LIB = open_template
LIBFILE = lib$(LIB).so


#
# Debug build settings
#
DBGDIR = debug
DBGLIB = $(DBGDIR)/$(LIBFILE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG
DBGLIBFLAGS = -shared

#
# Release build settings
#
RELDIR = release
RELLIB = $(RELDIR)/$(LIBFILE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG
LIBFLAGS = -shared -fvisibility=hidden

#
# Linker and macro Settings
#
LFLAGS = 
METAFLAGS = $(addprefix -I, $(INC_DIR)) $(addprefix -D, $(DEFINES)) $(addprefix -l, $(LFLAGS))

.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGLIB)

$(DBGLIB): $(DBGOBJS)
	$(CC) $(CFLAGS) $(METAFLAGS) $(DBGCFLAGS) $(DBGLIBFLAGS) -o $(DBGLIB) $^
	@mv $(DBGLIB) ./$(LIBFILE)

$(DBGDIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(METAFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELLIB)

$(RELLIB): $(RELOBJS)
	$(CC) $(CFLAGS) $(METAFLAGS) $(RELCFLAGS) $(LIBFLAGS) -o $(RELLIB) $^

$(RELDIR)/%.o: $(SRC_DIR)%.c
	$(CC) -c $(CFLAGS) $(METAFLAGS) $(RELCFLAGS) -o $@ $<
	@mv $(DBGLIB) ./$(LIBFILE)

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

#
# test rules
#

TEST_DIR = tests
TEST_CORE_DIR = test_core
TESTLFLAGS = -L$(CURDIR) -l$(LIB) -Wl,-rpath=$(CURDIR)

$(TEST_DIR)/%.elf: $(TEST_DIR)/%.c $(DBGOBJS) $(TEST_CORE_DIR)/test_core.c
	$(CC) $(CFLAGS) $(METAFLAGS) $(DBGCFLAGS) $(DBGOBJS) $(TEST_CORE_DIR)/test_core.c -I$(TEST_CORE_DIR) $< -o $@ $(TESTLFLAGS)

test: debug
	@for file in $(TEST_DIR)/*.c ; do \
		target="$${file%%.*}".elf ; \
		make $${target} && \
		./$${target} ; \
		rm -f /$${target} ; \
	done

test-valgrind: debug
	@for file in $(TEST_DIR)/*.c ; do \
		target="$${file%%.*}".elf ; \
		make $${target} && \
		valgrind ./$${target} ; \
		rm -f /$${target} ; \
	done

remake: clean all

clean:
	rm -f $(RELLIB) $(RELOBJS) $(DBGLIB) $(DBGOBJS) $(TEST_DIR)/*.test