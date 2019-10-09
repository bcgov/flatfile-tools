# to run a terminal command repeatedly, e.g. for monitoring processes, prefix it with "forever"
# e.g.: forever 1 ls -latrh to monitor files produced by a process
import os
import sys
import time
from misc import *

default = 10 # default interval (s) to repeat command
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
else:
    err("usage:\n\tforever [optional delay parameter in seconds] [your command(s) go here] [your command(s) go here]" +
        "\n\te.g. forever 1 ls -latrh to monitor files produced by a process")
