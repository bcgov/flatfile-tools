# list the fields in a cleaed dd file
import os
import sys

inf = sys.argv[1]
lines = open(inf).read().strip().split('\n')
def pr(i):
    labels = []
    for line in lines:
        labels.append(line.strip().split(',')[i])
    print labels

pr(3)

