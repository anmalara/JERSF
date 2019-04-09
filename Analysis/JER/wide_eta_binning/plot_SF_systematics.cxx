#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "TROOT.h"
#include "TMath.h"
#include "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_102X_v1/CMSSW_10_2_10/src/UHH2/JERSF/include/constants.hpp"
#include "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_102X_v1/CMSSW_10_2_10/src/UHH2/PersonalCode/tdrstyle_all.C"


int shift_SM = 8;
int shift_FE = 3;
int shift_barrel = 1;

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

  if (gSystem->AccessPathName(filename)) { std::cout << "check: " << filename << '\n'; return false;}

  while (!file.eof()) {
    getline(file, line);
    std::istringstream iss(line);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    for (unsigned int i = 0; i < results.size(); i++) { SF.at(i).push_back(std::stold(results[i]));}
  }

  SFs.push_back(SF);
  SF.clear();
  file.close();
}

void Load_all_SF(std::vector<std::vector<std::vector<double>>> & SFs, TString path, TString central_SF_txt, std::vector<TString> systematics_name) {
  TString filename = path+central_SF_txt;
  //std::cout << filename << '\n';
  LoadSF(SFs, filename);
  for (unsigned int i = 0; i < systematics_name.size(); i++) {
    TString temp = central_SF_txt.Copy();
    TString sys = systematics_name.at(i);
    // pt_dep values are saved in all the folder. But the one in the nominal folder are calculated wrt to nominal values. It is saved only the Max and Min value of the difference of the SF
    if (sys.Contains("gaustails")) filename = path+temp.ReplaceAll("standard",sys);
    else if (sys.Contains("_")) filename = path+temp.ReplaceAll("standard",sys(0,sys.First("_"))+"/"+sys(sys.First("_")+1,sys.Length()));
    else if (sys.Contains("pTdep")) filename = path+central_SF_txt;
    else filename = path+temp.ReplaceAll("standard",sys);
    //std::cout << filename << '\n';
    std::cout << filename  << " " << sys << '\n';
    LoadSF(SFs, filename);
  }
}

void GetValuesAndUncertainties(std::vector <double> &SF, std::vector <double> &stat, std::vector <double> &eta_bin_center, std::vector <double> &eta_bin_error, std::vector <std::vector <double> > &systematics, std::vector<std::vector<std::vector<double>>> & SFs, std::vector<TString> systematics_name, double etabins, double method, double pt_dep_method) {
  for (unsigned int i = 0; i < 1+systematics_name.size(); i++){
    if (i==0) {
      // Getting Central Values & Statistical Uncertainties
      for (unsigned int j = 0; j < etabins; j++){
        eta_bin_center.push_back(SFs.at(0).at(0).at(j));
        eta_bin_error.push_back(SFs.at(0).at(1).at(j));
        SF.push_back(SFs.at(0).at(method).at(j));
        stat.push_back(SFs.at(0).at(method+1).at(j));
      }
    } else if (i < systematics_name.size()) {
      // Getting Systematics Uncertainties. Diff wrt the nominal value
      std::vector <double> temp;
      for (unsigned int j = 0; j < etabins; j++) temp.push_back(TMath::Abs(SFs.at(0).at(method).at(j) - SFs.at(i).at(method).at(j)));
      systematics.push_back(temp);
      temp.clear();
    }
    else{
      std::vector <double> temp;
      // Getting Systematics Uncertainties for pt_dep. It is saved only the Max and Min value
      for (unsigned int j = 0; j < etabins; j++) temp.push_back((SFs.at(i).at(method+pt_dep_method).at(j) + SFs.at(i).at(method+pt_dep_method+1).at(j))/2);
      systematics.push_back(temp);
      temp.clear();
    }
  }
}

