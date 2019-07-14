#20190617 extract all fields from a fixed-width file, finding dd automatically!
import os
import sys
from misc import *

if len(sys.argv) < 2:
    err("dd_sliceapply_all.py: extract all fields from a .dat file, choosing the data dictionary automatically. usage: \n\tdd_sliceapply_all [dat file name]\nNote: dd/ directory expected in present working folder")

fn = sys.argv[1]
ddn = fn + ".dd"
if not os.path.exists(fn) or not os.path.isfile(fn):
    err("couldn't find input file: " + fn.strip())

if not os.path.exists(ddn):
    run("dd_match_data_file " + fn)

if not os.path.exists(ddn):
    err("data dictionary match not found")

dd = open(ddn).read().strip()

if not os.path.exists(dd):
    err("could not find dd file: " + dd.strip())

fields = os.popen("dd_fields " + dd).read().strip().split("\n")[-1].split(" ")
print "fields", fields

run("dd_slice_apply " + dd + " " + fn.strip() + " " + (" ".join([f for f in fields if f.upper() != "LINEFEED"])))