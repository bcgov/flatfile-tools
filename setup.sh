# 20190711
#echo setting up environment..
#source bash_profile
echo building command list..
/cygdrive/c/Program\ Files/Python35/python.exe setup.py > /dev/null
echo compiling multicore compiler..
g++ -w -O3 -o multicore.exe  wrap-py_multicore.cpp; rm -f wrap-py_multicore.cpp; icacls multicore.exe /grant everyone:F # compile_multicore.sh
echo compiling utilities..
multicore compile_jobs.sh
echo setting permissions..
icacls  .   /grant everyone:F  /t > /dev/null
echo done