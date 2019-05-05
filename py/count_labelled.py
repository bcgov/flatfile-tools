'''count occurrence of data observations, per field, per label: 
last col. of input is a label (e.g., cluster label)'''
#for each field, for each cluster:
# count how often things occur
import sys

# first line is header (field names)
occ = {}
f = open("fields.csv")
fields = f.readline().strip().split(",")
print fields
for k in range(0, len(fields)-1):
    i = fields[k]
    occ[i] = {}
    for j in range(0,6):
        occ[i][j] = {}

li =0 
while True:
    li += 1
    line = f.readline()
    if not line: break
    words = line.strip().split(",")
    # if li < 20: print words
    for k in range(0, len(words)): words[k] = words[k].strip()
    ci = int(words[-1]) # cluster index
    for k in range(0, len(fields)-1):
        i = fields[k] # field name
        if words[k] != "":
            d = int(words[k]) # value in csv col.
            if d not in occ[i][ci]:
                occ[i][ci][d] = 0
            occ[i][ci][d] += 1
           
print occ
print "field,clusterid,nonz-data,count"
# f.close()
# f = open("data_count.csv","wb"); f.write(",".join(fields[:len(fields)-1])+ "\n")
for k in occ:
    ok = occ[k]
    for ci in ok:
        okci = ok[ci]
        for d in okci:
            okcid = okci[d]
            line = ",".join([str(k), str(ci), str(d), str(okcid)])
            print line #f.write(line + "\n")
# f.close()

