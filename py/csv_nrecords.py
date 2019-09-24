# 20190311 supposing something is a csv file, with header, count the number of rows
import os
import sys
from misc import *

if len(sys.argv) < 2:
    err("usage: csv_nrecords [ csv file name ]")

fn = sys.argv[1]
f = open(fn)

l_1 = f.readline().strip().split(",")
l_2 = f.readline().strip().split(",")

if len(l_1) != len(l_2):
    err("probably not a csv file")

nrec = os.popen("wc -l " + fn).read().strip().split()[0]

try:
    nrec = int(nrec)
except:
    err("could not parse number: " + string(nrec))

print "number of records: ", str(nrec)