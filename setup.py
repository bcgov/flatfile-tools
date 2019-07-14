#!/usr/bin/env python
import os
import sys
from py.misc import *
from py.ansicolor import *

job_file = open("compile_jobs.sh", "wb")

def add_job(cmd):
    job_file.write((cmd + "\n").encode())

# need to be able to detect python, R, and vim (if exist)
# need to do the wrapping (as we did for py) for R programs, too!
# CHECK ON writing (and executing) the bash_profile..
# 20190401 this program now compatible with python3

sys_s, sys_n = os.popen("uname -a").read().strip().lower(), None
if sys_s[0:6] == 'cygwin': sys_n = 'cygwin'
elif sys_s[0:5] == 'mingw': sys_n = 'mingw'
elif sys_s[0:5] == 'linux': sys_n = 'linux'
else: err("system type: " + sys_s.split()[0] + " not yet supported")

file_dir = os.path.dirname(os.path.abspath(os.path.realpath(__file__)))  # where the file is
pwd = os.popen("pwd").read().strip()

if sys_n == "cygwin": pwd = os.popen("cygpath -w -a ./").read().strip()

work_dir = os.path.abspath(pwd)
if not file_dir == work_dir: err("file dir: " + file_dir + "\nwork_dir: " + work_dir + "\nrun setup.py from the directory it's in")

build = sys.argv[1] if len(sys.argv) > 1 else None # if len(sys.argv) > 1: build = sys.argv[1]
user_name = os.popen("whoami").read().strip()

# need to actually adjust these for the python / R vim paths found..
profile = ['alias vim="/cygdrive/c/Program\ Files\ \(x86\)/Vim/vim81/vim.exe"',
           'alias Rscript="/cygdrive/c/Program\ Files/R/R-3.5.2/bin/Rscript.exe"',
           'alias python="/cygdrive/c/Program\ Files/Python35/python/" #alias python="/cygdrive/c/Python27/python.exe"',
           'export PATH="$PATH:$(pwd)"']
#try:
#    open('./.bash_profile','wb').write(('\n'.join(profile)).encode())
#    f_contents = open('./.bash_profile').read().strip()
#    os.popen('source ./.bash_profile')
#except:
#    print("warning: not sure if we could set environment variables")

compiled = {}

def compile(f):
    # print(KRED + f.strip('./') + KGRN)
    global build, compiled
    ext = f.strip().split('.')[-1].strip()
    fn = f.split('/')[-1].strip().split('.')[0]
    # print("fn", fn)
    if ext == 'py' and build is not None and build != fn: return

    # don't compile helper funcs as a binary; no main()
    if fn == "misc" or fn == "__init__" or fn == "wrap_py": return

    if not fn in compiled: compiled[fn] = True
    else: return

    # print(KRED + f.strip('./') + KGRN)

    cmds = ""
    if ext == 'c' or ext == 'cpp':
        cmd = 'g++ -w -O4 -march=native -flto -o ' + fn + '.exe ' + f
        if ext == 'cpp': cmd += ' cpp/misc.cpp'
        cmd += ' -lopengl32 -lglu32 -lgdi32'
        if (not build) or (fn == build):
            print('\t' + cmd)
            cmds += cmd # add_job(cmd) # a = os.system(cmd) # note: didn't use run because compiling might brea

    if ext == 'py':
        wrap_file = 'wrap-py_' + fn + '.cpp'
        # for py files, write a cpp wrapper so we can call from bin folder
        cf = open(wrap_file, 'wb') #'wrap_py.cpp', 'wb')
        lines = ['#include<stdlib.h>',
                 '#include<iostream>',
                 '#include<string>',
                 'using namespace std;',
                 'int main(int argc, char ** argv){',
                 #'  string cmd("/cygdrive/c/Program\\\\ Files/Python35/python.exe ");',
                 '  string cmd("/cygdrive/c/Python27/python.exe ");',
                 '  cmd += string("R:/' + os.popen("whoami").read().strip() + '/bin/py/' + fn + '.py");',
                 '  for(int i=1; i<argc; i++){',
                 '    cmd += string(" ") + string(argv[i]);',
                 '  }',
                 'std::cout << cmd << endl;',
                 'system(cmd.c_str());',
                 'return(0);',
                 '}']
        cf.write('\n'.join(lines).encode())
        cf.close()
        cmd = 'g++ -w -O3 -o ' + fn + '.exe ' + ' ' + wrap_file #wrap_py.cpp '
        print('\t' + cmd)
        cmds += cmd # a = os.system(cmd)
        cmd =  ("rm -f " + wrap_file) #wrap_py.cpp")
        cmds += "; " + cmd; # a = os.system(cmd)
        #if os.path.exists(fn + ".exe"):
        if(False):
            cmd = "icacls " + fn + ".exe /grant everyone:F"
            cmds += "; " + cmd # a = os.popen(cmd).read()

    add_job(cmds)

# find source files and compile
cpp = os.popen('find ./cpp/ -name "*.cpp"').read().strip().split('\n')
py = os.popen('find ./py/ -name "*.py"').read().strip().split('\n')
c = os.popen('find ./c/ -name "*.c"').read().strip().split('\n')
n = len(cpp) + len(py) + len(c)

for f in py: compile(f)
for f in c: compile(f)
for f in cpp: compile(f)

job_file.close()