#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TROOT.h>


using namespace std;

const int MAXGENERATION = 5000;
const int DAYSOFYEAR = 365;

int main(int argc, char* argv[]){
  if(argc!=2){
    cout << "# Usage: "
	 << "./" << argv[0]
	 << " [path/to/formed_csv_data.root]"
	 << endl;
    return -1;
  }

  string input_filename = argv[1];

  TFile *ifile = new TFile(input_filename.c_str());
  //==== csv_tree ====//
  TTree *csv_tree = (TTree*)ifile -> Get("csv_tree");
  double b_generation[MAXGENERATION];
  double b_birthday[MAXGENERATION];
  csv_tree -> SetBranchAddress("generation", b_generation);
  csv_tree -> SetBranchAddress("birthday", b_birthday);
  csv_tree -> GetEntry(0);
  int max_generation = csv_tree -> GetMaximum("generation") + 1;
  
  TFile *ofile = new TFile("./data/real_data.root", "recreate");
  //==== real_data_tree ====//
  TTree *real_data_tree = new TTree("real_data_tree","real_data_tree");
  double b_num_of_filled_day[MAXGENERATION] = {};
  real_data_tree
    -> Branch("generation", b_generation,
	      Form("generation[%d]/D", max_generation));
  real_data_tree
    -> Branch("num_of_filled_day", b_num_of_filled_day,
	      Form("num_of_filled_day[%d]/D", max_generation));

  TH1D *h_birthday;
  for(int gen=0; gen<max_generation; gen++){
    csv_tree -> Draw(Form("birthday>>h(%d,0,%d)", DAYSOFYEAR, DAYSOFYEAR),
		     Form("generation<%d", gen),"goff");
    h_birthday = (TH1D*)gROOT -> FindObject("h");
    
    for(int day=0; day<DAYSOFYEAR; day++){
      if( (h_birthday->GetBinContent(day+1)) != 0){
	b_num_of_filled_day[gen] += 1.0;
      }
    }
  }
  real_data_tree -> Fill();
  real_data_tree -> Write();
  ofile -> Close();
  ifile -> Close();
}
