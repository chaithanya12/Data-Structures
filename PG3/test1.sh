#!/bin/sh
if valgrind --error-exitcode=1 --leak-check=yes ./test1; then
  echo "Valgrind: PASSED"
else
  echo "Valgrind: FAILED"
fi


