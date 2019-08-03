import pickle
from misc import *
args = sys.argv

if len(args) < 2:
    err("get and unzip most recent datfile\nusage:\n\tdf_get [dat file name: .gz or .dat]")

if not os.path.exists("recent_datfile_names.p"):
    run("df_list")

[recent_datfile_names] = pickle.load(open("recent_datfile_names.p"))

fn = args[1]
if fn not in recent_datfile_names:
    fn = fn + ".gz"
if fn not in recent_datfile_names:
    err(args[1] + " not found")

remote = recent_datfile_names[fn]

run("cp -v " + remote + " .")

run("unzp " + fn)

