# 20190706 find common fields between two data dictionaries
import os
import sys

if not os.path.exists("dd/2018-10-02_data_dictionary_pharmanet-january-1-1996-onwards.xlsx_clm_rpt.A.csv2"):
    print("expected certain data dictionary. Need to modify program for more generality")
    sys.exit(1)

dd_clm = os.popen("dd_fields dd/2018-10-02_data_dictionary_pharmanet-january-1-1996-onwards.xlsx_clm_rpt.A.csv2").read().strip()
dd_dsp = os.popen("dd_fields dd/2018-10-02_data_dictionary_pharmanet-january-1-1996-onwards.xlsx_dsp_rpt.A.csv2").read().strip()

dd_clm = dd_clm.split("\n")
dd_dsp = dd_dsp.split("\n")

clm_fields = dd_clm[-1].split()
dsp_fields = dd_dsp[-1].split()

clm_fields = [x.strip().split(".")[-1].lower() for x in clm_fields]
dsp_fields = [x.strip().split(".")[-1].lower() for x in dsp_fields]

print(clm_fields)
print(dsp_fields)

common = []
for f in dsp_fields:
    if (f.lower() != 'linefeed'):
        if (f.lower() in clm_fields): common.append(f)
        else: print("*** in file 2 but not in file 1: " + str(f))
for f in clm_fields:
    if (f.lower() != 'linefeed'):
        if (f.lower() in dsp_fields): pass
        else: print("*** in file 1 but not in file 2: " + str(f))

print("common fields:\n" + str(common))