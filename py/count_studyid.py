import os
import sys

def count_id(fn):
    f = open(fn)
    ci = 0
    idc = -1
    count = {}
    count_count = {}
    while True:
        line = f.readline()
        if not line:
            break
        if ci ==0:
            words = line.strip().split(",")
            for i in range(0, len(words)):
                if words[i]=="STUDYID" or words[i]=="studyid":
                    idc = i
        else:
            words = line.strip().split(",")
            id = words[idc]
            if id in count:
                count[id] += 1
            else:
                count[id] = 1
        ci += 1
        #if ci > 1000:
        #    break

    for id_ in count:
        if count[id_] > 1:
            print id_, count[id_]

        c = count[id_]
        if c in count_count:
            count_count[c] += 1
        else:
            count_count[c] = 1
            
    print "count of counts", str(count_count)

count_id(filename)

