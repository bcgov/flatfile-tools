import os
import sys
import time
from misc import *

default = 10
args = sys.argv
delay, arg = default, " ".join(args[1:])
if len(args) > 1:
    try:
        delay = int(args[1])
        arg = args[2:]
    except:
        pass

    print "sys.argv", args
    print "arg", arg

    cmds = " ".join(arg).split(";")
    print "cmds", cmds
    while(True):
        for c in cmds:
            print("\texec(" + c + ")")
            a = os.system(c)
            if a != 0:
                pass # could have a warning in here
        time.sleep(delay)
