import os
import sys
from misc import *
if len(sys.argv) < 2:
    err("rscript.py: usage: [name of R file to run] [first parameter for R script] ...[last parameter for R script]")

# convert to a path that works!
r_path = '/cygdrive/c/Program Files/R/R-3.5.3/bin/Rscript.exe'

if not exists(r_path):
    r_path = '/cygdrive/c/Program Files/R/R-3.6.1/bin/Rscript.exe'

cmd = os.popen("(cygpath -d '" + r_path + "')").read().strip()
a = os.system(cmd + " " + " ".join(sys.argv[1:]))
