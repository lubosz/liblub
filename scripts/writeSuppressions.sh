valgrind \
   --leak-check=full \
   \ #--show-reachable=yes \
   --error-limit=no \
   --gen-suppressions=all \
   --track-origins=yes \
   --log-file=minimalraw.log \
   --suppressions=scripts/valgrind/external.supp \
   --suppressions=scripts/valgrind/internal.supp \
   --suppressions=scripts/valgrind/gui.supp \
   $1

cat ./minimalraw.log | Parse_valgrind_suppressions.sh > scripts/valgrind/current.supp

./scripts/makeToFixList.py scripts/valgrind/current.supp > scripts/valgrind/toFix.supp

./scripts/makeLibSuppressions.py > scripts/valgrind/libs.supp
