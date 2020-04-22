#!/bin/bash
#
# Execute the generated code
#
# This script takes in the filename of the ORIGINAL program AFTER the codegen has been run
#   i.e. programs/3-semantics+codegen/valid/test.min
#
# It MUST then
#   (a) Compile the GENERATED file
#         i.e. programs/3-semantics+codegen/valid/test.c
#   (b) Execute the compiled code
#
# (if no compilation is needed, then only perform step b)
#
# To conform with the verification script, this script MUST:
#   (a) Output ONLY the execution
#   (b) Exit with status code 0 for success, not 0 otherwise

rm ${1%.*}.class 2> /dev/null

# You MUST replace the following line with the command to compile your generated code
# Note the bash replacement which changes:
#   programs/3-semantics+codegen/valid/test.min -> programs/3-semantics+codegen/valid/test.c
# stdout is redirected to /dev/null
FILE_DIR="$(dirname ${1%.*})"
FILENAME="$(basename ${1%.*})"
OUTPUT_DIR="output/"

cd $FILE_DIR
if [ ! -d $OUTPUT_DIR ] ; then
    mkdir $OUTPUT_DIR
fi

# Need to remove non alphabetical characters because the compiler generates Java file names
# that only contain alphabetical characters
FILENAME=${FILENAME//[0-9_-]/}
FILENAME="${FILENAME}GoLite"

javac -d $OUTPUT_DIR $FILENAME.java 

# You MUST replace the following line with the command to execute your compiled code
# Note the bash replacement which changes:
#   programs/3-semantics+codegen/valid/test.min -> programs/3-semantics+codegen/valid/test.out
java -cp $OUTPUT_DIR $FILENAME
EXITCODE="${?}"

# Lastly, we propagate the exit code
exit $EXITCODE
