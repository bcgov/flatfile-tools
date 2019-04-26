import os
import sys
import time
from misc import *

default = 10
args = sys.argv
delay, arg = default, args[1:]
if len(args) > 1:
    try:
        delay = int(args[1])
        arg = args[2:]
    except:
        pass

    cmd = ' '.join(arg)
    print cmd

    while(True):
        a = os.system(cmd)
        if a != 0:
            pass
        time.sleep(delay)
