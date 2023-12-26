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