FROM ubuntu:22.04

COPY ./ /c-copy/

RUN apt-get update && \
    apt-get install -y software-properties-common wget && \
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    wget -O /etc/apt/trusted.gpg.d/llvm.asc https://apt.llvm.org/llvm-snapshot.gpg.key && \
    apt-get update && \
    apt-get install -y z3 python3-venv cmake git libz-dev unzip vim emacs valgrind gdb libgmp3-dev libtbb-dev llvm-13-dev clang-13 libclang-13-dev libboost-all-dev sqlite3 libsqlite3-dev libclang-13-dev tree zip curl zsh 


RUN mv /usr/bin/clang-13 /usr/bin/clang
RUN mv /usr/bin/clang++-13 /usr/bin/clang++

ENV CC=clang
ENV CXX=clang++

