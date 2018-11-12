# compile to assembly language ( intel syntax )

clang -S -fno-asynchronous-unwind-tables -masm=intel return_2.c
