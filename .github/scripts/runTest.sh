#!/bin/bash

chmod +x "$1" || exit $?
valgrind --leak-check=full --show-leak-kinds=definite,indirect,possible \
    --errors-for-leak-kinds=definite,indirect,possible --error-exitcode=99 \
    --keep-debuginfo=yes \
    --undef-value-errors=no --suppressions=.github/scripts/valgrind.supp "$@" &> test_output.txt
code=$?
cat test_output.txt
exit "$code"
