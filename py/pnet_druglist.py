'''
2019-07-23 apply cohort file and mh-drug list, to concatenated pnet data
- product: various 

example use:
$ pnet_druglist psy_studypop_dad_15July2019.csv_studyid csv_cat_common_fields.csv

*** don't forget to delete pnet_druglist.p if the cohort or data change!!!!
'''
import os
import sys
import time
import pickle
import shutil
from misc import *

cohort_file, pnet_file = None, None
if len(sys.argv) >= 3:
    cohort_file, pnet_file = sys.argv[1], sys.argv[2]
else:
    err("druglist.py: search concatenated pnet file; apply mh-category drug list to determine mh-drug usage stats for study population\n\tdruglist [study population 1-col csv labelled studyid] [pnet concatenated file]")

print "cohort file: " + cohort_file
print "pnet file: " + pnet_file

fn_druglist = "NPDUIS_2019_002C_Drug_list_v1_00.xlsx"
if not os.path.exists(fn_druglist):
    err("required file: " + fn_druglist)

if not exists("dd"):
    run("dd_list")

hpf_file = "hlth_prod_final.dat_dd_sliceapply.csv"
if not os.path.exists(hpf_file):
    hpf_data_file = 'hlth_prod_final.dat'
    run('df_get ' + data_file)
    run('dd_sliceapply_all ' + hpf_data_file)

def load_hlth_labels():
    lines = open("hlth_prod_final.dat_dd_sliceapply.csv").readlines()
    hdr = lines[0].strip()
    lines = lines[1:]
    if hdr != "hp.din_pin,hp.drug_brand_nm,hp.gen_drug,hp.gcn_seq_num,hp.gen_drug_strgth_val,hp.gen_drug_strgth_unit,hp.gen_dsg_form_cd,hp.gen_dsg_form,hp.ahfs_3_cd,hp.tc5_cd":
        err("unexpected fields:\n\t" + hdr)
    hdr = hdr.strip().split(",")

    # return two dictionaries:
    hp_drug_brand_nm, hp_gen_drug ={}, {}
    for line in lines:
        w = line.strip().split(",")
        if len(w) != len(hdr):
            err("unexpected # of fields")
        hp_drug_brand_nm[w[0]] = w[1]
        hp_gen_drug[w[0]] = w[2]
    return hp_drug_brand_nm, hp_gen_drug

def count(x):
    c = {}
    for i in x:
        if i not in c:
            c[i] = 0
        c[i] += 1
    return c

def srt(x):
    y = [[x[i], i] for i in x]
    return sorted(y, key=lambda x: x[0], reverse=True)

'''
# e.g. testing the top3 concept:
x = [1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5]
print count(x)
srt(count(x))
print top3(x)
sys.exit(0)
'''

# top three concept
def top3(x):
    s = srt(count(x))
    if len(s) >= 3:
        return [i[1] for i in s[0:3]]
    elif len(s) == 2:
        return [s[0][1], s[1][1], str(None)]
    elif len(s) == 1:
        return [s[0][1], str(None), str(None)]
    else:
        return [str(None), str(None), str(None)]

# write R script to use readxl library to open the XLS file
open("dd_unpack.R", "wb").write('''# convert Excel files to CSV: one CSV per sheet in Excel file 20190213
# usage: Rscript dd_unpack.R [name of Excel file to unpack]
library(readxl)

fn<-""
args = commandArgs(trailingOnly=TRUE)

if(length(args)>=1) fn<-args[1]
if(length(args)==0) stop("Error: usage: Rscript dd_unpack.R [data dictionary file.xls/xlsx]")

for(s in excel_sheets(fn)){ out_fn<-paste(paste(fn, s, sep="_"), ".csv", sep=""); write.csv(read_excel(fn, sheet=s, skip=1), out_fn); }''')

a = os.system("rscript dd_unpack.R " + fn_druglist)
a = os.system("rm -f dd_unpack.R") # clean up
druglist = "NPDUIS\_2019\_002C\_Drug_list\_v1\_00.xlsx\_DIN\ list.csv"

for root, dirs, files in os.walk("."):
    for f in files:
        if len(f.split("DIN")) > 1:
            if os.path.exists(f):
                shutil.copyfile(f, "druglist.csv")
            else:
                err("problem copying file!")

