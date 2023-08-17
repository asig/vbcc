.PHONY: all clean install

all:
	@(cd vbcc; make TARGET=m68k)
	@(cd vasm; make CPU=m68k SYNTAX=mot)
	@(cd vlink; make)

clean:
	@(cd vasm; make clean)
	@(cd vlink; make clean)
	# TODO(asigner): Add make clean for vbcc

install:
	@echo "make install not implemented yet"
