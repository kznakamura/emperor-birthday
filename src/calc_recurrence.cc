#include <iostream>
#include <vector>
#include <cmath>

#include <TFile.h>
#include <TTree.h>

using namespace std;

const int MAXGENERATION = 10000;
const int DAYSOFYEAR = 365;

int main(int argc, char* argv[]){
  if(argc!=2){
    cout << "# Usage: "
	 << argv[0] << " [max_generation]"
	 << endl;
    return -1;
  }

  int max_generation = atoi(argv[1]);
  if((max_generation+3)>MAXGENERATION){
    cout << "# Message: input max_generation < " << MAXGENERATION << endl;
    return -1;
  }
  
  TFile *ofile = new TFile("data/recurrence.root", "recreate");

  //==== recurrence_header ====//
  TTree *recurrence_header = new TTree("recurrence_header", "recurrence_header");
  int b_DAYSOFYEAR = DAYSOFYEAR;
  recurrence_header -> Branch("DAYSOFYEAR", &b_DAYSOFYEAR);
  recurrence_header -> Fill();
  recurrence_header -> Write();

  //==== recurrence_tree ====//
  TTree *recurrence_tree = new TTree("recurrence_tree", "recurrence_tree");
  double b_generation = 0;
  double b_probability[DAYSOFYEAR] ={};
  double b_expected_filled_day;
  double b_filled_day_sigma;
  recurrence_tree -> Branch("generation", &b_generation);
  recurrence_tree -> Branch("probability", b_probability, Form("probability[%d]/D", DAYSOFYEAR));
  recurrence_tree -> Branch("expected_filled_day", &b_expected_filled_day);
  recurrence_tree -> Branch("filled_day_sigma", &b_filled_day_sigma);

  //普通の配列だとコアダンプしてしまった
  vector<vector<double>> prob(MAXGENERATION, vector<double>(DAYSOFYEAR));
  
  for(int gen=1; gen<max_generation; gen++){
    cout << "### gen = " << gen << ": " << endl;
    for(int day=1; day<DAYSOFYEAR; day++){
      if(day>gen) continue;
      double days_of_year = double(DAYSOFYEAR);
      double dday = double(day);
      prob[gen][1] = 1.0/pow(days_of_year, gen-1);
      prob[gen+1][day+1]
	= prob[gen][day]*(days_of_year-dday)/days_of_year + prob[gen][day+1]*(dday + 1.0)/days_of_year;
      if(gen<20) cout << prob[gen][day] << ", ";
      b_generation= (double)gen;
      b_probability[day] = prob[gen][day];
    }
    if(gen<20) cout << endl;
    double k_mean = 0.0, k_square_mean = 0.0;
    for(int day=0; day<=DAYSOFYEAR; day++){
      k_mean += double(day)*prob[gen][day];
      k_square_mean += double(pow(day,2))*prob[gen][day];
    }
    b_expected_filled_day = k_mean;
    b_filled_day_sigma = sqrt(k_square_mean - pow(k_mean,2));
    cout << "expected_filled_day=" << b_expected_filled_day
	 << ", filled_day_sigma=" << b_filled_day_sigma
	 << endl;
    recurrence_tree -> Fill();
  }
  recurrence_tree -> Write();
  ofile -> Close();
  
}
