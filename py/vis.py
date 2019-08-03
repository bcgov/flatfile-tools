import os
import sys
from misc import *

if len(sys.argv) < 2:
    err("vis.py [input file name]")

open("./.vis", "wb").write(",".join(sys.argv[1:]))

run("glut")


