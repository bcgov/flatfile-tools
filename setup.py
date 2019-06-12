#!/usr/bin/env python2.7
import os
import sys
import py.misc

# need to be able to detect python, R, and vim (if exist)
# need to do the wrapping (as we did for py) for R programs, too!
# CHECK ON writing (and executing) the bash_profile..
# 20190401 this program now compatible with python3

def err(m):
    print("Error: " + m)
    sys.exit(1)

sys_s = os.popen("uname -a").read().strip().lower()
sys_n = None

if sys_s[0:6] == 'cygwin':
    sys_n = 'cygwin'
elif sys_s[0:5] == 'mingw':
    sys_n = 'mingw'
elif sys_s[0:5] == 'linux':
    sys_n = 'linux'
else:
    err("system type: " + sys_s.split()[0] + " not yet supported")

file_dir = os.path.dirname(os.path.abspath(os.path.realpath(__file__)))  # where the file is
pwd = os.popen("pwd").read().strip()

if sys_n == "cygwin":
    pwd = os.popen("cygpath -w -a ./").read().strip()

work_dir = os.path.abspath(pwd)

if not file_dir == work_dir:
    err("file dir: " + file_dir + "\nwork_dir: " + work_dir + "\nmust run setup.py from the directory it lives in")

build = None
if len(sys.argv) > 1:
    build = sys.argv[1]

user_name = os.popen("whoami").read().strip()

def err(m):
    print("Error: " + m)
    sys.exit(1)

def run(c):
    print('running:' + str(c))
    a = os.system(c)
    if(a!=0):
        err("command failed: " + c)


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
    ext = f.strip().split('.')[-1].strip()
    fn = f.split('/')[-1].strip().split('.')[0]

    # don't compile helper funcs as a binary; no main()
    if fn == "misc" or fn == "__init__" or fn == "wrap_py":
        return

    if not fn in compiled:
        compiled[fn] = True
    else:
        return
    if ext == 'c' or ext == 'cpp':
        cmd = 'g++ -w -O4 -march=native -flto -o ' + fn + '.exe ' + f
        if ext == 'cpp':
            cmd += ' cpp/misc.cpp'
        cmd += ' -lopengl32 -lglu32 -lgdi32'
        if (not build) or (fn == build):
            print(cmd)
            a = os.system(cmd)

    if ext == 'py':
        if build != None:
            if build != fn:
                return

        # for py files, write a cpp wrapper so we can call from bin folder
        cf = open('wrap_py.cpp', 'wb')
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
        cmd = 'g++ -w -O3 -o ' + fn + '.exe ' + ' wrap_py.cpp '
        print(cmd)
        a = os.system(cmd)
        a = os.system("rm -f wrap_py.cpp")
    cmd = "icacls " + fn + ".exe /grant everyone:F"
    a = os.popen(cmd).read()

# find source files and compile
cpp = os.popen('find ./ -name "*.cpp"').read().strip().split('\n')
py = os.popen('find ./ -name "*.py"').read().strip().split('\n')
c = os.popen('find ./ -name "*.c"').read().strip().split('\n')

ci = 1
n = len(cpp) + len(py) + len(c)

for f in py:
    if build is None:
        sys.stdout.write(str(ci) + "/" + str(n) + " ")
    compile(f)
    ci += 1

for f in c:
    if build is None:
        sys.stdout.write(str(ci) + "/" + str(n) + " ")
    compile(f)
    ci += 1

for f in cpp:
    if build is None:
        sys.stdout.write(str(ci) + "/" + str(n) + " ")
    compile(f)
    ci += 1