''' N07B,ADDICTIVE DISORDER DRUGS
    N06C,PSYCHOLEPTICS AND PSYCHOANALEPTICS
    N06B,STIMULANTS
    N06A,ANTIDEPRESSANTS
    N05B,ANXIOLYTICS
    N05A,ANTIPSYCHOTICS'''

expected_fields = ["DIN_PDIN",
                   "BRAND_NAME_E_DESC",
                   "PDIN_FLAG*",
                   "ATC_LEVEL_4_CODE",
                   "ATC_LEVEL_4_E_DESC",
                   "ATC_LEVEL_5_CODE",
                   "ATC_LEVEL_5_E_DESC"]

# correct the drugslist csv to remove quotation marks, and remove leading zeros
lines, DIN_PDIN = open("druglist.csv").readlines(), -1
for i in range(0, len(lines)):
    w = lines[i].strip().split(",")
    for j in range(0, len(w)):
        w[j] = w[j].strip('"')
    # remove leading zeros from dinpin col
    if i == 0:
        for j in range(0, len(w)):
            if w[j] == "DIN_PDIN":
                DIN_PDIN = j
    else:
        w[DIN_PDIN] = w[DIN_PDIN].lstrip('0')
    lines[i] = ",".join(w)
open("druglist.csv", "wb").write("\n".join(lines))

lines = open("druglist.csv").readlines()
hdr, data = lines[0], lines[1:]
fields = hdr.strip().split(",")
fields = [f.strip('"') for f in fields]
f_i = {fields[i] : i for i in range(0, len(fields))}
for f in expected_fields:
    if f not in fields:
        err("expected field: " + f + " not found in druglist.csv")

atc_codes = {'N07B':'ADDICTIVE DISORDER DRUGS',
             'N06C':'PSYCHOLEPTICS AND PSYCHOANALEPTICS',
             'N06B':'STIMULANTS',
             'N06A':'ANTIDEPRESSANTS',
             'N05B':'ANXIOLYTICS',
             'N05A':'ANTIPSYCHOTICS'} # map 4-character atc code stem, to a readable description

ci, di = f_i['ATC_LEVEL_4_CODE'], f_i['DIN_PDIN']

# create categorized dinpin lists, that match 4-character ATC code stem, to dinpin
dinpin_lists = {} # match 4-character ATC code stem, to dinpin list (list of dinpin within that category)
reverse_lookup = {} # match dinpin bck to 4-character ATC code stem
for d in data:
    words = d.strip().split(",")
    atc4 = words[ci].strip().strip('"')
    dinpin = words[di].strip().strip('"').lstrip('0')
    if len(atc4) >= 4:
        atc44 = atc4[0:4]
        if atc44 in atc_codes:
            if atc44 not in dinpin_lists: dinpin_lists[atc44] = []
            dinpin_lists[atc44].append(dinpin)
            reverse_lookup[dinpin] = atc44 # be able to go backwards, too

print "\n**************************************"
print "  ATC Code, Drug Category Name, Count"
for c in dinpin_lists:
    print "  " +str(c) + "," + str(atc_codes[c]) + "," + str(len(dinpin_lists[c]))
print "************************************"

# read studyid, discard header (not a studyid)
studyid = open(cohort_file).read().strip().split("\n")[1:]

# make sure no whitespace
studyid = [i.strip() for i in studyid]

print "study population size:", len(studyid)

# get pnet file size and open it
pnet_file_size, f = os.stat(pnet_file).st_size, open(pnet_file)

# check for required fields in pnet file
hdr = f.readline().strip().split(",")
hdr_fi = {hdr[i] : i for i in range(0, len(hdr))}
si = hdr_fi['studyid'] if 'studyid' in hdr_fi else -1
di = hdr_fi['hlth_prod_label'] if 'hlth_prod_label' in hdr_fi else -1
sdi = hdr_fi['srv_date'] if 'srv_date' in hdr_fi else -1
if si == -1 or di == -1 or sdi == -1:
    err("required field not found in pnet file. studyid, hlth_prod_label, or srv_date")

# where --> is "map" (apply dictionary)
pins = {} # studyid --> list of pins (count top3) (single nested dictionary)
first = {} # studyid --> category --> earliest prescribe date, given category (double nested dictionary)
l_i, line, n_inc = 0, None, 100000

if os.path.exists("pnet_druglist.p"):
    print "load previously generated data.."
    [pins, first] = pickle.load(open("pnet_druglist.p"))
