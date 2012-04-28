valgrind \
   --leak-check=full \
   --show-reachable=yes \
   --error-limit=no \
   --gen-suppressions=all \
   --track-origins=yes \
   --log-file=minimalraw.log \
   --suppressions=scripts/valgrind/external.supp \
   --suppressions=scripts/valgrind/toFix.supp \
   ./test/mesh

cat ./minimalraw.log | Parse_valgrind_suppressions.sh > scripts/valgrind/current.supp
