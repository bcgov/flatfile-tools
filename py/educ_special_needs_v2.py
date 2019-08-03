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

'''special_need_code_this_coll
R,M,N,H
'''

if not exists("slice.csv"):
    run("csv_slice idomed1991-2017.ft_schlstud.A.dat_dd_sliceapply_cohort.csv spec_need_perf_grp_this_coll special_need_group_this_coll special_need_code_this_coll special_need_this_coll studyid")
    run("idomed1991-2017.ft_schlstud.A.dat_dd_sliceapply_cohort.csv_slice.csv slice.csv")

f = open("slice.csv")
hdr = f.readline().strip().lower().split(",")
print "fields", hdr
f_i = {hdr[i]: i for i in range(0, len(hdr))}

print f_i
c0 = f_i['spec_need_perf_grp_this_coll']
c1 = f_i['special_need_group_this_coll']
c2 = f_i['special_need_code_this_coll']
c3 = f_i['special_need_this_coll']
studyid_i = f_i['studyid']


# 'spec_need_perf_grp_this_coll,special_need_group_this_coll,special_need_code_this_coll,special_need_this_coll'

special_codes = ['r', 'm', 'n', 'h']
'''
r - moderate behaviour support / mental illness
h - intensive behaviour intervention / serious mental illness
m - behaviour disorder: moderate
n - behavoiur disorder: rehabilitation
'''


special_needs_flag_ever = {} # studyid 
mental_health_flag_ever = {}
gifted_flag_ever = {}

col0_counts = {}
col1_counts = {}
col2_counts = {}
col3_counts = {}

while True:
    line = f.readline()
    if not line: break

    w = line.strip().lower().split(',')
    try:
        studyid = w[studyid_i]
    except:
        print "line", line
        print w
        err("err")

    if not studyid in special_needs_flag_ever:
        special_needs_flag_ever[studyid] = False

    if not studyid in mental_health_flag_ever:
        mental_health_flag_ever[studyid] = {}
 
    if w[c2] in special_codes:
        special_needs_flag_ever[studyid] = True

        mh_code = w[c2]
        if not mh_code in mental_health_flag_ever[studyid]:
            mental_health_flag_ever[studyid][mh_code] = 0
        mental_health_flag_ever[studyid][mh_code] += 1

    if not(w[c0][0:3] == 'non' or w[c2][0:3] == 'non' or w[c3][0:3] == 'non'):
        special_needs_flag_ever[studyid] = True

    if not studyid in gifted_flag_ever:
        gifted_flag_ever[studyid] = False

    if w[c0] == 'gifted' or w[c3] == 'gifted':
        gifted_flag_ever[studyid] = True
    
    if studyid not in col0_counts: col0_counts[studyid] = {}
    if studyid not in col1_counts: col1_counts[studyid] = {}
    if studyid not in col2_counts: col2_counts[studyid] = {}
    if studyid not in col3_counts: col3_counts[studyid] = {}

    if w[c0] not in col0_counts[studyid]:
        col0_counts[studyid][w[c0]] = 0
    col0_counts[studyid][w[c0]] += 1

    if w[c1] not in col1_counts[studyid]:
        col1_counts[studyid][w[c1]] = 0
    col1_counts[studyid][w[c1]] += 1

    if w[c2] not in col2_counts[studyid]:
        col2_counts[studyid][w[c2]] = 0
    col2_counts[studyid][w[c2]] += 1

    if w[c3] not in col3_counts[studyid]:
        col3_counts[studyid][w[c3]] = 0
    col3_counts[studyid][w[c3]] += 1

def mode_special(s, col_counts, numeric = False):
    #print s, "mode_special", col_counts[s], numeric
    if s not in col_counts:
        return 'NA'

    max_code = 'NA'
    max_count = 0
    for c in col_counts[s].keys():
        #print "\t", c, col_counts[s][c]
        non_special = False
        if numeric:
            if c == '5':
                non_special = True
        else:
            if c[0:3] == 'non':
                non_special = True
        #print "non_special=", non_special
        if not non_special:
            if col_counts[s][c] > max_count:
                max_code, max_count = c, col_counts[s][c]
    return max_code
 

        
print "studyid,special_needs_ever,mental_health_flag_ever,r_flag_ever,h_flag_ever,m_flag_ever,n_flag_ever,gifted_ever,spec_need_perf_grp_this_coll_special_mode,pscial_need_group_this_coll_special_mode,special_need_code_this_coll_special_mode,special_need_this_coll_special_mode"
for s in special_needs_flag_ever:

    col0_special_most_frequent = mode_special(s, col0_counts)
    col1_special_most_frequent = mode_special(s, col1_counts, True)
    col2_special_most_frequent = mode_special(s, col2_counts)
    col3_special_most_frequent = mode_special(s, col3_counts)

    output = [str(s),
              'Yes' if special_needs_flag_ever[s] else 'No',
              'Yes' if len(mental_health_flag_ever[s].keys()) > 0 else 'No',
              'Yes' if 'r' in mental_health_flag_ever[s] else 'No',
              'Yes' if 'h' in mental_health_flag_ever[s] else 'No',
              'Yes' if 'm' in mental_health_flag_ever[s] else 'No',
              'Yes' if 'n' in mental_health_flag_ever[s] else 'No',        
              'Yes' if gifted_flag_ever[s] else 'No',
    col0_special_most_frequent,
    col1_special_most_frequent,
    col2_special_most_frequent,
    col3_special_most_frequent
              ]
            
    print ",".join(output)
