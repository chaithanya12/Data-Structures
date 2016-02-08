#!/bin/sh

if valgrind --error-exitcode=1 --leak-check=yes ./test2a; then
  echo "Valgrind: PASSED"
else
  echo "Valgrind: FAILED"
fi

./measure ./test2b | awk '
{ print; }

/seconds.*Mbytes/ {
  if ($1 <= 20) print "speed requirement: PASSED";
  else print "speed requirement: FAILED";
  if ($3 <= 200) print "memory requirement: PASSED";
  else print "memory requirement: FAILED";
}
'

