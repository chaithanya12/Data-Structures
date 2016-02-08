#!/bin/sh

REQUIREMENTS="regexp(\"d16copt.*\", Name, \"i\")"

while getopts r: o; do
    case "$o" in
    	r)
	case "$OPTARG" in
	    dirt|disc|cclsun)
		REQUIREMENTS="MachineGroup == \"$OPTARG\""
		;;
	    *copt|matous)
		# dqcopt, d6copt, d8copt, d16copt, q16copt
		REQUIREMENTS="regexp(\"$OPTARG.*\", Name, \"i\")"
		;;
	    *)
		echo "Invalid requirement option: $OPTARG"
		exit 1
		;;
	esac
	;;
    esac
done

if [ $OPTIND -gt 1 ]; then
    shift $(($OPTIND - 1))
fi

if [ $# -ne 4 ]; then
    echo usage: $0 program database latitude longitude
    exit 1
fi

DIST=$1
if [ ! -x $DIST ]; then
    echo "$DIST does not exist."
    exit 1
fi
PROG=$(basename $DIST)

BASE=$2
if [ ! -f $BASE ]; then
    echo $BASE does not exist.
    exit 1
fi

LAT=$3
LON=$4

if [ ! -x ./measure ]; then
    echo "couldn't find ./measure (try make measure)"
    exit 1
fi

if [ -r $DIST.output ]; then
    echo "warning: this program overwrites the $DIST.output file!"
fi

if [ -z "$USER" ]; then
    USER=$(id -un)
fi

export PATH=/afs/nd.edu/user37/condor/software/bin:$PATH
export PATH=/afs/nd.edu/user37/condor/software/sbin:$PATH

WORKDIR=${TMPDIR:-/tmp}/$USER-pg2.$$
mkdir -p $WORKDIR

JOB=""

cleanup () {
    echo -n "Cleaning up ... "

    if [ -n "$JOB" ]; then
	condor_rm $JOB > /dev/null 2>&1
    fi
    if rm -fr $WORKDIR; then
    	echo "Ok!"
    fi
}

trap "cleanup; exit 1" SIGHUP SIGINT SIGTERM

echo -n "Staging files ... "
if ! cp measure $DIST $BASE $WORKDIR/ ; then
    cleanup
    exit 1
else
    echo "Ok!"
fi

echo "Submitting Condor Job ... "
cat > $WORKDIR/$DIST.submit <<EOF
universe     = vanilla
executable   = ./measure
arguments    = ./$PROG $BASE $LAT $LON
output	     = $PROG.output
error        = $PROG.error
log	     = $PROG.log
getenv	     = true
requirements = regexp("d16copt.*", Name, "i")
transfer_input_files	= measure,$DIST,$BASE
should_transfer_files	= yes
when_to_transfer_output = on_exit
queue
EOF

if ! (cd $WORKDIR ; condor_submit $DIST.submit) ; then
    cleanup
    exit 1
fi

JOB=$(condor_wait -wait 1 -status $WORKDIR/$PROG.log | awk '{print $1}' | head -n 1 | cut -d . -f 1)

echo "Waiting for Condor Job $JOB ... "
while ! condor_wait -wait 1 $WORKDIR/$PROG.log $JOB > /dev/null 2>&1; do
    printf "%s\r" "$(condor_q -submitter $USER $JOB | grep measure)"
done

echo
echo "Finished!"

echo -n "Copying results ... "
if cp $WORKDIR/$PROG.* . ; then
    echo "Ok!"
fi

echo "Measured time and memory usage: $(tail -n 1 $PROG.output)"

cleanup
