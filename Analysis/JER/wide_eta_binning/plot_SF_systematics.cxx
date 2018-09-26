#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "TROOT.h"
#include "TMath.h"
#include "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/include/constants.hpp"
#include "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/PersonalCode/tdrstyle_all.C"
// #include "constants.hpp"
// #include "tdrstyle_all.C"

void LoadSF(std::vector<std::vector<std::vector<double>>> &SFs, TString filename) {
  std::vector<std::vector<double> > SF;
  std::vector<double> eta_bin_SM_center; 						//0
  std::vector<double> eta_bin_SM_error;    					//1
  std::vector<double> SF_uncorrelated_SM;       		//2
  std::vector<double> SF_uncorrelated_SM_error;			//3
  std::vector<double> SF_correlated_SM;        		 	//4
  std::vector<double> SF_correlated_SM_error;   		//5
  std::vector<double> SF_uncorrelated_SM_ptdep_min; //6
  std::vector<double> SF_uncorrelated_SM_ptdep_max;	//7
  std::vector<double> SF_correlated_SM_ptdep_min;   //8
  std::vector<double> SF_correlated_SM_ptdep_max;   //9

  // SF_uncorrelated_FE, SF_uncorrelated_FE_error, SF_correlated_FE, SF_correlated_FE_error, eta_bin_FE_center, eta_bin_FE_error;
  std::ifstream file(filename, ios::in);
  SF.push_back(eta_bin_SM_center);
  SF.push_back(eta_bin_SM_error);
  SF.push_back(SF_uncorrelated_SM);
  SF.push_back(SF_uncorrelated_SM_error);
  SF.push_back(SF_correlated_SM);
  SF.push_back(SF_correlated_SM_error);
  SF.push_back(SF_uncorrelated_SM_ptdep_min);
  SF.push_back(SF_uncorrelated_SM_ptdep_max);
  SF.push_back(SF_correlated_SM_ptdep_min);
  SF.push_back(SF_correlated_SM_ptdep_max);

  std::string line;
  getline(file, line);

  if (gSystem->AccessPathName(filename)) return false;

  while (!file.eof()) {
    getline(file, line);
    std::istringstream iss(line);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    for (unsigned int i = 0; i < results.size(); i++) { SF.at(i).push_back(std::stod(results[i]));}
  }

  SFs.push_back(SF);
  SF.clear();
  file.close();
}

