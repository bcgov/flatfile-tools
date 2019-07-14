# 20190617 apply pqt command to all files in a folder
import os
import sys
from misc import *

if len(sys.argv) != 1:
    err("apply pqt.cpp to all files in present folder.")

run("mkdir pqt")

files = os.popen("ls -1 *csv*").readlines()

for f in files:
    run("pqt " + f.strip())

run("mv -v *.pqt pqt")

# new pattern: commands always produce results in a (sub) folder,
# where the subfolder has the name of the command applied : )