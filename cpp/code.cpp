#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include"misc.h"
#include<map>
#include<stdint.h>
using namespace std;
int main(int argc, char ** argv){

  cout << "sizeof(unsigned char) " << sizeof(unsigned char) << " 0-" << UCHAR_MAX << endl;
  cout << "sizeof(unsigned short int) " << sizeof(unsigned short int) << " 0-" << USHRT_MAX << endl;
  cout << "sizeof(unsigned int) " << sizeof(unsigned int) << " 0-" << UINT_MAX << endl;

  str s;
  vector<set<str>> c;
  vector<str> words;
  long unsigned int nrow = 0;
  ifstream f("idomed1991-2017.ft_schlstud.A.dat_dd_sliceapply.csv");

  long unsigned int dfile_pos;
  f.seekg(0, f.end);
  long unsigned int dfile_len = f.tellg();
  f.seekg(0, f.beg);

  getline(f, s);
  vector<str> fields(split(s));
  for(int i = 0; i < fields.size(); i++){
    c.push_back(set<str>());

  }
  unsigned int ci;
  vector<str>::iterator it;
  while(getline(f, s)){
    words = split(s);
    for0(ci, words.size()){
      c[ci].insert(words[ci]);
    }
    words.clear();
    nrow ++; // count the lines (need this later)

    if(nrow % 100000 == 0){
      dfile_pos = f.tellg();
      float p = 100. * (float)dfile_pos / (float) dfile_len;
      cout << p << endl;
    }
  }

  vector< map<str, unsigned char> > type1;
  vector< map<str, unsigned short int> > type2;
  vector< map<str, unsigned int>> type3;
  vector<unsigned short int> my_type;
  vector<unsigned int> next_code;
  vector<unsigned int> my_i;

  for(int i = 0; i < fields.size(); i++){
    cout << "|" << fields[i] << "|= " << c[i].size() << endl;
    long unsigned int s = c[i].size();
    my_type.push_back(0);
    next_code.push_back(0);
    my_i.push_back(0);
    if(s < UCHAR_MAX) my_type[i] = 1; // use less than, to include field name
    else if(s < USHRT_MAX) my_type[i] = 2;
    else if(s < UINT_MAX) my_type[i] = 3;
    else err("too many items to code for in this scheme");

    switch(my_type[i]){
      case 1:
      type1.push_back(map<str, unsigned char>());
      my_i[i] = type1.size() -1;
      break;
      case 2:
      type2.push_back(map<str, unsigned short int>());
      my_i[i] = type2.size() - 1;
      break;
      case 3:
      type3.push_back(map<str, unsigned int>());
      my_i[i] = type3.size() - 1;
      break;
    }
  }

  f.clear();
  f.seekg(0);
  
  ofstream g("code.dat");
  // don't forget to write the header info in here, too, not just the other stuff!

  // also need to write out the tables

  // need to write the below out by cases!
  // might as well unstack first!
  // AS IT IS THIS IS AN EXAMPLE OF CONTEXT SWITCHING!!!!! FOR BAD RESULTS...
  cout << "pass 2.." << endl;
  nrow = 0;
  unsigned int i;
  // build the maps
  while(getline(f, s)){
    // for each line
    words = split(s);
    for0(i, words.size()){
      //for each word
      switch(my_type[i]){
        case 1:{
          if((type1[my_i[i]]).count(words[i]) < 1){
            type1[my_i[i]][words[i]] = (unsigned char)next_code[i];
            next_code[i]++;
          }
          g << type1[my_i[i]];
        }
        break;
        case 2:{
          if((type2[my_i[i]]).count(words[i]) < 1){
            type2[my_i[i]][words[i]] = (unsigned short int)next_code[i];
            next_code[i]++;
          }
          g << type2[my_i[i]];
        }
        break;
        case 3:{
          if((type3[my_i[i]]).count(words[i]) < 1){
            type3[my_i[i]][words[i]] = (unsigned int)next_code[i];
            next_code[i]++;
          }
          g << type3[my_i[i]];
        }
        break;
        default: err("unknown type");
        break;
      }

    }
    nrow ++; // count the lines (need this later)

    if(nrow % 10000 == 0){
      dfile_pos = f.tellg();
      float p = 100. * (float)dfile_pos / (float) dfile_len;
      cout << p << endl;
    }
  }
  f.close();
  g.close();

  /*
  cout << "writing\n";

  if(c.size() >= UINT_MAX){
    cout << "Error: UINT type not large enough.\n";
    exit(1);
  }

  ofstream g("code.csv");
  g << to_string(c.size()) << "\n";

  map<str, unsigned int> l_i;
  unsigned int c_i = 0;
  for(set<str>::iterator it = c.begin(); it != c.end(); it++){
    g << *it << "\n";
    l_i[*it] = c_i ++; // index (unsigned int) as a function of string (code)
  }

  // code table in hand, reiterate file and dump codes
  unsigned int i = 0;

  // get the header
  getline(f,s);
  words = split(s);
  unsigned int ncol = words.size();

  g << to_string(nrow) << str(",") << to_string(ncol) << str("\n");

  nrow = 0;
  // record the dims
  while(getline(f,s)){
    words = split(s);
    for(vector<str>::iterator it = words.begin(); it != words.end(); it++){
      g << l_i[*it];
    }
    nrow ++;
    if(nrow % 100000 == 0){
      dfile_pos = f.tellg();
      float p = 100. * (float)dfile_pos / (float) dfile_len;
      cout << p << endl;
    }
  }
  f.close();
  g.close();
  */
  return 0;
}
