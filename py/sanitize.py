# 20190421 make sure code in present directory doesn't have any potential studyid's
import os
import sys

a = os.popen('find ./ -name "*.*"  | xargs grep -n s[0-9][0-9]').read().strip().split("\n")
for i in a:
    print(i.strip())