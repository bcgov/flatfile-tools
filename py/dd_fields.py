# list the fields in a cleaed dd file
import os
import sys

if len(sys.argv) < 2:
    print "dd_fields.py [cleaned data dictionary file]"
    sys.exit(1)

lines = open(sys.argv[1]).read().strip().split('\n')
def pr(i):
    labels = []
    for line in lines:
        labels.append(line.strip().split(',')[i])
    s = ""
    for i in range(1, len(labels)):
        s += labels[i] + " "
    print s
print "field names:"
pr(3)
