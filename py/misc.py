import os
import sys

def err(m):
    print("Error: " + str(m))
    sys.exit(1)

def run(cmd):
    print("run(" + cmd + ")")
    a = os.system(cmd)
    if a != 0:
        err("command [" + cmd + "] failed with code: " + str(a))

def exists(f):
    return os.path.exists(f)

def assert_exists(f):
    if not exists(f): err("file does not exist: " + str(f))

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
