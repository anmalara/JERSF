#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <TChain.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TFile.h>
#include <TLine.h>
#include <TStyle.h>
#include <TMath.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TGraphAsymmErrors.h>
#include <TFrame.h>
#include <TString.h>

#include "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/include/constants.hpp"
#include "functions.C"
#include "tdrstyle_all.C"


void print1(std::vector< double > a) {
  std::cout << a.size() << '\n';
}

void print1H(std::vector< TH1F* > a) {
  std::cout << a.size() << '\n';
}

void print2(std::vector< std::vector< double > > a) {
  std::cout << a.size() << " " << a.at(0).size() << '\n';
}

void print2H(std::vector< std::vector< TH1F* > > a) {
  std::cout << a.size() << " " << a.at(0).size() << '\n';
}

void print2G(std::vector< std::vector< TGraphErrors* > > a) {
  std::cout << a.size() << " " << a.at(0).size() << '\n';
}

void print3(std::vector< std::vector< std::vector< double > > > a) {
  std::cout << a.size() << " " << a.at(0).size() << " " << a.at(0).at(0).size() << '\n';
}

void print3H(std::vector< std::vector< std::vector< TH1F* > > > a) {
  std::cout << a.size() << " " << a.at(0).size() << " " << a.at(0).at(0).size() << '\n';
}

void asdf(std::vector< std::vector< std::vector< double > > > a) {
  for (size_t i = 0; i < a.size(); i++) {
    std::cout << i << '\n';
    for (size_t j = 0; j < a.at(i).size(); j++) {
      std::cout << j << '\n';
      for (size_t k = 0; k < a.at(i).at(j).size(); k++) {
        // std::cout << a.at(i).at(j).at(k) << "";
        std::cout << *std::max_element(a.at(i).at(j).begin(),a.at(i).at(j).end()) << " ";
      }
      std::cout << '\n';
    }
  }
}

void asdf1(std::vector< std::vector< std::vector< double > > > a) {
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a.at(i).size(); j++) {
      std::cout << i << " " << j << " " << *std::max_element(a.at(i).at(j).begin(),a.at(i).at(j).end()) << " ";
      std::cout << '\n';
    }
  }
}


void asdf2(std::vector< std::vector< std::vector< double > > > a, std::vector< std::vector< std::vector< double > > > b) {
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a.at(i).size(); j++) {
      std::cout << i << " " << j << " " << *std::max_element(a.at(i).at(j).begin(),a.at(i).at(j).end()) << " " << *std::max_element(b.at(i).at(j).begin(),b.at(i).at(j).end()) << " ";
      std::cout << '\n';
    }
  }
}


void asdf23(std::vector< std::vector< std::vector< double > > > a, std::vector< std::vector< std::vector< double > > > b) {
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a.at(i).size(); j++) {
      double diff = TMath::Abs(*std::max_element(a.at(i).at(j).begin(),a.at(i).at(j).end()) - *std::max_element(b.at(i).at(j).begin(),b.at(i).at(j).end()));
      if (diff>5) {
        std::cout << i << " " << j << " " << diff << " ";
        std::cout << '\n';
      }
    }
  }
}


void asdf21(std::vector< std::vector< std::vector< double > > > a, std::vector< std::vector< std::vector< double > > > b) {
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a.at(i).size(); j++) {
      double diff = TMath::Abs(*std::max_element(a.at(i).at(j).begin(),a.at(i).at(j).end()) - *std::max_element(b.at(i).at(j).begin(),b.at(i).at(j).end()));
      if (diff>5) {
        std::cout << i << " " << j << " " << *std::max_element(a.at(i).at(j).begin(),a.at(i).at(j).end()) << " " << *std::max_element(b.at(i).at(j).begin(),b.at(i).at(j).end()) << " ";
        std::cout << '\n';
      }
    }
  }
}


void asdfasdf2(std::vector< std::vector< double > > a, std::vector< std::vector< double > > b, std::vector< std::vector< double > > c){
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a.at(i).size(); j++) {
      double temp = TMath::Sqrt(2)*TMath::Sqrt( a.at(i).at(j) * a.at(i).at(j) - b.at(i).at(j) * b.at(i).at(j) );
      if (TMath::Abs(temp-c.at(i).at(j))>0.0001) {
        std::cout << i << " " << j << " " << temp << " " << c.at(i).at(j) << '\n';
      }
    }
  }
}

// Code by Marek Niedziela
// Based on code by Matthias Schröder, Kristin Goebel

// ----------------------------------------------------------------------------------------------------
#define PLOT_1(h1,c1,m)                                                                     \
TString canvName = h1.at(m)->GetTitle();                                                    \
TString nameXaxis = h1.at(m)->GetXaxis()->GetTitle();                                       \
TString nameYaxis = h1.at(m)->GetYaxis()->GetTitle();                                       \
std::vector<TH1*> vec;                                                                      \
vec.push_back(h1.at(m));                                                                    \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
TCanvas* canv = tdrCanvas(canvName, x_min, x_max, y_min, y_max, nameXaxis, nameYaxis);      \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m), "", kFullCircle, c1 );                                                    \
// ----------------------------------------------------------------------------------------------------
#define PLOT_1_2(h1,c1,m)                                                                   \
canvName = h1.at(m)->GetTitle();                                                            \
nameXaxis = h1.at(m)->GetXaxis()->GetTitle();                                               \
nameYaxis = h1.at(m)->GetYaxis()->GetTitle();                                               \
vec.clear();                                                                                \
vec.push_back(h1.at(m));                                                                    \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
canv = tdrCanvas(canvName, x_min, x_max, y_min, y_max, nameXaxis, nameYaxis);               \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m), "", kFullCircle, c1 );                                                    \
// ----------------------------------------------------------------------------------------------------
#define PLOT_2(h1,h2,c1,c2,m)                                                               \
TString canvName = h1.at(m)->GetTitle();                                                    \
TString nameXaxis = h1.at(m)->GetXaxis()->GetTitle();                                       \
TString nameYaxis = h1.at(m)->GetYaxis()->GetTitle();                                       \
std::vector<TH1*> vec;                                                                      \
vec.push_back(h1.at(m));                                                                    \
vec.push_back(h2.at(m));                                                                    \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
TCanvas* canv = tdrCanvas(canvName, x_min, x_max, y_min, y_max, nameXaxis, nameYaxis);      \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m), "", kFullCircle, c1 );                                                    \
tdrDraw(h2.at(m), "", kFullCircle, c2 );                                                    \
// ----------------------------------------------------------------------------------------------------
#define PLOT_3(h1,h2,h3,c1,c2,c3,m)                                                         \
TString canvName = h1.at(m)->GetTitle();                                                    \
TString nameXaxis = h1.at(m)->GetXaxis()->GetTitle();                                       \
TString nameYaxis = h1.at(m)->GetYaxis()->GetTitle();                                       \
nameXaxis = "p_{T} [GeV]";                                                                  \
std::vector<TH1*> vec;                                                                      \
vec.push_back(h1.at(m));                                                                    \
vec.push_back(h2.at(m));                                                                    \
vec.push_back(h3.at(m));                                                                    \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
TCanvas* canv = tdrCanvas(canvName, x_min, x_max, 0, 3, nameXaxis, nameYaxis);              \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m), "", kFullCircle, c1 );                                                    \
tdrDraw(h2.at(m), "", kFullCircle, c2 );                                                    \
tdrDraw(h3.at(m), "", kFullCircle, c3 );                                                    \
// ----------------------------------------------------------------------------------------------------
#define PLOT_4_MCT(h1,h2,h3,h4,c1,c2,c3,c4,m)                                               \
TString canvName = h1.at(m)->GetTitle();                                                    \
TString nameXaxis = h1.at(m)->GetXaxis()->GetTitle();                                       \
TString nameYaxis = h1.at(m)->GetYaxis()->GetTitle();                                       \
nameXaxis = "p_{T} [GeV]";                                                                  \
std::vector<TH1*> vec;                                                                      \
vec.push_back(h1.at(m));                                                                    \
vec.push_back(h2.at(m));                                                                    \
vec.push_back(h3.at(m));                                                                    \
vec.push_back(h4.at(m));                                                                    \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
TCanvas* canv = tdrCanvas(canvName, x_min, x_max, y_min, y_max, nameXaxis, nameYaxis);      \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m), "", kFullCircle, c1 );                                                    \
tdrDraw(h2.at(m), "", kFullCircle, c2 );                                                    \
tdrDraw(h3.at(m), "", kFullCircle, c3 );                                                    \
tdrDraw(h4.at(m), "", kFullCircle, c4 );                                                    \
// ----------------------------------------------------------------------------------------------------
#define PLOT_3_2(h1,h2,h3,c1,c2,c3,m,p)                                                     \
TString canvName = h1.at(m).at(p)->GetTitle();                                              \
TString nameXaxis = h1.at(m).at(p)->GetXaxis()->GetTitle();                                 \
TString nameYaxis = h1.at(m).at(p)->GetYaxis()->GetTitle();                                 \
std::vector<TH1*> vec;                                                                      \
vec.push_back(h1.at(m).at(p));                                                              \
vec.push_back(h2.at(m).at(p));                                                              \
vec.push_back(h3.at(m).at(p));                                                              \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme2(vec, &x_min, &x_max, &y_min, &y_max);                                          \
TCanvas* canv = tdrCanvas(canvName, 0, 0.35, 0, y_max*1.6, nameXaxis, nameYaxis);           \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m).at(p), "", kFullCircle, c1 );                                              \
tdrDraw(h2.at(m).at(p), "", kFullCircle, c2 );                                              \
tdrDraw(h3.at(m).at(p), "", kFullCircle, c3 );                                              \
// ----------------------------------------------------------------------------------------------------
#define PLOT_3_2_gr(h1,h2,h3,c1,c2,c3,m,p)                                                  \
TString canvName = h1.at(m).at(p)->GetTitle();                                              \
TString nameXaxis = h1.at(m).at(p)->GetXaxis()->GetTitle();                                 \
TString nameYaxis = h1.at(m).at(p)->GetYaxis()->GetTitle();                                 \
std::vector<TGraphErrors*> vec;                                                             \
vec.push_back(h1.at(m).at(p));                                                              \
vec.push_back(h2.at(m).at(p));                                                              \
vec.push_back(h3.at(m).at(p));                                                              \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme_gr(vec, &x_min, &x_max, &y_min, &y_max);                                        \
TCanvas* canv = tdrCanvas(canvName, 0, 0.35, 0, y_max*1.6, "#alpha_{max}", "#sigma_{A}");   \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m).at(p), "P", kFullDotLarge, c1 );                                           \
tdrDraw(h2.at(m).at(p), "P", kFullDotLarge, c2 );                                           \
tdrDraw(h3.at(m).at(p), "P", kFullDotLarge, c3 );                                           \
// ----------------------------------------------------------------------------------------------------
#define PLOT_3_3(h1,h2,h3,c1,c2,c3,m,p,r)                                                   \
TString canvName = h1.at(m).at(p).at(r)->GetTitle();                                        \
TString nameXaxis = h1.at(m).at(p).at(r)->GetXaxis()->GetTitle();                           \
TString nameYaxis = h1.at(m).at(p).at(r)->GetYaxis()->GetTitle();                           \
std::vector<TH1*> vec;                                                                      \
vec.push_back(h1.at(m).at(p).at(r));                                                        \
vec.push_back(h2.at(m).at(p).at(r));                                                        \
vec.push_back(h3.at(m).at(p).at(r));                                                        \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
TCanvas* canv = tdrCanvas(canvName, x_min, x_max, y_min, y_max*1.5, nameXaxis, nameYaxis);  \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m).at(p).at(r), "", kFullCircle, c1 );                                        \
tdrDraw(h2.at(m).at(p).at(r), "", kFullCircle, c2 );                                        \
tdrDraw(h3.at(m).at(p).at(r), "", kFullCircle, c3 );                                        \
// ----------------------------------------------------------------------------------------------------

// ------------------------------
//          MAIN PROGRAM
// ------------------------------
//
// bool data_ = false;
// bool real_data = true;
// const char* filename = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/hist_preparation/MC/wide_eta_bin/file/Single/Fall17_17Nov2017_V10/AK4CHS/histograms_mc_incl_full.root"
// const char* filename_data = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/hist_preparation/data/wide_eta_bin/file/Single/Fall17_17Nov2017_V10/AK4CHS/RunBCDEF/histograms_data_incl_full.root"
//
// TString Trigger = "Single"
//
// double gaustails = 0.985
// float shiftForPLI = 0.0
// int ref_shift = 3
// int shift = ref_shift

