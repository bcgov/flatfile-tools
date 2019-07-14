# examine data dictionary cleaned files, after processing by:
#   1) dd_list.py
#   2) dd_apply.py
#   3) dd_clean.py
#   4) dd_examine.py

import os
import sys

fn = os.popen('ls -1 ./dd/*.csv2')

for f in fn:
    f = f.strip()
    a = os.system('vi ' + f)