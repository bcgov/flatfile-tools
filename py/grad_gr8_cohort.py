# 20190430 version; grad concept: > grad 1 psy_pssg_cohort_20190423.csv_unnest.csv 
import os, sys, math, pickle, datetime
from misc import load_fields, assert_exists, err

if not os.path.exists("dd") or not os.path.isdir("dd"):
    err("expected data dictionaries (cleaned, csv) in folder ./dd/")

# data dictionary file
dd_schlstud, dd_studcrd = "dd/data_dictionary_education.xlsx_schlstud.csv2", "dd/data_dictionary_education.xlsx_studcrd.csv2"
for f in [dd_schlstud, dd_studcrd]:
    assert_exists(f)

cohort_file = sys.argv[1] #'youth_cohort.csv' # delete *.p files and other intermediary files if cohort changes
schlstud_file = 'idomed1991-2017.ft_schlstud.A.dat_dd_sliceapply.csv'
studcrd_file = 'idomed1991-2017.ft_studcrd.A.dat_dd_sliceapply.csv'
files = [cohort_file, schlstud_file, studcrd_file]

# prepare school student file, if not yet
if not os.path.exists(schlstud_file):
    schlstud_datfile = "dat/idomed1991-2017.ft_schlstud.A.dat"; assert_exists(schlstud_datfile)
    slice_fields = "birthdate_yymm school_year age_group_jun_30 age_in_years_jun_30 age_group_dec_31 age_in_years_dec_31 school_postal_code grade_this_enrol studyid"
    a = os.system("dd_slice_apply " + dd_schlstud + " " + schlstud_datfile + " "  + slice_fields)
    assert_exists(schlstud_datfile + "_dd_sliceapply.csv")
    a = os.system("mv " + schlstud_datfile + "_dd_sliceapply.csv" + " " + schlstud_file)

# prepare student credit data, if not yet
if not os.path.exists(studcrd_file):
    studcrd_datfile = "dat/idomed1991-2017.ft_studcrd.A.dat"; assert_exists(studcrd_datfile)
    slice_fields = "credential_cnt trax_school_year sp_need_perf_grp_lst_knwn_coll sp_need_code_lst_knwn_coll credential_name studyid"
    a = os.system("dd_slice_apply " + dd_studcrd + " " + studcrd_datfile + " " + slice_fields)
    assert_exists(studcrd_datfile + "_dd_sliceapply.csv")
    a = os.system("mv " + studcrd_datfile + "_dd_sliceapply.csv" + " " + studcrd_file)

print("loading..")
for f in files: assert_exists(f) # check for files

# extract studyid
cohort_id_file = cohort_file + "_studyid"
a = os.system("csv_split " + cohort_file) if not os.path.exists(cohort_id_file) else None
assert_exists(cohort_id_file) # make sure we got the result

# load filtered student credit table
dat_cohort, datf_cohort = None, None
if not os.path.exists('dat_cohort.p'):
    dat_cohort, datf_cohort = load_fields([cohort_file, 'dob'])
    pickle.dump([dat_cohort, datf_cohort], open('dat_cohort.p', 'wb'))
else:
    dat_cohort, datf_cohort = pickle.load(open('dat_cohort.p', 'rb'))
studyid, dob = list(dat_cohort.keys()), {}
fdat_cohort = {datf_cohort[i] : i for i in range(0, len(datf_cohort))}

# express dob as a function of studyid
for i in studyid:
    dob[i] = dat_cohort[i][fdat_cohort['dob']][0]

def filter_table_for_cohort(cohort_id_file, table_file):
    select_file = table_file + "_select.csv"
    if not os.path.exists(select_file):
        a = os.system("csv_select " + cohort_id_file + " studyid " + table_file)
    assert_exists(select_file)
    return select_file

