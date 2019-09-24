#include"misc.h"
#include<algorithm>
#include<iterator>
// 20190710 this program is not very efficient, need to improve it
//
/* 20190303 split CSV file into multiple data sets, for efficient memory usage
note: scan / qc / qa should be same program: take a pass over the file,
verify quality / consistency (should have same # of fields)
note: the integrity of one of these datasets would need to be confirmed by checking:
the folder name matches the data set name.. the names on the files, match the first
line of each file, and that the files all have the same number of lines */

int main(int argc, char ** argv){
  long unsigned int n_error = 0;

  if(argc < 2) err("split csv file into multiple data sets, for efficiency. usage:\n\tcsv_split.cpp [input file to split]");
  string f_n(argv[1]); //mfile t(f_n, "rb");
  ifstream t(f_n);
  string s;

  int error;
  unsigned int i;
  unsigned int n_f = 0; // number of fields
  long unsigned int l_i = 0; // line index
  vector<string> words; // comma delimited chunks
  vector<string> field_names; // names of the fields
  FILE ** f = NULL;

  string newline("\n");

  //while(t.getline(s))
  while(getline(t,s)){
    error = false;
    words = split(s);

    if(l_i == 0){
      field_names = words;
      n_f = words.size();

      /* open a file for each field */
      f = (FILE **) alloc(sizeof(FILE *) * n_f);
      for0(i, n_f){
        str field_name(words[i]);
        std::replace(field_name.begin(), field_name.end(), '.', '_');
        str result("");
        std::remove_copy(field_name.begin(), field_name.end(), std::back_inserter(result), '*');
        field_name = result;
        string fn_i(string(f_n) + string("_") + field_name);
        cout << " +w " << fn_i << endl;
        f[i] = wopen(fn_i);
      }
      cout << "field_names: " << field_names << endl;
    }
    else{
      if(words.size() != n_f){
        cout << "l_i " << l_i << " " << words.size() << " n_f=" << n_f << " " << words << endl;
        error = true;
        n_error ++;
        exit(1);
      }
    }

    if(!error){
      for0(i, n_f){
        const char * word;
        if(l_i == 0){
          str field(words[i]);
          std::replace(field.begin(), field.end(), '.', '_');
          str result("");
          std::remove_copy(field.begin(), field.end(), std::back_inserter(result), '*');
          words[i] = result;
        }
        else{
          fprintf(f[i], "\n");
        }
        word = words[i].c_str();
        fwrite(word, strlen(word), 1, f[i]);
      }
    }

    if((++l_i) % 1000000 ==0){
      //t.status();
      cout << words << endl;
    }
  }
  t.close();
  for0(i, n_f) fclose(f[i]);
  cout << "n_error " << n_error << endl;
  return 0;
}