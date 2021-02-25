#!/bin/sh
export PREFIX="$HOME/opt/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

# Prerequisites
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install build-essential binutils-multiarch ncurses-dev alien
sudo apt-get install -y libmpc-dev g++ nasm gcc-multilib xorriso grub-pc-bin grub-common

# : Get the version of the compiler and
# : the linker installed on the system
gcc_ver="gcc-"`gcc -dumpfullversion`
binutils_ver="binutils-2.34"

# Download binutils & GCC
cd $HOME/src/
if [ ! -e $binutils_ver ]; then 
    mkdir $binutils_ver
fi
if [ ! -e $gcc_ver ]; then
    mkdir $gcc_ver
fi
if [ ! -f $binutils_ver.tar.gz ]; then 
    wget ftp://ftp.gnu.org/gnu/binutils/$binutils_ver.tar.gz
    tar -xvzf $binutils_ver.tar.gz
fi
if [ ! -f $gcc_ver.gz ]; then
    wget ftp://ftp.gnu.org/gnu/gcc/$gcc_ver/$gcc_ver.tar.gz
    tar -xvzf $gcc_ver.tar.gz
fi

# Build binutils
cd $HOME/src
if [ ! -e build-binutils ]; then
    mkdir build-binutils 
fi
cd build-binutils
../$binutils_ver/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

# Build GCC
cd $HOME/src
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
if [ ! -e build-gcc ]; then
    mkdir build-gcc
fi
cd build-gcc
../$gcc_ver/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

export PATH="$HOME/opt/cross/bin:$PATH"