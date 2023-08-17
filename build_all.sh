#!/bin/bash

(cd vbcc; make TARGET=m68k)
(cd vasm; make CPU=m68k SYNTAX=mot)
(cd vlink; make)

