# 20190709 test script for csv_cat_common_fields.sh
mkdir -p test
rm -f csv_cat_common_fields.csv
# example use case, not test:
# csv_cat_common_fields dsp_rpt.dat_dd_sliceapply.csv clm_rpt_1995_2000.dat_dd_sliceapply.csv clm_rpt_2001_2005.dat_dd_sliceapply.csv clm_rpt_2006_2010.dat_dd_sliceapply.csv clm_rpt_2011_2014.dat_dd_sliceapply.csv clm_rpt_2015_2018.dat_dd_sliceapply.csv

head -5 clm_rpt_1995_2000.dat_dd_sliceapply.csv > test/1.csv
head -5 clm_rpt_2001_2005.dat_dd_sliceapply.csv > test/2.csv
head -5 clm_rpt_2006_2010.dat_dd_sliceapply.csv > test/3.csv
head -5 clm_rpt_2011_2014.dat_dd_sliceapply.csv > test/4.csv
head -5 clm_rpt_2015_2018.dat_dd_sliceapply.csv > test/5.csv
head -5 dsp_rpt.dat_dd_sliceapply.csv > test/6.csv

# run test case
cd ./test
csv_cat_common_fields 6.csv 1.csv 2.csv 3.csv 4.csv 5.csv
cd ..