#include"misc.h"
// 2019 07 24 should update this program to identify the studyid col
int main(int argc, char** argv){
  if(argc < 2) err("count_studyid.cpp: usage: count_studyid [input file, csv, last col STUDYID");
  string fn(argv[1]);
  cout << "input file: " << fn << endl;

  ifstream infile(fn);
  if(!infile.is_open()) err(string("failed to open input file: ") + fn);

  string line;
  string studyid;
  long unsigned int ci = 0;

  map<string, long unsigned int> count;
  count.clear();

  long unsigned int nf = 0;
  while(getline(infile, line)){
    vector<string> w(split(line, ','));
    for(vector<string>::iterator it = w.begin(); it != w.end(); it++) trim(*it);
    if(ci == 0){
      nf = w.size();
      if(w[nf - 1] != "STUDYID" && w[nf - 1] != "study-id" && w[nf - 1] != "studyid"){
        err("expected last field studyid or STUDYID");
      }
    }
    else{
      if(w.size() != nf){
        cout << "line : " << ci << endl;
        err("field count mismatch");
      }
      studyid = w[nf - 1];
      map<string, long unsigned int>::iterator it = count.find(studyid);
      if(it == count.end()) count[studyid] = 0;
      count[studyid] += 1;
    }
    ci += 1;
  }

  cout << "Unique studyid: " << count.size() << endl;

  map<long unsigned int, long unsigned int> cc;
  for(map<string, long unsigned int>::iterator it = count.begin(); it != count.end(); it++){
    long unsigned int cci = it->second;
    map<long unsigned int, long unsigned int>::iterator cit = cc.find(cci);
    if(cit == cc.end()) cc[cci] = 0;
    cc[cci] += 1;
  }

  cout << "count of studyid counts: " << cc << endl;
  return 0;
}
