//---------------------------------------------------------------------------//
// co_occur.cpp: co-occurence for multi-variate table
// date: 20190120 
// info: input table given as cmd. arg: file assumed CSV: rows unique obs 
//---------------------------------------------------------------------------//
#include<map>
#include<set>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include"misc.h"
#include"n_choose_m.h"

using namespace std;

/* statistical co-occurence */
std::map<std::string, long unsigned int> counts;

/* count string occurrences */
void incr(std::string label){

  /* are we tracking this label yet? */
  std::map<std::string, long unsigned int>::iterator ci = counts.find(label);

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
  int s_s = 3; // selection size;
  std::string filename(argv[1]);
  std::ifstream infile(filename);
  std::string line;

  long int ci = 0; // line count
  long int n_f = 0;
  vector<string> fields;

  set<string> select;
  if(argc >= 3){
    for(int i=2; i < argc; i++){
      select.insert(argv[i]);
    }
  }
  cout << select << endl;

  vector<int> select_i;

  /* process the file line by line */

  vector<vector<int>> combs;
  vector<vector<int>>::iterator it;
  for(it= combs.begin(); it!=combs.end(); it++){
    cout << *it << endl;
  }
  while(std::getline(infile, line)){

    /* split csv line into separate fields */
    std::vector<std::string> w(split(line, ','));

    if(ci == 0){
      fields = w;
      n_f = w.size();
      cout << select << endl;

      if(select.size()>0){
        for(int i=0; i<n_f; i++){
          if(select.count(fields[i]) >0){
            select_i.push_back(i);
          }
        }

        combs = n_choose_up_to_m(select_i.size(),s_s); // fill this in..
      }
      else{
        combs = n_choose_up_to_m(n_f,s_s); // fill this in..
      }
    }
    else{
      if(w.size() != n_f){
        cout << "Error: " << std::endl; return -1;
      }
    }
    vector<string> w_f;
    if(select.size()>0){
      for(int i=0; i<select_i.size(); i++){
        w_f.push_back(w[select_i[i]]);
      }
    }
    else w_f = w;

    for(it=combs.begin(); it!=combs.end(); it++){
      string s("");
      for(vector<int>::iterator i=it->begin(); i!=it->end(); i++){
        if(i!=it->begin()){
          s += string(",");
        }
        s += w_f[*i];
      }
      incr(s);
    }
    ci += 1;
  }

  /* output string counts */
  for(std::map<std::string, unsigned long int>::iterator i=counts.begin(); i!=counts.end(); i++){
    std::cout << i->first << "," << i->second << std::endl;
  }

  return 0;
}
