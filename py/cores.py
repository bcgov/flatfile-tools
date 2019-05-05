#20190212 count the number of cpu cores
import os
import sys
ncores = 0
lines = os.popen('cat /proc/cpuinfo | grep "cpu cores"').read().strip().split('\n')
for line in lines:
    ncores += int(line.strip().split()[-1])
print "number of cpu cores", ncores

