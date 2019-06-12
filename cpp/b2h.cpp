#include"misc.h"
/* for version-control appl'n: binary to hex..
.. ideally would use stdin instead of file

for version control: generate hexdiffs so we don't lose
any special characters, etc. */
int main(int argc, char ** argv){
  str fn;
  str fnf_fn("./.b2h");
  str fnf_fnp(fnf_fn + str("p"));

  unsigned char c;
  const char * f_n;
  size_t i = 0;
  size_t f_p = 0;

  if(argc > 1){
    f_n, fn = argv[1], str(argv[1]);
    ofstream f(fnf_fn); f << fn; f.close();
    ofstream g(fnf_fnp); g << 0; g.close();
  }
  else{
    ifstream g(fnf_fnp); g >> f_p; g.close();
  }

  if(fsize(fnf_fn) == (size_t)0) err("b2h [file name]");
  ifstream fnf(fnf_fn); fnf >> fn; fnf.close();
  f_n = fn.c_str();

  FILE * f = fopen(f_n, "rb");
  if(!f) err(str("failed to open file: ") + str(f_n));
  size_t f_s = size(f);

  if(f_p >= f_s - 1){
    system((str("rm -f ") + fnf_fn).c_str());
    system((str("rm -f ") + fnf_fnp).c_str());
    exit(0);
  }
  printf("%s%d%s\n\n    ", KNRM, f_p, KGRN);
  unsigned long int l_i, c_i;
  l_i = c_i = 0;
  fseek(f, f_p, SEEK_SET);
  for(i = f_p; i < f_s; i++){
    c = fgetc(f);
    printf("%.2X", (unsigned char)c);
    if(++c_i % 32 == 0){
      if(++ l_i > 32) break;
      printf("\n    ");
    }
  }
  ofstream g(fnf_fnp);
  g << string(to_string(++i));
  g.close();

  printf("\n\n%s%d\n", KNRM, i);

  return 0;
}