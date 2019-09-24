# 20190212 this program modified from an old code "apply_dd.py" to work with data that's non-corrections
import os
import sys
from misc import *
#   1) dd_list.py
#   2) dd_clean.py
#   3) dd_examine.py

args = sys.argv

if len(args) != 3:
    err('usage: apply_dd [dd.csv] [data file.dat]')

# basic error checking on the arguments
ddf, dtf = args[1].strip(), args[2].strip()
if ddf.split('.')[-1] != 'csv':
    err('arg 1 must be csv')
if dtf.split('.')[-1] != 'dat':
    err('arg 2 must be dat')


lines = open(ddf).read().strip().split('\n')
start, stop, length, label = [], [], [], []
for i in range(0, len(lines)):
    line = lines[i].strip()
    words = line.split(',')
    if i==0:
        # pragmatic programming: make sure dd is as expected
        if words[0] != "start" or words[1] != "stop" or words[2] != "length" or words[3] != "label":
            error("header mismatch")
    else:
        start.append(int(words[0]))
        stop.append(int(words[1]))
        length.append(int(words[2]))
        label.append(words[3])
        # pragmatic programming
        if int(words[1]) + 1 - int(words[0]) != int(words[2]):
            error("length mismatch error")

print start, stop, length, label

print "applying data dictionary"

f = open(dtf)
if not f:
    err("Could not open file: " + dtf.strip())

of = open(dtf + "_dd_apply.csv", "wb")

if label[-1] == "LINEFEED":
    of.write(",".join(label[0:len(label)-1]) + "\n")
else:
    of.write(",".join(label) + "\n")

ul = len(label) if label[-1] != "LINEFEED" else len(label) - 1
while(True):
    row = []
    line = f.readline()
    for i in range(0, ul):
        d = (line[start[i] - 1 : stop[i]]).strip()
        row.append(d.replace(",", ";"))
    of.write(",".join(row)+ "\n")
of.close()