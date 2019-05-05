# 2019/01/07 mode flattening operation
import os
import sys
import csv
import operator
from misc import *
import time; t = time.time(); tt = time.time(); ttt = None;

infile = "CB_STULVLCB..*" # insert file name here

reverse_mode = True
mod = "" if reverse_mode == False else "_revmod"
of = open(infile + mod + "_flatten.csv", "wb")

f = open(infile)
line = f.readline().strip()
fields = line.split(","); n_f = len(fields)
if not (fields[-1] == 'STUDYID' or fields[-1] == 'studyid'):
    err("study id field expected")
d = {}
ci = 0

while True:
    line = f.readline()
    if not line:
        break
    line = line.rstrip()
    w = line.split(",")
    if len(w) != n_f:
        g=open("tmp.csv","wb"); g.write(fields+"\n"+line); g.close()
        reader = csv.reader(open("tmp.csv"), dialect='my')
        hdr, lin = reader
        w = lin
    if len(w) != n_f:
        err("")
    if w[-1] not in d:
        d[w[-1]] = []
    d[w[-1]].append(w[0:len(w)-1])
    ci += 1
    if ci % 10000 == 0:
        ttt = tt
        tt = time.time()
        print ci/1000, "1/2 k, S/10 k: ", tt-ttt

of.write(",".join(fields) + "\n")
ci = 0
for k in d:
    if len(d[k]) > 1000:
        # scrap studyid with unreasonably many entries
        continue 
    else:
        dd = d[k]
        out = dd[0]
        #print "dd", dd
        for j in range(0, len(dd[0])):
            # for each field:
            c = {}
            for i in range(0, len(dd)):
                di = dd[i][j]
                if di not in c:
                    c[di] = 0
                c[di] += 1 #  print c
            if reverse_mode:
                sorted_c = sorted(c.items(), key=operator.itemgetter(1), reverse=False)
            else:
                sorted_c = sorted(c.items(), key=operator.itemgetter(1), reverse=True)
            out[j] = sorted_c[0][0]
            #if len(d[k]) > 1:
            #    print "  cc", sorted_c
            #    print "   -> ", out[j] #print "s[0]", s[0], "c[s[0]]", c[s[0]]
        out.append(k) # dont forget studyid
        #print "* out", out
        if len(out) != len(fields):
            err("")
        of.write(",".join(out) + "\n")
    ci += 1
    if ci % 10000 == 0:
        ttt = tt
        tt = time.time()
        print ci/1000, "2/2 k, S/10k: ", tt-ttt
        
f.close(); of.close()
print "done"
print "  ", int((ci / (time.time() - t))/1000.), "K lines per sec"
