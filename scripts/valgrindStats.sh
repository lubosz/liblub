   #--show-reachable=yes \
valgrind \
   --leak-check=full \
   --error-limit=no \
   --track-origins=yes \
   --suppressions=scripts/valgrind/external.supp \
   --suppressions=scripts/valgrind/internal.supp \
   --suppressions=scripts/valgrind/gui.supp \
   --suppressions=scripts/valgrind/toFix.supp \
   -v \
   $1
