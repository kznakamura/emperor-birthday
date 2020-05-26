#include <iostream>
#include <vector>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TStyle.h>

using namespace std;

const int MAXGENERATION = 5000;
const int DAYSOFYEAR = 365;

int main(int argc, char* argv[]){
  if(argc<2){
    cout << "# Usage: "
	 << argv[0]
	 << " [max_entry]"
	 << " ([plot (if need, answer \"yes\")])"
	 << endl;
    return -1;
  }

  int max_entry = atoi(argv[1]);
  string check_plot = "no";
  if(argc==3) check_plot = argv[2];
  bool plot_flag = false;
  if(check_plot == "yes") plot_flag = true;

  //gROOT -> SetStyle("ATLAS");
  gStyle -> SetTitleSize(0.06, "XY");
  gStyle -> SetLabelSize(0.05, "XY");
  
  TFile *ofile = new TFile("data/mc.root", "recreate");

  //==== mc_header ====//
  TTree *mc_header = new TTree("mc_header", "mc_header");
  int b_MAXGENERATION = MAXGENERATION;
  int b_DAYSOFYEAR = DAYSOFYEAR;
  mc_header -> Branch("MAXGENERATION", &b_MAXGENERATION);
  mc_header -> Branch("DAYSOFYEAR", &b_DAYSOFYEAR);
  mc_header -> Fill();
  mc_header -> Write();

  //==== nmc_tree ====//
  TTree *mc_tree = new TTree("mc_tree", "mc_tree");
  double b_generation[MAXGENERATION] = {};
  double b_num_of_filled_day[MAXGENERATION] = {};
  mc_tree -> Branch("generation", b_generation,
		    Form("generation[%d]/D", MAXGENERATION));
  mc_tree -> Branch("num_of_filled_day", b_num_of_filled_day,
		    Form("num_of_filled_day[%d]/D", MAXGENERATION));
  
  gRandom -> SetSeed(time(NULL));
  if(plot_flag) gSystem -> Unlink("plot/mc_event.gif");
  TH1D *h_birthday;
  TCanvas *c_birthday = new TCanvas("c_birthday", "c_birthday", 700, 500);
  c_birthday -> cd();
  gPad -> SetLeftMargin(0.14);
  gPad -> SetBottomMargin(0.15);
  gPad->SetTicks(1,1);
  for(int ev=0; ev<max_entry; ev++){
    cout << "### " << ev << " / " << max_entry << " ###" << endl;
    h_birthday = new TH1D("h_birthday", "distribution of emperor's birthday;day;# of entries", DAYSOFYEAR, 0, DAYSOFYEAR);
    for(int gen=0; gen<MAXGENERATION; gen++) b_num_of_filled_day[gen] = 0.0;
    for(int gen=0; gen<MAXGENERATION; gen++){
      if(ev==0){
	if((gen+1)%10==0){
	  cout << "|" << flush;
	}else{
	  cout << "." << flush;
	}
	if((gen+1)%100==0) cout << endl;
      }
      b_generation[gen] = (double)gen;
      h_birthday -> Fill(gRandom -> Uniform(0, DAYSOFYEAR));
      for(int day=0; day<DAYSOFYEAR; day++){
	if( (h_birthday->GetBinContent(day+1)) != 0){
	  b_num_of_filled_day[gen] += 1.0;
	}
      }
      if(ev==0 && gen<500 && plot_flag){
	h_birthday -> Draw("same");
	c_birthday -> Modified();
	c_birthday -> Update();
	c_birthday -> Print("plot/mc_event.gif+");
      }      
    }
    cout << endl;
    mc_tree -> Fill();
    delete h_birthday;
  }
  mc_tree -> Write();
  ofile -> Close();
  
}
