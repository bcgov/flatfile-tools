#include<iostream>
#include<fstream>
#include"stdlib.h"
#include"stdio.h"
#include<string>
/* oddly, python jumps out of the unix terminal. C++ does not! This hook brings python back into the place where we can have nice things */
using namespace std;
int main(int argc, char** argv){
        system((string("gunzip ") + string(argv[1])).c_str());
        return 0;
}