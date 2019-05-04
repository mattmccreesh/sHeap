#! /bin/sh
sleep 10
LD_PRELOAD=/home/czapfel/Documents/sHeap/sheap.so exec python3 /home/czapfel/Documents/sHeap/pytest.py
#cat /proc/self/status | grep "VmRSS\|VmPTE\|VmPeak\|VmSize"
