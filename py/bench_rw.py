# 20190709 this script should work in a proper unix environment
import os
import sys
args = sys.argv

def err(m): print("Error: " + str(m)); sys.exit(1)
if len(args) < 2: err("Usage:\n\tbench_rw.py [outfileination file]")
outfile = args[1]

if os.path.exists(outfile):
    err('file exists: ' + outfile + '\n\tpls. delete and try again')

device = '/dev/zero/'
blk_sz = str(1024)
count = str(1024*1024)

cmd = ' '.join(['dd if=' + device,
                'bs=' + blk_sz,
                'count=' + count,
                'of=' + outfile])
print cmd
a = os.system(cmd)