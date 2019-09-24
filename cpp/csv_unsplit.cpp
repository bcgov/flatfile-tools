// blind paste of csv's left to right (insert comma between files)
#include"misc.h"
int main(int argc, char ** argv){
  if(argc < 3){
    err("csv_unsplit [input file 1] .. [input file n]\n\tinput files are 1-col csv's of same length");
  }
  int i;
  int n_files = argc - 1;
  ifstream f[n_files];
  for0(i, n_files){
    cout << "fopen " << argv[i + 1] << endl;
    f[i].open(argv[i + 1]);
  }
  for0(i, n_files) if(!f[i].is_open()) err("failed to open input file");

  ofstream g("csv_unsplit.csv");
  str s;
  str comma(",");
  while(getline(f[0], s)){
    //trim(s);
    g << s;
    for0(i, n_files - 1){
      getline(f[i + 1], s);
      //trim(s);
      g << comma << s;
    }
    g << std::endl;
  }

  g.close();
  for0(i, n_files) f[i].close();
  free(f);
  return 0;
}