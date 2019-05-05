#ifndef _N_CHOOSE_M_H_
#define _N_CHOOSE_M_H_
//---------------------------------------------------------------------------//
// n_choose_m.h: combinations for multi-variate table                        //
// date: 20190124                                                            //
// info: the combination indices always in order                             //
//---------------------------------------------------------------------------//
#include<map>
#include"misc.h"
#include<vector>
#include<iostream>
using namespace std;

void comb(vector<int> & items, int start, int end, int m, vector< vector<int> > & combs){
if(start > end) return;
  if(items.size() == m){
    combs.push_back(items);
    return;
  }

  vector<int> items1;  vector<int> items2;
  for(int i=0; i<items.size(); i++) items1.push_back(items[i]);
  for(int i=0; i<items.size(); i++) items2.push_back(items[i]);
  
  items2.push_back(start);
  comb(items1, start + 1, end, m, combs);
  comb(items2, start + 1, end, m, combs);
}

vector< vector<int> > n_choose_m(int n, int m){
	vector<int> s;
	vector< vector<int> >  combs;
	comb(s, 0, n, m, combs);
	return combs;
}

vector< vector<int> > n_choose_up_to_m(int n, int m){
	vector<int> s;
	vector<vector<int>> ret;
	for(int i=1; i<=m; i++){
	  vector<vector<int>> combs;
	  comb(s, 0, n, i, combs);
 	  ret.reserve(ret.size() + distance(combs.begin(),combs.end()));
 	  ret.insert(ret.end(),combs.begin(),combs.end());
	}
	for(int i=0; i<ret.size(); i++){
		cout << ret[i] << endl;
	}
  return ret;
}

#endif
