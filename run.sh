#!/bin/sh
rm ./compiler.out ; make ; ./compiler.out ./return_2.c -e; gcc -m32 ./return_2.s ; ./a.out ; echo $?
