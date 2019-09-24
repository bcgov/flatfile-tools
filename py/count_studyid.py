import os
import sys
from misc import *

debug = False

if len(sys.argv) < 2:
    err("count_studyid [csv filename]")

def count_id(fn):
    f = open(fn)
    ci = 0 # line index
    idc = -1 # col. index of studyid field
    count, count_count = {}, {}

    fields = f.readline().strip().split(",")
    ci += 1

    for i in range(0, len(fields)):
        if fields[i].lower() == "studyid":
            idc = i

    while True:
        line = f.readline()
        if not line:
            break
        words = line.strip().split(",")

        id = words[idc]
        if id not in count:
            count[id] = 0
        count[id] += 1
        ci += 1

    for id_ in count:
        if debug and (count[id_] > 1):
            print id_, count[id_]

        c = count[id_]
        if c not in count_count:
            count_count[c] = 0
        count_count[c] += 1

    print "studyid count, instances of count"
    for c in count_count:
        print str(c) + "," + str(count_count[c])
    print "number of distinct studyid,", str(len(count.keys()))

filename = sys.argv[1]
count_id(filename)