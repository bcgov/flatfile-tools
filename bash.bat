@echo off
R:
chdir R:\%USERNAME%\bin\
C:
cp R:\%USERNAME%\bin\bash_profile C:\cygwin64\home\%USERNAME%\.bash_profile
chdir C:\cygwin64\bin
bash --login -i
