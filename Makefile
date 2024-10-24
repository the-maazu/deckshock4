.SECONDEXPANSION:
include .env

version := 0.0.2
objdir := build
objects := $(patsubst src%.c,$(objdir)%.o, $(wildcard src/*.c))
outdir := out

.PHONY : all
all : $(outdir)/deckshock4

$(objdir):
	mkdir $(objdir)
$(outdir):
	mkdir $(outdir)

$(objects) : $$(patsubst $(objdir)%.o,src%.c, $$@) | $(objdir)
	gcc  -c $^ -o $@

$(outdir)/deckshock4 : $(objects)
	gcc  $^ -o $@ -lsystemd -lpthread

.PHONY: push
push: $(outdir)/deckshock4
	scp -O -o "IdentityAgent=none" -i ${SSH_KEY} ${JUMP_HOST} $^ ${USER}@${HOST}:/home/${USER}/.local/bin

.PHONY: package
package : deckshock4-v$(version).tar.gz
deckshock4-v$(version).tar.gz : $(objdir)/deckshock4 scripts/uninstall.sh scripts/install.sh etc | $(outdir)
	tar -cz etc -C$(outdir) deckshock4 -C"../scripts" install.sh uninstall.sh > deckshock4-v$(version).tar.gz

.PHONY: clean
clean:
	rm -r $(objdir)/
	rm -r $(outdir)/