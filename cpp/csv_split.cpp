#include"misc.h"
/* 20190303  split CSV file into multiple data sets, for efficient memory usage 
  note: scan / qc / qa should be same program: take a pass over the file,
 verify quality / consistency (should have same # of fields)
 note: the integrity of one of these datasets would need to be confirmed by checking:
 the folder name matches the data set name.. the names on the files, match the first
 line of each file, and that the files all have the same number of lines

 MAKE ALL THESE PROGRAMS HAVE COLOR CODED OUTPUT */

 int main(int argc, char ** argv){
  long unsigned int n_error = 0;

  if(argc < 2) err("usage:\n\tcsv_split.cpp [input file to split]");
  string f_n(argv[1]);
  mfile t(f_n, "rb");
  string s;

  unsigned int i;
  unsigned int n_f = 0; // number of fields
  long unsigned int l_i = 0; // line index
  vector<string> words; // comma delimited chunks
  vector<string> field_names; // names of the fields
  FILE ** f = NULL;

  vector< vector<string> > data;

  int error;

  string newline("\n");
  while(t.getline(s)){
    error = false;

    words = split(s);
    if(l_i == 0){
      field_names = words;
      n_f = words.size();

      /* open a file for each field */
      f = (FILE **) alloc(sizeof(FILE *) * n_f);
      for0(i, n_f){
        data.push_back(vector<string>());
        string fn_i(string(f_n) + string("_") + words[i]);
        cout << " +w " << fn_i << endl;
        f[i] = wopen(fn_i);
      }
    }
    else{
      if(words.size() != n_f){
        cout << words << endl;
        error = true;
        n_error ++;
      }
    }

    if(!error){
      for0(i, n_f){
        const char * word;
        if(l_i == 0){
          data[i].push_back(words[i]);
        }
        else{
          data[i].push_back(newline + words[i]);
        }
      }
    }

    if((++l_i) % 1000000 ==0){
      t.status();
      cout << words << endl;
    }
  }
  t.close();

  for0(i, n_f){
    cout << "write file " << (string(f_n) + string("_") + field_names[i]) << endl;
    vector<string>::iterator it;
    for(it = data[i].begin(); it != data[i].end(); it++){
      const char * word = it->c_str();
      fwrite(word, strlen(word), 1, f[i]);
    }
    fclose(f[i]);
  }

  cout << "n_error " << n_error << endl;
  return 0;
}
