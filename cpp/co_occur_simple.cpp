//---------------------------------------------------------------------------//
// co_occur_simple.cpp: co-occurence for bivariate table (first 2 cols)
// date: 20190120
// info: input table assumed CSV with header: rows unique obs
//---------------------------------------------------------------------------//
#include<map>
#include"misc.h"
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
using namespace std;

/* statistical co-occurence */
map<string, long unsigned int> counts;

/* count string occurrences */
void incr(string label){

  /* are we tracking this label yet? */
  map<string, long unsigned int>::iterator ci = counts.find(label);

  if(ci == counts.end()){
    /* if not, init count */
    counts[label] = 1;
  }
  else{
    /* increase count */
    counts[label] += 1;
  }
}

int main(int argc, char ** argv){
  string filename(argv[1]);
  ifstream infile(filename);
  if(!infile.is_open()){
    err(string("could not open file: ") + filename);
  }
  string line;
  long unsigned int ci = 0;

  /* process the file line by line */
  while(getline(infile, line)){
    ci += 1;
    if(ci==1) continue;

    /* split csv line into separate fields */
    vector<string> w(split(line, ','));

    /* count individual codes as well as joint-codes */
    incr(w[0]); //first field
    incr(w[1]); //second field
    incr(w[0] + string(",") + w[1]); // define joint-occurrence code and count that
  }

  /* output string counts */
  for(map<string, unsigned long int>::iterator i=counts.begin(); i!=counts.end(); i++){
    if(i != counts.begin()) cout << endl;
    cout << i->first << "," << i->second;
  }

  return 0;
}