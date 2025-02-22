#!/bin/bash

chmod +x $1
$1 &> test_output.txt
export code=$?
cat test_output.txt
if [ $code -ne 0 ]; then
    exit $code
fi
if grep -q "Detected memory leaks!" test_output.txt; then
    exit 1
fi