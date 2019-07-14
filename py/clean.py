'''updated 2019/03/13: clean up messy files: e.g., *~, .swp, .bak, .exe'''
import os
import sys
from misc import *

def yes_no(question):
    while True:
        reply = str(raw_input(question+' (y/n): ')).lower().strip()
        return True if reply[0] == 'y' else False

def find(ext):
    ret = {}
    cmd = 'find ./ -name "*' + ext + '"'
    lines = os.popen(cmd).read().strip().split("\n")
    for f in lines:
        ff = f.strip()
        if ff != "":
            ret[ff] = ""

    cmd = 'find ./ -name ".*' + ext + '"'
    lines = os.popen(cmd).read().strip().split("\n")
    for f in lines:
        ff = f.strip()
        if ff != "":
            ret[ff] = ""
    return ret

d = { }
for e in ["~", ".swp", ".bak", ".exe", ".pyc", "__pycache__"]:
    f = find(e)
    for ff in f:
        d[ff] = ""

if len(d) == 0:
    sys.exit(0)


for f in d:
    print(f.strip())

print("")

if yes_no("\ndelete the above files?"):
    for f in d:
        if os.path.exists(f):
            cmd = 'rm -rf "' + f + '"'
            print("\t" + cmd)
            a = os.system(cmd)
            if a != 0:
                err("failed to delete file: " + f)