# grep a csv file for an expression and copy the csv header into the output
import os
import sys
from misc import *

if len(sys.argv) < 3:
    err("usage: csv_grep [pattern] [input csv file]")

pattern, filename = sys.argv[1], sys.argv[2]
grep_file = filename + "_grep" # grep results go here
a = os.system("grep " + pattern + " " + filename + " > " + grep_file)

f = open(filename)
fields = f.readline().strip() + "\n"
f.close()
fields_file = filename + "_fields" # names of fields go in this file
open(fields_file, "wb").write(fields)

a = os.system("cat " + fields_file + " " + grep_file + " > " + filename + "_grep_" + pattern + ".csv")
run("rm -f " + grep_file)
run("rm -f " + fields_file)