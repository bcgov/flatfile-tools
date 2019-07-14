# qc by checking number of fields per line
import os
import sys
f = open("School_Student.csv","rb")
ci = 0
line_last, nfl = None, None
while True:
    line = f.readline()
    if line.strip() == "": continue
    if not line:
        break
    lines = line.strip().split(",")
    nf = len(lines)
    #print nf

    ##if nf != 63:
    #    print nf, line

    ci += 1

    if nf != 63:
        print nf
        print line

    line_last = line
    nfl = nf
    #if ci > 500:
    #    break