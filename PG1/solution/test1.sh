#!/bin/bash

if [ $# -ne 1 ]; then
  echo usage: $0 program
  exit 1
fi

CALC=$1
if [ ! -x $CALC ]; then
  echo $CALC does not exist.
  exit 1
fi

WORKDIR=${TMPDIR:-/tmp}/test1.$$
mkdir -p $WORKDIR

runtest() {
  $(dirname $CALC)/$(basename $CALC) < $WORKDIR/input > $WORKDIR/output 2>&1
  if diff -q $WORKDIR/output $WORKDIR/correct >/dev/null; then
    echo "passed"
  else
    echo "failed"
    echo "Program output on left, expected output on right:"
    diff -y $WORKDIR/output $WORKDIR/correct
  fi
}

echo -n "Testing push: "

cat << EOF > $WORKDIR/input
1
2
EOF

cat << EOF > $WORKDIR/correct
1
2
EOF

runtest

echo -n "Testing arithmetic operations: "

cat << EOF > $WORKDIR/input
1
2
3
+
/
EOF

cat << EOF > $WORKDIR/correct
1
2
3
5
0.2
EOF

runtest

echo -n "Testing error handling: "

cat << EOF > $WORKDIR/input
1
2
+
+
EOF

cat << EOF > $WORKDIR/correct
1
2
3
error: stack underflow on line 4
EOF

runtest

echo -n "Testing other commands: "

cat << EOF > $WORKDIR/input
4
1
3
swap
-
drop
EOF

cat << EOF > $WORKDIR/correct
4
1
3
1
2
4
EOF

runtest

rm -rf $WORKDIR
