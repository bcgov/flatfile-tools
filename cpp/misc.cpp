#include"misc.h"
string cwd(){
  char s[PATH_MAX];
  _cwd(s, PATH_MAX);
  return string(s);
}

/* split a string (a-la python) */
vector<string> split(string s, char delim){
  std::vector<std::string> ret;
  std::istringstream iss(s);
  std::string token;
  while(getline(iss,token,delim)) ret.push_back(token);
  return ret;
}

/* split a string (a-la python) */
vector<string> split(string s){
  std::vector<std::string> ret;
  std::istringstream iss(s);
  const char delim = ',';
  string token;
  while(getline(iss,token,delim)) ret.push_back(token);

  vector<string>::iterator it;
  for(it = ret.begin(); it != ret.end(); it++){
    trim(*it);
  }
  return ret;
}

string join(const char * delim, vector<string> s){
  string ret("");
  string d(delim);
  for(vector<string>::iterator it = s.begin(); it!=s.end(); it++){
    if(it!=s.begin()) ret += d;
    ret += *it;
  }
  return ret;
}

void err(string msg){
  cout << "Error: " << msg << endl;
  exit(1);
}

void err(const char * msg){
  err(string(msg));
}

void * balloc(long unsigned int nb){
  void * d = malloc(nb);
  memset(d, '\0', nb);
  return (void *)d;
}

FILE * wopen(string fn){
  FILE * f = fopen(fn.c_str(), "wb");
  if(!f) err("failed to open file for writing");
  return f;
}

mfile::mfile(string f_n, char * mode){
  /*
  need a flag for if we don't open the whole thing in ram..
  */
  if(mode[0] !='r') err("write mode not yet supported");
  if(mode[1] != 'b') err("incorrect mode str: rb, wb");
  if(mode[0] != 'r' && mode[0] != 'b') err("incorrect mode str: rb, wb");
  fp = fopen(f_n.c_str(), mode);
  if(!fp) err("failed to open file");
  fs = size(fp);
  d = (char *)alloc(fs);
  size_t nr = fread(d, fs, 1, fp);
  if(nr != 1) err("failed to read file");
  c_pos = l_pos = 0;
  last_pos = 0;
  start_t = clock();
}

int mfile::getline(string & buf){
  c_pos = l_pos;
  for(c_pos == l_pos; c_pos < fs; c_pos++){
    c = d[c_pos]; // printf("\nc '%c'\n", c);
    if(c == '\n' || c == '\r') break;
  }
  if(c_pos == l_pos) return false;
  memcpy(&s_bf[0], &d[l_pos], c_pos - l_pos);
  s_bf[c_pos - l_pos] = '\0'; // null termintor
  buf = s_bf;
  while(c= d[c_pos] == '\n' || c == '\r') c_pos ++;
  l_pos = c_pos;
  return true;
}

void mfile::rewind(){
  c_pos = l_pos = 0;
}

void mfile::close(){
  fclose(fp);
  free(d);
}

void mfile::status(){
  cout << "%" << 100.* ((float)l_pos / (float)fs); // << " MB/s " ;
  clock_t t = clock();
  if(last_pos > 0){
    clock_t duration = t - last_t;
    size_t d_pos = l_pos - last_pos;
    double dt = (double) duration / (double) CLOCKS_PER_SEC;
    cout << " MB/s: " << ((float)d_pos / (float)1000000.) / (float)dt << " ";
  }
  last_pos = l_pos;
  last_t = t;
}

size_t mfile::tellg(){
  return c_pos;
}
size_t mfile::len(){
  return fs;
}

/* get size of file pointer */
size_t size(FILE * f){
  fseek(f, 0L, SEEK_END);
  size_t sz = ftell(f);
  rewind(f);
  return sz;
}

size_t fsize(string fn){
  FILE * f = fopen(fn.c_str(), "rb");
  if(!f) return (size_t) 0;
  size_t fs = size(f);
  fclose(f);
  return fs;
}