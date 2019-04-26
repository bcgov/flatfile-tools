/* 20190226 unique.cpp: filter for unique last col. assert that the header for that col says "studyid" */
#include"misc.h"
using namespace std;

int main(int argc, char ** argv){
  if(argc < 2) err("usage: unique.cpp [input file]");

  string dfn(argv[1]);
  string ofn(dfn + string("_unique.csv"));

  cout << "data input file: " << dfn << endl;
  cout << "data output file: " << ofn << endl;

  ifstream dfile(dfn);
  if(!dfile.is_open()) err(string("failed to open input data file:") + dfn);

  ofstream outfile(ofn);
  if(!outfile.is_open()) err(string("failed to write-open file:") + ofn);

  string d;
  string line;
  vector<string> row;
  long unsigned int ci = 0;
  unsigned int col_index = 0;

  map<string, string> unique; 

  // in the future we should reimplement getline to read whole file into ram if can, or use ramless, different interleaves or latencies
  while(getline(dfile, line)){
    row = split(line, ',');
    if(ci == 0){
	    // in an ideal implementation, logic for first row appears outside of for loop
      col_index = row.size() - 1;
      d = row[col_index];
      trim(d);
      outfile << line << endl;
      lower(d);
      if(d != "studyid" && d != "study-id") err("last col field name studyid expected");
    }
    else{
	d = row[col_index];
	trim(d);
	if(unique.count(d) < 1) unique[d] = line;
    }
    ci ++;
  }
  dfile.close();

  cout << "outputting last unique lines..." << endl;
  for(map<string, string>::iterator it = unique.begin(); it != unique.end(); it++){
	outfile << it->second << endl; 
  }
  outfile.close();

  return 0;
}
