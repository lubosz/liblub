#!/bin/bash

function error {
  echo "ERROR $1 could not be run"
  exit
}


EXAMPLES=`ls bin`

for EXAMPLE in $EXAMPLES; do
echo -n "Press Enter to start $EXAMPLE"
read answer
  bin/$EXAMPLE || error $EXAMPLE
done

SCENES=`ls media/scenes/ | grep "xml" | sed 's/\(.*\)\..*/\1/'`

for SCENE in $SCENES; do
echo -n "Press Enter to start $SCENE.xml"
read answer
  bin/load $SCENE || error $SCENE
done


