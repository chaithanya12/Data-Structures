#!/bin/sh

if [ -x valgrind ]; then

if valgrind --error-exitcode=1 --leak-check=yes ./test1; then
  echo "Valgrind: PASSED"
else
  echo "Valgrind: FAILED"
fi

else

echo "Valgrind: SKIPPED"
./test1

fi


./measure ./test2 | awk '
{ print; }

/seconds.*Mbytes/ {
  if ($1 <= 5) print "speed requirement: PASSED";
  else print "speed requirement: FAILED";
}
'

