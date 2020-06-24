#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/oj2zf/Documents/ActsOfAGraph/vhls_proj/actsofagraph/solution1/.autopilot/db/a.g.bc ${1+"$@"}
