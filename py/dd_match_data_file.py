# 20190617 find an acceptable data dictionary for a given file
import os
import sys
from misc import *
from ansicolor import *

if len(sys.argv) < 2:
    err("dd_match_data_file.py: find data dictionary for data file.\n\tusage: dd_match_data_file [data file.dat]\nNote: data dictionaries expected in folder dd/, produced by dd_list.py")

if not os.path.exists("dd") or not os.path.isdir("dd"):
    err("dd/ expected. Please run dd_list")

f = sys.argv[1]
if not os.path.exists(f) or not os.path.isfile(f):
    err("could not find input file: " + str(f))

fn = os.popen("cygpath -d " + f.strip()).read().strip()
print f, fn
files = {}
dd_files = os.popen('find ./dd').readlines()
for d in dd_files:
    d = d.strip()
    if d[-4:] == "csv2":
        files[d] = True

datfiles = [fn]
matches = []

def chars_per_line(fn, datfiles):
    # use data dictionary file to predict number of characters per line, in a data file
    global matches
    print fn
    csv_l = open(fn).read().strip().split('\n')
    fields = csv_l[0].split(",")
    if fields[0].lower() != "start":
        print("csv2 file expected first field name: start")
        return -1
    if fields[1].lower() != "stop":
        print("csv2 file expected second field name: stop")
        return -1

    # for l in csv_l: print l.strip()
    csv_l = csv_l[1:] # remove header and process

    total_len = 0
    for i in range(0, len(csv_l)):
        w = csv_l[i].strip().split(",")
        start = int(w[0])
        stop = int(w[1])
        length = stop - start + 1
        total_len += length

    print "\tdata record length (chars)", total_len
    had_match = False
    for i in range(0, len(datfiles)):
        line = open(datfiles[i]).readline()
        if len(line) == total_len:
            if len(matches) <= 1000: # <= 10
                print "\tmatch", datfiles[i]
                had_match = True
            matches.append([fn, len(line), total_len])
    if len(matches) != 1 and had_match:
        print "****** Warning: nonunique match", len(line), total_len

for d in files:
    chars_per_line(d, datfiles)

print "\n\n******* MATCHES: [file, len(line), len(line) predicted by file]"
for i in range(0, len(matches)):
    m = matches[i]
    print("\t(" + str(i+1) + ") " + str(m))


# default to not using files with "cimg" in them..
matches_keep = []
for i in range(0, len(matches)):
    m = matches[i]
    if len(m[0].lower().split("cmg")) < 2:
        matches_keep.append(m)
    else:
        print("discarding match: " + str(m))
matches = matches_keep

# find a match!
match_use = None
if len(matches) == 1:
    match_use = 0
else:
    while True:
        try:
            answer = input("please select one of the above matches: ")
            match_use = int(answer) - 1
            if match_use >=0 and match_use < len(matches):
                break
        except:
            print("please enter a number from 1 to however many matches there are")
    # err("unique dd match not found")
open(f + ".dd", "wb").write(matches[match_use][0])