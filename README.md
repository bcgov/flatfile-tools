# csv
Scripts for effective "flat file" data wrangling; python, C/C++ and R syntax leveraged. Python3 now supported for building

# requirements
Presently the scripts are tested for Cywin ("linux for Windows" platform) with gnu g++ and python installed, under a Windows 10 environment. Build script is now updated for Python 3! Hope to support Mac OS, Windows/MinGW, Ubuntu soon

# Instructions
1. At a cygwin terminal (c:/cygwin64/Cygwin.bat) in Windows with gnu g++ and python installed, type:

>**source bash_profile**
to add the compiled code to the system path.

2) Type:

>**python setup.py**

to compile the programs.

Alternately, for example:

>**python setup.py dd_slice_apply**

could be used to compile a single program (in this case, dd_slice_apply.cpp)

## License

Copyright 2019 Province of British Columbia

Licensed under the Apache License, Version 2.0 (the "License");
you mayn't use these files except in compliance with the License.

You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0

Unless req'd by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
either express or implied.

See the License for specific language governing permissions
and limitations under the License.