void CalculateTotalError(std::vector <double> &total_error, std::vector <double> &systematics_all, std::vector <std::vector <double> > systematics, std::vector <double> &stat, std::vector<TString> systematics_name, double etabins, std::vector <double> SF, std::vector <double> eta_bin_center, std::vector <double> eta_bin_err) {
  TString text = " \t\t\t & stat ";
  for (unsigned int j = 0; j < systematics_name.size(); j++){
    text +=  " & " + systematics_name.at(j);
  }
  text += "\\\\";
  std::cout << text << '\n';
  for (unsigned int i = 0; i < etabins; i++) {
    double err = 0;
    text = Form("$[ %.2f-%.2f]$ & $%.2f $ & $ %.2f $", eta_bin_center.at(i)-eta_bin_err.at(i), eta_bin_center.at(i)+eta_bin_err.at(i), SF.at(i), stat.at(i)/SF.at(i)*100);
    for (unsigned int j = 0; j < systematics_name.size(); j++) {
      err += TMath::Power(systematics.at(j).at(i),2);
      // std::cout << "err FE " << i << " " << systematics_name.at(j) << " " << systematics.at(j).at(i)/SF.at(i)*100 <<  " " << TMath::Sqrt(err)/SF.at(i)*100 << '\n';
      text += Form(" & $ %.2f $", 100*systematics.at(j).at(i)/SF.at(i));
    }
    err = TMath::Sqrt(err);
    systematics_all.push_back(err);
    total_error.push_back(TMath::Sqrt(TMath::Power(err,2)+TMath::Power(stat.at(i),2)));
    text += Form("&$%.2f$", 100*TMath::Sqrt(TMath::Power(err,2)+TMath::Power(stat.at(i),2)-TMath::Power(systematics.at(0).at(i),2))/SF.at(i));
    text += Form(" & $ %.2f $\\\\", 100*TMath::Sqrt(TMath::Power(err,2)+TMath::Power(stat.at(i),2))/SF.at(i));
    std::cout << text << '\n';
  }
}

void Plot_Uncertainties(TString name_method, std::vector<double> eta_bins, std::vector<double> eta_bin_center, std::vector<double> SF, std::vector<double> eta_bin_error, std::vector<double> stat, std::vector<double> systematics_all, std::vector <std::vector <double> > systematics, std::vector<double> total_error, TString path, std::vector<TString> systematics_name,std::vector<double> SF_2) {
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
  colors.push_back(kCyan+2);


  TCanvas* canv_SF = tdrCanvas("Statistics_"+name_method, eta_bins[0]-0.1, eta_bins[eta_bins.size()-1]+0.1, 0.5, 3.0, "#eta", "JER SF");
  canv_SF->SetTickx(0);
  canv_SF->SetTicky(0);
  TLegend *leg = tdrLeg(0.60,0.67,0.80,0.92, 0.025, 42, kBlack);
  tdrHeader(leg,"Uncertainties", 12);
  TGraphErrors* gr_stat = new TGraphErrors(eta_bins.size()-1, &(eta_bin_center[0]), &SF[0], &(eta_bin_error[0]), &stat[0]);
  TGraphErrors* gr_syst = new TGraphErrors(eta_bins.size()-1, &(eta_bin_center[0]), &SF[0], &(eta_bin_error[0]), &systematics_all[0]);
  TGraphErrors* gr_tot = new TGraphErrors(eta_bins.size()-1, &(eta_bin_center[0]), &SF[0], &(eta_bin_error[0]), &total_error[0]);
  leg->AddEntry(gr_stat, "stat","f");
  leg->AddEntry(gr_syst, "syst","f");
  leg->AddEntry(gr_tot, "stat+syst","f");
  tdrDraw(gr_tot, "P5", kFullDotLarge, kGreen-2, kSolid, kGreen-2, 3005, kGreen-2);
  tdrDraw(gr_syst, "P5", kFullDotLarge, kBlue-4, kSolid, kBlue-4, 3005, kBlue-4);
  tdrDraw(gr_stat, "P5", kFullDotLarge, kRed+1, kSolid, kRed+1, 3005, kRed+1);
  leg->Draw("same");

  canv_SF->Print(path+"SF_"+name_method+".pdf","pdf");
  canv_SF->Print(path+"SF_"+name_method+".png","png");

  TCanvas* canv_stat = tdrCanvas("Uncertainties_"+name_method,eta_bins[0]-0.1, eta_bins[eta_bins.size()-1]+0.1, 0.0001, 100.0, "#eta", "Uncertainties");
  canv_stat->SetTickx(0);
  canv_stat->SetTicky(0);
  canv_stat->SetLogy();
  leg = tdrLeg(0.55,0.67,0.80,0.92, 0.025, 42, kBlack);
  leg->SetNColumns(2);
  tdrHeader(leg,"Uncertainties", 12);
  TH1* h = new TH1F("", "", eta_bins.size()-1, &eta_bins[0] );
  for (unsigned int i = 0; i < eta_bins.size()-1; i++) { h->SetBinContent(i+1,stat.at(i));}
  leg->AddEntry(h, "stat","l");
  for (unsigned int j = 0; j <= systematics_name.size(); j++){
    TH1F* h = new TH1F("", "", eta_bins.size()-1, &eta_bins[0] );
    h->SetLineWidth(5);
    if (j != systematics_name.size()) {
      for (unsigned int i = 0; i < eta_bins.size()-1; i++) {h->SetBinContent(i+1,systematics.at(j).at(i));}
      tdrDraw(h, "hist", kFullDotLarge, colors[j], kSolid, colors[j], 0, colors[j]);
      leg->AddEntry(h, systematics_name[j],"l");
    } else {
      int shift = shift_barrel;
      if (SF.size() > SF_2.size()) shift = - shift_barrel;
      for (unsigned int i = 0; i < eta_bins.size()-1; i++) {
        if (i < shift_FE && SF.size() < SF_2.size()) continue;
        if (i < shift_FE + shift && SF.size() > SF_2.size()) continue;
        if (i<SF_2.size()-1){h->SetBinContent(i+1, (TMath::Abs(SF.at(i)-SF_2.at(i-shift))/2));}
      }
      tdrDraw(h, "hist", kFullDotLarge, colors[j], kSolid, colors[j], 0, colors[j]);
      leg->AddEntry(h, "|SM-FE|/2","l");
    }
  }

  tdrDraw(h, "hist", kFullDotLarge, kRed+1, kSolid, kRed+1, 0, kRed+1);
  h->SetLineWidth(8); h->SetLineStyle(9);
  leg->Draw("same");

  canv_stat->Print(path+"Uncertainties_"+name_method+".pdf","pdf");
  canv_stat->Print(path+"Uncertainties_"+name_method+".png","png");

}


