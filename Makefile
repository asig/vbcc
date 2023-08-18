.PHONY: all clean install

INSTALL_DIR ?= /opt/vbcc

all:
	@(cd vbcc; make TARGET=m68k)
	@(cd vasm; make CPU=m68k SYNTAX=mot)
	@(cd vlink; make)
	@mkdir bin 2> /dev/null
	@cp vbcc/bin/v* bin
	@cp vasm/vasmm68k_mot bin
	@cp vasm/vobjdump bin
	@cp vlink/vlink bin

clean:
	@(cd vasm; make clean)
	@(cd vlink; make clean)
	# TODO(asigner): Add make clean for vbcc
	@rm -rf bin

install:
	@mkdir -p $(INSTALL_DIR) 2> /dev/null
	@cp -R targets $(INSTALL_DIR)/
	@cp -R config $(INSTALL_DIR)/
	@cp -R bin $(INSTALL_DIR)/
	@cp -R docs $(INSTALL_DIR)/
	@echo vbcc installed in $(INSTALL_DIR)
