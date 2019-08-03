from misc import *
csv = os.popen("ls -1 *.csv").readlines()

n_d_files = 0
n_records = 0
n_headers = 0

next_ri = 0
records = {}
records_i = {}

next_fi = 0
d_files = {}
d_files_i = {}

next_hi = 0
headers = {}
headers_i = {}

ri_to_hi = {}
ri_to_dfi = {}

for c in csv:
    fn = c.strip()
    if fn == "":
        continue

    n_d_files += 1
    
    # use data file name as table name
    dfn = fn.split(".dat")[0] + ".dat"

    # assign index to table
    if dfn not in d_files:
        d_files[dfn] = next_fi
        next_fi += 1

    fi = d_files[dfn]
    d_files_i[fi] = dfn

    # assign index to record

    lines = open(fn).read().strip().lower().split("\n")
    lines = [lines[i].strip() for i in range(0, len(lines))]

    if len(lines) > 1:
        print len(lines), fn, dfn
        hdr = lines[0]
        n_headers += 1

        if hdr not in headers:
            headers[hdr] = next_hi
        next_hi += 1
        hdr_i = headers[hdr] # map the record onto this
        headers_i[hdr_i] = hdr


        for i in range(1, len(lines)):
            n_records += 1
            record = lines[i]
            if record not in records:
                records[record] = next_ri
                next_ri += 1
            record_i = records[record]
            ri_to_hi[record_i] = hdr_i
            records_i[record_i] = record
            ri_to_dfi[record_i] = fi

# print "n_records_unique", next_ri, "n_records", n_records
# print "n_headers_unique", next_hi, "n_headers", n_headers
# print "n_datafil_unique", next_fi, "n_datafil", n_d_files


# also group by studyid

for i in range(0, next_ri):
    record = records_i[i]
    hdr = headers_i[ri_to_hi[i]]
    print i, d_files_i[ri_to_dfi[i]]
    wh = hdr.split(",")
    wr = record.split(",")

    if len(wh) != len(wr):
        #print "record", record
        # err("wh " + str(wh) + " wr " + str(wr))
        in_quotes = False
        chars = list(record)
        for j in range(0, len(record)):
            if chars[j] == '"':
                in_quotes = not in_quotes
            else:
                if in_quotes and chars[j] == ',':
                    chars[j] = ';'
        record = ''.join(chars)
        wr = record.split(",")

        if len(wh) != len(wr):
            print hdr
            print "record", record
            err("failed to remediate record")


    #print "\t", hdr
    #print "\t", record
    d = {}
    for j in range(0, len(wh)):
        float_zero = False
        try:
            if(float(wr[j]) == 0.):
                float_zero = True
        except:
            pass # not a number

        if wr[j] == '' or float_zero or wr[j] == 'unspecified' or wr[j] == 'not applicable':
            pass
        else:
            d[wh[j]] = wr[j]

    yyyy_mm, dd = "", ""
    if 'c_dobyyyy' in d and 'c_dobmm' in d:
        yyyy_mm = d['c_dobyyyy'] + d['c_dobmm']
        print yyyy_mm + dd +  " " + str(d)
    elif 'servdate' in d:
        servdate = d['servdate']
        yyyy_mm = servdate[0:6]
        dd = servdate[6:9]
        


    '''

for record in records:
    record_i = records[record]
    hdr_i = ri_to_hi[record_i]
    hdr = headers_i[hdr_i]
    print record_i, hdr, record


    '''
