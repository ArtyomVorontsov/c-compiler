
#!/bin/sh

COMPILER_NAME="$1"
DIR="$2"


CMD="rm ${COMPILER_NAME%.c}.out ; make; rm ${DIR}/*.s"
eval ${CMD}

let count=0
let passed_count=0
let failed_count=0
for f in "${DIR}"/*
do
	echo $(basename $f)
	let count=count+1

	# RUN NGCC COMPILER
	${COMPILER_NAME%.c}.out ${f} ; 
	gcc -m32 ${f%.c}.s; 
	cat ${f}; 
	echo; 
	echo "NGCC output:"; 
	./a.out; 
	let NGCC_OUT=$?
	echo "$NGCC_OUT"

	# RUN GCC COMPILER
	gcc -S ${f} -o ${f%.c}_gcc.s; 
	gcc ${f%.c}_gcc.s ; 
	echo "GCC output:"; 
	./a.out ; 
	let GCC_OUT=$?
	echo "$GCC_OUT"

	# Compare outputs
	if [[ "$GCC_OUT" == "$NGCC_OUT" ]]
	then
		let passed_count=passed_count+1
	else
		let failed_count=failed_count+1
	fi	
	

	rm ${DIR}/*.s
done

echo ""
echo "COUNT: $count"
echo "PASSED: $passed_count"
echo "FAILED: $failed_count"

