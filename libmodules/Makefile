.PHONY: all
all: 
	cd nanojsonc && cmake . && make
	cp nanojsonc/libnanojsonc.a ../lib/
	cp nanojsonc/include/* ../lib/include/
	cd nanojsonc && git clean -f
	