#include"misc.h"
/* oddly, python jumps out of the unix terminal. C++ does not! This hook brings python back into the place where we can have nice things */
using namespace std;
int main(int argc, char** argv){
  if(argc > 1){
    system((str("gunzip ") + str(argv[1])).c_str());
  }
  else{
    system("ls -1 *.gz > ./.ls_1_gz.txt");
    ifstream f;
    f.open("./.ls_1_gz.txt");
    if(!f.is_open()){
      err("failed to open file: ./.ls_1_gz.txt");
    }

    ofstream o_f;
    o_f.open("./.unzp_jobs.txt");
    if(!o_f.is_open()) err("failed to open file: ./.unzp_jobs.txt");

    str s;
    while(getline(f, s)){
      trim(s);
      size_t f_siz = fsize(s);
      if(f_siz > 0){
        cout << "\tunzp " << s << endl;
        o_f << "unzp " << s << endl;

      }
    }
    f.close();
    o_f.close();

    // hypothetically 4 is enough unzip jobs to run at once!
    system("multicore ./.unzp_jobs.txt 4");
    system("rm -f ./.unzp_jobs.txt");
    system("rm -f ./.ls_1_gz.txt");
  }

  // 20190726 new mandate: do anything path-related in C environment:
  // this is where the paths follow the unix conventions
  return 0;
}