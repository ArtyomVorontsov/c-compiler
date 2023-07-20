#!/bin/sh

COMPILER_NAME="$1"
FILE_NAME="$2"
CMD="rm ${COMPILER_NAME%.c}.out ; make ; ${COMPILER_NAME%.c}.out ${FILE_NAME} -e; gcc -m32 ${FILE_NAME%.c}.s ; cat ${FILE_NAME}; echo ; ./a.out ; echo \$?"
echo ${CMD}
eval ${CMD}
