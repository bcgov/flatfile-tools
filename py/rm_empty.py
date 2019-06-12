# 20190401 remove empty folders, recursively
import os
import sys

def rm_empty(path, rm_root=True):
    if not os.path.isdir(path):
        return
    # rm empty subdirs
    files = os.listdir(path)
    for f in files:
        p = os.path.join(path, f)
        if os.path.isdir(p):
            rm_empty(p)

    # if folder empty, delete it
    files = os.listdir(path)
    if len(files) == 0 and rm_root:
        print "rm " + str(path)
        os.rmdir(path)

rm_root = False
rm_empty(os.getcwd(), rm_root)