int mainRun( bool data_, const char* filename, const char* filename_data, TString lumi, TString label_mc, TString label_data, TString Trigger, double gaustails = 0.985, float shiftForPLI = 0.0, int ref_shift = 3){

  double hist_max_value = 0.3;
  double hist_max_value_SF = 3.0;
  std::cout << "Start" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //    I load all histograms I will need                                   //
  ////////////////////////////////////////////////////////////////////////////

  bool real_data = true; // If false: compare MC to MC:
  // read out genlevel info from both MC and data.
  // If true: compare MC to data:
  // use MC gen info for data as well.
  //bool data = false;     // MC is not data and gets this boolean.

  if ( !real_data && (label_data == "Data" || label_data == "data")){
    std::cout << "WARNING: are you using data?" << std::endl;
    std::cout << "Program has set real_data=" << real_data << std::endl;
    std::cout << "PLI will be corrected with MC gen info only: " << real_data <<std::endl;
  }

  // ------------------------------
  //           bin values
  // ------------------------------

  int EtaBins_SM            = 10; // st method bins
  int EtaBins_SM_control     =  3; // st method bins control
  int EtaBins_FE_reference  =  3; // fe method bins reference
  int EtaBins_FE_control    =  7; // fe method bins control
  int EtaBins_FE            =  3; // fe method bins

  int etaShift_SM           = 0;
  int etaShift_SM_control   = EtaBins_SM;
  int etaShift_FE_reference = 0;
  int etaShift_FE_control   = EtaBins_FE_reference;
  int etaShift_FE           = EtaBins_FE_reference + EtaBins_FE_control;


  std::vector<double> Eta_bins_SM(             eta_bins + etaShift_SM,            eta_bins + etaShift_SM + EtaBins_SM + 1);
  std::vector<double> Eta_bins_SM_control(    eta_bins + etaShift_SM_control,    eta_bins + etaShift_SM_control + EtaBins_SM_control + 1);
  std::vector<double> Eta_bins_FE_reference(  eta_bins + etaShift_FE_reference,  eta_bins + etaShift_FE_reference + EtaBins_FE_reference + 1);
  std::vector<double> Eta_bins_FE_control(     eta_bins + etaShift_FE_control,    eta_bins + etaShift_FE_control + EtaBins_FE_control + 1);
  std::vector<double> Eta_bins_FE(             eta_bins + etaShift_FE,            eta_bins + etaShift_FE + EtaBins_FE + 1);

  int PtBins_Central = 9, PtBins_HF = 6, AlphaBins = 6;

  std::vector<double> Pt_bins_Central, Pt_bins_HF;

  if (Trigger.Contains("Single")) {
    PtBins_Central  = n_pt_bins_Si;
    PtBins_HF       = n_pt_bins_Si_HF;
    for (int i = 0; i < n_pt_bins_Si; i++) Pt_bins_Central.push_back(pt_bins_Si[i]);
    for (int i = 0; i < n_pt_bins_Si_HF; i++) Pt_bins_HF.push_back(pt_bins_Si_HF[i]);
  } else if (Trigger.Contains("DiJet")) {
    PtBins_Central  = n_pt_bins_Di;
    PtBins_HF       = n_pt_bins_Di_HF;
    for (int i = 0; i < n_pt_bins_Di; i++) Pt_bins_Central.push_back(pt_bins_Di[i]);
    for (int i = 0; i < n_pt_bins_Di_HF; i++) Pt_bins_HF.push_back(pt_bins_Di_HF[i]);
  } else if (Trigger.Contains("Single_MB")) {
    PtBins_Central  = n_pt_bins_MB;
    PtBins_HF       = n_pt_bins_MB_HF;
    for (int i = 0; i < n_pt_bins_MB; i++) Pt_bins_Central.push_back(pt_bins_MB[i]);
    for (int i = 0; i < n_pt_bins_MB_HF; i++) Pt_bins_HF.push_back(pt_bins_MB_HF[i]);
  }

  Pt_bins_Central.push_back(1500);
  Pt_bins_HF.push_back(1500);

  std::vector<double> eta_bins_edge_SM(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_bins_edge_FE(eta_bins+1, eta_bins + sizeof(eta_bins)/sizeof(double));

  // EtaBins_FE   = 3;
  // EtaBins_SM       = n_eta_bins - EtaBins_FE - 1;
  // etaShift_FE_control  = eta_bins_edge_FE.size() -1 ;

  std::vector<double> alpha;
  alpha.push_back(0.05); alpha.push_back(0.1);  alpha.push_back(0.15);
  alpha.push_back(0.20); alpha.push_back(0.25); alpha.push_back(0.3);

  TFile *f, *f_data;
  f = new TFile( filename, "READ");
  f_data = new TFile( filename_data, "READ");

  // ------------------------------
  //      loading histograms
  // ------------------------------

  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_SM, gen_asymmetries_SM, asymmetries_data_SM, MC_Truth_asymmetries_SM;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_pt_SM, gen_asymmetries_pt_SM, asymmetries_pt_data_SM;

  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_FE, gen_asymmetries_FE, asymmetries_data_FE, MC_Truth_asymmetries_FE;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_pt_FE, gen_asymmetries_pt_FE, asymmetries_pt_data_FE;

  std::vector< std::vector< std::vector< TH1F* > > > gen_asymmetries_data_SM, gen_asymmetries_data_FE, gen_asymmetries_pt_data_SM, gen_asymmetries_pt_data_FE;

  std::vector< std::vector< std::vector< TH1F* > > > dummy_hists;

  // real_data = true, data = false
  histLoadAsym( *f_data,  real_data,  "asymm_SM",           asymmetries_data_SM,  gen_asymmetries_data_SM,  EtaBins_SM,           PtBins_Central, AlphaBins, etaShift_SM);
  histLoadAsym( *f,       data_,       "asymm_SM",           asymmetries_SM,       gen_asymmetries_SM,       EtaBins_SM,           PtBins_Central, AlphaBins, etaShift_SM);
  histLoadAsym( *f_data,  real_data,  "asymm_SM_control",   asymmetries_data_SM,  gen_asymmetries_data_SM,  EtaBins_SM_control,   PtBins_HF,      AlphaBins, etaShift_SM_control);
  histLoadAsym( *f,       data_,       "asymm_SM_control",   asymmetries_SM,       gen_asymmetries_SM,       EtaBins_SM_control,   PtBins_HF,      AlphaBins, etaShift_SM_control);
  histLoadAsym( *f_data,  real_data,  "asymm_FE_reference", asymmetries_data_FE,  gen_asymmetries_data_FE,  EtaBins_FE_reference, PtBins_Central, AlphaBins, etaShift_FE_reference);
  histLoadAsym( *f,       data_,       "asymm_FE_reference", asymmetries_FE,       gen_asymmetries_FE,       EtaBins_FE_reference, PtBins_Central, AlphaBins, etaShift_FE_reference);
  histLoadAsym( *f_data,  real_data,  "asymm_FE_control",   asymmetries_data_FE,  gen_asymmetries_data_FE,  EtaBins_FE_control,   PtBins_Central, AlphaBins, etaShift_FE_control);
  histLoadAsym( *f,       data_,       "asymm_FE_control",   asymmetries_FE,       gen_asymmetries_FE,       EtaBins_FE_control,   PtBins_Central, AlphaBins, etaShift_FE_control);
  histLoadAsym( *f_data,  real_data,  "asymm_FE",           asymmetries_data_FE,  gen_asymmetries_data_FE,  EtaBins_FE,           PtBins_HF,      AlphaBins, etaShift_FE);
  histLoadAsym( *f,       data_,       "asymm_FE",           asymmetries_FE,       gen_asymmetries_FE,       EtaBins_FE,           PtBins_HF,      AlphaBins, etaShift_FE);

  histLoadAsym( *f_data,  real_data,  "asymmpt_SM",           asymmetries_pt_data_SM,  gen_asymmetries_pt_data_SM,  EtaBins_SM,           PtBins_Central, AlphaBins, etaShift_SM);
  histLoadAsym( *f,       data_,       "asymmpt_SM",           asymmetries_pt_SM,       gen_asymmetries_pt_SM,       EtaBins_SM,           PtBins_Central, AlphaBins, etaShift_SM);
  histLoadAsym( *f_data,  real_data,  "asymmpt_SM_control",   asymmetries_pt_data_SM,  gen_asymmetries_pt_data_SM,  EtaBins_SM_control,   PtBins_HF,      AlphaBins, etaShift_SM_control);
  histLoadAsym( *f,       data_,       "asymmpt_SM_control",   asymmetries_pt_SM,       gen_asymmetries_pt_SM,       EtaBins_SM_control,   PtBins_HF,      AlphaBins, etaShift_SM_control);
  histLoadAsym( *f_data,  real_data,  "asymmpt_FE_reference", asymmetries_pt_data_FE,  gen_asymmetries_pt_data_FE,  EtaBins_FE_reference, PtBins_Central, AlphaBins, etaShift_FE_reference);
  histLoadAsym( *f,       data_,       "asymmpt_FE_reference", asymmetries_pt_FE,       gen_asymmetries_pt_FE,       EtaBins_FE_reference, PtBins_Central, AlphaBins, etaShift_FE_reference);
  histLoadAsym( *f_data,  real_data,  "asymmpt_FE_control",   asymmetries_pt_data_FE,  gen_asymmetries_pt_data_FE,  EtaBins_FE_control,   PtBins_Central, AlphaBins, etaShift_FE_control);
  histLoadAsym( *f,       data_,       "asymmpt_FE_control",   asymmetries_pt_FE,       gen_asymmetries_pt_FE,       EtaBins_FE_control,   PtBins_Central, AlphaBins, etaShift_FE_control);
  histLoadAsym( *f_data,  real_data,  "asymmpt_FE",           asymmetries_pt_data_FE,  gen_asymmetries_pt_data_FE,  EtaBins_FE,           PtBins_HF,      AlphaBins, etaShift_FE);
  histLoadAsym( *f,       data_,       "asymmpt_FE",           asymmetries_pt_FE,       gen_asymmetries_pt_FE,       EtaBins_FE,           PtBins_HF,      AlphaBins, etaShift_FE);

  histLoadAsym( *f,       data_,       "mctruth_SM",           MC_Truth_asymmetries_SM,  dummy_hists,       EtaBins_SM,           PtBins_Central, AlphaBins, etaShift_SM);
  histLoadAsym( *f,       data_,       "mctruth_SM_control",   MC_Truth_asymmetries_SM,  dummy_hists,       EtaBins_SM_control,   PtBins_HF,      AlphaBins, etaShift_SM_control);
  histLoadAsym( *f,       data_,       "mctruth_FE_reference", MC_Truth_asymmetries_FE,  dummy_hists,       EtaBins_FE_reference, PtBins_Central, AlphaBins, etaShift_FE_reference);
  histLoadAsym( *f,       data_,       "mctruth_FE_control",   MC_Truth_asymmetries_FE,  dummy_hists,       EtaBins_FE_control,   PtBins_Central, AlphaBins, etaShift_FE_control);
  histLoadAsym( *f,       data_,       "mctruth_FE",           MC_Truth_asymmetries_FE,  dummy_hists,       EtaBins_FE,           PtBins_HF,      AlphaBins, etaShift_FE);


  print3H(asymmetries_SM);
  print3H(asymmetries_FE);
  std::cout << "1 Done" << '\n';

  std::vector < TH2F* > Map_mean_data;

  Fill_Map3D(asymmetries_data_SM, Map_mean_data, eta_bins_edge_SM, Pt_bins_Central);

  TFile maps("pdfy/maps/maps.root","RECREATE");
  for (unsigned int r = 0; r < alpha.size(); r++) {
    TString canvName = "Map_mean_"; canvName += r;
    TString nameXaxis = "#eta";
    TString nameYaxis = "p_{T} (GeV)";
    TCanvas* canv = tdrCanvas(canvName, 0, 6, 0, 1100, nameXaxis, nameYaxis);
    canv->SetTickx(0);
    canv->SetTicky(0);
    gStyle->SetPalette(kLightTemperature);
    Map_mean_data.at(r)->Draw("colz");
    canv->Update();
    Map_mean_data.at(r)->Write();
    canv -> Print("pdfy/maps/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/maps/"+canvName+".png","png");
  }
  maps.Close();

  ////////////////////////////////////////////////////////////////////////////
  //    I calculate pt_mean for each alpha and pt bin and eta bin.          //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > width_pt_SM, gen_width_pt_SM, width_pt_data_SM, gen_width_pt_data_SM;
  std::vector< std::vector< std::vector< double > > > width_pt_FE, gen_width_pt_FE, width_pt_data_FE, gen_width_pt_data_FE;

  histMeanPt( asymmetries_pt_SM,          width_pt_SM);
  histMeanPt( gen_asymmetries_pt_SM,      gen_width_pt_SM);
  histMeanPt( asymmetries_pt_data_SM,     width_pt_data_SM);
  histMeanPt( gen_asymmetries_pt_data_SM, gen_width_pt_data_SM);

  histMeanPt( asymmetries_pt_FE,          width_pt_FE );
  histMeanPt( gen_asymmetries_pt_FE,      gen_width_pt_FE );
  histMeanPt( asymmetries_pt_data_FE,     width_pt_data_FE );
  histMeanPt( gen_asymmetries_pt_data_FE, gen_width_pt_data_FE );

  // gen_all_data is for running a cross check with smeared MC.

  print3(width_pt_SM);
  print3(width_pt_FE);
  std::cout << "2 Done" << '\n';
  ////////////////////////////////////////////////////////////////////////////
  //    I calculate width of asymmetry distributions only for               //
  //    alpha bins above 10 GeV thresholds (too soft contriubtions)         //
  //    e.g. for bin p_T_ave (55-75) alpha 0.1 corresponds to 57 GeV jets   //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > asymmetries_width_SM, gen_asymmetries_width_SM, asymmetries_width_data_SM, gen_asymmetries_width_data_SM;
  std::vector< std::vector< std::vector< double > > > asymmetries_width_SM_error, gen_asymmetries_width_SM_error, asymmetries_width_data_SM_error, gen_asymmetries_width_data_SM_error;
  std::vector< std::vector< std::vector< double > > > asymmetries_width_FE, gen_asymmetries_width_FE, asymmetries_width_data_FE, gen_asymmetries_width_data_FE;
  std::vector< std::vector< std::vector< double > > > asymmetries_width_FE_error, gen_asymmetries_width_FE_error, asymmetries_width_data_FE_error, gen_asymmetries_width_data_FE_error;

  histWidthAsym( asymmetries_SM , asymmetries_width_SM, asymmetries_width_SM_error, false, gaustails);
  histWidthAsym( gen_asymmetries_SM , gen_asymmetries_width_SM, gen_asymmetries_width_SM_error, false, gaustails);
  histWidthAsym( asymmetries_data_SM , asymmetries_width_data_SM, asymmetries_width_data_SM_error, false, gaustails);
  histWidthAsym( gen_asymmetries_data_SM , gen_asymmetries_width_data_SM, gen_asymmetries_width_data_SM_error, false, gaustails);

  histWidthAsym( asymmetries_FE , asymmetries_width_FE, asymmetries_width_FE_error, false, gaustails);
  histWidthAsym( gen_asymmetries_FE , gen_asymmetries_width_FE, gen_asymmetries_width_FE_error, false, gaustails);
  histWidthAsym( asymmetries_data_FE , asymmetries_width_data_FE, asymmetries_width_data_FE_error, false, gaustails);
  histWidthAsym( gen_asymmetries_data_FE , gen_asymmetries_width_data_FE, gen_asymmetries_width_data_FE_error, false, gaustails);

  ////////////////////////////////////////////////////////////////////////////
  //    I calculate widths, this time also including                        //
  //    alpha bins below 10GeV threshold                                    //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > soft_asymmetries_width_SM, soft_gen_asymmetries_width_SM, soft_asymmetries_width_data_SM, soft_gen_asymmetries_width_data_SM;
  std::vector< std::vector< std::vector< double > > > soft_asymmetries_width_SM_error, soft_gen_asymmetries_width_SM_error, soft_asymmetries_width_data_SM_error, soft_gen_asymmetries_width_data_SM_error;

  std::vector< std::vector< std::vector< double > > > soft_asymmetries_width_FE, gen_soft_asymmetries_width_FE, soft_asymmetries_width_data_FE, gen_soft_asymmetries_width_data_FE;
  std::vector< std::vector< std::vector< double > > > soft_asymmetries_width_FE_error, gen_soft_asymmetries_width_FE_error, soft_asymmetries_width_data_FE_error, gen_soft_asymmetries_width_data_FE_error;

  histWidthAsym( asymmetries_SM , soft_asymmetries_width_SM, soft_asymmetries_width_SM_error, true, gaustails);
  histWidthAsym( gen_asymmetries_SM , soft_gen_asymmetries_width_SM, soft_gen_asymmetries_width_SM_error, true, gaustails);
  histWidthAsym( asymmetries_data_SM , soft_asymmetries_width_data_SM, soft_asymmetries_width_data_SM_error, true, gaustails);
  histWidthAsym( gen_asymmetries_data_SM , soft_gen_asymmetries_width_data_SM, soft_gen_asymmetries_width_data_SM_error, true, gaustails);

  histWidthAsym( asymmetries_FE , soft_asymmetries_width_FE, soft_asymmetries_width_FE_error, true, gaustails);
  histWidthAsym( gen_asymmetries_FE , gen_soft_asymmetries_width_FE, gen_soft_asymmetries_width_FE_error, true, gaustails);
  histWidthAsym( asymmetries_data_FE , soft_asymmetries_width_data_FE, soft_asymmetries_width_data_FE_error, true, gaustails);
  histWidthAsym( gen_asymmetries_data_FE , gen_soft_asymmetries_width_data_FE, gen_soft_asymmetries_width_data_FE_error, true, gaustails);

  print3(asymmetries_width_SM);
  print3(asymmetries_width_FE);
  print3(soft_asymmetries_width_SM);
  print3(soft_asymmetries_width_FE);
  std::cout << "3 Done" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //    Calculate mcTruth resolution for cross check with dijet calculation //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > mcTruth_res_SM, mcTruth_res_FE;
  std::vector< std::vector< std::vector< double > > > mcTruth_res_SM_error, mcTruth_res_FE_error;

  histWidthMCTruth( MC_Truth_asymmetries_SM, mcTruth_res_SM, mcTruth_res_SM_error);
  histWidthMCTruth( MC_Truth_asymmetries_FE, mcTruth_res_FE, mcTruth_res_FE_error);

  print3(mcTruth_res_SM);
  print3(mcTruth_res_FE);
  std::cout << "4 Done" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //     I fill width(alpha_max) histograms                                 //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< TH1F* > > widths_hist_SM, gen_widths_hist_SM, widths_hist_data_SM, gen_widths_hist_data_SM;
  std::vector< std::vector< TH1F* > > widths_hist_FE, gen_widths_hist_FE, widths_hist_data_FE, gen_widths_hist_data_FE;

  fill_widths_hists( "widths", widths_hist_SM , asymmetries_width_SM, asymmetries_width_SM_error );
  fill_widths_hists( "widths_gen", gen_widths_hist_SM , gen_asymmetries_width_SM, gen_asymmetries_width_SM_error );
  fill_widths_hists( "widths_data", widths_hist_data_SM , asymmetries_width_data_SM, asymmetries_width_data_SM_error );
  fill_widths_hists( "widths_gen_data", gen_widths_hist_data_SM , gen_asymmetries_width_data_SM, gen_asymmetries_width_data_SM_error );

  fill_widths_hists( "widths_fe", widths_hist_FE , asymmetries_width_FE, asymmetries_width_FE_error );
  fill_widths_hists( "widths_gen_fe", gen_widths_hist_FE , gen_asymmetries_width_FE, gen_asymmetries_width_FE_error );
  fill_widths_hists( "widths_data_fe", widths_hist_data_FE , asymmetries_width_data_FE, asymmetries_width_data_FE_error );
  fill_widths_hists( "widths_gen_data_fe", gen_widths_hist_data_FE , gen_asymmetries_width_data_FE, gen_asymmetries_width_data_FE_error );

  ////////////////////////////////////////////////////////////////////////////
  //    I do same for alpha unconstrained widths                            //
  //    one needs these plots to prove which points should be rejected!     //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< TH1F* > > soft_widths_hist_SM, soft_gen_widths_hist_SM, soft_widths_hist_data_SM, soft_gen_widths_hist_data_SM;
  std::vector< std::vector< TH1F* > > soft_widths_hist_FE, soft_gen_widths_hist_FE, soft_widths_hist_data_FE, soft_gen_widths_hist_data_FE;

  fill_widths_hists( "all_widths", soft_widths_hist_SM, soft_asymmetries_width_SM, soft_asymmetries_width_SM_error );
  fill_widths_hists( "all_widths_gen", soft_gen_widths_hist_SM, soft_gen_asymmetries_width_SM, soft_gen_asymmetries_width_SM_error );
  fill_widths_hists( "all_widths_data", soft_widths_hist_data_SM, soft_asymmetries_width_data_SM, soft_asymmetries_width_data_SM_error );
  fill_widths_hists( "all_widths_gen_data", soft_gen_widths_hist_data_SM, soft_gen_asymmetries_width_data_SM, soft_gen_asymmetries_width_data_SM_error );

  fill_widths_hists( "all_widths_fe", soft_widths_hist_FE, soft_asymmetries_width_FE, soft_asymmetries_width_FE_error );
  fill_widths_hists( "all_widths_gen_fe", soft_gen_widths_hist_FE, gen_soft_asymmetries_width_FE, gen_soft_asymmetries_width_FE_error );
  fill_widths_hists( "all_widths_data_fe", soft_widths_hist_data_FE, soft_asymmetries_width_data_FE, soft_asymmetries_width_data_FE_error );
  fill_widths_hists( "all_widths_gen_data_fe", soft_gen_widths_hist_data_FE, gen_soft_asymmetries_width_data_FE, gen_soft_asymmetries_width_data_FE_error );

  print2H(widths_hist_SM);
  print2H(widths_hist_FE);
  print2H(soft_widths_hist_SM);
  print2H(soft_widths_hist_FE);
  std::cout << "5 Done" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //    I fit line or const to width(alpha_max)                             //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > extrapolated_widths_SM, extrapolated_gen_widths_SM, extrapolated_widths_data_SM, extrapolated_gen_widths_data_SM;
  std::vector< std::vector< double > > extrapolated_widths_SM_error, extrapolated_gen_widths_SM_error, extrapolated_widths_data_SM_error, extrapolated_gen_widths_data_SM_error;

  std::vector< std::vector< double > > extrapolated_widths_FE, extrapolated_gen_widths_FE, extrapolated_widths_data_FE, extrapolated_gen_widths_data_FE;
  std::vector< std::vector< double > > extrapolated_widths_FE_error, extrapolated_gen_widths_FE_error, extrapolated_widths_data_FE_error, extrapolated_gen_widths_data_FE_error;

  histLinFit( widths_hist_SM , extrapolated_widths_SM, extrapolated_widths_SM_error );
  histLinFit( gen_widths_hist_SM , extrapolated_gen_widths_SM, extrapolated_gen_widths_SM_error );
  histLinFit( widths_hist_data_SM , extrapolated_widths_data_SM, extrapolated_widths_data_SM_error );
  histLinFit( gen_widths_hist_data_SM , extrapolated_gen_widths_data_SM, extrapolated_gen_widths_data_SM_error );

  histLinFit( widths_hist_FE , extrapolated_widths_FE, extrapolated_widths_FE_error );
  histLinFit( gen_widths_hist_FE , extrapolated_gen_widths_FE, extrapolated_gen_widths_FE_error );
  histLinFit( widths_hist_data_FE , extrapolated_widths_data_FE, extrapolated_widths_data_FE_error );
  histLinFit( gen_widths_hist_data_FE , extrapolated_gen_widths_data_FE, extrapolated_gen_widths_data_FE_error );

  print2(extrapolated_widths_SM);
  print2(extrapolated_widths_FE);
  std::cout << "6 Done" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //    Correlated fit                                                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > extrapolated_widths_correlated_SM, extrapolated_gen_widths_correlated_SM, extrapolated_widths_correlated_data_SM, extrapolated_gen_widths_correlated_data_SM;
  std::vector< std::vector< double > > extrapolated_widths_correlated_SM_error, extrapolated_gen_widths_correlated_SM_error, extrapolated_widths_correlated_data_SM_error, extrapolated_gen_widths_correlated_data_SM_error;

  std::vector< std::vector< double > > extrapolated_widths_correlated_FE, extrapolated_gen_widths_correlated_FE, extrapolated_widths_correlated_data_FE, extrapolated_gen_widths_correlated_data_FE;
  std::vector< std::vector< double > > extrapolated_widths_correlated_FE_error, extrapolated_gen_widths_correlated_FE_error, extrapolated_widths_correlated_data_FE_error, extrapolated_gen_widths_correlated_data_FE_error;

  std::vector< std::vector< TGraphErrors *> > MC_correlated_graphs_SM, data_correlated_graphs_SM, gen_correlated_graphs_SM, gen_data_correlated_graphs_SM;
  std::vector< std::vector< TGraphErrors *> > MC_correlated_graphs_FE, data_correlated_graphs_FE, gen_correlated_graphs_FE, gen_data_correlated_graphs_FE;

  histLinCorFit(asymmetries_width_SM, asymmetries_width_SM_error, MC_correlated_graphs_SM, extrapolated_widths_correlated_SM, extrapolated_widths_correlated_SM_error, false);
  histLinCorFit(asymmetries_width_data_SM, asymmetries_width_data_SM_error, data_correlated_graphs_SM, extrapolated_widths_correlated_data_SM, extrapolated_widths_correlated_data_SM_error, false);
  histLinCorFit(gen_asymmetries_width_SM, gen_asymmetries_width_SM_error, gen_correlated_graphs_SM, extrapolated_gen_widths_correlated_SM, extrapolated_gen_widths_correlated_SM_error, false);
  histLinCorFit(gen_asymmetries_width_data_SM, gen_asymmetries_width_data_SM_error, gen_data_correlated_graphs_SM, extrapolated_gen_widths_correlated_data_SM, extrapolated_gen_widths_correlated_data_SM_error, false);

  histLinCorFit(asymmetries_width_FE, asymmetries_width_FE_error, MC_correlated_graphs_FE, extrapolated_widths_correlated_FE, extrapolated_widths_correlated_FE_error, true);
  histLinCorFit(asymmetries_width_data_FE, asymmetries_width_data_FE_error, data_correlated_graphs_FE, extrapolated_widths_correlated_data_FE, extrapolated_widths_correlated_data_FE_error, true);
  histLinCorFit(gen_asymmetries_width_FE, gen_asymmetries_width_FE_error, gen_correlated_graphs_FE, extrapolated_gen_widths_correlated_FE, extrapolated_gen_widths_correlated_FE_error, true);
  histLinCorFit(gen_asymmetries_width_data_FE, gen_asymmetries_width_data_FE_error, gen_data_correlated_graphs_FE, extrapolated_gen_widths_correlated_data_FE, extrapolated_gen_widths_correlated_data_FE_error, true);

  print2(extrapolated_widths_correlated_SM);
  print2(extrapolated_widths_correlated_FE);
  print2G(MC_correlated_graphs_SM);
  print2G(MC_correlated_graphs_FE);
  std::cout << "7 Done" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //    I make histograms ratio of widths(alpha=0.15)                       //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< TH1F* > widths_015_ratios_SM, widths_015_ratios_FE;

  for (size_t i = 0; i < widths_015_ratios_SM.size(); i++) {
    std::cout << widths_015_ratios_SM.at(i)->GetEntries() << '\n';
  }

  widths_015_ratios( "widths_015_SM_ratios", widths_015_ratios_SM, asymmetries_width_data_SM, asymmetries_width_data_SM_error, asymmetries_width_SM, asymmetries_width_SM_error, width_pt_SM );
  widths_015_ratios( "widths_015_FE_ratios", widths_015_ratios_FE, asymmetries_width_data_FE, asymmetries_width_data_FE_error, asymmetries_width_FE, asymmetries_width_FE_error, width_pt_FE );
  print1H(widths_015_ratios_SM);
  print1H(widths_015_ratios_FE);
  std::cout << "8 Done" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //    I correct for PLI using b parameter from line fit to sigma_A(alpha) //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_uncorrelated_corrected_MC_SM, JER_uncorrelated_corrected_data_SM;
  std::vector< std::vector< double > > JER_uncorrelated_corrected_MC_SM_error, JER_uncorrelated_corrected_data_SM_error;

  std::vector< std::vector< double > > JER_uncorrelated_corrected_MC_FE_ref, JER_uncorrelated_corrected_data_FE_ref;
  std::vector< std::vector< double > > JER_uncorrelated_corrected_MC_FE_ref_error, JER_uncorrelated_corrected_data_FE_ref_error;

  correctJERwithPLI( JER_uncorrelated_corrected_MC_SM, JER_uncorrelated_corrected_MC_SM_error, extrapolated_widths_SM, extrapolated_widths_SM_error, extrapolated_gen_widths_SM, extrapolated_gen_widths_SM_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_uncorrelated_corrected_data_SM, JER_uncorrelated_corrected_data_SM_error, extrapolated_widths_data_SM, extrapolated_widths_data_SM_error, extrapolated_gen_widths_SM, extrapolated_gen_widths_SM_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_uncorrelated_corrected_data_SM, JER_uncorrelated_corrected_data_SM_error, extrapolated_widths_data_SM, extrapolated_widths_data_SM_error, extrapolated_gen_widths_data_SM, extrapolated_gen_widths_data_SM_error, shiftForPLI);
  }

  correctJERwithPLI( JER_uncorrelated_corrected_MC_FE_ref, JER_uncorrelated_corrected_MC_FE_ref_error, extrapolated_widths_FE, extrapolated_widths_FE_error, extrapolated_gen_widths_FE, extrapolated_gen_widths_FE_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_uncorrelated_corrected_data_FE_ref, JER_uncorrelated_corrected_data_FE_ref_error, extrapolated_widths_data_FE, extrapolated_widths_data_FE_error, extrapolated_gen_widths_FE, extrapolated_gen_widths_FE_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_uncorrelated_corrected_data_FE_ref, JER_uncorrelated_corrected_data_FE_ref_error, extrapolated_widths_data_FE, extrapolated_widths_data_FE_error, extrapolated_gen_widths_data_FE, extrapolated_gen_widths_data_FE_error, shiftForPLI);
  }

  ////////////////////////////////////////////////////////////////////////////
  //    PLI corrected using b parameters                                    //
  ////////////////////////////////////////////////////////////////////////////
  //    Same correction but for correlated fit results                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_correlated_corrected_MC_SM, JER_correlated_corrected_data_SM;
  std::vector< std::vector< double > > JER_correlated_corrected_MC_SM_error, JER_correlated_corrected_data_SM_error;

  std::vector< std::vector< double > > JER_correlated_corrected_MC_FE_ref, JER_correlated_corrected_data_FE_ref;
  std::vector< std::vector< double > > JER_correlated_corrected_MC_FE_ref_error, JER_correlated_corrected_data_FE_ref_error;

  correctJERwithPLI( JER_correlated_corrected_MC_SM, JER_correlated_corrected_MC_SM_error, extrapolated_widths_correlated_SM, extrapolated_widths_correlated_SM_error, extrapolated_gen_widths_correlated_SM, extrapolated_gen_widths_correlated_SM_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_correlated_corrected_data_SM, JER_correlated_corrected_data_SM_error, extrapolated_widths_correlated_data_SM, extrapolated_widths_correlated_data_SM_error, extrapolated_gen_widths_correlated_SM, extrapolated_gen_widths_correlated_SM_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_correlated_corrected_data_SM, JER_correlated_corrected_data_SM_error, extrapolated_widths_correlated_data_SM, extrapolated_widths_correlated_data_SM_error, extrapolated_gen_widths_correlated_data_SM, extrapolated_gen_widths_correlated_data_SM_error, shiftForPLI);
  }

  correctJERwithPLI( JER_correlated_corrected_MC_FE_ref, JER_correlated_corrected_MC_FE_ref_error, extrapolated_widths_correlated_FE, extrapolated_widths_correlated_FE_error, extrapolated_gen_widths_correlated_FE, extrapolated_gen_widths_correlated_FE_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_correlated_corrected_data_FE_ref, JER_correlated_corrected_data_FE_ref_error, extrapolated_widths_correlated_data_FE, extrapolated_widths_correlated_data_FE_error, extrapolated_gen_widths_correlated_FE, extrapolated_gen_widths_correlated_FE_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_correlated_corrected_data_FE_ref, JER_correlated_corrected_data_FE_ref_error, extrapolated_widths_correlated_data_FE, extrapolated_widths_correlated_data_FE_error, extrapolated_gen_widths_correlated_data_FE, extrapolated_gen_widths_correlated_data_FE_error, shiftForPLI);
  }

  ////////////////////////////////////////////////////////////////////////////
  //    I do the same for widths at alpha = 0.15                            //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER015_MC_SM, JER015_data_SM;
  std::vector< std::vector< double > > JER015_MC_SM_error, JER015_data_SM_error;

  std::vector< std::vector< double > > JER015_MC_FE_ref, JER015_data_FE_ref;
  std::vector< std::vector< double > > JER015_MC_FE_ref_error, JER015_data_FE_ref_error;

  correctJERwithPLI015(JER015_MC_SM, JER015_MC_SM_error, asymmetries_width_SM, asymmetries_width_SM_error, gen_asymmetries_width_SM, gen_asymmetries_width_SM_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI015(JER015_data_SM, JER015_data_SM_error, asymmetries_width_data_SM, asymmetries_width_data_SM_error, gen_asymmetries_width_SM, gen_asymmetries_width_SM_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI015(JER015_data_SM, JER015_data_SM_error, asymmetries_width_data_SM, asymmetries_width_data_SM_error, gen_asymmetries_width_data_SM, gen_asymmetries_width_data_SM_error, shiftForPLI);
  }
  correctJERwithPLI015(JER015_MC_FE_ref, JER015_MC_FE_ref_error, asymmetries_width_FE, asymmetries_width_FE_error, gen_asymmetries_width_FE, gen_asymmetries_width_FE_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI015(JER015_data_FE_ref, JER015_data_FE_ref_error, asymmetries_width_data_FE, asymmetries_width_data_FE_error, gen_asymmetries_width_FE, gen_asymmetries_width_FE_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI015(JER015_data_FE_ref, JER015_data_FE_ref_error, asymmetries_width_data_FE, asymmetries_width_data_FE_error, gen_asymmetries_width_data_FE, gen_asymmetries_width_data_FE_error, shiftForPLI);
  }

  print2(JER_uncorrelated_corrected_MC_SM);
  print2(JER_uncorrelated_corrected_data_FE_ref);
  std::cout << "9 Done" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //    I corrected alpha = 0.15 widhs for PLI correct way                  //
  ////////////////////////////////////////////////////////////////////////////
  //    I correct forward widths for Ref region.                            //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_uncorrelated_corrected_MC_FE, JER_uncorrelated_corrected_data_FE;
  std::vector< std::vector< double > > JER_uncorrelated_corrected_MC_FE_error, JER_uncorrelated_corrected_data_FE_error;

  correctForRef( "mccorrected", JER_uncorrelated_corrected_MC_FE, JER_uncorrelated_corrected_MC_FE_error, JER_uncorrelated_corrected_MC_FE_ref, JER_uncorrelated_corrected_MC_FE_ref_error, width_pt_FE, ref_shift);
  correctForRef( "datacorrect", JER_uncorrelated_corrected_data_FE, JER_uncorrelated_corrected_data_FE_error, JER_uncorrelated_corrected_data_FE_ref, JER_uncorrelated_corrected_data_FE_ref_error, width_pt_FE, ref_shift);

  ////////////////////////////////////////////////////////////////////////////
  //    forward widths corrected for Ref widths!                            //
  ////////////////////////////////////////////////////////////////////////////
  //    same correction for correlated fit results                          //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_correlated_corrected_MC_FE, JER_correlated_corrected_data_FE;
  std::vector< std::vector< double > > JER_correlated_corrected_MC_FE_error, JER_correlated_corrected_data_FE_error;

  correctForRef( "mc_cor_corrected", JER_correlated_corrected_MC_FE,   JER_correlated_corrected_MC_FE_error,   JER_correlated_corrected_MC_FE_ref,   JER_correlated_corrected_MC_FE_ref_error,   width_pt_FE, ref_shift);
  correctForRef( "data_cor_correct", JER_correlated_corrected_data_FE, JER_correlated_corrected_data_FE_error, JER_correlated_corrected_data_FE_ref, JER_correlated_corrected_data_FE_ref_error, width_pt_FE, ref_shift);

  // std::vector<std::vector<double> > Output
  // std::vector<std::vector<double> > OutputError
  // TString name1 = "mc_cor_corrected"
  // std::vector<std::vector<double> > Input = JER_correlated_corrected_MC_FE_ref
  // std::vector<std::vector<double> > InputError = JER_correlated_corrected_MC_FE_ref_error
  // std::vector<std::vector<std::vector<double> > > width_pt = width_pt_FE
  //
  // TMath::Sqrt(2)*TMath::Sqrt(0.056636 * 0.056636 - 0.018630*0.018630)
  //
  // for (size_t i = 0; i < extrapolated_widths_correlated_FE.size(); i++) {
  //   for (size_t j = 0; j < extrapolated_widths_correlated_FE.at(0).size(); j++) {
  //     std::cout << i+1 << " " << j+1 << " " << extrapolated_widths_correlated_FE.at(i).at(j) << '\n';
  //   }
  // }
  //
  // for (size_t i = 0; i < Output.size(); i++) {
  //   for (size_t j = 0; j < Output.at(0).size(); j++) {
  //     double pT = (double)(*std::max_element(width_pt.at(0).at(j).begin(),width_pt.at(0).at(j).end()));
  //     std::cout << i+2 << " " << j+1 << " " << hist->GetBinContent(hist->FindBin(pT)) << " " << Input.at(i+1).at(j) <<  " " << JER_correlated_corrected_MC_FE_ref.at(i+1).at(j) <<  " " << Output.at(i).at(j) << '\n';
  //   }
  // }
  //
  // TMath::Sqrt( 2*0.0634263*0.0634263 - 0.0558558*0.0558558)
  // Output.at(3).at(6)
  //
  //
  //
  // std::vector<std::vector<double> > Output2
  // std::vector<std::vector<double> > OutputError2

  ////////////////////////////////////////////////////////////////////////////
  //    ref region corrected for correlated fit                             //
  ////////////////////////////////////////////////////////////////////////////
  //    and again, Ref region for widths at alpha = 0.15                    //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER015_MC_FE, JER015_data_FE;
  std::vector< std::vector< double > > JER015_MC_FE_error, JER015_data_FE_error;

  correctForRef( "mccorrected015", JER015_MC_FE,   JER015_MC_FE_error,   JER015_MC_FE_ref,   JER015_MC_FE_ref_error,   width_pt_FE, ref_shift);
  correctForRef( "datacorrect015", JER015_data_FE, JER015_data_FE_error, JER015_data_FE_ref, JER015_data_FE_ref_error, width_pt_FE, ref_shift);

  ////////////////////////////////////////////////////////////////////////////
  //    Ref reg corrected for widths at alpha = 0.15                        //
  ////////////////////////////////////////////////////////////////////////////
  //    I make make vectors with ratios of widths                           //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > scales_uncorrelated_SM, scales_uncorrelated_FE, scales_uncorrelated_FE_control;
  std::vector< std::vector< double > > scales_uncorrelated_SM_error, scales_uncorrelated_FE_error, scales_uncorrelated_FE_control_error;

  makeScales( scales_uncorrelated_SM,         scales_uncorrelated_SM_error,         JER_uncorrelated_corrected_data_SM,     JER_uncorrelated_corrected_data_SM_error,     JER_uncorrelated_corrected_MC_SM,     JER_uncorrelated_corrected_MC_SM_error );
  makeScales( scales_uncorrelated_FE,         scales_uncorrelated_FE_error,         JER_uncorrelated_corrected_data_FE,     JER_uncorrelated_corrected_data_FE_error,     JER_uncorrelated_corrected_MC_FE,     JER_uncorrelated_corrected_MC_FE_error );
  makeScales( scales_uncorrelated_FE_control, scales_uncorrelated_FE_control_error, JER_uncorrelated_corrected_data_FE_ref, JER_uncorrelated_corrected_data_FE_ref_error, JER_uncorrelated_corrected_MC_FE_ref, JER_uncorrelated_corrected_MC_FE_ref_error );

  print2(scales_uncorrelated_SM);
  print2(scales_uncorrelated_FE);
  print2(scales_uncorrelated_FE_control);

  print2(JER_uncorrelated_corrected_data_SM);
  print2(JER_uncorrelated_corrected_data_FE);
  print2(JER_uncorrelated_corrected_data_FE_ref);

  // same thing for correlated fit results
  std::vector< std::vector< double > > scales_correlated_SM, scales_correlated_FE, scales_correlated_FE_control;
  std::vector< std::vector< double > > scales_correlated_SM_error, scales_correlated_FE_error, scales_correlated_FE_control_error;

  makeScales( scales_correlated_SM,         scales_correlated_SM_error,         JER_correlated_corrected_data_SM,          JER_correlated_corrected_data_SM_error,          JER_correlated_corrected_MC_SM,          JER_correlated_corrected_MC_SM_error );
  makeScales( scales_correlated_FE,         scales_correlated_FE_error,         JER_correlated_corrected_data_FE,       JER_correlated_corrected_data_FE_error,       JER_correlated_corrected_MC_FE,       JER_correlated_corrected_MC_FE_error );
  makeScales( scales_correlated_FE_control, scales_correlated_FE_control_error, JER_correlated_corrected_data_FE_ref, JER_correlated_corrected_data_FE_ref_error, JER_correlated_corrected_MC_FE_ref, JER_correlated_corrected_MC_FE_ref_error );  // uncorrected for reference region width, as a cross check. (i think)

  std::vector< std::vector< double > > scales015_SM, scales015_FE;
  std::vector< std::vector< double > > scales015_SM_error, scales015_FE_error;

  makeScales( scales015_SM, scales015_SM_error, JER015_data_SM, JER015_data_SM_error, JER015_MC_SM, JER015_MC_SM_error );
  makeScales( scales015_FE, scales015_FE_error, JER015_data_FE, JER015_data_FE_error, JER015_MC_FE, JER015_MC_FE_error );

  ////////////////////////////////////////////////////////////////////////////
  //    I make plots with MCTruth: Res from dijet                           //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< TH1F* > JER_MC_Truth_SM, JER_MC_Truth_FE;

  fill_mctruth_hist( "MC_Truth"    , JER_MC_Truth_SM, mcTruth_res_SM, mcTruth_res_SM_error, width_pt_SM, hist_max_value);
  fill_mctruth_hist( "MC_Truth_Fwd", JER_MC_Truth_FE, mcTruth_res_FE, mcTruth_res_FE_error, width_pt_FE, hist_max_value);

  ////////////////////////////////////////////////////////////////////////////
  //    I make histograms with  JERs and scale factors                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< TH1F* > JER_uncorrelated_MC_hist_SM,         JER_uncorrelated_data_hist_SM,          JER_uncorrelated_scale_hist_SM,         JER015_uncorrelated_MC_hist_SM;
  std::vector< TH1F* > JER_uncorrelated_MC_hist_FE,         JER_uncorrelated_data_hist_FE,          JER_uncorrelated_scale_hist_FE,         JER015_uncorrelated_MC_hist_FE;
  std::vector< TH1F* > JER_uncorrelated_MC_hist_FE_control, JER_uncorrelated_data_hist_FE_control,  JER_FE_uncorrelated_scale_control_hist, JER015_uncorrelated_MC_hist_SM_control;

  fill_hist( "MC_JER_uncorrelated_SM",    JER_uncorrelated_MC_hist_SM,    JER_uncorrelated_corrected_MC_SM,   JER_uncorrelated_corrected_MC_SM_error,   width_pt_SM, hist_max_value);
  fill_hist( "data_JER_uncorrelated_SM",  JER_uncorrelated_data_hist_SM , JER_uncorrelated_corrected_data_SM, JER_uncorrelated_corrected_data_SM_error, width_pt_SM, hist_max_value);
  fill_hist( "SF_uncorrelated_SM",        JER_uncorrelated_scale_hist_SM, scales_uncorrelated_SM,             scales_uncorrelated_SM_error,             width_pt_SM, hist_max_value_SF);

  fill_hist( "MC_JER_uncorrelated_FE",    JER_uncorrelated_MC_hist_FE,    JER_uncorrelated_corrected_MC_FE,   JER_uncorrelated_corrected_MC_FE_error,   width_pt_FE, hist_max_value);
  fill_hist( "data_JER_uncorrelated_FE",  JER_uncorrelated_data_hist_FE,  JER_uncorrelated_corrected_data_FE, JER_uncorrelated_corrected_data_FE_error, width_pt_FE, hist_max_value);
  fill_hist( "SF_uncorrelated_FE",        JER_uncorrelated_scale_hist_FE, scales_uncorrelated_FE,             scales_uncorrelated_FE_error,             width_pt_FE, hist_max_value_SF);

  fill_hist( "MC_JER_uncorrelated_FE_control",    JER_uncorrelated_MC_hist_FE_control,    JER_uncorrelated_corrected_MC_FE_ref,   JER_uncorrelated_corrected_MC_FE_ref_error,   width_pt_FE, hist_max_value);
  fill_hist( "data_JER_uncorrelated_FE_control",  JER_uncorrelated_data_hist_FE_control,  JER_uncorrelated_corrected_data_FE_ref, JER_uncorrelated_corrected_data_FE_ref_error, width_pt_FE, hist_max_value);
  fill_hist( "SF_uncorrelated_FE_control",        JER_FE_uncorrelated_scale_control_hist, scales_uncorrelated_FE_control,         scales_uncorrelated_FE_control_error,           width_pt_FE, hist_max_value_SF);

  fill_hist( "MC_JER015_uncorrelated_SM", JER015_uncorrelated_MC_hist_SM, JER015_MC_SM,                       JER015_MC_SM_error,                       width_pt_SM,      hist_max_value);
  fill_hist( "MC_JER015_uncorrelated_FE", JER015_uncorrelated_MC_hist_FE, JER015_MC_FE,                       JER015_MC_FE_error,                       width_pt_FE,      hist_max_value);
  // fill_hist( "MC_JER015_uncorrelated_FE_control", JER015_uncorrelated_MC_hist_FE_control, JER015_MC_FE_control,                   JER015_MC_FE_error,                           width_pt_FE,      0.2);

  std::vector< TH1F* > JER015_scale_hist_SM, JER015_scale_hist_FE;

  fill_hist( "SF_SM015", JER015_scale_hist_SM,  scales015_SM, scales015_SM_error, width_pt_SM, hist_max_value_SF);
  fill_hist( "SF_FE015", JER015_scale_hist_FE,  scales015_FE, scales015_FE_error, width_pt_FE, hist_max_value_SF);

  std::vector< TH1F* > JER_correlated_MC_hist_SM,         JER_correlated_data_hist_SM,         JER_correlated_scale_hist_SM;
  std::vector< TH1F* > JER_correlated_MC_hist_FE,         JER_correlated_data_hist_FE,         JER_correlated_scale_hist_FE;
  std::vector< TH1F* > JER_correlated_MC_hist_FE_control, JER_correlated_data_hist_FE_control, JER_correlated_scale_hist_FE_control;

  fill_hist( "MC_JER_correlated_SM",   JER_correlated_MC_hist_SM,    JER_correlated_corrected_MC_SM,         JER_correlated_corrected_MC_SM_error,         width_pt_SM, hist_max_value);
  fill_hist( "data_JER_correlated_SM", JER_correlated_data_hist_SM,  JER_correlated_corrected_data_SM,       JER_correlated_corrected_data_SM_error,       width_pt_SM, hist_max_value);
  fill_hist( "SF_correlated_SM",       JER_correlated_scale_hist_SM, scales_correlated_SM, scales_correlated_SM_error, width_pt_SM, hist_max_value_SF);

  fill_hist( "MC_JER_correlated_FE",   JER_correlated_MC_hist_FE,    JER_correlated_corrected_MC_FE,   JER_correlated_corrected_MC_FE_error,   width_pt_FE, hist_max_value);
  fill_hist( "data_JER_correlated_FE", JER_correlated_data_hist_FE,  JER_correlated_corrected_data_FE, JER_correlated_corrected_data_FE_error, width_pt_FE, hist_max_value);
  fill_hist( "SF_correlated_FE",       JER_correlated_scale_hist_FE, scales_correlated_FE,   scales_correlated_FE_error,   width_pt_FE, hist_max_value_SF);

  fill_hist( "MC_JER_correlated_FE_control",   JER_correlated_MC_hist_FE_control,    JER_correlated_corrected_MC_FE_ref,   JER_correlated_corrected_MC_FE_ref_error,   width_pt_FE, hist_max_value);
  fill_hist( "data_JER_correlated_FE_control", JER_correlated_data_hist_FE_control,  JER_correlated_corrected_data_FE_ref, JER_correlated_corrected_data_FE_ref_error, width_pt_FE, hist_max_value);
  fill_hist( "SF_correlated_FE_control",       JER_correlated_scale_hist_FE_control, scales_correlated_FE_control, scales_correlated_FE_control_error, width_pt_FE, hist_max_value_SF);

  print1H(JER_correlated_MC_hist_SM);
  print1H(JER_correlated_MC_hist_FE);
  print1H(JER_correlated_MC_hist_FE_control);

  //////////////////////////////////////////////////////////////////////////////////////////
  //    resolution cross check with mcTruth                                               //
  //////////////////////////////////////////////////////////////////////////////////////////
  int iPeriod = 4;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
  extraText  = "Preliminary";  // default extra text is "Preliminary"
  lumi_13TeV = lumi; //"2.1 fb^{-1}";
  lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  TFile fMCTruth("output/MCTruth.root","RECREATE");
  for( unsigned int m = 0; m < JER_MC_Truth_SM.size(); m++ ){
    PLOT_4_MCT(JER_MC_Truth_SM,JER_uncorrelated_MC_hist_SM,JER_correlated_MC_hist_SM,JER015_uncorrelated_MC_hist_SM,kMagenta,kRed,kBlue,kGreen+2,m)

    char legTitle[100];
    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1]);
    leg->AddEntry(JER_MC_Truth_SM.at(m),                 "MC Truth",         "lep");
    leg->AddEntry(JER_uncorrelated_MC_hist_SM.at(m),     "MC Uncorrelated",  "lep");
    leg->AddEntry(JER_correlated_MC_hist_SM.at(m),       "MC Correlated",    "lep");
    leg->AddEntry(JER015_uncorrelated_MC_hist_SM.at(m),  "MC 015",           "lep");

    canv->Print("pdfy/MCTruth/"+canvName+".pdf","pdf");
    canv->Print("pdfy/MCTruth/"+canvName+".png","png");
  }

  std::cout << "help" << '\n';
  std::cout << JER_MC_Truth_FE.size() << '\n';
  std::cout << JER_uncorrelated_MC_hist_FE.size() << '\n';
  std::cout << JER_correlated_MC_hist_FE.size() << '\n';
  std::cout << JER015_uncorrelated_MC_hist_FE.size() << '\n';


  for( unsigned int m = 0; m < JER_MC_Truth_FE.size()-1; m++ ){

    PLOT_4_MCT(JER_MC_Truth_FE,JER_uncorrelated_MC_hist_FE,JER_correlated_MC_hist_FE,JER015_uncorrelated_MC_hist_FE,kMagenta,kRed,kBlue,kGreen+2,m)

    char legTitle[100];
    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_FE[m], eta_bins_edge_FE[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_MC_Truth_FE.at(m),                "MC Truth",         "lep");
    leg->AddEntry(JER_uncorrelated_MC_hist_FE.at(m),    "MC Uncorrelated",  "lep");
    leg->AddEntry(JER_correlated_MC_hist_FE.at(m),      "MC Correlated",    "lep");
    leg->AddEntry(JER015_uncorrelated_MC_hist_FE.at(m), "MC 015",           "lep");

    canv->Print("pdfy/MCTruth/"+canvName+".pdf","pdf");
    canv->Print("pdfy/MCTruth/"+canvName+".png","png");
  }

  fMCTruth.Close();


  for( unsigned int m = 0; m < asymmetries_SM.size(); m++ ){
    for( unsigned int p = 0; p < asymmetries_SM.at(m).size(); p++ ){
      for( unsigned int r = 0; r < asymmetries_SM.at(m).at(p).size(); r++ ){
        asymmetries_SM.at(m).at(p).at(r)      -> Scale(1./asymmetries_SM.at(m).at(p).at(r) -> Integral());
        gen_asymmetries_SM.at(m).at(p).at(r)  -> Scale(1./gen_asymmetries_SM.at(m).at(p).at(r) -> Integral());
        asymmetries_data_SM.at(m).at(p).at(r) -> Scale(1./asymmetries_data_SM.at(m).at(p).at(r) -> Integral());

        PLOT_3_3(asymmetries_SM,gen_asymmetries_SM,asymmetries_data_SM,kRed,kGreen+2,kBlue,m,p,r)

        char name_mcreco[100];
        char name_mcgen[100];
        char name_data[100];
        char legTitle[100];

        sprintf(name_mcreco,  "MC,   %.4f +- %.4f", asymmetries_width_SM.at(m).at(p).at(r), asymmetries_width_SM_error.at(m).at(p).at(r));
        sprintf(name_mcgen,   "gen,  %.4f +- %.4f", gen_asymmetries_width_SM.at(m).at(p).at(r), gen_asymmetries_width_SM_error.at(m).at(p).at(r));
        sprintf(name_data,    "data, %.4f +- %.4f", asymmetries_width_data_SM.at(m).at(p).at(r), asymmetries_width_data_SM_error.at(m).at(p).at(r));
        sprintf(legTitle,     "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1], Pt_bins_Central[p], Pt_bins_Central[p+1]);

        TLegend *leg = tdrLeg(0.45,0.7,0.95,0.9);
        tdrHeader(leg,legTitle);
        sprintf(legTitle,     "#alpha < %.2f", alpha.at(r));
        leg->AddEntry((TObject*)0, legTitle, "");
        leg->AddEntry(asymmetries_data_SM.at(m).at(p).at(r),  name_data,"lep");
        leg->AddEntry(asymmetries_SM.at(m).at(p).at(r),      name_mcreco,"lep");
        leg->AddEntry(gen_asymmetries_SM.at(m).at(p).at(r), name_mcgen,"lep");
        leg->Draw("same");

        canv->Update();
        canv -> Print("output/asymmetries/"+canvName+".pdf","pdf");
        canv -> Print("output/asymmetries/"+canvName+".png","png");
      }
    }
  }

  for( unsigned int m = 0; m < asymmetries_FE.size(); m++ ){
    for( unsigned int p = 0; p < asymmetries_FE.at(m).size(); p++ ){
      for( unsigned int r = 0; r < asymmetries_FE.at(m).at(p).size(); r++ ){
        asymmetries_FE.at(m).at(p).at(r)      -> Scale(1./asymmetries_FE.at(m).at(p).at(r) -> Integral());
        gen_asymmetries_FE.at(m).at(p).at(r)  -> Scale(1./gen_asymmetries_FE.at(m).at(p).at(r) -> Integral());
        asymmetries_data_FE.at(m).at(p).at(r) -> Scale(1./asymmetries_data_FE.at(m).at(p).at(r) -> Integral());

        PLOT_3_3(asymmetries_FE,gen_asymmetries_FE,asymmetries_data_FE,kRed,kGreen+2,kBlue,m,p,r)

        char name_mcreco[100];
        char name_mcgen[100];
        char name_data[100];
        char legTitle[100];

        sprintf(name_mcreco,  "MC,   %.4f+-%.4f", asymmetries_width_FE.at(m).at(p).at(r), asymmetries_width_FE_error.at(m).at(p).at(r));
        sprintf(name_mcgen,   "gen,  %.4f+-%.4f", gen_asymmetries_width_FE.at(m).at(p).at(r), gen_asymmetries_width_FE_error.at(m).at(p).at(r));
        sprintf(name_data,    "data, %.4f+-%.4f", asymmetries_width_data_FE.at(m).at(p).at(r), asymmetries_width_data_FE_error.at(m).at(p).at(r));
        sprintf(legTitle,     "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
        eta_bins_edge_SM[EtaBins_SM + EtaBins_FE - asymmetries_FE.size() + m],
        eta_bins_edge_SM[EtaBins_SM + EtaBins_FE - asymmetries_FE.size() + m+1],
        Pt_bins_HF[p], Pt_bins_HF[p+1]);

        TLegend *leg = tdrLeg(0.45,0.7,0.95,0.9);
        tdrHeader(leg,legTitle);
        sprintf(legTitle,     "#alpha < %.2f", alpha.at(r));
        leg->AddEntry((TObject*)0, legTitle, "");
        leg->AddEntry(asymmetries_data_FE.at(m).at(p).at(r), name_data,"lep");
        leg->AddEntry(asymmetries_FE.at(m).at(p).at(r),      name_mcreco,"lep");
        leg->AddEntry(gen_asymmetries_FE.at(m).at(p).at(r),  name_mcgen,"lep");

        canv->Update();
        canv -> Print("output/asymmetries/"+canvName+".pdf","pdf");
        canv -> Print("output/asymmetries/"+canvName+".png","png");

      }
    }
  }

  TFile asyroot("output/asymmetries.root","RECREATE");
  for( unsigned int m = 0; m < asymmetries_SM.size(); m++){
    for( unsigned int p = 0; p < asymmetries_SM.at(m).size(); p++){
      for( unsigned int r = 0; r < asymmetries_SM.at(m).at(p).size(); r++){
        asymmetries_SM.at(m).at(p).at(r) -> Write();
        gen_asymmetries_SM.at(m).at(p).at(r) -> Write();
        asymmetries_data_SM.at(m).at(p).at(r) -> Write();
      }
    }
  }

  for( unsigned int m = 0; m < asymmetries_FE.size(); m++){
    for( unsigned int p = 0; p < asymmetries_FE.at(m).size(); p++){
      for( unsigned int r = 0; r < asymmetries_FE.at(m).at(p).size(); r++){
        asymmetries_FE.at(m).at(p).at(r) -> Write();
        gen_asymmetries_FE.at(m).at(p).at(r) -> Write();
        asymmetries_data_FE.at(m).at(p).at(r) -> Write();
      }
    }
  }

  // asyroot.Close();
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Plots with widths(alpha)                                                                                                             //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < widths_hist_SM.size(); m++ ){
    for( unsigned int p = 0; p < widths_hist_SM.at(m).size(); p++ ){
      PLOT_3_2(widths_hist_SM,gen_widths_hist_SM,widths_hist_data_SM,kRed,kGreen+2,kBlue,m,p)

      if( widths_hist_SM.at(m).at(p) -> GetEntries() != 0 )     widths_hist_SM.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kRed);
      if( gen_widths_hist_SM.at(m).at(p) -> GetEntries() != 0 ) gen_widths_hist_SM.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kGreen+2);
      if( widths_hist_data_SM.at(m).at(p) -> GetEntries() != 0 )widths_hist_data_SM.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kBlue);

      TF1* f;
      char line[100];
      TLegend *legend;

      if (widths_hist_SM.at(m).at(p) -> GetFunction("linfit")) {
        f = widths_hist_SM.at(m).at(p) -> GetFunction("linfit");
        legend = tdrLeg(0.50,0.70,0.70,0.85);
        legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
        tdrHeader(legend,"MC", 22);
        sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
        legend->Draw("same");
      }

      if (gen_widths_hist_SM.at(m).at(p) -> GetFunction("linfit")) {
        f = gen_widths_hist_SM.at(m).at(p) -> GetFunction("linfit");
        legend = tdrLeg(0.70,0.70,0.90,0.85);
        legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
        tdrHeader(legend,"gen", 22);
        sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
        legend->Draw("same");
      }

      if (widths_hist_data_SM.at(m).at(p) -> GetFunction("linfit")) {
        f = widths_hist_data_SM.at(m).at(p) -> GetFunction("linfit");
        legend = tdrLeg(0.30,0.70,0.50,0.85);
        legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
        tdrHeader(legend,"Data", 22);
        sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
        legend->Draw("same");
      }

      TLegend *leg = tdrLeg(0.35,0.85,0.9,0.89);
      char legTitle[100];
      sprintf(legTitle, "#eta#in [%.3f,%.3f], p_{T}#in [%.0f,%.0f] GeV", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1], Pt_bins_Central[p], Pt_bins_Central[p+1]);
      tdrHeader(leg,legTitle);
      leg->SetTextFont(42);  leg->SetTextSize(0.035);  leg->SetTextColor(kBlack);
      leg->Draw("same");

      canv -> Update();
      canv -> Print("pdfy/widths/"+canvName+".pdf","pdf");
      canv -> Print("pdfy/widths/"+canvName+".png","png");
    }
  }

  for( unsigned int m = 0; m < widths_hist_FE.size(); m++ ){
    for( unsigned int p = 0; p < widths_hist_FE.at(m).size(); p++ ){
      PLOT_3_2(widths_hist_FE,gen_widths_hist_FE,widths_hist_data_FE,kRed,kGreen+2,kBlue,m,p)

      if( widths_hist_FE.at(m).at(p)->GetEntries() != 0 ){
        widths_hist_FE.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kRed);
      }
      if( gen_widths_hist_FE.at(m).at(p)->GetEntries() != 0 ){
        gen_widths_hist_FE.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kGreen+2);
      }
      if( widths_hist_data_FE.at(m).at(p)->GetEntries() != 0 ){
        widths_hist_data_FE.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kBlue);
      }

      TF1* f;
      char line[100];
      TLegend *legend;

      if( widths_hist_FE.at(m).at(p)->GetEntries() != 0 ){
        f = widths_hist_FE.at(m).at(p) -> GetFunction("linfit");
      }
      legend = tdrLeg(0.50,0.70,0.70,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
      tdrHeader(legend,"MC", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      if( gen_widths_hist_FE.at(m).at(p)->GetEntries() != 0 ){
        f = gen_widths_hist_FE.at(m).at(p) -> GetFunction("linfit");
      }
      legend = tdrLeg(0.70,0.70,0.90,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
      tdrHeader(legend,"gen", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      if( widths_hist_data_FE.at(m).at(p)->GetEntries() != 0 ){
        f = widths_hist_data_FE.at(m).at(p) -> GetFunction("linfit");
      }
      legend = tdrLeg(0.30,0.70,0.50,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      tdrHeader(legend,"Data", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      TLegend *leg = tdrLeg(0.35,0.85,0.9,0.89);
      char legTitle[100];
      sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
      eta_bins_edge_SM[EtaBins_SM + EtaBins_FE - widths_hist_FE.size() + m],
      eta_bins_edge_SM[EtaBins_SM + EtaBins_FE - widths_hist_FE.size() + m+1],
      Pt_bins_HF[p], Pt_bins_HF[p+1]);
      tdrHeader(leg,legTitle);
      leg->SetTextFont(42);  leg->SetTextSize(0.035);  leg->SetTextColor(kBlack);
      leg->Draw("same");

      canv -> Update();
      canv -> Print("pdfy/widths/"+canvName+".pdf","pdf");
      canv -> Print("pdfy/widths/"+canvName+".png","png");
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Plots with all points of widths(alpha)                                                                                               //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < soft_widths_hist_SM.size(); m++ ){
    for( unsigned int p = 0; p < soft_widths_hist_SM.at(m).size(); p++ ){
      PLOT_3_2(soft_widths_hist_SM,soft_gen_widths_hist_SM,soft_widths_hist_data_SM,kRed,kGreen+2,kBlue,m,p)
      canv -> Update();
      canv -> Print("pdfy/widths/allPoints_"+canvName+".png","png");
    }
  }

  for( unsigned int m = 0; m < soft_widths_hist_FE.size(); m++ ){
    for( unsigned int p = 0; p < soft_widths_hist_FE.at(m).size(); p++ ){
      PLOT_3_2(soft_widths_hist_FE,soft_gen_widths_hist_FE,soft_widths_hist_data_FE,kRed,kGreen+2,kBlue,m,p)
      canv -> Update();
      canv -> Print("pdfy/widths/allPoints_"+canvName+".png","png");
    }
  }

  TFile widthroot("output/widths.root","RECREATE");

  for( unsigned int m = 0; m < widths_hist_SM.size(); m++ ){
    for( unsigned int p = 0; p < widths_hist_SM.at(m).size(); p++ ){
      widths_hist_SM.at(m).at(p) -> Write();
      gen_widths_hist_SM.at(m).at(p) -> Write();
      widths_hist_data_SM.at(m).at(p) -> Write();
    }
  }
  for( unsigned int m = 0; m < widths_hist_FE.size(); m++ ){
    for( unsigned int p = 0; p < widths_hist_FE.at(m).size(); p++ ){
      widths_hist_FE.at(m).at(p) -> Write();
      gen_widths_hist_FE.at(m).at(p) -> Write();
      widths_hist_data_FE.at(m).at(p) -> Write();
    }
  }

  widthroot.Close();

  // ------------------------------
  //       correlation plots
  // ------------------------------

  for( unsigned int m = 0; m < MC_correlated_graphs_SM.size(); m++ ){
    for( unsigned int p = 0; p < MC_correlated_graphs_SM.at(m).size(); p++ ){
      // draw extrapolations data + mc
      TF1* f;
      TF1* Temp = new TF1();
      double range = 0.05;
      if ( p == 0) range = 0.25;
      if ( p == 1) range = 0.2;
      if ( p == 2) range = 0.15;
      if ( p == 3) range = 0.1;
      if ( p == 4) range = 0.1;
      if ( p == 5) range = 0.1;
      if ( m == 6 && p == 2) range = 0.2;
      if ( m == 6 && p == 3) range = 0.15;
      if ( m == 6 && p == 6) range = 0.1;
      if ( m == 6 && p == 9) range = 0.1;
      if ( m == 7 && p == 6) range = 0.1;
      if ( m == 7 && p == 7) range = 0.1;
      if ( m == 7 && p == 8) range = 0.1;

      char line[100];
      TLegend *legend;
      // gStyle -> SetOptFit(0000);
      // gStyle -> SetOptStat(0000);
      PLOT_3_2_gr(MC_correlated_graphs_SM,gen_correlated_graphs_SM,data_correlated_graphs_SM,kRed,kGreen+2,kBlue,m,p)

      f = MC_correlated_graphs_SM.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kRed); f->SetLineStyle(2);
      Temp = (TF1*) MC_correlated_graphs_SM.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.50,0.70,0.70,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
      tdrHeader(legend,"MC", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      f = gen_correlated_graphs_SM.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kGreen+2); f->SetLineStyle(2);
      Temp = (TF1*) gen_correlated_graphs_SM.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.70,0.70,0.90,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
      tdrHeader(legend,"gen", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      f = data_correlated_graphs_SM.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kBlue); f->SetLineStyle(2);
      Temp = (TF1*) data_correlated_graphs_SM.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.30,0.70,0.50,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      tdrHeader(legend,"Data", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      TLegend *leg = tdrLeg(0.35,0.85,0.9,0.89);
      char legTitle[100];
      sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1], Pt_bins_Central[p], Pt_bins_Central[p+1]);
      tdrHeader(leg,legTitle);
      leg->SetTextFont(42);  leg->SetTextSize(0.035);  leg->SetTextColor(kBlack);
      leg->Draw("same");

      TString name;
      name = Form("ClosureTest/Extrapol_Eta%i_pt%i.png", m+1, p+1);
      canv->Print(name);
      name = Form("ClosureTest/Extrapol_Eta%i_pt%i.pdf", m+1, p+1);
      canv->Print(name);
    }
  }

  for( unsigned int m = 0; m < MC_correlated_graphs_FE.size(); m++ ){
    for( unsigned int p = 0; p < MC_correlated_graphs_FE.at(m).size(); p++ ){
      // draw extrapolations data + mc
      TF1* f;
      TF1* Temp = new TF1();
      double range = 0.05;
      if ( p == 0) range = 0.25;
      if ( p == 1) range = 0.2;
      if ( p == 2) range = 0.15;
      if ( p == 3) range = 0.1;
      if ( p == 4) range = 0.1;
      if ( p == 5) range = 0.1;
      if ( m == 6 && p == 2) range = 0.2;
      if ( m == 6 && p == 3) range = 0.15;
      if ( m == 6 && p == 6) range = 0.1;
      if ( m == 6 && p == 9) range = 0.1;
      if ( m == 7 && p == 6) range = 0.1;
      if ( m == 7 && p == 7) range = 0.1;
      if ( m == 7 && p == 8) range = 0.1;

      char line[100];
      TLegend *legend;

      PLOT_3_2_gr(MC_correlated_graphs_FE,gen_correlated_graphs_FE,data_correlated_graphs_FE,kRed,kGreen+2,kBlue,m,p)

      f = MC_correlated_graphs_FE.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kRed); f->SetLineStyle(2);
      Temp = (TF1*) MC_correlated_graphs_FE.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.50,0.70,0.70,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
      tdrHeader(legend,"MC", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      f = gen_correlated_graphs_FE.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kGreen+2); f->SetLineStyle(2);
      Temp = (TF1*) gen_correlated_graphs_FE.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.70,0.70,0.90,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
      tdrHeader(legend,"gen", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      f = data_correlated_graphs_FE.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kBlue); f->SetLineStyle(2);
      Temp = (TF1*) data_correlated_graphs_FE.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.30,0.70,0.50,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      tdrHeader(legend,"Data", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      TLegend *leg = tdrLeg(0.35,0.85,0.9,0.89);
      char legTitle[100];
      sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
      eta_bins_edge_SM[EtaBins_SM + EtaBins_FE - MC_correlated_graphs_FE.size() + m],
      eta_bins_edge_SM[EtaBins_SM + EtaBins_FE - MC_correlated_graphs_FE.size() + m+1],
      Pt_bins_HF[p], Pt_bins_HF[p+1]);
      tdrHeader(leg,legTitle);
      leg->SetTextFont(42);  leg->SetTextSize(0.035);  leg->SetTextColor(kBlack);
      leg->Draw("same");

      TString name;
      name = Form("ClosureTest/ExtrapolFw_Eta%i_pt%i.png", m+1, p+1);
      canv->Print(name);
      name = Form("ClosureTest/ExtrapolFw_Eta%i_pt%i.pdf", m+1, p+1);
      canv->Print(name);
    }
  }

  // ------------------------------
  //    end of correlation plots
  // ------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // plot with JERs with NSC fit
  /////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_uncorrelated_data_hist_SM.size(); m++ ){
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1100);
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    mcFIT->SetParameters(0.00015, 0.8, 0.04);
    mcFIT->SetParLimits(0, 0., 0.1);
    mcFIT->SetParLimits(1, 0., 2.);
    mcFIT->SetParLimits(2, 0., 1.);
    JER_uncorrelated_MC_hist_SM.at(m) -> Fit("mcFIT", "RMQ+");
    double N = mcFIT -> GetParameter( 0 );
    double S = mcFIT -> GetParameter( 1 );
    double C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])", 50, 1100);
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(0, N);
    dtFIT -> FixParameter(1, S);
    dtFIT -> FixParameter(2, C);
    dtFIT -> SetParameter(3, 1.);
    dtFIT -> SetParameter(4, 1.);
    dtFIT -> SetParLimits(3, 0., 5);
    dtFIT -> SetParLimits(4, 0., 5);
    JER_uncorrelated_data_hist_SM.at(m) -> Fit("dtFIT", "RMQ+");
    double kNS = dtFIT -> GetParameter( 3 );
    double kC = dtFIT -> GetParameter( 4 );
    JER_uncorrelated_data_hist_SM.at(m)->SetStats(kFALSE);
    JER_uncorrelated_MC_hist_SM.at(m)->SetStats(kFALSE);

    PLOT_2(JER_uncorrelated_data_hist_SM,JER_uncorrelated_MC_hist_SM,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_uncorrelated_data_hist_SM.at(m),"data","lep");
    leg->AddEntry(JER_uncorrelated_MC_hist_SM.at(m),"MC","lep");

    char line[100];
    TLegend *legend;

    legend = tdrLeg(0.50,0.55,0.70,0.7);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
    legend->AddEntry((TObject*)0, "MC", "");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", mcFIT->GetChisquare(), mcFIT->GetNDF());    legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "N = %.5f #pm %.5f", mcFIT->GetParameter(0), mcFIT->GetParError(0));  legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "S = %.5f #pm %.5f", mcFIT->GetParameter(1), mcFIT->GetParError(1));  legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "C = %.5f #pm %.5f", mcFIT->GetParameter(2), mcFIT->GetParError(2));  legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    legend = tdrLeg(0.70,0.55,0.85,0.7);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
    legend->AddEntry((TObject*)0, "data", "");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", dtFIT->GetChisquare(), dtFIT->GetNDF());        legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "k_{NS} = %.5f #pm %.5f", dtFIT->GetParameter(3), dtFIT->GetParError(3)); legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "k_{C}  = %.5f #pm %.5f", dtFIT->GetParameter(4), dtFIT->GetParError(4)); legend->AddEntry((TObject*)0, line, "");
    legend->AddEntry((TObject*)0, "", "");
    legend->Draw("same");

    canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/"+canvName+".png","png");

    PLOT_1_2(JER_uncorrelated_scale_hist_SM,kBlue-4,m)

    TF1 * constfit = new TF1( "constfit", "pol0", 0, 1100 );
    constfit->SetLineColor(kRed+1);
    constfit->SetLineWidth(2);
    JER_uncorrelated_scale_hist_SM.at(m) -> Fit("constfit","RMQ+");

    TF1 *NSC_ratio = new TF1("NSC_ratio","TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])/TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )",0,1100);
    NSC_ratio -> SetParameters(N, S, C, kNS, kC);
    NSC_ratio->SetLineColor(kGreen+2);
    NSC_ratio->SetLineWidth(2);
    NSC_ratio->Draw("same");
    JER_uncorrelated_scale_hist_SM.at(m)->GetListOfFunctions()->Add(NSC_ratio);

    leg = tdrLeg(0.55,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_uncorrelated_scale_hist_SM.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc} uncorrelated","lep");
    leg->AddEntry(constfit,"Constant Fit","l");
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1]);
    leg->AddEntry(NSC_ratio,"Ratio of NSC-Fits","l");

    canv -> Print("pdfy/NSC_SFs/NSC"+canvName+".pdf","pdf");
    canv -> Print("pdfy/NSC_SFs/NSC"+canvName+".png","png");
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // same kind of plot but for correlated results
  /////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_correlated_data_hist_SM.size(); m++ ){ // plot with JERs with NSC fit
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1100);
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    mcFIT->SetParameters(0.00015, 0.8, 0.04);
    mcFIT->SetParLimits(0, 0., 0.1);
    mcFIT->SetParLimits(1, 0., 2.);
    mcFIT->SetParLimits(2, 0., 1.);
    JER_correlated_MC_hist_SM.at(m) -> Fit("mcFIT", "RMQ+");
    double N = mcFIT -> GetParameter( 0 );
    double S = mcFIT -> GetParameter( 1 );
    double C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])", 50, 1100);
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(0, N);
    dtFIT -> FixParameter(1, S);
    dtFIT -> FixParameter(2, C);
    dtFIT -> SetParameter(3, 1.);
    dtFIT -> SetParameter(4, 1.);
    dtFIT -> SetParLimits(3, 0., 5);
    dtFIT -> SetParLimits(4, 0., 5);
    JER_correlated_data_hist_SM.at(m) -> Fit("dtFIT", "RMQ+");
    double kNS = dtFIT -> GetParameter( 3 );
    double kC = dtFIT -> GetParameter( 4 );

    JER_correlated_data_hist_SM.at(m)->SetStats(kFALSE);
    JER_correlated_MC_hist_SM.at(m)->SetStats(kFALSE);

    PLOT_2(JER_correlated_data_hist_SM,JER_correlated_MC_hist_SM,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_correlated_data_hist_SM.at(m),"data","lep");
    leg->AddEntry(JER_correlated_MC_hist_SM.at(m),"MC","lep");

    char line[100];
    TLegend *legend;

    legend = tdrLeg(0.50,0.55,0.70,0.7);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
    legend->AddEntry((TObject*)0, "MC", "");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", mcFIT->GetChisquare(), mcFIT->GetNDF());    legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "N = %.5f #pm %.5f", mcFIT->GetParameter(0), mcFIT->GetParError(0));  legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "S = %.5f #pm %.5f", mcFIT->GetParameter(1), mcFIT->GetParError(1));  legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "C = %.5f #pm %.5f", mcFIT->GetParameter(2), mcFIT->GetParError(2));  legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    legend = tdrLeg(0.70,0.55,0.85,0.7);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
    legend->AddEntry((TObject*)0, "data", "");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", dtFIT->GetChisquare(), dtFIT->GetNDF());        legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "k_{NS} = %.5f #pm %.5f", dtFIT->GetParameter(3), dtFIT->GetParError(3)); legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "k_{C}  = %.5f #pm %.5f", dtFIT->GetParameter(4), dtFIT->GetParError(4)); legend->AddEntry((TObject*)0, line, "");
    legend->AddEntry((TObject*)0, "", "");
    legend->Draw("same");

    canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/"+canvName+".png","png");

    PLOT_1_2(JER_correlated_scale_hist_SM,kBlue-4,m)

    TF1 * constfit = new TF1( "constfit", "pol0", 0, 1100 );
    constfit->SetLineColor(kRed+1);
    constfit->SetLineWidth(2);
    JER_correlated_scale_hist_SM.at(m) -> Fit("constfit","RMQ+");

    TF1 *NSC_ratio = new TF1("NSC_ratio","TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])/TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )",0,1100);
    NSC_ratio -> SetParameters(N, S, C, kNS, kC);
    NSC_ratio->SetLineColor(kGreen+2);
    NSC_ratio->SetLineWidth(2);
    NSC_ratio->Draw("same");
    JER_correlated_scale_hist_SM.at(m)->GetListOfFunctions()->Add(NSC_ratio);

    leg = tdrLeg(0.55,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_correlated_scale_hist_SM.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc} correlated","lep");
    leg->AddEntry(constfit,"Constant Fit","l");
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_SM[m], eta_bins_edge_SM[m+1]);
    leg->AddEntry(NSC_ratio,"Ratio of NSC-Fits","l");

    canv -> Print("pdfy/NSC_SFs/NSC_"+canvName+".pdf","pdf");
    canv -> Print("pdfy/NSC_SFs/NSC_"+canvName+".png","png");
  }


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  JER plots for FE with NSC fits
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_uncorrelated_data_hist_FE.size(); m++ ){
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1100);
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    mcFIT->SetParameters(0.00015, 0.8, 0.04);
    mcFIT->SetParLimits(0, 0., 0.1);
    mcFIT->SetParLimits(1, 0., 2.);
    mcFIT->SetParLimits(2, 0., 1.);
    JER_uncorrelated_MC_hist_FE.at(m) -> Fit("mcFIT", "RMQ+");
    double N = mcFIT -> GetParameter( 0 );
    double S = mcFIT -> GetParameter( 1 );
    double C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])", 50, 1100);
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(0, N);
    dtFIT -> FixParameter(1, S);
    dtFIT -> FixParameter(2, C);
    dtFIT -> SetParameter(3, 1.);
    dtFIT -> SetParameter(4, 1.);
    dtFIT -> SetParLimits(3, 0., 5);
    dtFIT -> SetParLimits(4, 0., 5);
    JER_uncorrelated_data_hist_FE.at(m) -> Fit("dtFIT", "RMQ+");
    double kNS = dtFIT -> GetParameter( 3 );
    double kC = dtFIT -> GetParameter( 4 );
    JER_uncorrelated_data_hist_FE.at(m)->SetStats(kFALSE);
    JER_uncorrelated_MC_hist_FE.at(m)->SetStats(kFALSE);

    PLOT_2(JER_uncorrelated_data_hist_FE,JER_uncorrelated_MC_hist_FE,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_FE[m], eta_bins_edge_FE[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_uncorrelated_data_hist_FE.at(m),"data","lep");
    leg->AddEntry(JER_uncorrelated_MC_hist_FE.at(m),"MC","lep");

    char line[100];
    TLegend *legend;

    legend = tdrLeg(0.50,0.55,0.70,0.7);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
    legend->AddEntry((TObject*)0, "MC", "");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", mcFIT->GetChisquare(), mcFIT->GetNDF());    legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "N = %.5f #pm %.5f", mcFIT->GetParameter(0), mcFIT->GetParError(0));  legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "S = %.5f #pm %.5f", mcFIT->GetParameter(1), mcFIT->GetParError(1));  legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "C = %.5f #pm %.5f", mcFIT->GetParameter(2), mcFIT->GetParError(2));  legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    legend = tdrLeg(0.70,0.55,0.85,0.7);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
    legend->AddEntry((TObject*)0, "data", "");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", dtFIT->GetChisquare(), dtFIT->GetNDF());        legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "k_{NS} = %.5f #pm %.5f", dtFIT->GetParameter(3), dtFIT->GetParError(3)); legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "k_{C}  = %.5f #pm %.5f", dtFIT->GetParameter(4), dtFIT->GetParError(4)); legend->AddEntry((TObject*)0, line, "");
    legend->AddEntry((TObject*)0, "", "");
    legend->Draw("same");

    if (m<9) canvName = canvName(0, canvName.Length()-1);
    else canvName = canvName(0, canvName.Length()-2);
    canvName += (m+2);
    canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/"+canvName+".png","png");

    PLOT_1_2(JER_uncorrelated_scale_hist_FE,kBlue-4,m)

    TF1 * constfit = new TF1( "constfit", "pol0", 0, 1100 );
    constfit->SetLineColor(kRed+1);
    constfit->SetLineWidth(2);
    JER_uncorrelated_scale_hist_FE.at(m) -> Fit("constfit","RMQ+");

    TF1 *NSC_ratio = new TF1("NSC_ratio","TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])/TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )",0,1100);
    NSC_ratio -> SetParameters(N, S, C, kNS, kC);
    NSC_ratio->SetLineColor(kGreen+2);
    NSC_ratio->SetLineWidth(2);
    NSC_ratio->Draw("same");
    JER_uncorrelated_scale_hist_FE.at(m)->GetListOfFunctions()->Add(NSC_ratio);

    leg = tdrLeg(0.55,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_FE[m], eta_bins_edge_FE[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_uncorrelated_scale_hist_FE.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc} uncorrelated","lep");
    leg->AddEntry(constfit,"Constant Fit","l");
    leg->AddEntry(NSC_ratio,"Ratio of NSC-Fits","l");

    if (m<9) canvName = canvName(0, canvName.Length()-1);
    else canvName = canvName(0, canvName.Length()-2);
    canvName += (m+2);
    canv -> Print("pdfy/NSC_SFs/NSC"+canvName+".pdf","pdf");
    canv -> Print("pdfy/NSC_SFs/NSC"+canvName+".png","png");
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // same kind of plot but for correlated results
  /////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_correlated_data_hist_FE.size(); m++ ){ // plot with JERs with NSC fit
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1100);
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    mcFIT->SetParameters(0.00015, 0.8, 0.04);
    mcFIT->SetParLimits(0, 0., 0.1);
    mcFIT->SetParLimits(1, 0., 2.);
    mcFIT->SetParLimits(2, 0., 1.);
    JER_correlated_MC_hist_FE.at(m) -> Fit("mcFIT", "RMQ+");
    double N = mcFIT -> GetParameter( 0 );
    double S = mcFIT -> GetParameter( 1 );
    double C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])", 50, 1100);
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(0, N);
    dtFIT -> FixParameter(1, S);
    dtFIT -> FixParameter(2, C);
    dtFIT -> SetParameter(3, 1.);
    dtFIT -> SetParameter(4, 1.);
    dtFIT -> SetParLimits(3, 0., 5);
    dtFIT -> SetParLimits(4, 0., 5);
    JER_correlated_data_hist_FE.at(m) -> Fit("dtFIT", "RMQ+");
    double kNS = dtFIT -> GetParameter( 3 );
    double kC = dtFIT -> GetParameter( 4 );
    JER_correlated_data_hist_FE.at(m)->SetStats(kFALSE);
    JER_correlated_MC_hist_FE.at(m)->SetStats(kFALSE);

    PLOT_2(JER_correlated_data_hist_FE,JER_correlated_MC_hist_FE,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_FE[m], eta_bins_edge_FE[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_correlated_data_hist_FE.at(m),"data","lep");
    leg->AddEntry(JER_correlated_MC_hist_FE.at(m),"MC","lep");

    char line[100];
    TLegend *legend;

    legend = tdrLeg(0.50,0.55,0.70,0.7);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
    legend->AddEntry((TObject*)0, "MC", "");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", mcFIT->GetChisquare(), mcFIT->GetNDF());    legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "N = %.5f #pm %.5f", mcFIT->GetParameter(0), mcFIT->GetParError(0));  legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "S = %.5f #pm %.5f", mcFIT->GetParameter(1), mcFIT->GetParError(1));  legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "C = %.5f #pm %.5f", mcFIT->GetParameter(2), mcFIT->GetParError(2));  legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    legend = tdrLeg(0.70,0.55,0.85,0.7);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
    legend->AddEntry((TObject*)0, "data", "");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", dtFIT->GetChisquare(), dtFIT->GetNDF());        legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "k_{NS} = %.5f #pm %.5f", dtFIT->GetParameter(3), dtFIT->GetParError(3)); legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "k_{C}  = %.5f #pm %.5f", dtFIT->GetParameter(4), dtFIT->GetParError(4)); legend->AddEntry((TObject*)0, line, "");
    legend->AddEntry((TObject*)0, "", "");
    legend->Draw("same");

    if (m<9) canvName = canvName(0, canvName.Length()-1);
    else canvName = canvName(0, canvName.Length()-2);
    canvName += (m+2);
    canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/"+canvName+".png","png");

    PLOT_1_2(JER_correlated_scale_hist_FE,kBlue-4,m)

    TF1 * constfit = new TF1( "constfit", "pol0", 0, 1100 );
    constfit->SetLineColor(kRed+1);
    constfit->SetLineWidth(2);
    JER_correlated_scale_hist_FE.at(m) -> Fit("constfit","RMQ+");

    TF1 *NSC_ratio = new TF1("NSC_ratio","TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])/TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )",0,1100);
    NSC_ratio -> SetParameters(N, S, C, kNS, kC);
    NSC_ratio->SetLineColor(kGreen+2);
    NSC_ratio->SetLineWidth(2);
    NSC_ratio->Draw("same");
    JER_correlated_scale_hist_FE.at(m)->GetListOfFunctions()->Add(NSC_ratio);

    leg = tdrLeg(0.55,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_FE[m], eta_bins_edge_FE[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_correlated_scale_hist_FE.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc} correlated","lep");
    leg->AddEntry(constfit,"Constant Fit","l");
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge_FE[m], eta_bins_edge_FE[m+1]);
    leg->AddEntry(NSC_ratio,"Ratio of NSC-Fits","l");

    if (m<9) canvName = canvName(0, canvName.Length()-1);
    else canvName = canvName(0, canvName.Length()-2);
    canvName += (m+2);
    canv -> Print("pdfy/NSC_SFs/NSC_"+canvName+".pdf","pdf");
    canv -> Print("pdfy/NSC_SFs/NSC_"+canvName+".png","png");
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TFile JERroot("output/JERs.root","RECREATE");

  for( unsigned int m = 0; m < JER_uncorrelated_MC_hist_SM.size(); m++ ){              JER_uncorrelated_MC_hist_SM.at(m)              -> Write();}
  for( unsigned int m = 0; m < JER_uncorrelated_data_hist_SM.size(); m++ ){            JER_uncorrelated_data_hist_SM.at(m)            -> Write();}
  for( unsigned int m = 0; m < JER_uncorrelated_MC_hist_FE.size(); m++ ){           JER_uncorrelated_MC_hist_FE.at(m)           -> Write();}
  for( unsigned int m = 0; m < JER_uncorrelated_data_hist_FE.size(); m++ ){         JER_uncorrelated_data_hist_FE.at(m)         -> Write();}
  for( unsigned int m = 0; m < JER_uncorrelated_MC_hist_FE_control.size(); m++ ){   JER_uncorrelated_MC_hist_FE_control.at(m)   -> Write();}
  for( unsigned int m = 0; m < JER_uncorrelated_data_hist_FE_control.size(); m++ ){ JER_uncorrelated_data_hist_FE_control.at(m) -> Write();}
  for( unsigned int m = 0; m < JER_correlated_MC_hist_SM.size(); m++ ){                JER_correlated_MC_hist_SM.at(m)                -> Write();}
  for( unsigned int m = 0; m < JER_correlated_data_hist_SM.size(); m++ ){              JER_correlated_data_hist_SM.at(m)              -> Write();}
  for( unsigned int m = 0; m < JER_correlated_MC_hist_FE.size(); m++ ){             JER_correlated_MC_hist_FE.at(m)             -> Write();}
  for( unsigned int m = 0; m < JER_correlated_data_hist_FE.size(); m++ ){           JER_correlated_data_hist_FE.at(m)           -> Write();}
  for( unsigned int m = 0; m < JER_correlated_MC_hist_FE_control.size(); m++ ){     JER_correlated_MC_hist_FE_control.at(m)     -> Write();}
  for( unsigned int m = 0; m < JER_correlated_data_hist_FE_control.size(); m++ ){   JER_correlated_data_hist_FE_control.at(m)   -> Write();}

  JERroot.Close();

  TFile gbis("output/SFs.root","RECREATE");

  // for( unsigned int m = 0; m < JER_FE_uncorrelated_scale_control_hist.size(); m++ ){
  //   TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  //   JER_FE_uncorrelated_scale_control_hist.at(m) -> Fit("constfit","RMQ+");
  //   JER_FE_uncorrelated_scale_control_hist.at(m) -> Write();
  // }
  //
  // for( unsigned int m = 0; m < JER015_scale_hist_SM.size(); m++ ){
  //   TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  //   JER015_scale_hist_SM.at(m) -> Fit("constfit","RMQ+");
  //   JER015_scale_hist_SM.at(m) -> Write();
  // }
  //
  // for( unsigned int m = 0; m < JER015_scale_hist_FE.size(); m++ ){
  //   TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  //   JER015_scale_hist_FE.at(m) -> Fit("constfit","RMQ+");
  //   JER015_scale_hist_FE.at(m) -> Write();
  // }

  std::vector<double> SF_uncorrelated_SM, SF_uncorrelated_SM_error, SF_correlated_SM, SF_correlated_SM_error, SF_uncorrelated_FE, SF_uncorrelated_FE_error, SF_correlated_FE, SF_correlated_FE_error, eta_bin_SM_center, eta_bin_SM_error, eta_bin_FE_center, eta_bin_FE_error;
  std::vector<double> SF_uncorrelated_SM_ptdep_min, SF_correlated_SM_ptdep_min, SF_uncorrelated_FE_ptdep_min, SF_correlated_FE_ptdep_min;
  std::vector<double> SF_uncorrelated_SM_ptdep_max, SF_correlated_SM_ptdep_max, SF_uncorrelated_FE_ptdep_max, SF_correlated_FE_ptdep_max;
  ofstream texfile;
  texfile.open ("output/scalefactors_tex.txt");

  texfile << "standard method\n";
  for( unsigned int m = 0; m < JER_uncorrelated_scale_hist_SM.size(); m++ ){
    TF1 * constfit = JER_uncorrelated_scale_hist_SM.at(m) -> GetFunction("constfit");
    TF1 * NSC_ratio = JER_uncorrelated_scale_hist_SM.at(m) -> GetFunction("NSC_ratio");
    JER_uncorrelated_scale_hist_SM.at(m)->GetFunction("NSC_ratio")->SetBit(TF1::kNotDraw);
    SF_uncorrelated_SM_ptdep_min.push_back(findMinMax(width_pt_data_SM.at(m)[0], NSC_ratio, constfit, 0));
    SF_uncorrelated_SM_ptdep_max.push_back(findMinMax(width_pt_data_SM.at(m)[0], NSC_ratio, constfit, 1));
    JER_uncorrelated_scale_hist_SM.at(m) -> Write();
    eta_bin_SM_center.push_back((eta_bins_edge_SM[m+1]+eta_bins_edge_SM[m]) /2);
    eta_bin_SM_error.push_back((eta_bins_edge_SM[m+1]-eta_bins_edge_SM[m]) /2);
    SF_uncorrelated_SM.push_back(constfit -> GetParameter( 0 ));
    SF_uncorrelated_SM_error.push_back(constfit -> GetParError( 0 ));
    texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
    if(m == JER_uncorrelated_scale_hist_SM.size()-1 ) texfile << "\\\\";
  }

  texfile << "\n forward extension \n";
  for( unsigned int m = 0; m < JER_uncorrelated_scale_hist_FE.size(); m++ ){
    TF1 * constfit = JER_uncorrelated_scale_hist_FE.at(m) -> GetFunction("constfit");
    TF1 * NSC_ratio = JER_uncorrelated_scale_hist_FE.at(m) -> GetFunction("NSC_ratio");
    JER_uncorrelated_scale_hist_FE.at(m)->GetFunction("NSC_ratio")->SetBit(TF1::kNotDraw);
    SF_uncorrelated_FE_ptdep_min.push_back(findMinMax(width_pt_data_FE.at(m)[0], NSC_ratio, constfit, 0));
    SF_uncorrelated_FE_ptdep_max.push_back(findMinMax(width_pt_data_FE.at(m)[0], NSC_ratio, constfit, 1));
    JER_uncorrelated_scale_hist_FE.at(m) -> Write();
    int diff = EtaBins_SM + EtaBins_FE - JER_uncorrelated_scale_hist_FE.size();
    eta_bin_FE_center.push_back((eta_bins_edge_SM[diff+m+1]+eta_bins_edge_SM[diff+m]) /2);
    eta_bin_FE_error.push_back((eta_bins_edge_SM[diff+m+1]-eta_bins_edge_SM[diff+m]) /2);
    SF_uncorrelated_FE.push_back(constfit -> GetParameter( 0 ));
    SF_uncorrelated_FE_error.push_back(constfit -> GetParError( 0 ));
    texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
    if(m == JER_uncorrelated_scale_hist_FE.size()-1 ) texfile << "\\\\";
  }

  texfile << "\n standard method, correlated fit\n";
  for( unsigned int m = 0; m < JER_correlated_scale_hist_SM.size(); m++ ){
    TF1 * constfit = JER_correlated_scale_hist_SM.at(m) -> GetFunction("constfit");
    TF1 * NSC_ratio = JER_correlated_scale_hist_SM.at(m) -> GetFunction("NSC_ratio");
    JER_correlated_scale_hist_SM.at(m)->GetFunction("NSC_ratio")->SetBit(TF1::kNotDraw);
    SF_correlated_SM_ptdep_min.push_back(findMinMax(width_pt_data_SM.at(m)[0], NSC_ratio, constfit, 0));
    SF_correlated_SM_ptdep_max.push_back(findMinMax(width_pt_data_SM.at(m)[0], NSC_ratio, constfit, 1));
    JER_correlated_scale_hist_SM.at(m) -> Write();
    SF_correlated_SM.push_back(constfit -> GetParameter( 0 ));
    SF_correlated_SM_error.push_back(constfit -> GetParError( 0 ));
    texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
    if(m == JER_correlated_scale_hist_SM.size()-1 ) texfile << "\\\\";
  }

  texfile << "\n forward extension, correlated fit \n";
  for( unsigned int m = 0; m < JER_correlated_scale_hist_FE.size(); m++ ){
    TF1 * constfit = JER_correlated_scale_hist_FE.at(m) -> GetFunction("constfit");
    TF1 * NSC_ratio = JER_correlated_scale_hist_FE.at(m) -> GetFunction("NSC_ratio");
    JER_correlated_scale_hist_FE.at(m)->GetFunction("NSC_ratio")->SetBit(TF1::kNotDraw);
    SF_correlated_FE_ptdep_min.push_back(findMinMax(width_pt_data_FE.at(m)[0], NSC_ratio, constfit, 0));
    SF_correlated_FE_ptdep_max.push_back(findMinMax(width_pt_data_FE.at(m)[0], NSC_ratio, constfit, 1));
    JER_correlated_scale_hist_FE.at(m) -> Write();
    SF_correlated_FE.push_back(constfit -> GetParameter( 0 ));
    SF_correlated_FE_error.push_back(constfit -> GetParError( 0 ));
    texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
    if(m == JER_correlated_scale_hist_FE.size()-1 ) texfile << "\\\\";
  }
  texfile << "\n";
  texfile.close();

  gbis.Close();
  ofstream txt_ST, txt_FE;
  txt_ST.open ("output/scalefactors_ST.txt");
  txt_FE.open ("output/scalefactors_FE.txt");
  std::cout << "#eta_bin_SM_center" << " " << "eta_bin_SM_error" << " " << "SF_uncorrelated_SM" << " " << "SF_uncorrelated_SM_error" << " " << "SF_correlated_SM" << " " << "SF_correlated_SM_error" << "\n";
  txt_ST << "#eta_bin_SM_center" << " " << "eta_bin_SM_error" << " " << "SF_uncorrelated_SM" << " " << "SF_uncorrelated_SM_error" << " " << "SF_correlated_SM" << " " << "SF_correlated_SM_error" << "\n";
  txt_FE << "#eta_bin_FE_center" << " " << "eta_bin_FE_error" << " " << "SF_uncorrelated_FE" << " " << "SF_uncorrelated_FE_error" << " " << "SF_correlated_FE" << " " << "SF_correlated_FE_error" << "\n";
  for (unsigned int i = 0; i < JER_uncorrelated_scale_hist_SM.size(); i++) {
    txt_ST << eta_bin_SM_center[i] << " " << eta_bin_SM_error[i] << " " << SF_uncorrelated_SM[i] << " " << SF_uncorrelated_SM_error[i] << " " << SF_correlated_SM[i] << " " << SF_correlated_SM_error[i] << " " << SF_uncorrelated_SM_ptdep_min[i] << " " << SF_uncorrelated_SM_ptdep_max[i] << " " << SF_correlated_SM_ptdep_min[i] << " " << SF_correlated_SM_ptdep_max[i] << "\n";
    // std::cout << eta_bin_SM_center[i] << " " << eta_bin_SM_error[i] << " " << SF_uncorrelated_SM[i] << " " << SF_uncorrelated_SM_error[i] << " " << SF_correlated_SM[i] << " " << SF_correlated_SM_error[i] << "\n";
  }
  for (unsigned int i = 0; i < JER_uncorrelated_scale_hist_FE.size(); i++) {
    txt_FE << eta_bin_FE_center[i] << " " << eta_bin_FE_error[i] << " " << SF_uncorrelated_FE[i] << " " << SF_uncorrelated_FE_error[i] << " " << SF_correlated_FE[i] << " " << SF_correlated_FE_error[i] << " " << SF_uncorrelated_FE_ptdep_min[i] << " " << SF_uncorrelated_FE_ptdep_max[i] << " " << SF_correlated_FE_ptdep_min[i] << " " << SF_correlated_FE_ptdep_max[i] << "\n";
    // std::cout << eta_bin_FE_center[i] << " " << eta_bin_FE_error[i] << " " << SF_uncorrelated_FE[i] << " " << SF_uncorrelated_FE_error[i] << " " << SF_correlated_FE[i] << " " << SF_correlated_FE_error[i] << "\n";
  }
  txt_ST.close();
  txt_FE.close();

  TCanvas* canv_SF = tdrCanvas("JER SF",eta_bins_edge_SM[0]-0.1, eta_bins_edge_SM[EtaBins_SM + EtaBins_FE]+0.5, 0.8, 3.0, "#eta", "JER SF");
  canv_SF->SetTickx(0);
  canv_SF->SetTicky(0);
  TGraphErrors *gr_st = new TGraphErrors(JER_correlated_scale_hist_SM.size(), &(eta_bin_SM_center[0]), &(SF_correlated_SM[0]), &(eta_bin_SM_error[0]), &(SF_correlated_SM_error[0]));
  TGraphErrors *gr_fe = new TGraphErrors(JER_correlated_scale_hist_FE.size(), &(eta_bin_FE_center[0]), &(SF_correlated_FE[0]), &(eta_bin_FE_error[0]), &(SF_correlated_FE_error[0]));
  tdrDraw(gr_st, "P", kFullDotLarge, kRed);
  tdrDraw(gr_fe, "P", kFullDotLarge, kBlue);

  TLegend *leg = tdrLeg(0.70,0.75,0.9,0.9);
  leg->AddEntry(gr_st, "Standard","lep");
  leg->AddEntry(gr_fe, "Forward ext", "lep");
  leg->Draw("same");

  canv_SF->SaveAs("output/SF.png");
  canv_SF->SaveAs("output/SF.pdf");
  canv_SF->SaveAs("output/SF.root");

  //////////////////////////////////////////////////////////////////////////////////////////
  //    SF plots overlayed with ...                                                       //
  //////////////////////////////////////////////////////////////////////////////////////////
  TFile SFsoverlayed("output/SFsoverlayed.root","RECREATE");

  for( unsigned int m = 0; m < JER_uncorrelated_scale_hist_SM.size(); m++ ){
    TF1* f;
    char line[100];
    TLegend *legend;

    PLOT_3(JER_uncorrelated_scale_hist_SM,JER_correlated_scale_hist_SM,JER015_scale_hist_SM,kBlue,kRed, kGreen+2,m)

    if(JER_uncorrelated_scale_hist_SM.at(m)->GetFunction("constfit")){
      f = JER_uncorrelated_scale_hist_SM.at(m) -> GetFunction("constfit");
      f->SetLineColor(kBlue);
      legend = tdrLeg(0.30,0.70,0.50,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      tdrHeader(legend,"JER", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

    if(JER_correlated_scale_hist_SM.at(m)->GetFunction("constfit")){
      f = JER_correlated_scale_hist_SM.at(m) -> GetFunction("constfit");
      f->SetLineColor(kRed);
      legend = tdrLeg(0.50,0.70,0.70,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
      tdrHeader(legend,"JER cor", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

    if(JER015_scale_hist_SM.at(m)->GetFunction("constfit")){
      f = JER015_scale_hist_SM.at(m) -> GetFunction("constfit");
      f->SetLineColor(kGreen+2);
      legend = tdrLeg(0.70,0.70,0.90,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
      tdrHeader(legend,"JER 0.15", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

    char legTitle[100];
    TLegend *leg = tdrLeg(0.6,0.15,0.9,0.35);
    sprintf(legTitle, "#eta #in [%.3f,%.3f]",eta_bins_edge_SM[m], eta_bins_edge_SM[m+1]);

    tdrHeader(leg,legTitle);
    leg->SetTextFont(42);  leg->SetTextSize(0.04);
    leg->AddEntry(JER_uncorrelated_scale_hist_SM.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{MC}","lep");
    leg->AddEntry(JER_correlated_scale_hist_SM.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{MC} correlated","lep");
    leg->AddEntry(JER015_scale_hist_SM.at(m),"#sigma_{0.15}^{data}/#sigma_{0.15}^{MC}","lep");
    leg->Draw("same");

    canv -> Print("pdfy/SFs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/SFs/"+canvName+".png","png");
  }

  for( unsigned int m = 0; m < JER_uncorrelated_scale_hist_FE.size(); m++){
    TF1* f;
    char line[100];
    TLegend *legend;

    PLOT_3(JER_uncorrelated_scale_hist_FE,JER_correlated_scale_hist_FE,JER015_scale_hist_FE,kBlue,kRed,kGreen+2,m)

    if(JER_uncorrelated_scale_hist_FE.at(m)->GetFunction("constfit")){
      f = JER_uncorrelated_scale_hist_FE.at(m) -> GetFunction("constfit");   f->SetLineColor(kBlue);
      legend = tdrLeg(0.30,0.70,0.50,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);     legend->SetTextColor(kBlue);
      tdrHeader(legend,"JER", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl;
  }

  if(JER_correlated_scale_hist_FE.at(m)->GetFunction("constfit")){
    f = JER_correlated_scale_hist_FE.at(m) -> GetFunction("constfit");   f->SetLineColor(kRed);
    legend = tdrLeg(0.50,0.70,0.70,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);         legend->SetTextColor(kRed);
    tdrHeader(legend,"JER cor", 22);
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");
  } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

  if(JER015_scale_hist_FE.at(m)->GetFunction("constfit")){
    f = JER015_scale_hist_FE.at(m) -> GetFunction("constfit");    f->SetLineColor(kGreen+2);
    legend = tdrLeg(0.70,0.70,0.90,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);         legend->SetTextColor(kGreen+2);
    tdrHeader(legend,"JER 0.15", 22);
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");
  } else { std::cout << "Fit function at bin " << m << " was not found " << std::endl;}

  char legTitle[100];
  TLegend *leg = tdrLeg(0.6,0.15,0.9,0.35);
  sprintf(legTitle, "#eta #in [%.3f,%.3f]",eta_bins_edge_SM[EtaBins_SM + EtaBins_FE - JER_uncorrelated_scale_hist_SM.size() + m], eta_bins_edge_SM[EtaBins_SM + EtaBins_FE - JER_uncorrelated_scale_hist_SM.size() + m+1]);

  tdrHeader(leg,legTitle);
  leg->SetTextFont(42);  leg->SetTextSize(0.04);
  leg->AddEntry(JER_uncorrelated_scale_hist_FE.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{MC}","lep");
  leg->AddEntry(JER_correlated_scale_hist_FE.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{MC} correlated","lep");
  leg->AddEntry(JER015_scale_hist_FE.at(m),"#sigma_{0.15}^{data}/#sigma_{0.15}^{MC}","lep");
  leg->Draw("same");

  if (m<9) canvName = canvName(0, canvName.Length()-1);
  else canvName = canvName(0, canvName.Length()-2);
  canvName += (m+2);
  canv -> Print("pdfy/SFs/"+canvName+".pdf","pdf");
  canv -> Print("pdfy/SFs/"+canvName+".png","png");
}

SFsoverlayed.Close();

return true;

}
