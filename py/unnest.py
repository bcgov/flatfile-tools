# 2019/01/07: slice csv with header
# nested commas tolerated!
import os
import sys
import csv
import time; t = time.time(); print(t)
csv.register_dialect('my',
                     delimiter=",",
                     quoting=csv.QUOTE_ALL,
                     skipinitialspace=True)
infile = sys.argv[1] #infile = "CB_STULVLCB_FT_SCHLSTUD_VWD_LK.csv"

outfile = infile+"_unnest.csv"; of = open(outfile, "wb")
f, ci, f_names, n_f, fields = open(infile), 0, None, 0, None
tt = time.time()
ttt = None
r_c = 0
while True:
    line = f.readline()
    if not line: break
    line = line.strip()
    if ci == 0:
        f_names = line.split(",")
        print(f_names)
        n_f, fields = len(f_names), ",".join(f_names)
        # write out header
        for i in range(0, len(f_names)):
            f_names[i] = f_names[i].strip('"')
        of.write((",".join(f_names) + "\n").encode())
    else:
        remediate = False
        w = line.split(",")
        if len(w) != n_f:
            tmp_fn = "tu.csv"
            g=open(tmp_fn,"wb"); g.write(fields+"\n"+line); g.close()
            reader = csv.reader(open(tmp_fn), dialect='my')
            hdr, lin = reader
            w = lin
            remediate = True
        if len(w) != n_f:
            print("error", len(w), n_f)
            sys.exit(1)
        if remediate:
            r_c += 1
            for i in range(0, len(w)):
                w[i] = w[i].replace(',',';')
        for i in range(0, len(w)):
            w[i] = w[i].strip('"')
        # write out line
        of.write((','.join(w) + '\n').encode())
    ci += 1
    if ci % 1000 == 0:
        ttt = tt
        tt = time.time()
        print(ci/1000, "k, S/k: ", tt-ttt)
print("done")
print("  ", int((ci / (time.time() - t))/1000.), "K lines per sec")
print("remediate count", r_c)
f.close(); of.close()
