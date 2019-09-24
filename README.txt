how to build/ run command-line utilities in SRE: (would be nice to have a
version that allows people to choose a version for the data)

1. copy this bin folder into your private folder (R:/$USER/bin/)
- copy the tar.gz file into your home folder, and extract it there...
    - this file, bash.bat, etc. should reside in R:/$USER/bin/...
       .. for example, if they're in R:/$USER/bin/bin/ the files.. 
 	.. would need to be moved up a level!

2. navigate there (R:/$USER/bin) in Windows Explorer (the file manager)
	- for example, if my user name was "bob", the place to go is:
		R:/bob/bin

3. double-click on bash.bat (bash) to enter the cygwin bash prompt (the
programs should be built automatically)


You can check if the utilities are working by typing (followed by pressing
return):

	csv_slice

If the programs built correctly you would see:

Error: usage:
        slice.exe [infile] [Field name] .. [Field name n]

4. How to find out where a particular script is:
at the terminal in the bin/ folder,

to find csv_slice:
  find ./ -name "csv_slice*"

in this instance the output was:
  ./cpp/csv_slice.cpp
  ./csv_slice.exe

the .cpp file is c++ code in the appropriate folder; the .exe file is the verb
deployed at the terminal
  - These utilities use a language-agnostic format where all scripts are
    represented as verbs available on the command-line


Notes:
How to find out your user name in linux / Cygwin prompt:

type (followed by return):
	
	whoami

For example, if my user name was bob, the terminal should come back and say:

bob 

Example uses:

1. Opening and unpacking all the data for a cohort (may be slow and require
some manual intervention):

	a) copy a cohort file to the tmp folder:
		cp /cygdrive/r/.../cohort.csv .

	b) type: ls and press return to see the file is there
	c) slice out the studyid field (it's a terminal command so have to
press return)
		csv_slice studyid cohort.csv

	d) examine the first 10 lines of the result file:
		head -10 cohort.csv_slice.csv

	e) move the studyid-only file to a simpler filename:
		mv cohort.csv_slice.csv studyid.csv

	f) fetch and extract all data for a cohort:
	 	sup_cohort studyid.csv		
	

2. Downloading, fetching and unpacking the latest version of a specific data
file:

	a) find pharmanet data

		$ find /cygdrive/r/DATA/ -name "*pharmanet*"
		/cygdrive/r/DATA/2019-04-24/docs/data_dictionary_pharmanet-january-1-1996-onwards.xlsx
		/cygdrive/r/DATA/2019-04-24/pharmanet

	b) make a local copy of pharmanet files:
		pnet_get studyid.csv


3. Converting a "flat file" to csv:
	df_get hlth_prod_final.dat

	dd_sliceapply_all hlth_prod_final.dat

4. Removing whitespace characters from the end of a file (some programs could
interpret terminating newline character as a record)

	snip studyid.csv

5. concatenating pharmanet files
	already covered in 2. b)

6. checking pharmanet files for bad data
	
	pnet_check dsp_rpt.dat_dd_sliceapply.csv
		
	(bad data, if detected, will appear in a separate file)


7. getting mh drug usage from pnet:
 (note: this script should now download, fetch, unpack, clean, concatenate,
and analyze pharmanet data for a cohort-- no intervention required)
	pnet_druglist studyid.csv
