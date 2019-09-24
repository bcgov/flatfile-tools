from misc import * # using a module

if len(args) < 2:
    err("pnet_get [cohort file name, 1-col csv with header]")

cohort_file = args[1]

files = ['dsp_rpt.dat',
        'clm_rpt_1995_2000.dat',
        'clm_rpt_2001_2005.dat',
        'clm_rpt_2006_2010.dat',
        'clm_rpt_2011_2014.dat',
        'clm_rpt_2015_2018.dat']

run("dd_list") # find and clean all data dictionaries

h_file = 'hlth_prod_final.dat'
if not exists(h_file + '_dd_sliceapply.csv'):
    run('df_get ' + h_file)
    run('dd_sliceapply_all ' + h_file)

csv = []

for f in files:
    csv.append(f + '_dd_sliceapply_cohort.csv')

    if not exists(csv[-1]):
        run("df_get " + f)
        run("dd_sliceapply_all " + f + " " + cohort_file)

        # clean up files
        run("rm -f *.dat *.gz")

        # data cleaning
        run('pnet_check ' + csv[-1])

run("csv_cat_common_fields " + " ".join([cf + '_clean' for cf in csv]))
run("mv csv_cat_common_fields.csv pnet.csv")