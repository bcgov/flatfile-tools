import string
import os

def tv(c):
    return [c.isalpha(), c.isupper(), c in string.punctuation, c.isspace()]

def eq(x, y):
    if len(x) != len(y):
        return False
    for i in range(0, len(x)):
        if x[i] != y[i]:
            return False
    return True

def fuzzy_chunks(filename):
    chunks = []
    starti, stopi = 0, 0
    t_last = tv(filename[starti])
    for i in range(0, len(filename)):
        c = filename[i]
        if i == len(filename) - 1:
            chunk = filename[starti:]
            if chunk != ".":
                chunks.append(chunk)
        if not eq(tv(c), t_last):
            stopi  = i
            chunk = filename[starti:stopi]
            if chunk != ".":
                chunks.append(chunk)
            starti, stopi = i, i + 1
        t_last = tv(c)
    return [c.strip().lower() for c in chunks]

def fuzzy_filename_match_i(filename, filenames_to_match):
    filename = filename.split("/")[-1]

    high_score, high_score_i = 0, -1
    ch = fuzzy_chunks(filename)

    for i in range(0, len(filenames_to_match)):
        score = 0
        fn = filenames_to_match[i].split("/")[-1]
        ch_i = fuzzy_chunks(fn)
        for c in ch:
            if c in ch_i:
                score += 1
        if score > high_score:
            high_score, high_score_i = score, i

    if score > 0:
        "MATCH", filename, fn
        return high_score_i, high_score  #, high_score#filenames_to_match[high_score_i]
    else:
        return None

a = os.system("rm -f *exclude*") # remove this line
print fuzzy_filename_match_i('rpblite1995.C.dat', ['./dd/2019-04-08_data_dictionary_consolidation-file-january-1-1986-onwards.xlsx_registry.D.csv2', './dd/2019-04-08_data_dictionary_consolidation-file-january-1-1986-onwards.xlsx_rpblite.D.csv2'])