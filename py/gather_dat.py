# 20190401 gather .dat files into a flat structure. check for name collisions before executing!
# after, delete any empty subdirs
import os
import sys
import shutil

# default to present directory, if argument not supplied
pwd = os.getcwd() if len(sys.argv) <= 1 else sys.argv[1]

fn, tgt = {}, {}
# traverse subdirectories in filesystem tree
for r, d, f in os.walk(pwd):
        for _file in f:
            p = os.path.join(r,_file) # + os.linesep
            if os.path.isfile(p):
                fn[p] = 1 if not p in fn else fn[p] + 1
            tgt[p] = os.path.join(pwd, _file)

for p in fn:
    if not os.path.exists(tgt[p]):
        print p, "-->", tgt[p]
        shutil.move(p, tgt[p])