void plot_expected_filled_day(){
  gStyle -> SetTitleSize(0.06, "XY");
  gStyle -> SetLabelSize(0.05, "XY");
  
  TFile *recurrence = new TFile("./data/recurrence.root");
  TFile *mc = new TFile("./data/mc.root");
  TFile *real_data = new TFile("./data/real_data.root");
  TTree *recurrence_tree = (TTree*)recurrence -> Get("recurrence_tree");
  TTree *mc_tree = (TTree*)mc -> Get("mc_tree");
  TTree *real_data_tree = (TTree*)real_data -> Get("real_data_tree");    

  TCanvas *c = new TCanvas("c", "c", 1400, 1000);
  c -> Divide(2,2,0.001,0.001);
  c -> cd(1);
  gPad -> SetLeftMargin(0.14);
  gPad -> SetBottomMargin(0.15);
  gPad -> SetTicks(1,1);
  TH2D *frame1 = new TH2D("frame1", "filled day;generation;filled day",5500,0,5500,400,0,400);
  frame1 -> SetStats(0);  
  frame1 -> Draw();
  recurrence_tree -> SetMarkerColor(4);
  recurrence_tree -> SetMarkerStyle(7);
  recurrence_tree -> Draw("generation:expected_filled_day:filled_day_sigma","","goff");
  TGraphErrors *g = new TGraphErrors(recurrence_tree->GetSelectedRows(),
				     recurrence_tree->GetV1(), recurrence_tree->GetV2(),
				     0, recurrence_tree->GetV3());
  g -> SetFillColor(4);
  g -> SetFillStyle(3010);
  g -> Draw("3same");
  
  mc_tree -> SetMarkerColor(2);
  mc_tree -> SetMarkerStyle(2);
  mc_tree -> Draw("num_of_filled_day:generation","Entry$==0","psame");
  real_data_tree -> SetMarkerColor(3);
  real_data_tree -> SetMarkerStyle(3);
  real_data_tree -> Draw("num_of_filled_day:generation","Entry$==0","same");
  TLegend *legend = new TLegend( 0.5, 0.65, 0.65, 0.8);
  legend -> AddEntry(g,"1 sigma");
  legend -> AddEntry(mc_tree,"MC", "p");
  legend -> AddEntry(real_data_tree,"real data", "p");
  legend -> Draw();
    
  c -> cd(2);
  gPad -> SetLeftMargin(0.14);
  gPad -> SetBottomMargin(0.15);
  gPad -> SetTicks(1,1);
  TH2D *frame1_small = new TH2D("frame1_small", "filled day small region;generation;filled day",150,0,150,150,0,150);
  frame1_small -> SetStats(0);
  frame1_small -> Draw();
  g -> Draw("3same");
  mc_tree -> Draw("num_of_filled_day:generation","Entry$==0","same");
  real_data_tree -> Draw("num_of_filled_day:generation","Entry$==0","same");
  legend -> Draw();

  c -> cd(3);
  gPad -> SetLeftMargin(0.14);
  gPad -> SetBottomMargin(0.15);
  gPad -> SetTicks(1,1);
  TH2D *frame2 = new TH2D("frame2", "residual;generation;residual",5500,0,5500,10,0.9,1.1);
  frame2 -> SetStats(0);  
  frame2 -> Draw();
  
  recurrence_tree -> Draw("generation:expected_filled_day/expected_filled_day:filled_day_sigma/expected_filled_day","","goff");
  TGraphErrors *g_recurrence_residual = new TGraphErrors(recurrence_tree->GetSelectedRows(),
					      recurrence_tree->GetV1(), recurrence_tree->GetV2(), 0, recurrence_tree->GetV3());
  g_recurrence_residual -> SetFillColor(4);
  g_recurrence_residual -> SetFillStyle(3010);
  g_recurrence_residual -> Draw("3same");
  TF1 *residual_base = new TF1("residual_base", "1", 0, 5000);
  residual_base -> SetLineStyle(2);
  residual_base -> SetLineColor(4);
  residual_base -> Draw("same");
  double expected_filled_day, generation[5000] = {}, mc_residual[5000] = {}, real_residual[5000] = {};
  double mc_filled_day[5000] = {}, real_filled_day[5000] = {};
  mc_tree -> SetBranchAddress("num_of_filled_day",mc_filled_day);
  real_data_tree -> SetBranchAddress("num_of_filled_day", real_filled_day);
  mc_tree -> GetEntry(0);
  real_data_tree -> GetEntry(0);
  int max_mc_generation = recurrence_tree -> GetMaximum("generation");
  int max_real_generation = real_data_tree -> GetMaximum("generation");
  for(int gen=0; gen<max_mc_generation; gen++){
    TTree *copy_recurrence = recurrence_tree -> CopyTree(Form("generation==%d",gen));
    copy_recurrence -> GetEntry(0);
    expected_filled_day = copy_recurrence -> GetLeaf("expected_filled_day") -> GetValue(0);
    generation[gen] = (double)gen;
    mc_residual[gen] = mc_filled_day[gen]/expected_filled_day;
    if(gen<max_real_generation){
      real_residual[gen] = real_filled_day[gen]/expected_filled_day;
    }
    //cout << generation[gen] << ", " << num_of_filled_day[gen] << ", " << expected_filled_day << ", " << mc_residual[gen] << endl;
  }
  
  TGraph *g_mc_residual = new TGraph(max_mc_generation, generation, mc_residual);
  g_mc_residual -> SetMarkerStyle(2);
  g_mc_residual -> SetMarkerColor(2);
  g_mc_residual -> Draw("psame");
  TGraph *g_real_residual = new TGraph(max_real_generation, generation, real_residual);
  g_real_residual -> SetMarkerStyle(3);
  g_real_residual -> SetMarkerColor(3);
  g_real_residual -> Draw("psame");
  legend -> Draw();

  c -> cd(4);
  gPad -> SetLeftMargin(0.14);
  gPad -> SetBottomMargin(0.15);
  gPad -> SetTicks(1,1);
  TH2D *frame2_small = new TH2D("frame2_small", "residual small region;generation;residual",150,0,150,10,0.9,1.1);
  frame2_small -> SetStats(0);  
  frame2_small -> Draw();
  g_recurrence_residual -> Draw("3same");
  residual_base -> Draw("same");
  g_mc_residual -> Draw("psame");
  g_real_residual -> Draw("psame");
  legend -> Draw();
  c -> Print("./plot/expected_filled_day.pdf");
}