else:  
    # for each line of the data file
    while(True):
        line = f.readline()
        if not line:
            break
        w = line.strip().split(",")
        if len(w) != len(hdr):
            if w != ['']:
                print line
                print w
                print l_i
                err("unexpected # of fields")
            else:
                continue
        dinpin, my_id, srv_date = w[di].lstrip('0'), w[si].strip(), w[sdi].strip()
        if srv_date[4] != '-' or srv_date[7] != '-':
            err('unknown date fmt.: ' + srv_date)
        # for this record, check if the studyid is associated with the study population
        if my_id in studyid: # individual is in study pop
            if not my_id in pins:
                pins[my_id] = []
            for c in dinpin_lists:
                if dinpin in dinpin_lists[c]: # mh drug use: positive
                    pins[my_id].append(dinpin)
                    if not my_id in first:
                        first[my_id] = {}
                    if not c in first[my_id]:
                        first[my_id][c] = srv_date
                    if srv_date < first[my_id][c]:
                        first[my_id][c] = srv_date
        # progress bar
        if l_i > n_inc and l_i % n_inc == 0:
            print "%", 100. * (float(f.tell()) / float(pnet_file_size))
        l_i += 1 # next line
    pickle.dump([pins, first], open("pnet_druglist.p", "wb"))

print "\nstudyid, {first mh-drug category : prescribe date}"
for p in first:
    if p in first: print str(p) + "," + str(first[p])

# top 3 MH-related dinpin
# ever prescribed MH drug (any category)
# first prescribe date each category

categories = dinpin_lists.keys()
output_file_name = cohort_file  + "_mh_drugs.csv"
f = open(output_file_name, "wb")
f.write("studyid," +
        "mh_top3_1_din,mh_top3_2_din,mh_top3_3_din," +
        "mh_top3_1_atc,mh_top3_2_atc,mh_top3_3_atc," +
        "mh_top3_1_str,mh_top3_2_str,mh_top3_3_str," +
        (",".join([(c + "_ever") for c in categories])) + "," +
        (",".join([c + "_firstdate" for c in categories])) + "," +
        "mh_drug_ever," +
        "hp.drug_brand_nm_top3_1,hp.drug_brand_nm_top3_2,hp.drug_brand_nm_top3_3," + 
        "hp.gen_drug_top3_1,hp.gen_drug_top3_2,hp.gen_drug_top3_3\n")

a, b = load_hlth_labels()

for p in pins:
    s, t3, mh_ever = str(p), top3(pins[p]), False
    for i in range(0, 3):
        s += "," + (t3[i] if t3[i]!="None" else "NA")

    for i in range(0, 3):
        s += "," + (reverse_lookup[t3[i]] if (t3[i]!="None" and t3[i] in reverse_lookup) else "NA")

    for i in range(0, 3):
        s += "," + (atc_codes[reverse_lookup[t3[i]]] if (t3[i]!="None" and t3[i] in reverse_lookup and reverse_lookup[t3[i]] in atc_codes) else "NA")
    '''
    s += "," + (t3[0] if t3[0]!="None" else "NA")
    s += "," + (t3[1] if t3[1]!="None" else "NA")
    s += "," + (t3[2] if t3[2]!="None" else "NA")
    '''
    for c in categories:
        x = ""
        if p not in first:
            x = "NO"
        else:
            if c not in first[p]:
                x = "NO"
            else:
                x = "YES"
                mh_ever = True
        s += ("," + x)

    for c in categories:
        x = ""
        if p not in first:
            x = "NA"
        else:
            if c not in first[p]:
                x = "NA"
            else:
                x = str(first[p][c])
        s += "," + x
    s += "," + ("YES" if mh_ever else "NO")

    # finally write out some human-readable drug labels (For the "top 3")
    for i in range(0, 3):
        s += "," + (a[t3[i]] if (t3[i] in a) else "NA")
    for i in range(0, 3):
        s += "," + (b[t3[i]] if (t3[i] in b) else "NA")
    '''    
    s += "," + a[t3[0]]
    s += "," + a[t3[1]]
    s += "," + a[t3[2]]
    s += "," + b[t3[0]]
    s += "," + b[t3[1]]
    s += "," + b[t3[2]]
    '''
    f.write(s + "\n")
f.close()
print "number of studyid:", len(pins.keys())
print "output file name:", output_file_name