// SFs == (n_systematics, columns in files, eta_bins)
void plot_SF_systematics_(TString path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/JER/wide_eta_binning/file/Single/Fall17_17Nov2017_V10/AK4CHS/") {

  int shift_SM = 3;
  int shift_FE = 3;
  int shift_barrel = 1;
  std::vector<double> eta_bins_all(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_bins_SM(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double) - shift_SM);
  std::vector<double> eta_bins_FE(eta_bins + shift_barrel, eta_bins + sizeof(eta_bins)/sizeof(double));

  int method = 4; //2-uncorr 4-corr
  int pt_dep_method = 4; //4-min value 5-max value
  std::vector<int> colors;
  colors.push_back(kBlue-4);
  colors.push_back(kGreen-2);
  colors.push_back(kOrange);
  colors.push_back(kViolet-3);
  colors.push_back(kCyan);
  colors.push_back(kMagenta);
  colors.push_back(kAzure+7);
  colors.push_back(kSpring);
  colors.push_back(kBlack);


  std::vector<TString> systematics;

  systematics.push_back("gaustails");
  systematics.push_back("JEC_up");
  systematics.push_back("JEC_down");
  systematics.push_back("PU_up");
  systematics.push_back("PU_down");
  systematics.push_back("PLI_down");
  systematics.push_back("PLI_up");
  systematics.push_back("alpha");
  systematics.push_back("pTdep");
  std::vector<std::vector<std::vector<double>>> SFs_SM, SFs_FE;
  TString central_SM, central_FE, filename;

  // path = "AK4CHS/";
  central_SM = "standard/RunBCDEF/output/scalefactors_ST.txt";
  central_FE = "standard/RunBCDEF/output/scalefactors_FE.txt";

  /////////////////////////
  //   Standard Method   //
  /////////////////////////

  filename = path+central_SM;
  LoadSF(SFs_SM, filename);

  for (unsigned int i = 0; i < systematics.size(); i++) {
    TString temp = central_SM.Copy();
    TString sys = systematics.at(i);
    // pt_dep values are saved in all the folder. But the one in the nominal folder are calculated wrt to nominal values. It is saved only the Max and Min value of the difference of the SF
    if (sys.Contains("_")) filename = path+temp.ReplaceAll("standard",sys(0,sys.First("_"))+"/"+sys(sys.First("_")+1,sys.Length()));
    else if (sys.Contains("pTdep"))filename = path+central_SM;
    else filename = path+temp.ReplaceAll("standard",sys);
    LoadSF(SFs_SM, filename);
  }

  if (SFs_SM.size() != systematics.size()+1) {
    return false;
  }

  std::vector <double> SF_SM, stat_SM, eta_bin_SM_center, eta_bin_SM_error;
  std::vector <std::vector <double> > systematics_SM;
  for (unsigned int i = 0; i < 1+systematics.size(); i++){
    if (i==0) {
      // Getting Central Values & Statistical Uncertainties
      for (unsigned int j = 0; j < eta_bins_SM.size()-1; j++){
        eta_bin_SM_center.push_back(SFs_SM.at(0).at(0).at(j));
        eta_bin_SM_error.push_back(SFs_SM.at(0).at(1).at(j));
        SF_SM.push_back(SFs_SM.at(0).at(method).at(j));
        stat_SM.push_back(SFs_SM.at(0).at(method+1).at(j));
      }
    } else if (i < systematics.size()) {
      // Getting Systematics Uncertainties. Diff wrt the nominal value
      std::vector <double> temp;
      for (unsigned int j = 0; j < eta_bins_SM.size()-1; j++) temp.push_back(TMath::Abs(SFs_SM.at(0).at(method).at(j) - SFs_SM.at(i).at(method).at(j)));
      systematics_SM.push_back(temp);
      temp.clear();
    }
    else{
      std::vector <double> temp;
      // Getting Systematics Uncertainties for pt_dep. It is saved only the Max and Min value
      for (unsigned int j = 0; j < eta_bins_SM.size()-1; j++) temp.push_back((SFs_SM.at(i).at(method+pt_dep_method).at(j) + SFs_SM.at(i).at(method+pt_dep_method+1).at(j))/2);
      systematics_SM.push_back(temp);
      temp.clear();
    }
  }

  std::vector <double> total_error_SM, systematics_SM_all;
  for (unsigned int i = 0; i < eta_bins_SM.size()-1; i++) {
    double err = 0;
    for (unsigned int j = 0; j < systematics.size(); j++) err += TMath::Power(systematics_SM.at(j).at(i),2);
    err = TMath::Sqrt(err);
    systematics_SM_all.push_back(err);
    total_error_SM.push_back(TMath::Sqrt(TMath::Power(err,2)+TMath::Power(stat_SM.at(i),2)));
  }

  TCanvas* canv_SF_SM = tdrCanvas("Statistics_SM",eta_bins_SM[0]-0.1, eta_bins_SM[eta_bins_SM.size()-1]+0.1, 0.5, 2.0, "#eta", "JER SF");
  canv_SF_SM->SetTickx(0);
  canv_SF_SM->SetTicky(0);
  TLegend *leg_SM = tdrLeg(0.45,0.67,0.80,0.92);
  leg_SM->SetTextFont(42);  leg_SM->SetTextSize(0.025);  leg_SM->SetTextColor(kBlack);
  tdrHeader(leg_SM,"Uncertainties", 12);
  TGraphErrors* gr_stat_SM = new TGraphErrors(eta_bins_SM.size()-1, &(eta_bin_SM_center[0]), &SF_SM[0], &(eta_bin_SM_error[0]), &stat_SM[0]);
  TGraphErrors* gr_syst_SM = new TGraphErrors(eta_bins_SM.size()-1, &(eta_bin_SM_center[0]), &SF_SM[0], &(eta_bin_SM_error[0]), &systematics_SM_all[0]);
  TGraphErrors* gr_tot_SM = new TGraphErrors(eta_bins_SM.size()-1, &(eta_bin_SM_center[0]), &SF_SM[0], &(eta_bin_SM_error[0]), &total_error_SM[0]);
  leg_SM->AddEntry(gr_stat_SM, "stat","f");
  leg_SM->AddEntry(gr_syst_SM, "syst","f");
  leg_SM->AddEntry(gr_tot_SM, "stat+syst","f");
  tdrDraw(gr_tot_SM, "P5", kFullDotLarge, kGreen-2, kSolid, kGreen-2, 3005, kGreen-2);
  tdrDraw(gr_syst_SM, "P5", kFullDotLarge, kBlue-4, kSolid, kBlue-4, 3005, kBlue-4);
  tdrDraw(gr_stat_SM, "P5", kFullDotLarge, kRed, kSolid, kRed, 3005, kRed);
  leg_SM->Draw("same");

  canv_SF_SM->Print(path+"SF_SM.pdf","pdf");
  canv_SF_SM->Print(path+"SF_SM.png","png");


  TCanvas* canv_stat_SM = tdrCanvas("Uncertainties_SM",eta_bins_SM[0]-0.1, eta_bins_SM[eta_bins_SM.size()-1]+0.1, 0.0001, 100.0, "#eta", "Uncertainties");
  canv_stat_SM->SetTickx(0);
  canv_stat_SM->SetTicky(0);
  canv_stat_SM->SetLogy();
  leg_SM = tdrLeg(0.55,0.67,0.80,0.92);
  leg_SM->SetTextFont(42);  leg_SM->SetTextSize(0.025);  leg_SM->SetTextColor(kBlack); leg_SM->SetNColumns(2);
  tdrHeader(leg_SM,"Uncertainties", 12);
  TH1* h_SM = new TH1F("", "", eta_bins_SM.size()-1, &eta_bins_SM[0] );
  for (unsigned int i = 0; i < eta_bins_SM.size()-1; i++) { h_SM->SetBinContent(i+1,stat_SM.at(i));}
  leg_SM->AddEntry(h_SM, "stat","l");
  for (unsigned int j = 0; j < systematics.size(); j++){
    TH1F* h_SM = new TH1F("", "", eta_bins_SM.size()-1, &eta_bins_SM[0] );
    h_SM->SetLineWidth(5);
    for (unsigned int i = 0; i < eta_bins_SM.size()-1; i++) {h_SM->SetBinContent(i+1,systematics_SM.at(j).at(i));}
    tdrDraw(h_SM, "hist", kFullDotLarge, colors[j], kSolid, colors[j], 0, colors[j]);
    leg_SM->AddEntry(h_SM, systematics[j],"l");
  }
  tdrDraw(h_SM, "hist", kFullDotLarge, kRed, kSolid, kRed, 0, kRed);
  h_SM->SetLineWidth(8); h_SM->SetLineStyle(9);
  leg_SM->Draw("same");

  canv_stat_SM->Print(path+"Uncertainties_SM.pdf","pdf");
  canv_stat_SM->Print(path+"Uncertainties_SM.png","png");


  /////////////////////////
  //  Forward Extention  //
  /////////////////////////


  filename = path+central_FE;
  LoadSF(SFs_FE, filename);

  for (unsigned int i = 0; i < systematics.size(); i++) {
    TString temp = central_FE.Copy();
    TString sys = systematics.at(i);
    if (sys.Contains("_")) filename = path+temp.ReplaceAll("standard",sys(0,sys.First("_"))+"/"+sys(sys.First("_")+1,sys.Length()));
    else if (sys.Contains("pTdep"))filename = path+central_FE;
    else filename = path+temp.ReplaceAll("standard",sys);
    LoadSF(SFs_FE, filename);
  }

  if (SFs_FE.size() != systematics.size()+1) {
    return false;
  }

  std::vector <double> SF_FE, stat_FE, eta_bin_FE_center, eta_bin_FE_error;
  std::vector <std::vector <double> > systematics_FE;
  for (unsigned int i = 0; i < 1+systematics.size(); i++){
    if (i==0) {
      for (unsigned int j = 0; j < eta_bins_FE.size()-1; j++){
        eta_bin_FE_center.push_back(SFs_FE.at(0).at(0).at(j));
        eta_bin_FE_error.push_back(SFs_FE.at(0).at(1).at(j));
        SF_FE.push_back(SFs_FE.at(0).at(method).at(j));
        stat_FE.push_back(SFs_FE.at(0).at(method+1).at(j));
      }
    } else if (i < systematics.size()) {
      std::vector <double> temp;
      for (unsigned int j = 0; j < eta_bins_FE.size()-1; j++) temp.push_back(TMath::Abs(SFs_FE.at(0).at(method).at(j) - SFs_FE.at(i).at(method).at(j)));
      systematics_FE.push_back(temp);
      temp.clear();
    } else{
      std::vector <double> temp;
      for (unsigned int j = 0; j < eta_bins_FE.size()-1; j++) temp.push_back((SFs_FE.at(i).at(method+pt_dep_method).at(j) + SFs_FE.at(i).at(method+pt_dep_method+1).at(j))/2);
      systematics_FE.push_back(temp);
      temp.clear();
    }
  }

  std::vector <double> total_error_FE, systematics_FE_all;
  for (unsigned int i = 0; i < eta_bins_FE.size()-1; i++) {
    double err = 0;
    for (unsigned int j = 0; j < systematics.size(); j++) err += TMath::Power(systematics_FE.at(j).at(i),2);
    err = TMath::Sqrt(err);
    systematics_FE_all.push_back(err);
    total_error_FE.push_back(TMath::Sqrt(TMath::Power(err,2)+TMath::Power(stat_FE.at(i),2)));
  }

  TCanvas* canv_SF_FE = tdrCanvas("Statistics_FE",eta_bins_FE[0]-0.1, eta_bins_FE[eta_bins_FE.size()-1]+0.1, 0., 3.0, "#eta", "JER SF");
  canv_SF_FE->SetTickx(0);
  canv_SF_FE->SetTicky(0);
  TLegend *leg_FE = tdrLeg(0.60,0.67,0.80,0.92);
  leg_FE->SetTextFont(42);  leg_FE->SetTextSize(0.025);  leg_FE->SetTextColor(kBlack);
  tdrHeader(leg_FE,"Uncertainties", 12);
  TGraphErrors* gr_stat_FE = new TGraphErrors(eta_bins_FE.size()-1, &(eta_bin_FE_center[0]), &SF_FE[0], &(eta_bin_FE_error[0]), &stat_FE[0]);
  TGraphErrors* gr_syst_FE = new TGraphErrors(eta_bins_FE.size()-1, &(eta_bin_FE_center[0]), &SF_FE[0], &(eta_bin_FE_error[0]), &systematics_FE_all[0]);
  TGraphErrors* gr_tot_FE = new TGraphErrors(eta_bins_FE.size()-1, &(eta_bin_FE_center[0]), &SF_FE[0], &(eta_bin_FE_error[0]), &total_error_FE[0]);
  leg_FE->AddEntry(gr_stat_FE, "stat","f");
  leg_FE->AddEntry(gr_syst_FE, "syst","f");
  leg_FE->AddEntry(gr_tot_FE, "stat+syst","f");
  tdrDraw(gr_tot_FE, "P5", kFullDotLarge, kGreen-2, kSolid, kGreen-2, 3005, kGreen-2);
  tdrDraw(gr_syst_FE, "P5", kFullDotLarge, kBlue-4, kSolid, kBlue-4, 3005, kBlue-4);
  tdrDraw(gr_stat_FE, "P5", kFullDotLarge, kRed, kSolid, kRed, 3005, kRed);
  leg_FE->Draw("same");

  canv_SF_FE->Print(path+"SF_FE.pdf","pdf");
  canv_SF_FE->Print(path+"SF_FE.png","png");

  TCanvas* canv_stat_FE = tdrCanvas("Uncertainties_FE",eta_bins_FE[0]-0.1, eta_bins_FE[eta_bins_FE.size()-1]+0.1, 0.0001, 100.0, "#eta", "Uncertainties");
  canv_stat_FE->SetTickx(0);
  canv_stat_FE->SetTicky(0);
  canv_stat_FE->SetLogy();
  leg_FE = tdrLeg(0.55,0.67,0.80,0.92);
  leg_FE->SetTextFont(42);  leg_FE->SetTextSize(0.025);  leg_FE->SetTextColor(kBlack); leg_FE->SetNColumns(2);
  tdrHeader(leg_FE,"Uncertainties", 12);
  TH1* h_FE = new TH1F("", "", eta_bins_FE.size()-1, &eta_bins_FE[0] );
  for (unsigned int i = 0; i < eta_bins_FE.size()-1; i++) { h_FE->SetBinContent(i+1,stat_FE.at(i));}
  leg_FE->AddEntry(h_FE, "stat","l");
  for (unsigned int j = 0; j < systematics.size(); j++){
    TH1F* h_FE = new TH1F("", "", eta_bins_FE.size()-1, &eta_bins_FE[0] );
    h_FE->SetLineWidth(5);
    for (unsigned int i = 0; i < eta_bins_FE.size()-1; i++) {h_FE->SetBinContent(i+1,systematics_FE.at(j).at(i));}
    tdrDraw(h_FE, "hist", kFullDotLarge, colors[j], kSolid, colors[j], 0, colors[j]);
    leg_FE->AddEntry(h_FE, systematics[j],"l");
  }
  tdrDraw(h_FE, "hist", kFullDotLarge, kRed, kSolid, kRed, 0, kRed);
  h_FE->SetLineWidth(8); h_FE->SetLineStyle(9);
  leg_FE->Draw("same");

  canv_stat_FE->Print(path+"Uncertainties_FE.pdf","pdf");
  canv_stat_FE->Print(path+"Uncertainties_FE.png","png");

  std::vector <double> SF_final, SF_final_error, eta_bin_all_center, eta_bin_all_error;
  SF_final.clear();
  SF_final_error.clear();
  eta_bin_all_center.clear();
  eta_bin_all_error.clear();

  for (unsigned int i = 0; i < eta_bins_all.size()-1; i++) {
    if (i < shift_FE){
      eta_bin_all_center.push_back(eta_bin_SM_center.at(i));
      eta_bin_all_error.push_back(eta_bin_SM_error.at(i));
      SF_final.push_back(SF_SM.at(i));
      SF_final_error.push_back(total_error_SM.at(i));
    } else if (i < eta_bins_all.size() - 1 - shift_SM){
      eta_bin_all_center.push_back(eta_bin_SM_center.at(i));
      eta_bin_all_error.push_back(eta_bin_SM_error.at(i));
      SF_final.push_back((SF_SM.at(i)+SF_FE.at(i-shift_FE+shift_barrel+1))/2);
      SF_final_error.push_back(TMath::Sqrt(TMath::Power((stat_SM.at(i)+stat_FE.at(i-shift_FE+shift_barrel+1))/2, 2)+TMath::Power((systematics_SM_all.at(i)+systematics_FE_all.at(i-shift_FE+shift_barrel+1))/2, 2)+TMath::Power((SF_SM.at(i)-SF_FE.at(i-shift_FE+shift_barrel+1))/2, 2)));
    } else {
      eta_bin_all_center.push_back(eta_bin_FE_center.at(i-1));
      eta_bin_all_error.push_back(eta_bin_FE_error.at(i-1));
      SF_final.push_back(SF_FE.at(i-shift_barrel));
      SF_final_error.push_back(total_error_FE.at(i-shift_barrel));
    }
  }


  TCanvas* canv_SF_final = tdrCanvas("SF_final", eta_bins_all.at(0)-0.1, eta_bins_all.at(eta_bins_all.size()-1)+0.5, 0., 3.0, "#eta", "JER SF");
  canv_SF_final->SetTickx(0);
  canv_SF_final->SetTicky(0);
  TLegend *leg_final = tdrLeg(0.65,0.67,0.80,0.92);
  leg_final->SetTextFont(42);  leg_final->SetTextSize(0.025);  leg_final->SetTextColor(kBlack);
  // tdrHeader(leg_final,"Uncertainties", 12);
  TGraphErrors* gr_final = new TGraphErrors(SF_final.size(), &(eta_bin_all_center[0]), &SF_final[0], &(eta_bin_all_error[0]), &SF_final_error[0]);
  // leg_final->AddEntry(gr_final, "stat","f");
  tdrDraw(gr_final, "P5", kFullDotLarge, kCyan+2, kSolid, kCyan+2, 3001, kCyan+2);
  // leg_final->Draw("same");

  canv_SF_final->Print(path+"SF_final.pdf","pdf");
  canv_SF_final->Print(path+"SF_final.png","png");

  ofstream SF_file_final;
  SF_file_final.open (path+"SF_final_tex.txt");
  for (unsigned int i = 0; i < SF_final.size(); i++) {
    SF_file_final << "{{" << eta_bin_all_center[i]+eta_bin_all_error[i] << ", " << SF_final[i] << ", " << SF_final[i]+SF_final_error[i] << ", " << SF_final[i]-SF_final_error[i] << "}},\n";
  }

  SF_file_final.close();
  return true;

}



void plot_SF_systematics() {

  TString path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/JER/wide_eta_binning/file/Single/Fall17_17Nov2017_V10/AK4CHS/";
  TString path_ = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/JER/wide_eta_binning/file/";

  std::vector<TString> studies;
  // studies.push_back("Single");
  // studies.push_back("Single_MB");
  // studies.push_back("LowPtJets");
  // studies.push_back("LowPtJets_MB");
  studies.push_back("StandardPtBins");

  std::vector<TString> JECs;
  JECs.push_back("Fall17_17Nov2017_V10");
  JECs.push_back("Fall17_17Nov2017_V24");

  std::vector<TString> JETs;
  JETs.push_back("AK4CHS");
  JETs.push_back("AK8PUPPI");


  for(TString study : studies){
    for(TString JEC : JECs){
      for(TString JET : JETs){
        path = path_+study+"/"+JEC+"/"+JET+"/";
        if (!gSystem->AccessPathName(path)) {
          std::cout << path << '\n';
          plot_SF_systematics_(path);
          sleep(5);
        }
      }
    }
  }
}
