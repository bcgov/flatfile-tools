from misc import *

if len(args) < 3:
    err("\n\tskipf [csv file] [skip factor]")

in_f = args[1]
skip_f = int(args[2])

print "input file: ", in_f
print "skip factor: ", skip_f

lines = open(in_f).readlines()


out_f = open(in_f + '_skipf.csv', 'wb')

if not out_f:
    err("failed to open output file")

out_f.write(lines[0].strip())

rc = 0
for i in range(1, len(lines)):
    if i % skip_f == 0:
        out_f.write('\n' + lines[i].strip())
        rc += 1

out_f.close()

print "non-header data lines written: ", rc