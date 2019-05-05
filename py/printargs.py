import os
import sys
args = sys.argv
print "number of arguments: ", len(args)
for i in range(0, len(args)):
    print "arg("+str(i)+"): " + str(args[i])
