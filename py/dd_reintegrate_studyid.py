from misc import *

if len(args) < 2:
    err("dd_reintegrate_studyid [csv2 file]")

fn = args[1]
if fn.split('.')[-1] != 'csv2':
    err("supposed to be run on csv2 file")

out_lines = []
dat = open(fn).read().strip().split("\n")
for i in range(0, len(dat)):
    w = dat[i].strip().split(',')
    if w[-1].lower() == 'studyida':
        continue
    if w[-1].lower() == 'studyid':
        if dat[i + 1].strip().split(',')[-1].lower() == 'studyida':
            out_lines.append(','.join([w[0], str(int(w[0]) + 9), str(10), 'STUDYID']))
    else:
        out_lines.append(','.join(w))

open(fn, 'w').write('\n'.join(out_lines))