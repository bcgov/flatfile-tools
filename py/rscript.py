import os
import sys
from misc import *
if len(sys.argv) < 2:
    err("rscript.py: usage: [name of R file to run] [first parameter for R script] ...[last parameter for R script]")

# convert to a path that works!
cmd = os.popen("(cygpath -d '/cygdrive/c/Program Files/R/R-3.5.2/bin/Rscript.exe')").read().strip()
a = os.system(cmd + " " + " ".join(sys.argv[1:]))

