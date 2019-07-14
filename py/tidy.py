# 20190711 : tidy up the folders by executing ws and dent
import os
import sys
from misc import *

if not (os.path.exists('R') and os.path.exists('c') and os.path.exists('cpp') and os.path.exists('py')):
    err("must execute from within bin folder")

py = os.popen("find ./ -name '*.py'").readlines()
py = [py[i].strip() for i in range(0, len(py))]
for p in py:
    run("ws " + p)

c = os.popen("find ./ -name '*.cpp'").readlines()
c = [c[i].strip() for i in range(0, len(c))]
for cc in c:
    run("dent " + cc)
    run("ws " + cc)

c = os.popen("find ./ -name '*.c'").readlines()
c = [c[i].strip() for i in range(0, len(c))]
for cc in c:
    run("dent " + cc)
    run("ws " + cc)


