import os
import sys
from misc import *

# constructive definition:
if len(sys.argv) > 1:
    fn = sys.argv[1]
    f = open(fn)
    hdr = f.readline().strip().split(",")
    for i in range(0, len(hdr)):
        hdr[i] = hdr[i].lower()

    print hdr
    print len(hdr), " number of fields"
else:
    err("usage: csv_count_fields.py [input file name: fields in header, to be counted]")