# filter tables for cohort
schlstud_select_file, studcrd_select_file = [filter_table_for_cohort(cohort_id_file, schlstud_file), filter_table_for_cohort(cohort_id_file, studcrd_file)]
# load filtered school student table
dat_schlstud, datf_schlstud = None, None
if not os.path.exists('dat_schlstud.p'):
    print("schlstud_select_file", schlstud_select_file)
    dat_schlstud, datf_schlstud = load_fields([schlstud_select_file, 'school_year', 'age_group_jun_30', 'birthdate_yymm', 'age_in_years_jun_30', 'grade_this_enrol'])
    pickle.dump([dat_schlstud, datf_schlstud], open('dat_schlstud.p', 'wb'))
else:
    dat_schlstud, datf_schlstud = pickle.load(open('dat_schlstud.p', 'rb'))

# load filtered student credit table
dat_studcrd, datf_studcrd = None, None
if not os.path.exists('dat_studcrd.p'):
    dat_studcrd, datf_studcrd = load_fields([studcrd_select_file, 'credential_cnt', 'trax_school_year', 'credential_name'])
    pickle.dump([dat_studcrd, datf_studcrd], open('dat_studcrd.p', 'wb'))
else:
    dat_studcrd, datf_studcrd = pickle.load(open('dat_studcrd.p', 'rb'))

if True:
    print("calculating some stats..")
    def p_avg(dat, name):
        avg = 0.
        for i in dat: avg += float(len(dat[i]))
        print(str(avg / float(len(dat))) + " " + name + " records per id, that table")

    p_avg(dat_schlstud, "schlstud")
    p_avg(dat_studcrd, "studcrd")

    def had(dat, studyid, name):
        had = 0.
        for i in studyid:
            if i in dat: had += 1.
        print(str(100. * had / float(len(studyid))) + " % of cohort, had " + name)
    
    had(dat_schlstud, studyid, "schlstud")
    had(dat_studcrd, studyid, "studcrd")

fdat_studcrd = {datf_studcrd[i] : i for i in range(0, len(datf_studcrd))}
fdat_schlstud = {datf_schlstud[i] : i for i in range(0, len(datf_schlstud))}
print("studcrd", fdat_studcrd)
print("schlstud", fdat_schlstud)

code = {}
for i in studyid: # for every studyid (call it "i")

    gr8_year, grad_year = None, None

    if i in dat_studcrd:
        for dataline in dat_studcrd[i]:
            credential_cnt = dataline[fdat_studcrd['credential_cnt']]
            trax_school_year = dataline[fdat_studcrd['trax_school_year']]
            credential_name = dataline[fdat_studcrd['credential_name']]
            # print credential_cnt, trax_school_year, credential_name
            if credential_cnt != '1':
                err('credential_cnt != 1')
            trax_school_year = int(trax_school_year.split('/')[0])
            # take the first grad year
            if grad_year is None:
                grad_year = trax_school_year
            else:
                if trax_school_year < grad_year:
                    grad_year = trax_school_year


    if i in dat_schlstud:
        for dataline in dat_schlstud[i]:
            school_year = dataline[fdat_schlstud['school_year']]
            school_year = int(school_year.split('/')[0])
            grade_this_enrol = dataline[fdat_schlstud['grade_this_enrol']]
            try:
                grade_this_enrol = int(grade_this_enrol)
            except:
                pass
            #print school_year, grade_this_enrol
            if grade_this_enrol == 8:
                gr8_year = school_year

    if gr8_year is None:
        code[i] = "N/A"
    else:
        if grad_year is None:
            code[i] = "NO_0"
        else:
            code[i] = "YES" if grad_year - gr8_year <= 4 else "NO_1"      
    #print gr8_year, grad_year, "" if (grad_year is None or gr8_year is None) else (grad_year - gr8_year)
   

lines = ["studyid,grad_output4_short, grad_output4"]
for i in studyid:
	w = code[i].split("_")
	code_short = code[i] if len(w) ==1 else w[0]
	lines.append(str(i) + "," + code_short + "," + str(code[i]))

out_fn = "grad_output4.csv"
open(out_fn, "wb").write(('\n'.join(lines)).encode())

# generate frequency table
a = os.system("count " + out_fn)
a = os.system("cat " + out_fn + "_frequ")
