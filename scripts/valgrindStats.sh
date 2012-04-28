valgrind \
   --leak-check=full \
   --show-reachable=yes \
   --error-limit=no \
   --track-origins=yes \
   --suppressions=scripts/valgrind/external.supp \
   --suppressions=scripts/valgrind/toFix.supp \
   -v \
   ./test/mesh