// SFs == (n_systematics, columns in files, eta_bins)
void plot_SF_systematics_(TString path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_10_2_10/src/UHH2/JERSF/Analysis/JER/wide_eta_binning/file/Single/Fall17_17Nov2017_V10/AK4CHS/", TString QCD_DATA = "QCDPt/RunBCDEF/") {
  gPrintViaErrorHandler = kTRUE;
  gErrorIgnoreLevel = kFatal;

  extraText  = "Preliminary";  // default extra text is "Preliminary"
  lumi_13TeV = "[MC 102X] Run2018 41.53 fb^{-1}";
  lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

  if (QCD_DATA.Contains("RunA"))     lumi_13TeV = "[MC 102X] Run2018A     14.00 fb^{-1}";
  if (QCD_DATA.Contains("RunB"))     lumi_13TeV = "[MC 102X] Run2018B      7.10 fb^{-1}";
  if (QCD_DATA.Contains("RunC"))     lumi_13TeV = "[MC 102X] Run2018C      6.94 fb^{-1}";
  if (QCD_DATA.Contains("RunD"))     lumi_13TeV = "[MC 102X] Run2018D     31.93 fb^{-1}";
  if (QCD_DATA.Contains("RunAB"))    lumi_13TeV = "[MC 102X] Run2018AB    14.10 fb^{-1}";
  if (QCD_DATA.Contains("RunABC"))   lumi_13TeV = "[MC 102X] Run2018ABC   28.04 fb^{-1}";
  if (QCD_DATA.Contains("RunABCD"))  lumi_13TeV = "[MC 102X] Run2018ABCD  59.97 fb^{-1}";

  //lumi_13TeV = "[MC Pythia8] RunII";


  std::vector<double> eta_bins_all(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_bins_SM(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double) - shift_SM);
  std::vector<double> eta_bins_FE(eta_bins + shift_barrel, eta_bins + sizeof(eta_bins)/sizeof(double));

  int method = 4; //2-uncorr 4-corr
  int pt_dep_method = 4; //4-min value 5-max value

  std::vector<TString> systematics_name;

  // systematics_name.push_back("gaustails");
  systematics_name.push_back("gaustails_0.95");
  systematics_name.push_back("JEC_up");
  systematics_name.push_back("JEC_down");
  systematics_name.push_back("PU_up");
  systematics_name.push_back("PU_down");
  systematics_name.push_back("PLI_down");
  systematics_name.push_back("PLI_up");
  systematics_name.push_back("alpha");
  systematics_name.push_back("pTdep");
  std::vector<std::vector<std::vector<double>>> SFs_SM, SFs_FE;
  TString central_SM, central_FE, filename;

  // path = "AK4CHS/";
  central_SM = "standard/"+QCD_DATA+"output/scalefactors_ST.txt";
  central_FE = "standard/"+QCD_DATA+"output/scalefactors_FE.txt";

  /////////////////////////
  //   Standard Method   //
  /////////////////////////

  Load_all_SF(SFs_SM, path, central_SM, systematics_name);

  if (SFs_SM.size() != systematics_name.size()+1) {
    std::cout << "ERROR SM" << '\n';
    std::cout << SFs_SM.size() << '\n';
    return false;
  }

  std::vector <double> SF_SM, stat_SM, eta_bin_SM_center, eta_bin_SM_error;
  std::vector <std::vector <double> > systematics_SM;
  std::vector <double> total_error_SM, systematics_SM_all;
  GetValuesAndUncertainties(SF_SM, stat_SM, eta_bin_SM_center, eta_bin_SM_error, systematics_SM, SFs_SM, systematics_name, eta_bins_SM.size()-1, method, pt_dep_method);
  CalculateTotalError(total_error_SM, systematics_SM_all, systematics_SM, stat_SM, systematics_name, eta_bins_SM.size()-1, SF_SM, eta_bin_SM_center, eta_bin_SM_error);

  /////////////////////////
  //  Forward Extention  //
  /////////////////////////

  Load_all_SF(SFs_FE, path, central_FE, systematics_name);

  if (SFs_FE.size() != systematics_name.size()+1) {
    std::cout << "ERROR FE" << '\n';
    return false;
  }

  std::vector <double> SF_FE, stat_FE, eta_bin_FE_center, eta_bin_FE_error;
  std::vector <std::vector <double> > systematics_FE;
  std::vector <double> total_error_FE, systematics_FE_all;
  GetValuesAndUncertainties(SF_FE, stat_FE, eta_bin_FE_center, eta_bin_FE_error, systematics_FE, SFs_FE, systematics_name, eta_bins_FE.size()-1, method, pt_dep_method);
  CalculateTotalError(total_error_FE, systematics_FE_all, systematics_FE, stat_FE, systematics_name, eta_bins_FE.size()-1, SF_FE, eta_bin_FE_center, eta_bin_FE_error);


  Plot_Uncertainties("SM", eta_bins_SM, eta_bin_SM_center, SF_SM, eta_bin_SM_error, stat_SM, systematics_SM_all, systematics_SM, total_error_SM, path+"standard/"+QCD_DATA, systematics_name, SF_FE);
  Plot_Uncertainties("FE", eta_bins_FE, eta_bin_FE_center, SF_FE, eta_bin_FE_error, stat_FE, systematics_FE_all, systematics_FE, total_error_FE, path+"standard/"+QCD_DATA, systematics_name, SF_SM);

  /////////////////////////
  //     Combination     //
  /////////////////////////

  std::vector <double> SF_final, SF_final_error, SF_final_error_stat, SF_final_error_syst, eta_bin_all_center, eta_bin_all_error;
  SF_final.clear();
  SF_final_error.clear();
  SF_final_error_stat.clear();
  SF_final_error_syst.clear();
  eta_bin_all_center.clear();
  eta_bin_all_error.clear();

  for (unsigned int i = 0; i < eta_bins_all.size()-1; i++) {
    if (i < shift_FE){
      eta_bin_all_center.push_back(eta_bin_SM_center.at(i));
      eta_bin_all_error.push_back(eta_bin_SM_error.at(i));
      SF_final.push_back(SF_SM.at(i));
      SF_final_error.push_back(total_error_SM.at(i));
      SF_final_error_stat.push_back(stat_SM.at(i));
      SF_final_error_syst.push_back(systematics_SM_all.at(i));
    } else if (i < eta_bins_all.size() - 1 - shift_SM){
      eta_bin_all_center.push_back(eta_bin_SM_center.at(i));
      eta_bin_all_error.push_back(eta_bin_SM_error.at(i));
      SF_final.push_back((SF_SM.at(i)+SF_FE.at(i-shift_FE+shift_barrel+1))/2);
      // SF_final_error.push_back(TMath::Sqrt(TMath::Power((stat_SM.at(i)+stat_FE.at(i-shift_FE+shift_barrel+1))/2, 2)+TMath::Power((systematics_SM_all.at(i)+systematics_FE_all.at(i-shift_FE+shift_barrel+1))/2, 2)+TMath::Power((SF_SM.at(i)-SF_FE.at(i-shift_FE+shift_barrel+1))/2, 2)));
      SF_final_error.push_back(TMath::Sqrt(stat_SM.at(i)*stat_SM.at(i)+stat_FE.at(i-shift_FE+shift_barrel+1)*stat_FE.at(i-shift_FE+shift_barrel+1) +systematics_SM_all.at(i)*systematics_SM_all.at(i)+systematics_FE_all.at(i-shift_FE+shift_barrel+1)*systematics_FE_all.at(i-shift_FE+shift_barrel+1)+TMath::Power((SF_SM.at(i)-SF_FE.at(i-shift_FE+shift_barrel+1))/2, 2)));
      SF_final_error_stat.push_back(TMath::Sqrt(stat_SM.at(i)*stat_SM.at(i)+stat_FE.at(i-shift_FE+shift_barrel+1)*stat_FE.at(i-shift_FE+shift_barrel+1) ));
      SF_final_error_syst.push_back(TMath::Sqrt(systematics_SM_all.at(i)*systematics_SM_all.at(i)+systematics_FE_all.at(i-shift_FE+shift_barrel+1)*systematics_FE_all.at(i-shift_FE+shift_barrel+1)+TMath::Power((SF_SM.at(i)-SF_FE.at(i-shift_FE+shift_barrel+1))/2, 2)));
    } else {
      eta_bin_all_center.push_back(eta_bin_FE_center.at(i-1));
      eta_bin_all_error.push_back(eta_bin_FE_error.at(i-1));
      SF_final.push_back(SF_FE.at(i-shift_barrel));
      SF_final_error.push_back(total_error_FE.at(i-shift_barrel));
      SF_final_error_stat.push_back(stat_FE.at(i-shift_barrel));
      SF_final_error_syst.push_back(systematics_FE_all.at(i-shift_barrel));
    }
  }


  TCanvas* canv_SF_final = tdrCanvas("SF_final", eta_bins_all.at(0)-0.1, eta_bins_all.at(eta_bins_all.size()-1)+0.5, 0., 3.0, "#eta", "JER SF");
  canv_SF_final->SetTickx(0);
  canv_SF_final->SetTicky(0);
  TLegend *leg_final = tdrLeg(0.65,0.67,0.80,0.92, 0.040, 42, kBlack);
  tdrHeader(leg_final,"", 12);

  double etaSummer16_25nsV1[] = {0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191};
  // double jerSummer16_25nsV1[13][2] = {{1.15953,0.0322243894},{1.19477,0.0337499818}, {1.1464, 0.0298593582},{1.1608608730818335, 0.0314114337}, {1.1277674503328965, 0.0227705413},{1.1000419290738948, 0.0448277854}, {1.1426190202932343, 0.0749862918},{1.1511576138738635, 0.0431038630}, {1.2962786307493799, 0.1221696907},{1.3418116992379743, 0.0896812121}, {1.77881,0.2007462079},{1.18695,0.1243701331}, {1.19218,0.1487939851}};
  double jerSummer16_25nsV1[13][2] = {{1.1595,0.0645},{1.1948,0.0652},{1.1464,0.0632},{1.1609,0.1025},{1.1278,0.0986},{1.1000,0.1079},{1.1426,0.1214},{1.1512,0.1140},{1.2963,0.2371},{1.3418,0.2091},{1.7788,0.2008},{1.1869,0.1243},{1.1922,0.1488}};
  // double jerSummer16_25nsV1[13][2] = {{1.17716, 0.04438},{1.21224, 0.04054},{1.14975, 0.082},{1.1395, 0.06360},{1.15978, 0.1112},{1.20805, 0.089},{1.39324, 0.302},{1.32341, 0.183},{1.58005, 0.598},{2.26888, 0.261},{2.65, 0.337},{1.27321, 0.103},{1.20094, 0.105}};

  // double etaSpring16_25nsV10[] = {0, 0.5, 0.8, 1.11, 1.3, 1.7, 1.9, 2.1, 2.3, 2.5, 2.8, 3.0, 3.2, 5.0};
  double etaSpring16_25nsV10[] = {0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191};
  double jerSpring16_25nsV10[13][2] = {{1.109,0.008},{1.138,0.013},{1.114,0.013},{1.123,0.024},{1.084,0.011},{1.082,0.035},{1.140,0.047},{1.067,0.053},{1.177,0.041},{1.364,0.039},{1.857,0.071},{1.328,0.022},{1.16,0.029}};

  double etaFall17_V3[] = {0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191};
  double jerFall17_V3[13][2] = {{1.1432,0.0222},{1.1815,0.0484},{1.0989,0.0456},{1.1137,0.1397},{1.1307,0.1470},{1.1600,0.0976},{1.2393,0.1909},{1.2604,0.1501},{1.4085,0.2020},{1.9909,0.5684},{2.2923,0.3743},{1.2696,0.1089},{1.1542,0.1524}};

  std::vector<double> etaSummer16_25nsV1_center, etaSummer16_25nsV1_err, SFSummer16_25nsV1, SFSummer16_25nsV1_Err;
  std::vector<double> etaSpring16_25nsV10_center, etaSpring16_25nsV10_err, SFSpring16_25nsV10, SFSpring16_25nsV10_Err;
  std::vector<double> etaFall17_V3_center, etaFall17_V3_err, SFFall17_V3, SFFall17_V3_Err;

  for (unsigned int i = 0; i < 13; i++) {
    etaSummer16_25nsV1_center.push_back((etaSummer16_25nsV1[i+1]+etaSummer16_25nsV1[i])/2);
    etaSummer16_25nsV1_err.push_back((etaSummer16_25nsV1[i+1]-etaSummer16_25nsV1[i])/2);
    SFSummer16_25nsV1.push_back(jerSummer16_25nsV1[i][0]);
    SFSummer16_25nsV1_Err.push_back(jerSummer16_25nsV1[i][1]);

    etaSpring16_25nsV10_center.push_back((etaSpring16_25nsV10[i+1]+etaSpring16_25nsV10[i])/2);
    etaSpring16_25nsV10_err.push_back((etaSpring16_25nsV10[i+1]-etaSpring16_25nsV10[i])/2);
    SFSpring16_25nsV10.push_back(jerSpring16_25nsV10[i][0]);
    SFSpring16_25nsV10_Err.push_back(jerSpring16_25nsV10[i][1]);

    etaFall17_V3_center.push_back((etaFall17_V3[i+1]+etaFall17_V3[i])/2);
    etaFall17_V3_err.push_back((etaFall17_V3[i+1]-etaFall17_V3[i])/2);
    SFFall17_V3.push_back(jerFall17_V3[i][0]);
    SFFall17_V3_Err.push_back(jerFall17_V3[i][1]);
  }


  TGraphErrors* gr_SFSummer16_25nsV1 = new TGraphErrors(SFSummer16_25nsV1.size(), &(etaSummer16_25nsV1_center[0]), &SFSummer16_25nsV1[0], &(etaSummer16_25nsV1_err[0]), &SFSummer16_25nsV1_Err[0]);
  TGraphErrors* gr_SFSpring16_25nsV10 = new TGraphErrors(SFSpring16_25nsV10.size(), &(etaSpring16_25nsV10_center[0]), &SFSpring16_25nsV10[0], &(etaSpring16_25nsV10_err[0]), &SFSpring16_25nsV10_Err[0]);
  TGraphErrors* gr_SFFall17_V3 = new TGraphErrors(SFFall17_V3.size(), &(etaFall17_V3_center[0]), &SFFall17_V3[0], &(etaFall17_V3_err[0]), &SFFall17_V3_Err[0]);
  TGraphErrors* gr_final = new TGraphErrors(SF_final.size(), &(eta_bin_all_center[0]), &SF_final[0], &(eta_bin_all_error[0]), &SF_final_error[0]);
  // tdrDraw(gr_SFSummer16_25nsV1, "P5", kFullDotLarge, kCyan+2, kSolid, kCyan+2, 3001, kCyan+2);
  tdrDraw(gr_SFSummer16_25nsV1, "P5", kFullDotLarge, kRed+1, kSolid, kRed+1, 3005, kRed+1);
  tdrDraw(gr_final, "P5", kFullDotLarge, kBlue-4, kSolid, kBlue-4, 3005, kBlue-4);
  tdrDraw(gr_SFSpring16_25nsV10, "P5", kFullDotLarge, kGreen-1, kSolid, kGreen-1, 3004, kGreen-1);
  tdrDraw(gr_SFFall17_V3, "P5", kFullDotLarge, kOrange-1, kSolid, kOrange-1, 3004, kOrange-1);
  leg_final->AddEntry(gr_SFSummer16_25nsV1, "Summer16_25nsV1","f");
  leg_final->AddEntry(gr_SFSpring16_25nsV10, "Spring16_25nsV10","f");
  leg_final->AddEntry(gr_SFFall17_V3, "Fall17_V3","f");
  leg_final->AddEntry(gr_final,  "Autumn18_V1_"+QCD_DATA(QCD_DATA.Index("Run"), QCD_DATA.Length()-QCD_DATA.Index("Run")-1),"f");
  // leg_final->AddEntry(gr_SFSummer16_25nsV1, "RunF_ECAL","f");
  // leg_final->AddEntry(gr_final,  "RunF","f");
  leg_final->Draw("same");

  canv_SF_final->Print(path+"standard/"+QCD_DATA+"SF_final.pdf","pdf");
  canv_SF_final->Print(path+"standard/"+QCD_DATA+"SF_final.png","png");

  ofstream SF_file_final;
  std::cout << path+"standard/"+QCD_DATA+"SF_final_tex.txt" << '\n';
  SF_file_final.open (path+"standard/"+QCD_DATA+"SF_final_tex.txt");
  for (unsigned int i = 0; i < SF_final.size(); i++) {
    SF_file_final << "{{" << eta_bin_all_center[i]+eta_bin_all_error[i] << ", " << SF_final[i] << ", " << SF_final[i]+SF_final_error[i] << ", " << SF_final[i]-SF_final_error[i] << "}},\n";
  }

  SF_file_final.close();
  std::cout << path+"standard/"+QCD_DATA+"SF_final.pdf" << '\n';

  std::cout << SF_final.size() << '\n';
  std::cout << path+"/standard/"+QCD_DATA << '\n';
  std::cout << "2017 vs 2018" << '\n';
  for (unsigned int i = 0; i < SFSummer16_25nsV1.size(); i++) {
    std::cout << Form("$[ %.2f-%.2f]$ & $%.2f \\pm %.2f $ \\% & $%.2f \\pm %.2f $ \\% \\\\", eta_bin_all_center[i]-eta_bin_all_error[i], eta_bin_all_center[i]+eta_bin_all_error[i], SFFall17_V3[i], SFFall17_V3_Err[i]/SFFall17_V3[i]*100, SF_final[i], SF_final_error[i]/SF_final[i]*100 ) << std::endl;
  }

  for (unsigned int i = 0; i < SFSummer16_25nsV1.size(); i++) {
    std::cout << Form("[ %.2f-%.2f] \t %.2f +- %.2f \% \t %.2f +- %.2f \% ", eta_bin_all_center[i]-eta_bin_all_error[i], eta_bin_all_center[i]+eta_bin_all_error[i], SFFall17_V3[i], SFFall17_V3_Err[i]/SFFall17_V3[i]*100, SF_final[i], SF_final_error[i]/SF_final[i]*100 ) << std::endl;
  }


  ofstream SF_file_twiki;
  SF_file_twiki.open (path+"standard/"+QCD_DATA+"SF_final_twiki.txt");

  std::cout << '\n' << "|  *abs(eta) region* |";
  SF_file_twiki << "|  *abs(eta) region* |";
  for (size_t i = 0; i < eta_bin_all_center.size(); i++) {
    std::cout << Form("|%.3f-%.3f|", eta_bin_all_center[i]-eta_bin_all_error[i], eta_bin_all_center[i]+eta_bin_all_error[i]);
    SF_file_twiki << Form("|%.3f-%.3f|", eta_bin_all_center[i]-eta_bin_all_error[i], eta_bin_all_center[i]+eta_bin_all_error[i]);
  }

  std::cout << '\n' << "|  *Data/MC SF*      |";
  SF_file_twiki << '\n' << "|  *Data/MC SF*      |";
  for (size_t i = 0; i < eta_bin_all_center.size(); i++) {
    std::cout << Form("|%.4f|", SF_final[i]);
    SF_file_twiki << Form("|%.4f|", SF_final[i]);
  }
  std::cout << '\n' << "|  *Stat.Unc*        |";
  SF_file_twiki << '\n' << "|  *Stat.Unc*        |";
  for (size_t i = 0; i < eta_bin_all_center.size(); i++) {
    std::cout << Form("|%.4f|", SF_final_error_stat[i]);
    SF_file_twiki << Form("|%.4f|", SF_final_error_stat[i]);
  }
  std::cout << '\n' << "|  *Syst.Unc*        |";
  SF_file_twiki << '\n' << "|  *Syst.Unc*        |";
  for (size_t i = 0; i < eta_bin_all_center.size(); i++) {
    std::cout << Form("|%.4f|", SF_final_error_syst[i]);
    SF_file_twiki << Form("|%.4f|", SF_final_error_syst[i]);
  }
  std::cout << '\n' << "|  *Total.Unc*       |";
  SF_file_twiki << '\n' << "|  *Total.Unc*       |";
  for (size_t i = 0; i < eta_bin_all_center.size(); i++) {
    std::cout << Form("|%.4f|", SF_final_error[i]);
    SF_file_twiki << Form("|%.4f|", SF_final_error[i]);
  }

  SF_file_twiki.close();


  std::cout << '\n' << "{1 JetEta 0 None ScaleFactor}" << '\n';

  for (unsigned int i = SF_final.size()-1; i > 0 ; i--) { std::cout << Form("%.3f %.3f 3 %.4f %.4f %.4f", -(eta_bin_all_center[i]+eta_bin_all_error[i]), -(eta_bin_all_center[i]-eta_bin_all_error[i]), SF_final[i], SF_final[i]-SF_final_error[i], SF_final[i]+SF_final_error[i] ) << std::endl; }
  for (unsigned int i = 0; i < SF_final.size(); i++) { std::cout << Form("%.3f %.3f 3 %.4f %.4f %.4f",   eta_bin_all_center[i]-eta_bin_all_error[i],    eta_bin_all_center[i]+eta_bin_all_error[i],  SF_final[i], SF_final[i]-SF_final_error[i], SF_final[i]+SF_final_error[i] ) << std::endl; }

  return true;

}



