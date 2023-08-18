# README

The `vbcc` toolchain (`vbcc`, `vasm`, `vlink`) taken as-is from http://compilers.de/
and http://sun.hasenbraten.de/, with a `Makefile` that builds the cross-compiler 
toolchain targeting Amiga with Kickstart 1.3.

The Makefile is built to suit my needs, but if you're running a halfway decent Linux
(i.e. Ubuntu) and have the essential build tools installed (`make` and `gcc`), 
just running `make && make install` should work for you.

By default, `vbcc` will be installed in `/opt/vbcc`. If you prefer a different
location, just run `make install INSTALL_DIR=/path/to/vbcc`.

Make sure that you set the `VBCC` env variable to the vbcc directory, and add
$VBCC/bin to your PATH.
