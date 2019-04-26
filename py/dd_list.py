'''20190212 finish this when we have xlrd installed? 20190214 used R-project incarnation

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
if os.path.exists('./dd'):
    err("data dictionary folder already initialized.")

csv.register_dialect('my', delimiter=",", quoting=csv.QUOTE_ALL, skipinitialspace=True)
whoami = os.popen("whoami").read().strip()
os.system("mkdir dd")
os.system("rm -rf dd/*")

excel = []
xls = os.popen('find R:/DATA/ -name "data_dictionary*.xls"').read().strip().split("\n")
for xlf in xls:
    excel.append(xlf.strip())
xlsx = os.popen('find R:/DATA/ -name "data_dictionary*.xlsx"').read().strip().split("\n")
for xlf in xlsx:
    excel.append(xlf.strip())

print "copying excel files into dd/ folder.."
for i in range(0, len(excel)):
    xlf = excel[i].strip()
    a = os.system("cp -v " + xlf + " dd/")

print "extracting data dictionaries"

excel = os.popen("ls -1 dd/").read().strip().split("\n")
for xlf in excel:
    localf = "./dd/" + xlf
    localfp=localf.replace('\ ','_')
    localfp=localfp.replace(' ','_')
    a = os.system("mv " + localf + " " + localfp)
    localf = localfp.strip("\'")
    print "\t" + "./dd/" + localf

    # need to update this line:
    dd_unpack = os.path.abspath("C:/cygwin64/home/" + whoami + "/bin/R/dd_unpack.R").replace('\\','/')
    cmd = "rscript " + dd_unpack + " " + localf
    print "\t\t" + cmd
    a = os.system(cmd)