void plot_SF_systematics() {

  TString path ;
  // TString path_ = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_10_2_10/src/UHH2/JERSF/Analysis/JER/wide_eta_binning/file/";
  TString path_ = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_102X_v1/CMSSW_10_2_10/src/UHH2/JERSF/Analysis/JER/wide_eta_binning/file/";

  std::vector<TString> studies;
  studies.push_back("StandardPtBins");

  std::vector<TString> JECs;
  //JECs.push_back("Autumn18_V4");
  //JECs.push_back("Autumn18_V5");
  JECs.push_back("Autumn18_V8");

  std::vector<TString> JETs;
  JETs.push_back("AK4CHS");
  // JETs.push_back("AK8PUPPI");

  std::vector<TString> QCDS;
  //QCDS.push_back("QCD_Flat2018");
  //QCDS.push_back("QCD_Flat");
  QCDS.push_back("QCDHT");

  std::vector<TString> DATAS;
  DATAS.push_back("RunA");
  DATAS.push_back("RunB");
  DATAS.push_back("RunC");
  DATAS.push_back("RunD");
  DATAS.push_back("RunABC");
  DATAS.push_back("RunABCD");



  for(TString study : studies){
    for(TString JEC : JECs){
      for(TString JET : JETs){
        path = path_+study+"/"+JEC+"/"+JET+"/";
        if (!gSystem->AccessPathName(path)) {
          std::cout << path << '\n';
          for(TString QCD : QCDS){
            for(TString DATA : DATAS){
              TString QCD_DATA = QCD+"/"+DATA+"/";
              std::cout << "start: " << QCD_DATA << '\n';
              plot_SF_systematics_(path, QCD_DATA);
              std::cout << "end: " << QCD_DATA << '\n';
              sleep(5);
            }
          }
        }
      }
    }
  }
}
