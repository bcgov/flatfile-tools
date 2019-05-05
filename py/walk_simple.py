import os
'''20190104 find.py: directory/ files listing, recursive (before we learned
cygwin available'''
from misc import *

# add this as a command line parameter
args = sys.argv
if len(args) < 2:
    err("usage: find [path]")
    
pwd = sys.argv[1].strip()
if not os.path.exists(pwd) and not os.path.isdir(pwd):
    print "Error: directory does not exist", pwd

ext = {}
for r, d, fs in os.walk(pwd):
    for f in fs:

        # output full path
        p = os.path.join(r,f) + os.linesep
        print p.strip()

        # file-ext count        
        fw = f.split(".")
        if len(fw) > 1:
            e = fw[-1]
            ext[e] = ext[e] + 1 if (e in ext) else 1


e = [[ext[ee],ee] for ee in ext]

e.sort(reverse=True)
print ""
print "file count, by extension:"
for ee in e:
    print "\t"+str(ee[1]) + "," + str(ee[0])
