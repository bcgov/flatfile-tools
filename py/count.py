# 20190311 accumulate observations in CSV file.. important for Q.A.!
import os
import sys
import operator
from misc import *

if len(sys.argv) < 2:
    err("usage: count.py [csv input file]")

f = open(sys.argv[1])
f_count = open(sys.argv[1] + "_count", "wb")
f_freq = open(sys.argv[1] + "_frequ", "wb")


fields = f.readline().strip().split(",")
print("fields", fields)
occ = []

for i in range(0, len(fields)):
    occ.append({})

ci = 1
import time
f_size = os.stat(sys.argv[1]).st_size
ttt = tt = t_0 = time.time()

while True:
    line = f.readline()
    if not line: break
    word = line.strip().split(",")
    if len(word) != len(fields):
        print("warning: incorrect number of fields, line: ", ci)
    for i in range(0, len(word)):
        d = word[i].strip()
        if d not in occ[i]:
            occ[i][d] = 0.
        occ[i][d] += 1.
    ci += 1
    if ci % 100000 == 0:
        ttt = tt
        tt = time.time()
        print("file", " %: ", 100. * (float(f.tell()) / float(f_size)), " MB/s:", (float(f.tell()) / 1000000.) / (tt- t_0))#

counts = []
frequs = []
for i in range(0, len(fields)):
    # if fields[i] == "studyid": continue
    print("sorting field ", fields[i], "..")
    if True: #len(occ[i]) < 100:
        n = 0  # number of observations
        s = str(fields[i])
        occ_i = sorted(occ[i].items(), key=operator.itemgetter(0))
        for j in occ_i:
            s += "\n  " + str(j)
            n += j[1]
        counts.append(s)

        s = str(fields[i])
        occ_i = sorted(occ[i].items(), key=operator.itemgetter(1), reverse=True)
        for k in range(0, len(occ_i)):
            j = occ_i[k]
            j = list(j)
            j[1] = j[1] * (100. / float(n))
            j = tuple(j)
            s += "\n  " + str(j)
        frequs.append(s)

f_count.write("COUNTS\n".encode())
for c in counts:
    f_count.write((str(c) + "\n").encode())

f_freq.write("FREQUENCIES\n".encode())
for f in frequs:
    f_freq.write((str(f) + "\n").encode())