SHELL = /bin/bash

.SUFFIXES:
.SUFFIXES: .c .h .o

CC	= gcc
INCLDIR	= include/
BINDIR	= bin/
OBJDIR	= obj/
SRCDIR	= src/

# Ficheiro final
_BIN	= imageraster
BIN	= $(addprefix $(BINDIR), $(_BIN))

SRC	= $(wildcard src/*.c)
_OBJS	= $(patsubst src/%.c, %.o, $(SRC))
OBJS	= $(addprefix $(OBJDIR), $(_OBJS))


FLEXFILES	= $(wildcard src/*.l)
# flex generated c
_FLEXGEN	= $(patsubst src/%.l, %.yy.c, $(FLEXFILES))
FLEXGEN	= $(addprefix $(INCLDIR), $(_FLEXGEN))
# flex objects
_FLEXOBJS	= $(patsubst src/%.l, %.yy.o, $(FLEXFILES))
FLEXOBJS	= $(addprefix $(OBJDIR), $(_FLEXOBJS))


BISONFILES	= $(wildcard src/*.y)
#generated bison c and h
_BISONGEN	= $(patsubst src/%.y, %.tab.c, $(BISONFILES))
BISONGEN	  = $(addprefix $(INCLDIR), $(_BISONGEN))
_BISONHEAD  = $(patsubst src/%.y, %.tab.h, $(BISONFILES))
BISONHEAD	= $(addprefix $(INCLDIR), $(_BISONHEAD))
#objects need by c
_BISONOBJS	= $(patsubst src/%.y, %.tab.o, $(BISONFILES))
BISONOBJS	  = $(addprefix $(OBJDIR), $(_BISONOBJS))

# compilation flags
CFLAGS = -lm#-Wall -std=c99 -pedantic -g -I$(INCLDIR)
OFLAGS = -lm

# compile binary and object files
.PHONY: all
all: $(BIN) #docs

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(INCLDIR):
	mkdir -p $(INCLDIR)

# Regras

$(BIN): $(BINDIR) $(BISONGEN) $(OBJS) $(BISONOBJS) $(FLEXOBJS)
	$(CC) -o $(BIN) $(OBJS) $(BISONOBJS) $(FLEXOBJS) $(OFLAGS)

$(OBJS): $(OBJDIR) $(SRC)
	$(CC) -c $(patsubst %.o, %.c, $(patsubst obj/%, src/%, $@)) -o $@ $(CFLAGS)

$(FLEXOBJS): $(FLEXGEN)
	$(CC) -c $(patsubst %.o, %.c, $(patsubst obj/%, include/%, $@)) -o $@ $(CFLAGS)

$(BISONOBJS): $(BISONGEN)
	$(CC) -c $(patsubst %.o, %.c, $(patsubst obj/%, include/%, $@)) -o $@ $(CFLAGS)

$(BISONGEN): $(INCLDIR) $(BISONFILES)
	bison -b include/grammar -d src/grammar.y

$(FLEXGEN): $(INCLDIR) $(FLEXFILES)
	flex -o include/lexer.yy.c -b src/lexer.l


DOCDIR = docs/
TEXDIR = latex/

.PHONY: docs docs-clean
docs: Doxyfile
	doxygen
#	generate PDF from LaTeX sources
	cd $(DOCDIR)$(TEXDIR) && $(MAKE)
	mv $(DOCDIR)$(TEXDIR)refman.pdf $(DOCDIR)

docs-clean:
	cd $(DOCDIR)$(TEXDIR) && $(MAKE) clean


# clean entire project directory
.PHONY: clean
clean:
	- rm -rf $(BINDIR) $(OBJDIR) $(DOCDIR)  $(INCLDIR)


# check code quality
.PHONY: cppcheck memcheck
cppcheck:
	cppcheck --enable=all --language=c --std=c99 --inconclusive \
	--suppress=missingInclude $(SRC) -i $(INCLDIR)

memcheck: all
	valgrind -v --show-leak-kinds=all --leak-check=full --track-origins=yes \
	./$(BIN)

# debugging make
print-% :
	@echo $* = $($*)
