//---------------------------------------------------------------------------//
// csv_reversecols.cpp: reverse the col. order in a CSV
// date: 20190521
//---------------------------------------------------------------------------//
#include<map>
#include"misc.h"
#include<ctime>
using namespace std;

int main(int argc, char ** argv){

  if(argc < 2){
    err("reverse the column order of a CSV\nusage:\n\tcsv_reversecol [infile]");
  }
  string filename(argv[1]);
  cout << "input file: " << filename << endl;

  ifstream infile(filename);
  if(!infile.is_open()) err(string("failed to open file: ") + filename);

  string outfilename(filename + string("_reversecols.csv"));
  ofstream outfile(outfilename);
  if(!outfile.is_open()) err(string("failed to open file: ") + outfilename);
  cout << "output file: " << outfilename << endl;

  string line;
  unsigned long int ci = 0;

  /* process the file line by line */
  while(std::getline(infile, line)){

    /* split csv line into separate fields */
    vector<string> w(split(line, ','));
    std::reverse(w.begin(), w.end());

    string outline("");
    if(ci > 0) outline += "\n";
    for(vector<string>::iterator it = w.begin(); it != w.end(); it++){
      if(it != w.begin()) outline += ",";
      outline += (*it);
    }
    outfile << outline;
    ci += 1;
  }
  return 0;
}