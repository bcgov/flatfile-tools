@echo off

R:
chdir R:\%USERNAME%\bin\
cp R:\%USERNAME%\bin\bash_profile C:\cygwin64\home\%USERNAME%\.bash_profile

C:
chdir C:\cygwin64\bin
bash --login -i
