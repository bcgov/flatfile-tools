'''20190212 finish this when we have xlrd installed? 20190214 used R-project incarnation
   20190501: update to accommodate multiple files with original datadictionary same file name
current interfaces:
  python rscript.py dd_unpack.R data_dictionary_education.xlsx
  rscript dd_unpack.R data_dictionary_education.xlsx

#   1) dd_list.py: list data dictionaries and make copy in local folder, extracting to csv
#   2) dd_clean.py: clean the data dictionary csv files (for example, they are broken if there's newlines in an excel cell)
#   3) dd_examine.py: loop through the results to check if the cleaned outputs are good
#   4) dd_extract.py:
#   5) dd_match.py

'''
import os
import sys
import csv
from misc import *

def run(cmd):
    print cmd
    a = os.system(cmd)
    if a != 0:
        err("command failed: [" +str(cmd) + "]")


if os.path.exists('./dd'):
    err("data dictionary folder already initialized.")

csv.register_dialect('my', delimiter=",", quoting=csv.QUOTE_ALL, skipinitialspace=True)
whoami = os.popen("whoami").read().strip()
os.system("mkdir dd")

# this script should be modified to match listed files, to the dd that are in the same folders!
# e.g., have a for loop that runs over all the folders!

excel = []
xls = os.popen('find R:/DATA/ -name "data_dictionary*.xls"').read().strip().split("\n")
for xlf in xls:
    excel.append(xlf.strip())
xlsx = os.popen('find R:/DATA/ -name "data_dictionary*.xlsx"').read().strip().split("\n")
for xlf in xlsx:
    excel.append(xlf.strip())
    print xlf.split("/")

print "copying excel files into dd/ folder.."
for xlf in excel:
    if os.path.exists(xlf):
        date_str = xlf.split('/')[2].strip().replace(" ", "_")
        run("cp -v " + xlf + " dd/" + date_str + "_" + xlf.split('/')[-1])

print "extracting data dictionaries"

excel = os.popen("ls -1 dd/").read().strip().split("\n")
for xlf in excel:
    localf = "./dd/" + xlf
    localfp=localf.replace('\ ','_')
    localfp=localfp.replace(' ','_')
    run("mv " + localf + " " + localfp)
    localf = localfp.strip("\'")
    print "\t" + "./dd/" + localf

    # need to update this line:
    dd_unpack = os.path.abspath("R:/" + whoami + "/bin/R/dd_unpack.R").replace('\\','/')
    run("rscript " + dd_unpack + " " + localf)