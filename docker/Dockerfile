FROM ubuntu:18.04 AS base
# Base Ubuntu updated to the latest version

ARG BINUTILS_VERSION=2.30
ARG GCC_VERSION=7.5.0
ARG TARGET

RUN \
DEBIAN_FRONTEND=noninteractive apt-get update && \
DEBIAN_FRONTEND=noninteractive apt-get -y install apt-utils && \
DEBIAN_FRONTEND=noninteractive apt-get -y upgrade && \
rm -rf /var/lib/apt/lists/*

FROM base AS build
# Updated Ubuntu with build requirements

RUN \
DEBIAN_FRONTEND=noninteractive apt-get update && \
DEBIAN_FRONTEND=noninteractive apt-get -y install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo wget && \
rm -rf /var/lib/apt/lists/*

ARG BINUTILS_VERSION=2.30
ARG GCC_VERSION=7.5.0

# Download binutils and gcc
WORKDIR /opt/src
RUN \
wget -q --show-progress --progress=bar:force:noscroll https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz && \
wget -q --show-progress --progress=bar:force:noscroll https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz && \
tar -xzf binutils-$BINUTILS_VERSION.tar.gz && \
tar -xzf gcc-$GCC_VERSION.tar.gz && \
mkdir build-binutils build-gcc

ARG TARGET
ENV TARGET ${TARGET:-i686-elf}
ENV PREFIX="/opt/cross"
ENV PATH="$PREFIX/bin:$PATH"

# Build and install binutils
WORKDIR /opt/src/build-binutils
RUN ../binutils-$BINUTILS_VERSION/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror && \
make -j $(getconf _NPROCESSORS_ONLN) && \
make install

# Build and install gcc
WORKDIR /opt/src/build-gcc
RUN \
../gcc-$GCC_VERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers && \
make all-gcc -j $(getconf _NPROCESSORS_ONLN) && \
make all-target-libgcc -j $(getconf _NPROCESSORS_ONLN) && \
make install-gcc && \
make install-target-libgcc

FROM base AS target-clean
# Final image with target build dependencies

ARG TARGET
ENV TARGET ${TARGET:-i686-elf}
ENV PREFIX="/opt/cross"
ENV PATH="$PREFIX/bin:$PATH"

COPY --from=build /opt/cross /opt/cross/

RUN \
DEBIAN_FRONTEND=noninteractive apt-get update && \
DEBIAN_FRONTEND=noninteractive apt-get -y install make libmpc-dev nasm git grub-pc-bin xorriso gcc && \
rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src

FROM target-clean AS target-build
# Final image with project build script

COPY build.sh /build.sh
RUN chmod 755 /build.sh
ENTRYPOINT ["/build.sh"]

FROM target-clean
