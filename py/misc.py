import os
import sys
import pickle #import cPickle as pickle

def err(m):
    print("Error: " + str(m))
    sys.exit(1)

def run(cmd):
    print('  run("' + cmd + '")')
    a = os.system(cmd)
    if a != 0:
        err("command [" + cmd + "] failed with code: " + str(a))

def exists(f):
    return os.path.exists(f)

def assert_exists(f):
    if not exists(f):
        err("could not find req'd file: " + str(f))


def load_fields(args): # load records and index by studyid
    print("load_fields " + str(args))
    dat, fn, load_fields = {}, args[0], args[1:]
    assert_exists(fn)
    f = open(fn)
    fields = f.readline().strip().split(",") # read the header
    field = {fields[i] : i for i in range(0, len(fields))}
    print("fields", fields)
    if 'studyid' not in fields: err("req'd field: studyid")
    # make sure other required fields are present
    for lf in load_fields:
        if lf not in fields: err("req'd field: " + str(lf))
    ci = 0
    while True:
        line = f.readline()
        if not line: break
        words = line.strip().split(",")
        i, loaded = words[field['studyid']], []
        if i not in dat: dat[i] = []
        for lf in load_fields:
            loaded.append(words[field[lf]])
        dat[i].append(loaded)
        ci += 1
    print("loaded ", ci, " records")
    return dat, load_fields


def printw(fn):
    print("+w " + fn.strip())

def printr(fn):
    print("+r " + fn.strip())


# open required output file, assert open
def wopen(fn):
    o_f = open(fn, "wb")
    if not o_f:
        err("failed to open output file: " + fn)
    printw(fn)
    return o_f

def ropen(fn):
    i_f = open(fn)
    if not i_f:
        err("failed to open input file: " + fn)
    printr(fn)
    return i_f


args = sys.argv

# calculate histogram for a list of values
def histo(x):
    h = {}
    for i in range(0, len(x)):
        d = x[i]
        if d not in h:
            h[d] = 0.
        h[d] += 1.
    return h

