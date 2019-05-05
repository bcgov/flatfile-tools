import os
import sys
import csv
from misc import *
#2019 01 09: paste two tables together left-right: assert last col. matches
import time; t = time.time(); tt = time.time(); ttt = None;

fn1 = "CB_STULVLCB_FT_SCHLSTUD_VWD_LK.csv_slice.csv_flatten.csv"
fn2 = "CB_STULVLCB_FT_SCHLSTUD_VWD_LK.csv_slice.csv_revmod_flatten.csv"

ofn = fn1 + "_join.csv"; of = open(ofn, "wb")

f1 = open(fn1); line1 = f1.readline().strip()
fields1 = line1.split(","); n_f1 = len(fields1)

f2 = open(fn2); line2 = f2.readline().strip()
fields2 = line2.split(","); n_f2 = len(fields2)

for i in range(0, len(fields2)):
    fields2[i] = "2"+fields2[i]

of.write(",".join(fields1)); of.write(",")
of.write(",".join(fields2)); of.write("\n"); ci = 0
while True:
    line1 = f1.readline();  line2 = f2.readline()
    if not line2: break
    if not line2: break
    line1 = line1.rstrip(); line2 = line2.rstrip()
    w1 = line1.split(","); w2 = line2.split(",")
    if w1[-1] != w2[-1]:
        err("mismatch")
        
    if len(w1) != n_f1:
        err("")

    of.write(",".join(w1)); of.write(",")
    of.write(",".join(w2)); of.write("\n")
    
    ci += 1
    if ci % 10000 == 0:
        ttt = tt
        tt = time.time()
        print ci/1000, "1/2 k, S/10 k: ", tt-ttt
of.close()
f1.close()
f2.close()
