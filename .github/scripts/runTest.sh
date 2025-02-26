#!/bin/bash

chmod +x $1
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=/usr/local/libexec/valgrind/default.supp $1 &> test_output.txt
export code=$?
cat test_output.txt
if [ $code -ne 0 ]; then
    exit $code
fi
if ! grep -q "no leaks are possible|definitely lost: 0 bytes in 0 blocks" test_output.txt; then
    exit 1
fi