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

if [ ! -x ./measure ]; then
  echo "couldn't find ./measure (try make measure)"
  exit 1
fi

WORKDIR=${TMPDIR:-/tmp}/test1.$$
mkdir -p $WORKDIR

echo "Testing efficiency..."

seq 1 100000 | ./measure $(dirname $CALC)/$(basename $CALC) | awk '
/seconds.*Mbytes/ {
  print;
  if ($1 <= 1) print "speed requirement: passed";
  else print "speed requirement: failed";
  if ($3 <= 10) print "memory requirement: passed";
  else print "memory requirement: failed";
}
'

rm -rf $WORKDIR
