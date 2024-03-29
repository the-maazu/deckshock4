.SECONDEXPANSION:
include .env

objdir := build
objects := $(patsubst src%.c,build%.o, $(wildcard src/*.c))

.PHONY : all
all : $(objdir)/deckshock4 push

$(objdir):
	mkdir $(objdir)

$(objects) : $$(patsubst build%.o,src%.c, $$@) | $(objdir)
	gcc  -c $^ -o $@

$(objdir)/deckshock4 : $(objects)
	gcc  $^ -o $@ -lsystemd -lpthread

.PHONY: push
push: $(objdir)/deckshock4
	scp -O -i ${SSH_KEY} ${JUMP_HOST} $^ ${USER}@${HOST}:/home/${USER}/.local/bin

.PHONY: clean
clean:
	rm -r $(objdir)/

.PHONY: dist
dist : deckshock4.tar.gz
deckshock4.tar.gz : $(objdir)/deckshock4 scripts/uninstall.sh scripts/install.sh etc
	tar -cz etc -Cbuild deckshock4 -C"../scripts" install.sh uninstall.sh > deckshock4.tar.gz