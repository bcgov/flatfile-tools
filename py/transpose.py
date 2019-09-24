# 20190910 transpose a csv file
from misc import *

if len(args) < 2:
    err("transpose.py: transpse csv\n\ttranspose [csv input file name")

lines = open(args[1]).readlines()
fields = lines[0].strip().split(',')
n_row = len(lines)

di = []
for i in range(0, len(lines)):
    di.append(lines[i].strip().split(','))

ofn = args[1] + "_transpose.csv"
o_f = wopen(ofn)

ci = 0
js = []
for j in range(0, len(fields)):
    js.append(j)

js.reverse()

for j in js:
    if ci > 0:
        o_f.write("\n")
    ci += 1

    datarow = [di[i][j] for i in range(0, len(lines))]
    o_f.write(','.join(datarow))

o_f.close()