#!/bin/sh

function error {
  echo "ERROR $1 could not be run"
  exit
}


EXAMPLES=`ls bin`

for EXAMPLE in $EXAMPLES; do
  bin/$EXAMPLE || error $EXAMPLE
done


