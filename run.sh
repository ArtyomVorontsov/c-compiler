#!/bin/sh

COMPILER_NAME="$1"
FILE_NAME="$2"
CMD="rm ${COMPILER_NAME%.c}.out ; make ; ${COMPILER_NAME%.c}.out ${FILE_NAME} -e; gcc -m32 ${FILE_NAME%.c}.s ; cat ${FILE_NAME}; echo ; echo 'NGCC output:'; ./a.out ; echo \$?"
eval ${CMD}
CMD="echo 'GCC output:' ; gcc -S ${FILE_NAME} -o ${FILE_NAME%.c}_gcc.s; gcc ${FILE_NAME%.c}_gcc.s ; ./a.out ; echo \$?"
eval ${CMD}

