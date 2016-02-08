#!/bin/bash

if [ $# -ne 1 ]; then
  echo usage: $0 program
  exit 1
fi

DIST=$1
if [ ! -x $DIST ]; then
  echo $DIST does not exist.
  exit 1
fi

BASE=ndfood.txt
if [ ! -f $BASE ]; then
  echo $BASE does not exist.
  exit 1
fi

LAT=41.7031036
LON=-86.2391732

WORKDIR=${TMPDIR:-/tmp}/test2.$$
mkdir -p $WORKDIR

runtest() {
  $DIST $BASE $LAT $LON > $WORKDIR/output 2>&1
  if diff -q $WORKDIR/output $WORKDIR/correct >/dev/null; then
    echo " passed"
  else
    echo " failed"
    echo "Program output on left, expected output on right:"
    diff -y $WORKDIR/output $WORKDIR/correct
  fi
}

echo -n "Testing sorted distances:"

cat << EOF > $WORKDIR/correct
0.112922 LaFortune Student Center
0.184448 North Dining Hall
0.185885 Cafe Poche (Bond Hall)
0.245472 Waddick's (O'Shaughnessy Hall)
0.251821 Au Bon Pain (Hesburgh Library)
0.278584 Reckers
0.278584 South Dining Hall
0.294962 Cafe de Grasta (Grace Hall)
0.29811 Decio Commons
0.312458 Crossings (Eck Hall of Law)
0.387542 Rohr's (Morris Inn)
0.387542 Sorin's (Morris Inn)
0.405334 Kitz Kafe (Stinson-Remick Hall of Engineering)
0.407421 A la Descartes (Jordan Hall of Science)
0.438527 Cafe Commons (Mendoza College of Business)
0.466671 Au Bon Pain (Hesburgh Center for International Studies)
0.526354 Legends of Notre Dame
EOF

runtest

rm -rf $WORKDIR
