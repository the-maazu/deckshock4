.SECONDEXPANSION:
include .env

version := 0.0.2
objdir := build
objects := $(patsubst src%.c,$(objdir)%.o, $(wildcard src/*.c))
dbgobjdir := build/debug
dbgobjects := $(patsubst src%.c,$(dbgobjdir)%.o, $(wildcard src/*.c))
outdir := out

$(objdir):
	mkdir -p $(objdir)
$(dbgobjdir):
	mkdir -p $(dbgobjdir)

$(objects) : $$(patsubst $(objdir)%.o,src%.c, $$@) | $(objdir)
	gcc -c $^ -o $@
$(dbgobjects) : $$(patsubst $(dbgobjdir)%.o,src%.c, $$@) | $(dbgobjdir)
	gcc -ggdb -c $^ -o $@

.PHONY : all
all : $(outdir)/deckshock4
.PHONY : debug
debug : $(outdir)/deckshock4-debug

$(outdir):
	mkdir -p $(outdir)
$(outdir)/deckshock4 : $(objects) | $(outdir)
	gcc $^ -o $@ -lsystemd
$(outdir)/deckshock4-debug : $(dbgobjects) | $(outdir)
	gcc -ggdb $^ -o $@ -lsystemd -lnanojsonc

.PHONY: push
push: $(outdir)/deckshock4-debug
	scp -O -o "IdentityAgent=none" -i ${SSH_KEY} ${JUMP_HOST} $^ ${USER}@${HOST}:/home/${USER}/.local/bin/

.PHONY: package
package : deckshock4-v$(version).tar.gz
deckshock4-v$(version).tar.gz : $(objdir)/deckshock4 scripts/uninstall.sh scripts/install.sh etc | $(outdir)
	tar -cz etc -C$(outdir) deckshock4 -C"../scripts" install.sh uninstall.sh > deckshock4-v$(version).tar.gz

.PHONY: clean
clean:
	rm -r $(objdir)/
	rm -r $(outdir)/

# for submodule and library updates
.PHONY: lib
lib:
	cd libmodules && $(MAKE)