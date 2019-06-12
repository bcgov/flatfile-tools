import os
import sys
files = open("msp_files.txt").read().strip().split("\n")

for f in files:
    cmd = "csv_select cohort.csv_studyid studyid " + f
    print cmd
    a = os.system(cmd)