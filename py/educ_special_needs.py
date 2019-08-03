from misc import *

if len(args) < 2:
    err("generate special needs cols from data\n\teduc_special_needs [cohort file]")

cohort_file = args[1]

data_file = 'idomed1991-2017.ft_schlstud.A.dat'
# get data dictionaries
run("dd_list")

# fetch and unzip data file
if not exists(data_file):
    run('df_get ' + data_file)

fields = ['spec_need_perf_grp_this_coll',
          'special_need_group_this_coll',
          'special_need_code_this_coll',
          'special_need_this_coll',
          'graduation_course_count',
          'other_course_count',
          'french_imm_this_coll_flag',
          'french_imm_in_year_flag',
          'esl_this_coll_flag',
          'esl_in_year_flag',
          'gender',
          'studyid']

# result from conversion to csv 
csv_file = data_file + '_dd_sliceapply_cohort.csv'

if not exists(csv_file):
    run('dd_sliceapply_all ' + data_file + ' ' + cohort_file) 

sliced_csv = csv_file + '_slice.csv'
# slice out the desired fields
if not exists(sliced_csv):
    run('csv_slice ' + csv_file + ' ' + ' '.join(fields))

mode_file = sliced_csv + '_flatten.csv'
amde_file = sliced_csv + '_revmod_flatten.csv'

# find mode (by studyid)
if not exists(mode_file):
    run('csv_mode_flatten ' + sliced_csv)

# find antimode (by studyid)
if not exists(amde_file):
    run('csv_mode_flatten ' + sliced_csv + ' 1')

join_file = mode_file + '_join.csv'
if not exists(join_file):
    run('simple_join ' + mode_file + ' ' + amde_file)

filter_file = join_file + '_filter_education.csv'
if not exists(filter_file):
    run('educ_special_need_ever ' + join_file)


# check for the right col's,

# do the sorting on the codes. 

# write out the stuff

