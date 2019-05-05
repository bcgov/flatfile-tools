'''2019 01 09
filter multi-col table (from join.py) to produce mental health cols that don't
throw away non-non-special-needs codes!'''
import os
import sys
from misc import *
import time; t = time.time(); tt = time.time(); ttt = None;
in_fn = "CB_STULVLCB_FT_SCHLSTUD_VWD_LK.csv_slice.csv_flatten.csv_join.csv"
f= open(in_fn); g = open(in_fn + "_filter_education.csv", "wb")
fields = f.readline().split(","); print fields
f_idx = {}
for i in range(0, len(fields)):
    fields[i] = fields[i].strip()
    f_idx[fields[i]] = i
compare = ["SPEC_NEED_PERF_GRP_THIS_COLL",
          "SPECIAL_NEED_GROUP_THIS_COLL",
          "SPECIAL_NEED_CODE_THIS_COLL",
          "SPECIAL_NEED_THIS_COLL"]

for i in range(0, len(compare)):
    fields[i] = compare[i] + "_EVER"

for i in range(0, len(compare)):
    fields[i] = fields[i].replace("_THIS_COLL", "")

g.write(",".join(fields)+"\n"); ci = 0    
while True:
    line = f.readline()
    if not line:
        break
    w = line.strip().split(",")
    if len(w) != len(fields):
        err("len(w) != len(fields)")
 
    # innocent until proven guilty
    special, special1, special2 = False, False, False
    def spec(c):
        return c[0:3].lower() != "non"

    if spec( w[f_idx[compare[0]]] ): special = True; special1 = True
    if spec( w[f_idx[compare[2]]] ): special = True; special1 = True
    if spec( w[f_idx[compare[3]]] ): special = True; special1 = True
    if spec( w[f_idx["2"+compare[0]]] ): special = True; special2 = True
    if spec( w[f_idx["2"+compare[2]]] ): special = True; special2 = True
    if spec( w[f_idx["2"+compare[3]]] ): special = True; special2 = True

    wc = ["", "", "", ""]
    if (special1 and not(special2)):
        for i in range(0, len(compare)):
            wc[i] = w[f_idx[compare[i]]]
    elif (not(special1) and special2):
        for i in range(0, len(compare)):
            wc[i] = w[f_idx["2"+compare[i]]]
    else:
        for i in range(0, len(compare)):
            wc[i] = w[f_idx[compare[i]]]
    for i in range(0, len(wc)):
        w[i] = wc[i]
    g.write(",".join(w) + "\n")
    ci += 1
    if ci % 10000 == 0:
        ttt = tt; tt = time.time(); print ci/1000, "1/2 k, S/10 k: ", tt-ttt  
        
        
