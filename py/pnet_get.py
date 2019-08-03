from misc import *

cohort_file = args[1]

run("dd_list") # find, extract and clean all data dictionaries 

files = ['dsp_rpt.dat',
        'clm_rpt_1995_2000.dat',
        'clm_rpt_2001_2005.dat',
        'clm_rpt_2006_2010.dat',
        'clm_rpt_2011_2014.dat',
        'clm_rpt_2015_2018.dat']

# get one file
run('df_get ' + 'hlth_prod_final.dat')

# convert to csv
run('dd_sliceapply_all hlth_prod_final.dat')

# for each pnet file:
for f in files:
    # get the data zip and unzip it
    run("df_get " + f)

    # convert to csv (cohort data only)
    run("dd_sliceapply_all " + f + " " + cohort_file)
    
    # clean up files
    run("rm -f *.dat *.gz")

# make a list of the output csv files from the last step
csv_files = [f + '_dd_sliceapply.csv' for f in files]

# this program actually looks in all the different files for fields that are in common, and merges the data together (common fields only)
run("csv_cat_common_fields " + " ".join(csv_files))

