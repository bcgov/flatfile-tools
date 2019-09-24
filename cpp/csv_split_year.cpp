#include"misc.h"
int main(int argc, char** argv){
  if(argc < 2) err("csv_split_date [input file csv] # split csv file according to date");

  str fn(argv[1]);
  size_t f_size = fsize(fn);

  ifstream f(fn);
  if(!f.is_open()) err("failed to open file");

  str s;
  getline(f, s);
  size_t l_i = 1;

  str hdr(s); // store fields for putting into new files
  int date_fi = -1;
  int n_date_fi = 0;
  vector<str> w(split(s));
  int j;
  for0(j, w.size()){
    lower(w[j]);
    size_t found = w[j].find(str("date"));
    if(found < w[j].size()){
      date_fi = j;
      n_date_fi ++;
    }
  }
  if(n_date_fi != 1) err("unable to parse date field name");

  map<str, ofstream *> files; // index file handle by year

  // first line: need to parse sep
  getline(f, s);
  l_i ++;
  w = split(s);
  str ss = w[date_fi];
  w = split(ss, '-');
  if(w.size() != 3) err("unexpected date format");
  str d(w[0]); // date string
  if(d < str("1900") || d.size() != 4) err("check date format");

  str ofn(fn + str("_") + d);
  ofstream * outf = new ofstream(ofn);
  if(!outf->is_open()) err("failed to open output file");
  files[d] = outf;
  *outf << hdr; // write header for every new file
  *outf << endl << s; // write the line

  while(getline(f, s)){
    w = split(s);
    ss = w[date_fi];
    w = split(ss, '-');
    if(w.size() != 3) err("unexpected date format");
    d = w[0]; // date string
    if(d < str("1900") || d.size() != 4) err("check date format");

    if(files.count(d) < 1){
      ofn = fn + str("_") + d;
      outf = new ofstream(ofn);
      if(!outf->is_open()) err("failed to open output file");
      files[d] = outf;
      *outf << hdr; // write header for every new file
    }
    else{
      outf = files[d];
    }
    *outf << endl << s;
    if(l_i % 100000 == 0){
      cout << "%" << 100. * float(f.tellg()) / float(f_size) << endl;
    }
    l_i ++;
  }

  // close files
  for(map<str, ofstream *>::iterator it = files.begin(); it != files.end(); it++){
    outf = it->second;
    outf->close();
  }
  return 0;
}