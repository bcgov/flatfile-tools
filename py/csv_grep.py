# grep a csv file for an expression and copy the csv header into the output
import os
import sys
from misc import *

if len(sys.argv) < 3:
    err("usage: csv_grep [pattern] [input csv file]")

pattern = sys.argv[1]
filename = sys.argv[2]

a = os.system("grep " + pattern + " " + filename + " > " + filename + "_grep")
if a != 0:
    err("grep failed")

f = open(filename)
fields = f.readline().strip() + "\n"
open(filename + "_fields", "wb").write(fields)

a = os.system("cat " + filename + "_fields " + filename + "_grep > " + filename + "_grep_" + pattern + ".csv")
if a != 0:
    err("cat failed")