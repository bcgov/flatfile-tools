#ifndef _MISC_H_
#define _MISC_H_
//---------------------------------------------------------------------------//
// misc.h: helper functions //
// date: 20190124 //
//---------------------------------------------------------------------------//

/* shorthand for for loops from 0 to N */
#define for0(i,n) for(i = 0; i < n; i++)

#define STR_MAX 16384

#include<map>
#include<set>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include<limits.h>
#include<memory.h>
#include<algorithm>
#include"ansicolor.h"

using namespace std;

#include <stdio.h> /* defines FILENAME_MAX */
#ifdef WINDOWS
#include <direct.h>
#define _cwd _getcwd
#else
#include <unistd.h>
#define _cwd getcwd
#endif

void rewind(ifstream &a);

#define str string
string cwd();

/* split a string (a-la python) */
vector<string> split(string s, char delim);
vector<string> split(string s); // comma

string join(const char * delim, vector<string> s);

template<class T> std::ostream& operator << (std::ostream& os, const std::vector<T>& v){
  os << "[";
  for (typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii){
    os << " '" << *ii << "'";
  }
  os << "]";
  return os;
}

template<class T> std::ostream& operator << (std::ostream& os, const std::set<T>& v){
  os << "{";
  for (typename std::set<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii){
    os << " " << *ii;
  }
  os << "}";
  return os;
}

template<class A, class B> std::ostream& operator << (std::ostream& os, const std::map<A, B>& v){
  os << "{" << endl;
  for (typename std::map<A, B>::const_iterator ii = v.begin(); ii != v.end(); ++ii){
    os << ii->first << "," << ii->second << endl;
  }
  os << "}";
  return os;
}

void err(string msg);

void err(const char * msg);

void * balloc(long unsigned int nb);

/* allocate memory */
inline void * alloc(size_t nb){
  void * d = malloc(nb);
  if(!d){
    printf("%zu\n", nb);
    err("failed to allocate memory");
  }
  memset(d, '\0', nb);
  return d;
}

#include <algorithm>
#include <cctype>
#include <locale>

//a trim from start (in place)
static inline void ltrim(std::string &s){
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch){
    return !std::isspace(ch);
  }
  ));
}

// trim from end (in place)
static inline void rtrim(std::string &s){
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch){
    return !std::isspace(ch);
  }
  ).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s){
  ltrim(s);
  rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s){
  ltrim(s);
  return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s){
  rtrim(s);
  return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s){
  trim(s);
  return s;
}

static inline void trim(std::string &s, char delim){
  str ret("");
  int end = s.size() - 1; 
  int start = 0;
  while(s[start] == delim) start += 1;
  while(s[end] == delim) end -= 1;
  s = s.substr(start, 1 + end - start);
}

#define strip trim

/* convert to lower case */
static inline lower(std::string & s){
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

static inline std::string lower_copy(std::string &s){
  string r(s);
  std::transform(r.begin(), r.end(), r.begin(), ::tolower);
  return r;
}

/* get size of file pointer */
size_t size(FILE * f);
size_t fsize(string fn);

// in-memory reader (writer to be implemented)
// note this should be able to modulate between available protocols (like ifstream, ofstream, etc. , fwrite, fread, if available)

FILE * wopen(string fn);

class mfile{
  FILE * fp;
  char * d;
  size_t fs;
  char s_bf[STR_MAX];
  char c;

  size_t l_pos; // position of start of current line
  size_t c_pos; // present read position within line

  //T x; // just using this to trick the compiler to let us implement the class in the header!
  public:

  mfile(string f_n, char * mode);
  int getline(string & buf);
  void rewind();
  void close();

  clock_t start_t, last_t;
  size_t last_pos;
  size_t tellg();
  size_t len();
  void status();

  };

  /* function to produce a distributed map of finite trajectory generation, with information-theoretically optimal compression */
  // this requires "class SA" (list / map hybrid)
  //
  // use this to algebraically encode e.g. lists of filenames in the format: msp2000-17, for continuous times..
  // e.g, [ab, ac] --> a[b,c]
  // [abc, ac] --> a[b[c],c]
  //
  #endif
