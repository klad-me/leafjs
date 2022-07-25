##############################################################################
##  Makefile with automatic dependencies generation
##############################################################################


##############################################################################
##  Source files with paths
##############################################################################

C_SOURCES+= \
	src/main.c				\
	src/js_process.c		\
	src/js_console.c		\
	src/js_path.c			\
	src/js_buffer.c			\
	src/js_global.c			\
	src/js_fs.c				\
	mujs.git/one.c

##############################################################################



##############################################################################
##  Directories and file names
##############################################################################
# Output file name
BIN=leaf
# Path for binary files
BINDIR=bin
# Path for object files
OBJDIR=.obj
# Path for dependencies information
DEPDIR=.dep
##############################################################################



##############################################################################
##  Compiler configuration
##############################################################################
CC?=$(CROSS_COMPILE)gcc
STRIP?=$(CROSS_COMPILE)strip
INCLUDES=-Isrc -Imujs.git
DEFINES=
CFLAGS=-Wall -Wstrict-prototypes -Wshadow -Os -fmerge-all-constants -g $(INCLUDES) $(DEFINES)
LIBS=-lm -lreadline
LDFLAGS=$(LIBS) -Wl,--relax,--gc-sections
##############################################################################



# Target ALL
all: $(BINDIR)/$(BIN)

# Target for linker
$(BINDIR)/$(BIN): $(C_SOURCES:%.c=$(OBJDIR)/%.c.o)
	@set -e; \
	echo "Linking..."; \
	mkdir -p $(BINDIR); \
	$(CC) $(LDFLAGS) -o $@ $+ && $(STRIP) $@

# && $(STRIP) $@

# Target for clean
clean:
	rm -rf $(DEPDIR)
	rm -rf $(OBJDIR)
	rm -f src/js_init.h

# Target for distclean
distclean:
	rm -f $(BINDIR)/$(BIN)
	rm -rf $(DEPDIR)
	rm -rf $(OBJDIR)
	rm -f src/js_init.h


# PHONY
.PHONY: all


# Rule for making include from all .js files
src/main.c: src/js_init.h
src/js_init.h: $(wildcard src/js_init/*.js)
	@echo "Creating $@"; \
	cat $+ | xxd -i >$@

# Rule for compiling & deps generation
$(OBJDIR)/%.c.o: %.c
	@set -e; \
	echo "Compiling $<"; \
	mkdir -p $(dir $(DEPDIR)/$<) $(dir $(OBJDIR)/$<); \
	$(CC) $(CFLAGS) -MM -MF $(DEPDIR)/$<.d -MT $(OBJDIR)/$<.o -MP $< || rm -f $(DEPDIR)/$<.d; \
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$<.o $<


# Including dependencies infomation
-include $(C_SOURCES:%.c=$(DEPDIR)/%.c.d)
