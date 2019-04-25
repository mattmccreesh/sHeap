LD_PRELOAD=$PWD/sheap.so ./espresso jbp

to evaluate
without our allocator:
/usr/bin/time -v ./espresso jbp

with our allocator:
/usr/bin/time -v ./espressotest.sh

Metrics
CPU percentage
user time
Resident Set Size is total physical memory used
