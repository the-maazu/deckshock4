.SECONDEXPANSION:
include .env

version = 0.1.2
objdir = build
objects := $(patsubst src%.c,$(objdir)%.o, $(wildcard src/*.c))
dbgobjdir = build/debug
dbgobjects := $(patsubst src%.c,$(dbgobjdir)%.o, $(wildcard src/*.c))
outdir := out
flags !=  tr '\n' ' ' < compile_flags.txt

# executables
$(outdir)/deckshock4 : $(objects) | $(outdir)
	gcc ${flags} $^ -o $@ -lsystemd -lnanojsonc -lpthread
.PHONY : debug
debug : $(outdir)/deckshock4-debug
$(outdir)/deckshock4-debug : $(dbgobjects) | $(outdir)
	gcc ${flags} -ggdb $^ -o $@ -lsystemd -lnanojsonc -lpthread
$(outdir):
	mkdir -p $(outdir)

# objects
$(objects) : $$(patsubst $(objdir)%.o,src%.c, $$@) | $(objdir)
	gcc ${flags} -c $^ -o $@
$(dbgobjects) : $$(patsubst $(dbgobjdir)%.o,src%.c, $$@) | $(dbgobjdir)
	gcc ${flags} -ggdb -c $^ -o $@
$(objdir):
	mkdir -p $(objdir)
$(dbgobjdir):
	mkdir -p $(dbgobjdir)


.PHONY: push
push: $(outdir)/deckshock4-debug
	scp -O -o "IdentityAgent=none" -i ${SSH_KEY} ${JUMP_HOST} $^ ${USER}@${HOST}:/home/${USER}/.local/bin/

.PHONY: package
package : deckshock4-v$(version).tar.gz
deckshock4-v$(version).tar.gz : $(outdir)/deckshock4 scripts/uninstall.sh scripts/install.sh etc
	tar -cz etc -C$(outdir) deckshock4 -C"../scripts" install.sh uninstall.sh > deckshock4-v$(version).tar.gz

.PHONY: clean
clean:
	rm -r $(objdir)/
	rm -r $(outdir)/

# for submodule and library updates
.PHONY: lib
lib:
	cd libmodules && $(MAKE)