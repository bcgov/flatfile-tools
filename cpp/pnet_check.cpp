/* 20190711 pnet_check.cpp: check pharmanet (clm*, dsp*) files for:
1) reversals: dspd_qty (quantity dispensed) and dspd_days_sply (days supplied) should not be negative
2) reversals: clam status (pcare_pmt_sts_label) in clm_rpt should contain "P" and "U" but not "R"
3) vet billing: practitioner identification number (prscr_prac_lic_body_idnt) shouldn't contain the code "V */

#include"misc.h"

// print debug info and crash
void debug(str fn, long int li, vector<str> w, const char * msg){
  cout << "file: " << fn << endl << "line: " << li << endl << w << endl;
  err(msg);
}

int main(int argc, char ** argv){
  if(argc < 2) err("pnet_check [input file 1] .. [input file n]\n\tCheck pharmanet csv files (clm*, dsp*) for reversals or veterinary billing\n");

  str s;
  int i, j;
  long int li = 0;
  size_t cur_pos = 0;
  size_t total_size = 0;
  int n_files = argc - 1;

  // field position idx
  int dspd_qty = -1;
  int dspd_days_sply = -1;
  int pcare_pmt_sts_label = -1;
  int prscr_prac_lic_body_idnt = -1;

  // check we can open all the files
  for0(i, n_files){
    ifstream f(argv[i + 1], ios::binary);
    if(!f.is_open()) err("failed to open input file\n");
    f.close();
    total_size += fsize(str(argv[i + 1]));
  }

  // do the work
  for0(i, n_files){
    // open file
    str fn(argv[i + 1]);
    ifstream f(fn, ios::binary);
    cout << fn << endl;
    getline(f, s);
    li = 1;

    // read the field names
    str fields(s);
    vector<str> w(split(s));
    for0(j, w.size()){
      trim(w[j]);
      lower(w[j]);
      vector<str> w2(split(w[j], '.'));
      if(w2.size() > 1){
        w[j] = w2[1];
      }
      trim(w[j], '*');
      if(w[j] == str("dspd_qty")) dspd_qty = j;
      if(w[j] == str("dspd_days_sply")) dspd_days_sply = j;
      if(w[j] == str("pcare_pmt_sts_label")) pcare_pmt_sts_label = j;
      if(w[j] == str("prscr_prac_lic_body_idnt")) prscr_prac_lic_body_idnt = j;
    }
    cout << "idx[dspd_qty]" << dspd_qty << endl;
    cout << "idx[dspd_days_sply]" << dspd_days_sply << endl;
    cout << "idx[pcare_pmt_sts_label]" << pcare_pmt_sts_label << endl;
    cout << "idx[prscr_prac_lic_body_idnt]" << prscr_prac_lic_body_idnt << endl;

    str r("r");
    str v("v");
    while(getline(f, s)){
      li += 1;
      w = split(s);
      if(dspd_qty >= 0 && atoi(w[dspd_qty].c_str()) < 0) debug(fn, li, w, "dspd_qty < 0");
      if(dspd_days_sply >=0 && atoi(w[dspd_days_sply].c_str()) < 0) debug(fn, li, w, "dspd_days_sply < 0)");
      if(pcare_pmt_sts_label >=0){
        lower(w[pcare_pmt_sts_label]);
        trim(w[pcare_pmt_sts_label]);
        if(w[pcare_pmt_sts_label] == r) debug(fn, li, w, "pcare_pmt_sts_label == \"R\"");
      }
      if(prscr_prac_lic_body_idnt >=0){
        trim(w[prscr_prac_lic_body_idnt]);
        lower(w[prscr_prac_lic_body_idnt]);
        if(w[prscr_prac_lic_body_idnt] == v) debug(fn, li, w, "prscr_prac_lic_body_idnt == \"V\"");
      }
      if(li % 100000 == 0){
        cout << "%" << 100. * float(cur_pos + f.tellg()) / float(total_size) << endl;
        FILE * pf = fopen((fn + str(".pnet_check_sts")).c_str(), "wb");
        if(!pf) err("failed to open file");
        fprintf(pf, "%s pnet_check INP\n", fn.c_str());
        fclose(pf);
      }
    }
    //getline
    f.close();
    cur_pos += fsize(fn);

    FILE * pf = fopen((fn + str(".pnet_check_sts")).c_str(), "wb");
    if(!pf) err("failed to open file");
    fprintf(pf, "%s pnet_check SUCCESS\n", fn.c_str());
    fclose(pf);
  }
  return 0;
}