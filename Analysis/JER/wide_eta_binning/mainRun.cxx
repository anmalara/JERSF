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

// Code by Marek Niedziela
// Based on code by Matthias Schröder, Kristin Goebel

// ----------------------------------------------------------------------------------------------------
#define PLOT_1SF(h1,c1,m)                                                                   \
TString canvSFName = h1.at(m)->GetTitle();                                                  \
TString nameXaxisSF = h1.at(m)->GetXaxis()->GetTitle();                                     \
TString nameYaxisSF = h1.at(m)->GetYaxis()->GetTitle();                                     \
vec.clear();                                                                                \
vec.push_back(h1.at(m));                                                                    \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
TCanvas* canvSF = tdrCanvas(canvSFName, x_min, x_max, y_min, y_max,nameXaxisSF,nameYaxisSF);\
canvSF->SetTickx(0);                                                                        \
canvSF->SetTicky(0);                                                                        \
tdrDraw(h1.at(m), "", kFullCircle, c1 );                                                    \
// ----------------------------------------------------------------------------------------------------
#define PLOT_2SF(h1,h2,c1,c2,m)                                                             \
TString canvSFName = h1.at(m)->GetTitle();                                                  \
TString nameXaxisSF = h1.at(m)->GetXaxis()->GetTitle();                                     \
TString nameYaxisSF = h1.at(m)->GetYaxis()->GetTitle();                                     \
std::vector<TH1*> vec;                                                                      \
vec.clear();                                                                                \
vec.push_back(h1.at(m));                                                                    \
vec.push_back(h2.at(m));                                                                    \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
TCanvas* canvSF = tdrCanvas(canvSFName, x_min, x_max, y_min, y_max,nameXaxisSF,nameYaxisSF);\
canvSF->SetTickx(0);                                                                        \
canvSF->SetTicky(0);                                                                        \
tdrDraw(h1.at(m), "", kFullCircle, c1 );                                                    \
tdrDraw(h2.at(m), "", kFullCircle, c2 );                                                    \
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
#define PLOT_4(h1,h2,h3,h4,c1,c2,c3,c4,m)                                                   \
TString canvName = h1.at(m+1)->GetTitle();                                                  \
TString nameXaxis = h1.at(m+1)->GetXaxis()->GetTitle();                                     \
TString nameYaxis = h1.at(m+1)->GetYaxis()->GetTitle();                                     \
std::vector<TH1*> vec;                                                                      \
vec.push_back(h1.at(m+1));                                                                  \
vec.push_back(h2.at(m));                                                                    \
vec.push_back(h3.at(m));                                                                    \
vec.push_back(h4.at(m));                                                                    \
double x_min, x_max, y_min, y_max;                                                          \
findExtreme(vec, &x_min, &x_max, &y_min, &y_max);                                           \
TCanvas* canv = tdrCanvas(canvName, x_min, x_max, y_min, y_max, nameXaxis, nameYaxis);      \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m+1), "", kFullCircle, c1 );                                                  \
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

int mainRun( bool data, const char* filename, const char* filename_data, TString lumi, TString label_mc, TString label_data, TString Trigger, double gaustails = 0.985, float shiftForPLI = 0.0){

  std::cout << "Start" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //		I load all histograms I will need                                   //
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

  int pt_bins = 9, pt_bins_fe = 6, eta_bin = 10, eta_bins_fe = 3, eta_bins_fec = 8, alpha_bins;

  std::vector<double> p_bins_edge, p_bins_edge_FT;

  if (Trigger.Contains("Single")) {
    pt_bins       = n_pt_bins_Si;
    pt_bins_fe    = n_pt_bins_Si_HF;
    for (int i = 0; i < n_pt_bins_Si; i++) p_bins_edge.push_back(pt_bins_Si[i]);
    for (int i = 0; i < n_pt_bins_Si_HF; i++) p_bins_edge_FT.push_back(pt_bins_Si_HF[i]);
  } else if (Trigger.Contains("DiJet")) {
    pt_bins       = n_pt_bins_Di;
    pt_bins_fe    = n_pt_bins_Di_HF;
    for (int i = 0; i < n_pt_bins_Di; i++) p_bins_edge.push_back(pt_bins_Di[i]);
    for (int i = 0; i < n_pt_bins_Di_HF; i++) p_bins_edge_FT.push_back(pt_bins_Di_HF[i]);
  } else if (Trigger.Contains("Single_MB")) {
    pt_bins       = n_pt_bins_MB;
    pt_bins_fe    = n_pt_bins_MB_HF;
    for (int i = 0; i < n_pt_bins_MB; i++) p_bins_edge.push_back(pt_bins_MB[i]);
    for (int i = 0; i < n_pt_bins_MB_HF; i++) p_bins_edge_FT.push_back(pt_bins_MB_HF[i]);
  }

  p_bins_edge.push_back(1000);
  p_bins_edge_FT.push_back(1000);

  std::vector<double> eta_bins_edge(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_bins_control_edge(eta_bins+3, eta_bins + sizeof(eta_bins)/sizeof(double)-3);
  eta_bins_control_edge.insert(eta_bins_control_edge.begin(), 0.);

  eta_bins_fe   = 3;
  eta_bin       = n_eta_bins - eta_bins_fe - 1;
  eta_bins_fec  = eta_bins_control_edge.size() -1 ;

  std::vector<double> alpha;
  alpha.push_back(0.05); alpha.push_back(0.1);  alpha.push_back(0.15);
  alpha.push_back(0.20); alpha.push_back(0.25); alpha.push_back(0.3);

  TFile *f, *f_data;
  f = new TFile( filename, "READ");
  f_data = new TFile( filename_data, "READ");

  // ------------------------------
  //      loading histograms
  // ------------------------------

  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all, asymmetries_gen_all, asymmetries_data_all, mc_truth_asymmetries_all;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all_pt, asymmetries_gen_all_pt, asymmetries_data_all_pt;

  std::vector< std::vector< std::vector< TH1F* > > > forward_hist, forward_gen_hist, forward_data_hist, mc_truth_forward_hist;
  std::vector< std::vector< std::vector< TH1F* > > > forward_hist_pt, forward_gen_hist_pt, forward_data_hist_pt;

  std::vector< std::vector< std::vector< TH1F* > > > forward_gen_data_hist, asymmetries_gen_data_all, forward_gen_data_hist_pt, asymmetries_gen_data_all_pt;

  std::vector< std::vector< std::vector< TH1F* > > > dummy_hists;


  // real_data = true, data = false
  histLoadAsym( *f_data, 	real_data, "asymm_eta", asymmetries_data_all, asymmetries_gen_data_all, pt_bins, eta_bin );	// load asymm from data
  histLoadAsym( *f, 	data, "asymm_eta", asymmetries_all, asymmetries_gen_all, pt_bins, eta_bin );		// load asymm from mc (all)
  histLoadAsym( *f_data, 	real_data, "forward_control_probe", forward_data_hist , forward_gen_data_hist , pt_bins, eta_bins_fec );
  histLoadAsym( *f, 	data, "forward_control_probe", forward_hist , forward_gen_hist, pt_bins, eta_bins_fec );
  histLoadAsym( *f_data, 	real_data, "forward_probe", forward_data_hist , forward_gen_data_hist , pt_bins_fe, eta_bins_fe );
  histLoadAsym( *f, 	data, "forward_probe", forward_hist, forward_gen_hist, pt_bins_fe, eta_bins_fe );

  histLoadAsym( *f_data, 	real_data, "asymmpt_probe", asymmetries_data_all_pt, asymmetries_gen_data_all_pt, pt_bins, eta_bin );	// load asymm pt from data
  histLoadAsym( *f, 	data, "asymmpt_probe", asymmetries_all_pt, asymmetries_gen_all_pt, pt_bins, eta_bin );		// load asymm pt from mc (all)
  histLoadAsym( *f_data, 	real_data, "forwardpt_control_probe", forward_data_hist_pt , forward_gen_data_hist_pt , pt_bins, eta_bins_fec );
  histLoadAsym( *f, 	data, "forwardpt_control_probe", forward_hist_pt , forward_gen_hist_pt, pt_bins, eta_bins_fec );
  histLoadAsym( *f_data, 	real_data, "forwardpt_probe", forward_data_hist_pt , forward_gen_data_hist_pt , pt_bins_fe, eta_bins_fe );
  histLoadAsym( *f, 	data, "forwardpt_probe", forward_hist_pt , forward_gen_hist_pt, pt_bins_fe, eta_bins_fe );

  histLoadAsym( *f, 	data, "mctruth_eta", mc_truth_asymmetries_all, dummy_hists, pt_bins, eta_bin );		// load asymm from mc (all)
  histLoadAsym( *f, 	data, "mctruth_forward_control_probe", mc_truth_forward_hist , dummy_hists, pt_bins, eta_bins_fec );
  histLoadAsym( *f, 	data, "mctruth_forward_probe",         mc_truth_forward_hist , dummy_hists, pt_bins_fe, eta_bins_fe );


  print3H(asymmetries_all);
  print3H(forward_hist);
  print3H(dummy_hists);
  std::cout << "1 Done" << '\n';

  std::vector < TH2F* > Map_mean_data;

  Fill_Map3D(asymmetries_data_all, Map_mean_data, eta_bins_edge, p_bins_edge);

  TFile maps("pdfy/maps/maps.root","RECREATE");
  for (unsigned int r = 0; r < alpha.size(); r++) {
    TString canvName = "Map_mean_"; canvName += r;
    TString nameXaxis = "#eta";
    TString nameYaxis = "p_{T} (GeV)";
    TCanvas* canv = tdrCanvas(canvName, 0, 6, 0, 1000, nameXaxis, nameYaxis);
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
  //		I calculate pt_mean for each alpha and pt bin and eta bin.          //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > asymmetries_width_pt, asymmetries_width_gen_pt, asymmetries_width_data_pt, asymmetries_width_gen_data_pt;
  std::vector< std::vector< std::vector< double > > > forward_width_pt, forward_width_gen_pt, forward_width_data_pt, forward_width_gen_data_pt;

  histMeanPt( asymmetries_all_pt , asymmetries_width_pt);
  histMeanPt( asymmetries_gen_all_pt , asymmetries_width_gen_pt);
  histMeanPt( asymmetries_data_all_pt , asymmetries_width_data_pt);
  histMeanPt( asymmetries_gen_data_all_pt , asymmetries_width_gen_data_pt);

  histMeanPt( forward_hist_pt , forward_width_pt );
  histMeanPt( forward_gen_hist_pt , forward_width_gen_pt );
  histMeanPt( forward_data_hist_pt , forward_width_data_pt );
  histMeanPt( forward_gen_data_hist_pt , forward_width_gen_data_pt );

  // gen_all_data is for running a cross check with smeared MC.

  print3(asymmetries_width_pt);
  print3(forward_width_pt);
  std::cout << "2 Done" << '\n';
  ////////////////////////////////////////////////////////////////////////////
  //		I calculate width of asymmetry distributions only for               //
  //		alpha bins above 10 GeV thresholds (too soft contriubtions)         //
  //		e.g. for bin p_T_ave (55-75) alpha 0.1 corresponds to 57 GeV jets   //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > asymmetries_width, asymmetries_width_gen, asymmetries_width_data, asymmetries_width_gen_data;
  std::vector< std::vector< std::vector< double > > > asymmetries_width_error, asymmetries_width_gen_error, asymmetries_width_data_error, asymmetries_width_gen_data_error;
  std::vector< std::vector< std::vector< double > > > forward_width, forward_width_gen, forward_width_data, forward_width_gen_data;
  std::vector< std::vector< std::vector< double > > > forward_width_error, forward_width_gen_error, forward_width_data_error, forward_width_gen_data_error;

  histWidthAsym2( asymmetries_all , asymmetries_width, asymmetries_width_error, false, gaustails);
  histWidthAsym2( asymmetries_gen_all , asymmetries_width_gen, asymmetries_width_gen_error, false, gaustails);
  histWidthAsym2( asymmetries_data_all , asymmetries_width_data, asymmetries_width_data_error, false, gaustails);
  histWidthAsym2( asymmetries_gen_data_all , asymmetries_width_gen_data, asymmetries_width_gen_data_error, false, gaustails);

  histWidthAsym2( forward_hist , forward_width, forward_width_error, false, gaustails);
  histWidthAsym2( forward_gen_hist , forward_width_gen, forward_width_gen_error, false, gaustails);
  histWidthAsym2( forward_data_hist , forward_width_data, forward_width_data_error, false, gaustails);
  histWidthAsym2( forward_gen_data_hist , forward_width_gen_data, forward_width_gen_data_error, false, gaustails);

  ////////////////////////////////////////////////////////////////////////////
  //    I calculate widths, this time also including                        //
  //    alpha bins below 10GeV threshold                                    //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > soft_asymmetries_width, soft_asymmetries_width_gen, soft_asymmetries_width_data, soft_asymmetries_width_gen_data;
  std::vector< std::vector< std::vector< double > > > soft_asymmetries_width_error, soft_asymmetries_width_gen_error, soft_asymmetries_width_data_error, soft_asymmetries_width_gen_data_error;

  std::vector< std::vector< std::vector< double > > > soft_forward_width, soft_forward_width_gen, soft_forward_width_data, soft_forward_width_gen_data;
  std::vector< std::vector< std::vector< double > > > soft_forward_width_error, soft_forward_width_gen_error, soft_forward_width_data_error, soft_forward_width_gen_data_error;

  histWidthAsym2( asymmetries_all , soft_asymmetries_width, soft_asymmetries_width_error, true, gaustails);
  histWidthAsym2( asymmetries_gen_all , soft_asymmetries_width_gen, soft_asymmetries_width_gen_error, true, gaustails);
  histWidthAsym2( asymmetries_data_all , soft_asymmetries_width_data, soft_asymmetries_width_data_error, true, gaustails);
  histWidthAsym2( asymmetries_gen_data_all , soft_asymmetries_width_gen_data, soft_asymmetries_width_gen_data_error, true, gaustails);

  histWidthAsym2( forward_hist , soft_forward_width, soft_forward_width_error, true, gaustails);
  histWidthAsym2( forward_gen_hist , soft_forward_width_gen, soft_forward_width_gen_error, true, gaustails);
  histWidthAsym2( forward_data_hist , soft_forward_width_data, soft_forward_width_data_error, true, gaustails);
  histWidthAsym2( forward_gen_data_hist , soft_forward_width_gen_data, soft_forward_width_gen_data_error, true, gaustails);

  print3(asymmetries_width);
  print3(forward_width);
  print3(soft_asymmetries_width);
  print3(soft_forward_width);
  std::cout << "3 Done" << '\n';
  ////////////////////////////////////////////////////////////////////////////
  //    Calculate mcTruth resolution for cross check with dijet calculation //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > mcTruth_res, forward_mcTruth_res;
  std::vector< std::vector< std::vector< double > > > mcTruth_res_error, forward_mcTruth_res_error;

  histWidthMCTruth( mc_truth_asymmetries_all, mcTruth_res        , mcTruth_res_error        );
  histWidthMCTruth( mc_truth_forward_hist   , forward_mcTruth_res, forward_mcTruth_res_error);

  print3(mcTruth_res);
  print3(forward_mcTruth_res);
  std::cout << "4 Done" << '\n';
  ////////////////////////////////////////////////////////////////////////////
  //    Calculate mcTruth resolution                                        //
  ////////////////////////////////////////////////////////////////////////////
  //		I define width(alpha_max) histograms                                //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< TH1F* > > widths_hist_all, widths_hist_gen_all, widths_hist_data_all, widths_hist_gen_data_all;
  std::vector< std::vector< TH1F* > > widths_hist_FE, widths_hist_gen_FE, widths_hist_data_FE, widths_hist_gen_data_FE;

  def_fill_widths( "widths", widths_hist_all , asymmetries_width, asymmetries_width_error );
  def_fill_widths( "widths_gen", widths_hist_gen_all , asymmetries_width_gen, asymmetries_width_gen_error );
  def_fill_widths( "widths_data", widths_hist_data_all , asymmetries_width_data, asymmetries_width_data_error );
  def_fill_widths( "widths_gen_data", widths_hist_gen_data_all , asymmetries_width_gen_data, asymmetries_width_gen_data_error );

  def_fill_widths( "widths_fe", widths_hist_FE , forward_width, forward_width_error );
  def_fill_widths( "widths_gen_fe", widths_hist_gen_FE , forward_width_gen, forward_width_gen_error );
  def_fill_widths( "widths_data_fe", widths_hist_data_FE , forward_width_data, forward_width_data_error );
  def_fill_widths( "widths_gen_data_fe", widths_hist_gen_data_FE , forward_width_gen_data, forward_width_gen_data_error );

  ////////////////////////////////////////////////////////////////////////////
  //		histograms filled                                                   //
  ////////////////////////////////////////////////////////////////////////////
  //    I do same for alpha unconstrained widths                            //
  //    one needs these plots to prove which points should be rejected!     //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< TH1F* > > soft_widths_hist_all, soft_widths_hist_gen_all, soft_widths_hist_data_all, soft_widths_hist_gen_data_all;
  std::vector< std::vector< TH1F* > > soft_widths_hist_FE, soft_widths_hist_gen_FE, soft_widths_hist_data_FE, soft_widths_hist_gen_data_FE;

  def_fill_widths( "all_widths", soft_widths_hist_all, soft_asymmetries_width, soft_asymmetries_width_error );
  def_fill_widths( "all_widths_gen", soft_widths_hist_gen_all, soft_asymmetries_width_gen, soft_asymmetries_width_gen_error );
  def_fill_widths( "all_widths_data", soft_widths_hist_data_all, soft_asymmetries_width_data, soft_asymmetries_width_data_error );
  def_fill_widths( "all_widths_gen_data", soft_widths_hist_gen_data_all, soft_asymmetries_width_gen_data, soft_asymmetries_width_gen_data_error );

  def_fill_widths( "all_widths_fe", soft_widths_hist_FE, soft_forward_width, soft_forward_width_error );
  def_fill_widths( "all_widths_gen_fe", soft_widths_hist_gen_FE, soft_forward_width_gen, soft_forward_width_gen_error );
  def_fill_widths( "all_widths_data_fe", soft_widths_hist_data_FE, soft_forward_width_data, soft_forward_width_data_error );
  def_fill_widths( "all_widths_gen_data_fe", soft_widths_hist_gen_data_FE, soft_forward_width_gen_data, soft_forward_width_gen_data_error );

  print2H(widths_hist_all);
  print2H(widths_hist_FE);
  print2H(soft_widths_hist_all);
  print2H(soft_widths_hist_FE);
  std::cout << "5 Done" << '\n';
  ////////////////////////////////////////////////////////////////////////////
  //		I fit line or const to width(alpha_max)                             //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > widths_all, widths_gen_all, widths_data_all, widths_gen_data_all;
  std::vector< std::vector< double > > widths_all_error, widths_gen_all_error, widths_data_all_error, widths_gen_data_all_error;

  std::vector< std::vector< double > > widths_FE, widths_gen_FE, widths_data_FE, widths_gen_data_FE;
  std::vector< std::vector< double > > widths_FE_error, widths_gen_FE_error, widths_data_FE_error, widths_gen_data_FE_error;

  histLinFit( widths_hist_all , widths_all, widths_all_error );
  histLinFit( widths_hist_gen_all , widths_gen_all, widths_gen_all_error );
  histLinFit( widths_hist_data_all , widths_data_all, widths_data_all_error );
  histLinFit( widths_hist_gen_data_all , widths_gen_data_all, widths_gen_data_all_error );

  histLinFit( widths_hist_FE , widths_FE, widths_FE_error );
  histLinFit( widths_hist_gen_FE , widths_gen_FE, widths_gen_FE_error );
  histLinFit( widths_hist_data_FE , widths_data_FE, widths_data_FE_error );
  histLinFit( widths_hist_gen_data_FE , widths_gen_data_FE, widths_gen_data_FE_error );

  print2(widths_all);
  print2(widths_FE);
  std::cout << "6 Done" << '\n';
  ////////////////////////////////////////////////////////////////////////////
  //		Correlated fit                                                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > widths_correlated_all, widths_correlated_gen_all, widths_correlated_data_all, widths_correlated_gen_data_all;
  std::vector< std::vector< double > > widths_correlated_all_error, widths_correlated_gen_all_error, widths_correlated_data_all_error, widths_correlated_gen_data_all_error;

  std::vector< std::vector< double > > widths_correlated_FE, widths_correlated_gen_FE, widths_correlated_data_FE, widths_correlated_gen_data_FE;
  std::vector< std::vector< double > > widths_correlated_FE_error, widths_correlated_gen_FE_error, widths_correlated_data_FE_error, widths_correlated_gen_data_FE_error;

  std::vector< std::vector< TGraphErrors *> > MC_correlated_graphs, data_correlated_graphs, gen_correlated_graphs, gen_data_correlated_graphs;
  std::vector< std::vector< TGraphErrors *> > FE_MC_correlated_graphs, FE_data_correlated_graphs, FE_gen_correlated_graphs, FE_gen_data_correlated_graphs;

  histLinCorFit(asymmetries_width, asymmetries_width_error, MC_correlated_graphs, widths_correlated_all, widths_correlated_all_error);
  histLinCorFit(asymmetries_width_data, asymmetries_width_data_error, data_correlated_graphs, widths_correlated_data_all, widths_correlated_data_all_error);
  histLinCorFit(asymmetries_width_gen, asymmetries_width_gen_error, gen_correlated_graphs, widths_correlated_gen_all, widths_correlated_gen_all_error);
  histLinCorFit(asymmetries_width_gen_data, asymmetries_width_gen_data_error, gen_data_correlated_graphs, widths_correlated_gen_data_all, widths_correlated_gen_data_all_error);

  histLinCorFit(forward_width, forward_width_error, FE_MC_correlated_graphs, widths_correlated_FE, widths_correlated_FE_error);
  histLinCorFit(forward_width_data, forward_width_data_error, FE_data_correlated_graphs, widths_correlated_data_FE, widths_correlated_data_FE_error);
  histLinCorFit(forward_width_gen, forward_width_gen_error, FE_gen_correlated_graphs, widths_correlated_gen_FE, widths_correlated_gen_FE_error);
  histLinCorFit(forward_width_gen_data, forward_width_gen_data_error, FE_gen_data_correlated_graphs, widths_correlated_gen_data_FE, widths_correlated_gen_data_FE_error);

  print2(widths_correlated_all);
  print2(widths_correlated_FE);
  print2G(MC_correlated_graphs);
  print2G(FE_MC_correlated_graphs);
  std::cout << "7 Done" << '\n';

  ////////////////////////////////////////////////////////////////////////////
  //		I make histograms ratio of widths(alpha=0.15)                       //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< TH1F* > widths_015_ratios_all, widths_015_ratios_FE;

  widths_015_ratios( "widths_015_ratios", widths_015_ratios_all, asymmetries_width_data, asymmetries_width_data_error, asymmetries_width, asymmetries_width_error, asymmetries_width_data_pt );
  widths_015_ratios( "widths_015_FE_ratios", widths_015_ratios_FE, forward_width_data, forward_width_data_error, forward_width, forward_width_error, forward_width_data_pt );
  print1H(widths_015_ratios_all);
  print1H(widths_015_ratios_FE);
  std::cout << "8 Done" << '\n';
  ////////////////////////////////////////////////////////////////////////////
  //		I correct for PLI using b parameter from line fit to sigma_A(alpha) //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_uncorrelated_corrected_MC, JER_uncorrelated_corrected_data;
  std::vector< std::vector< double > > JER_uncorrelated_corrected_MC_error, JER_uncorrelated_corrected_data_error;

  std::vector< std::vector< double > > JER_uncorrelated_corrected_FE_probe_MC, JER_uncorrelated_corrected_FE_probe_data;
  std::vector< std::vector< double > > JER_uncorrelated_corrected_FE_probe_MC_error, JER_uncorrelated_corrected_FE_probe_data_error;

  correctJERwithPLI( JER_uncorrelated_corrected_MC, JER_uncorrelated_corrected_MC_error, widths_all, widths_all_error, widths_gen_all, widths_gen_all_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_uncorrelated_corrected_data, JER_uncorrelated_corrected_data_error, widths_data_all, widths_data_all_error, widths_gen_all, widths_gen_all_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_uncorrelated_corrected_data, JER_uncorrelated_corrected_data_error, widths_data_all, widths_data_all_error, widths_gen_data_all, widths_gen_data_all_error, shiftForPLI);
  }

  correctJERwithPLI( JER_uncorrelated_corrected_FE_probe_MC, JER_uncorrelated_corrected_FE_probe_MC_error, widths_FE, widths_FE_error, widths_gen_FE, widths_gen_FE_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_uncorrelated_corrected_FE_probe_data, JER_uncorrelated_corrected_FE_probe_data_error, widths_data_FE, widths_data_FE_error, widths_gen_FE, widths_gen_FE_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_uncorrelated_corrected_FE_probe_data, JER_uncorrelated_corrected_FE_probe_data_error, widths_data_FE, widths_data_FE_error, widths_gen_data_FE, widths_gen_data_FE_error, shiftForPLI);
  }

  ////////////////////////////////////////////////////////////////////////////
  //		PLI corrected using b parameters                                    //
  ////////////////////////////////////////////////////////////////////////////
  //    Same correction but for correlated fit results                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_correlated_corrected_MC, JER_correlated_corrected_data;
  std::vector< std::vector< double > > JER_correlated_corrected_MC_error, JER_correlated_corrected_data_error;

  std::vector< std::vector< double > > JER_correlated_corrected_FE_probe_MC, JER_correlated_corrected_FE_probe_data;               //cor = correlated
  std::vector< std::vector< double > > JER_correlated_corrected_FE_probe_MC_error, JER_correlated_corrected_FE_probe_data_error;   //uncor = uncorrected fopr reference region

  correctJERwithPLI( JER_correlated_corrected_MC, JER_correlated_corrected_MC_error, widths_correlated_all, widths_correlated_all_error, widths_correlated_gen_all, widths_correlated_gen_all_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_correlated_corrected_data, JER_correlated_corrected_data_error, widths_correlated_data_all, widths_correlated_data_all_error, widths_correlated_gen_all, widths_correlated_gen_all_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_correlated_corrected_data, JER_correlated_corrected_data_error, widths_correlated_data_all, widths_correlated_data_all_error, widths_correlated_gen_data_all, widths_correlated_gen_data_all_error, shiftForPLI);
  }

  correctJERwithPLI( JER_correlated_corrected_FE_probe_MC, JER_correlated_corrected_FE_probe_MC_error, widths_correlated_FE, widths_correlated_FE_error, widths_correlated_gen_FE, widths_correlated_gen_FE_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_correlated_corrected_FE_probe_data, JER_correlated_corrected_FE_probe_data_error, widths_correlated_data_FE, widths_correlated_data_FE_error, widths_correlated_gen_FE, widths_correlated_gen_FE_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_correlated_corrected_FE_probe_data, JER_correlated_corrected_FE_probe_data_error, widths_correlated_data_FE, widths_correlated_data_FE_error, widths_correlated_gen_data_FE, widths_correlated_gen_data_FE_error, shiftForPLI);
  }

  ////////////////////////////////////////////////////////////////////////////
  //		I do the same for widths at alpha = 0.15                            //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER015_MC, JER015_data;
  std::vector< std::vector< double > > JER015_MC_error, JER015_data_error;

  std::vector< std::vector< double > > JER015_uncor_FE_probe_MC, JER015_uncor_FE_probe_data;
  std::vector< std::vector< double > > JER015_uncor_FE_probe_MC_error, JER015_uncor_FE_probe_data_error;

  correctJERwithPLI015(JER015_MC, JER015_MC_error, asymmetries_width, asymmetries_width_error, asymmetries_width_gen, asymmetries_width_gen_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI015(JER015_data, JER015_data_error, asymmetries_width_data, asymmetries_width_data_error, asymmetries_width_gen, asymmetries_width_gen_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI015(JER015_data, JER015_data_error, asymmetries_width_data, asymmetries_width_data_error, asymmetries_width_gen_data, asymmetries_width_gen_data_error, shiftForPLI);
  }
  correctJERwithPLI015(JER015_uncor_FE_probe_MC, JER015_uncor_FE_probe_MC_error, forward_width, forward_width_error, forward_width_gen, forward_width_gen_error, shiftForPLI);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI015(JER015_uncor_FE_probe_data, JER015_uncor_FE_probe_data_error, forward_width_data, forward_width_data_error, forward_width_gen, forward_width_gen_error, shiftForPLI);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI015(JER015_uncor_FE_probe_data, JER015_uncor_FE_probe_data_error, forward_width_data, forward_width_data_error, forward_width_gen_data, forward_width_gen_data_error, shiftForPLI);
  }

  print2(JER_uncorrelated_corrected_MC);
  print2(JER_uncorrelated_corrected_FE_probe_data);
  std::cout << "9 Done" << '\n';
  ////////////////////////////////////////////////////////////////////////////
  //		I corrected alpha = 0.15 widhs for PLI correct way                  //
  ////////////////////////////////////////////////////////////////////////////
  //		I correct forward widths for Ref region.                            //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_uncorrelated_corrected_FE_MC, JER_uncorrelated_corrected_FE_data;
  std::vector< std::vector< double > > JER_uncorrelated_corrected_FE_MC_error, JER_uncorrelated_corrected_FE_data_error;

  correctForRef( "mccorrected", JER_uncorrelated_corrected_FE_MC, JER_uncorrelated_corrected_FE_MC_error, JER_uncorrelated_corrected_FE_probe_MC, JER_uncorrelated_corrected_FE_probe_MC_error, forward_width_pt  );
  correctForRef( "datacorrect", JER_uncorrelated_corrected_FE_data, JER_uncorrelated_corrected_FE_data_error, JER_uncorrelated_corrected_FE_probe_data, JER_uncorrelated_corrected_FE_probe_data_error, forward_width_data_pt  );

  ////////////////////////////////////////////////////////////////////////////
  //		forward widths corrected for Ref widths!                            //
  ////////////////////////////////////////////////////////////////////////////
  //    same correction for correlated fit results                          //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_correlated_corrected_FE_MC, JER_correlated_corrected_FE_data;
  std::vector< std::vector< double > > JER_correlated_corrected_FE_MC_error, JER_correlated_corrected_FE_data_error;

  correctForRef( "mc_cor_corrected", JER_correlated_corrected_FE_MC,   JER_correlated_corrected_FE_MC_error,   JER_correlated_corrected_FE_probe_MC,   JER_correlated_corrected_FE_probe_MC_error,   forward_width_pt  );
  correctForRef( "data_cor_correct", JER_correlated_corrected_FE_data, JER_correlated_corrected_FE_data_error, JER_correlated_corrected_FE_probe_data, JER_correlated_corrected_FE_probe_data_error, forward_width_data_pt  );


  ////////////////////////////////////////////////////////////////////////////
  //		ref region corrected for correlated fit                             //
  ////////////////////////////////////////////////////////////////////////////
  //		and again, Ref region for widths at alpha = 0.15                    //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER015_FE_MC, JER015_FE_data;
  std::vector< std::vector< double > > JER015_FE_MC_error, JER015_FE_data_error;

  correctForRef( "mccorrected015", JER015_FE_MC,   JER015_FE_MC_error,   JER015_uncor_FE_probe_MC,   JER015_uncor_FE_probe_MC_error,   forward_width_pt  );
  correctForRef( "datacorrect015", JER015_FE_data, JER015_FE_data_error, JER015_uncor_FE_probe_data, JER015_uncor_FE_probe_data_error, forward_width_data_pt  );

  ////////////////////////////////////////////////////////////////////////////
  //		Ref reg corrected for widths at alpha = 0.15                        //
  ////////////////////////////////////////////////////////////////////////////
  //		I make make vectors with ratios of widths                           //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > scales_uncorrelated_SM, scales_uncorrelated_FE, scales_uncorrelated_FE_control;
  std::vector< std::vector< double > > scales_uncorrelated_SM_error, scales_uncorrelated_FE_error, scales_uncorrelated_FE_control_error;

  makeScales( scales_uncorrelated_SM,         scales_uncorrelated_SM_error,         JER_uncorrelated_corrected_data,          JER_uncorrelated_corrected_data_error,          JER_uncorrelated_corrected_MC,          JER_uncorrelated_corrected_MC_error );
  makeScales( scales_uncorrelated_FE,         scales_uncorrelated_FE_error,         JER_uncorrelated_corrected_FE_data,       JER_uncorrelated_corrected_FE_data_error,       JER_uncorrelated_corrected_FE_MC,       JER_uncorrelated_corrected_FE_MC_error );
  makeScales( scales_uncorrelated_FE_control, scales_uncorrelated_FE_control_error, JER_uncorrelated_corrected_FE_probe_data, JER_uncorrelated_corrected_FE_probe_data_error, JER_uncorrelated_corrected_FE_probe_MC, JER_uncorrelated_corrected_FE_probe_MC_error );	// uncorrected for reference region width, as a cross check. (i think)

  print2(scales_uncorrelated_SM);
  print2(scales_uncorrelated_FE);
  print2(scales_uncorrelated_FE_control);

  print2(JER_uncorrelated_corrected_data);
  print2(JER_uncorrelated_corrected_FE_data);
  print2(JER_uncorrelated_corrected_FE_probe_data);

  // same thing for correlated fit results
  std::vector< std::vector< double > > scales_correlated_SM, scales_correlated_FE, scales_correlated_FE_control;
  std::vector< std::vector< double > > scales_correlated_SM_error, scales_correlated_FE_error, scales_correlated_FE_control_error;

  makeScales( scales_correlated_SM,         scales_correlated_SM_error,         JER_correlated_corrected_data,          JER_correlated_corrected_data_error,          JER_correlated_corrected_MC,          JER_correlated_corrected_MC_error );
  makeScales( scales_correlated_FE,         scales_correlated_FE_error,         JER_correlated_corrected_FE_data,       JER_correlated_corrected_FE_data_error,       JER_correlated_corrected_FE_MC,       JER_correlated_corrected_FE_MC_error );
  makeScales( scales_correlated_FE_control, scales_correlated_FE_control_error, JER_correlated_corrected_FE_probe_data, JER_correlated_corrected_FE_probe_data_error, JER_correlated_corrected_FE_probe_MC, JER_correlated_corrected_FE_probe_MC_error );	// uncorrected for reference region width, as a cross check. (i think)

  // and same thing for using-b-parameter calculations

  std::vector< std::vector< double > > scales015_classic, scales015_fe;
  std::vector< std::vector< double > > scales015_classic_error, scales015_fe_error;

  makeScales( scales015_classic, scales015_classic_error, JER015_data,    JER015_data_error,    JER015_MC,    JER015_MC_error );
  makeScales( scales015_fe,      scales015_fe_error,      JER015_FE_data, JER015_FE_data_error, JER015_FE_MC, JER015_FE_MC_error );

  ////////////////////////////////////////////////////////////////////////////
  //		I have vectors with ratios of widths                                //
  ////////////////////////////////////////////////////////////////////////////
  //    I make plots with Res from dijet and MCTruth, a crosscheck          //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< TH1F* > JER_MC_Truth, JER_MC_Truth_FE;

  fill_mctruth_hist( "MC_Truth"    , JER_MC_Truth        , mcTruth_res,         mcTruth_res_error,         asymmetries_width_pt, 0.2);
  fill_mctruth_hist( "MC_Truth_Fwd", JER_MC_Truth_FE, forward_mcTruth_res, forward_mcTruth_res_error, forward_width_pt, 0.2);

  ////////////////////////////////////////////////////////////////////////////
  //		I make histograms with  JERs and scale factors                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< TH1F* > JER_uncorrelated_MC_hist, JER_uncorrelated_data_hist, JER_uncorrelated_scale_hist, JER015_uncorrelated_MC_hist;
  std::vector< TH1F* > JER_FE_uncorrelated_MC_hist, JER_FE_uncorrelated_data_hist, JER_FE_uncorrelated_scale_hist, JER015_FE_uncorrelated_MC_hist;
  std::vector< TH1F* > JER_FE_uncorrelated_MC_control_hist, JER_FE_uncorrelated_data_control_hist, JER_FE_uncorrelated_scale_control_hist;

  fill_hist( "MC_JER_uncorrelated_SM"  , JER_uncorrelated_MC_hist   , JER_uncorrelated_corrected_MC        , JER_uncorrelated_corrected_MC_error        , asymmetries_width_pt     , 0.2);
  fill_hist( "MC_JER_uncorrelated_FE015", JER015_uncorrelated_MC_hist, JER015_MC,   JER015_MC_error, asymmetries_width_pt, 0.2);
  fill_hist( "data_JER_uncorrelated_SM", JER_uncorrelated_data_hist , JER_uncorrelated_corrected_data      , JER_uncorrelated_corrected_data_error      , asymmetries_width_data_pt, 0.2);
  fill_hist( "SF_uncorrelated_SM"      , JER_uncorrelated_scale_hist, scales_uncorrelated_SM, scales_uncorrelated_SM_error, asymmetries_width_data_pt, 3.0);

  fill_hist( "MC_JER_uncorrelated_FE", JER_FE_uncorrelated_MC_hist, JER_uncorrelated_corrected_FE_MC, JER_uncorrelated_corrected_FE_MC_error, forward_width_pt, 0.2);
  fill_hist( "MC_JER_uncorrelated_FE015", JER015_FE_uncorrelated_MC_hist, JER015_FE_MC,   JER015_FE_MC_error, forward_width_pt, 0.2);
  fill_hist( "data_JER_uncorrelated_FE", JER_FE_uncorrelated_data_hist, JER_uncorrelated_corrected_FE_data, JER_uncorrelated_corrected_FE_data_error, forward_width_data_pt, 0.2);
  fill_hist( "SF_uncorrelated_FE", JER_FE_uncorrelated_scale_hist, scales_uncorrelated_FE, scales_uncorrelated_FE_error, forward_width_data_pt, 3.0);

  fill_hist( "MC_JER_uncorrelated_FE_control", JER_FE_uncorrelated_MC_control_hist, JER_uncorrelated_corrected_FE_probe_MC, JER_uncorrelated_corrected_FE_probe_MC_error, forward_width_pt, 0.2);
  fill_hist( "data_JER_uncorrelated_FE_control", JER_FE_uncorrelated_data_control_hist, JER_uncorrelated_corrected_FE_probe_data, JER_uncorrelated_corrected_FE_probe_data_error, forward_width_data_pt, 0.2);
  fill_hist( "SF_uncorrelated_FE_control", JER_FE_uncorrelated_scale_control_hist, scales_uncorrelated_FE_control, scales_uncorrelated_FE_control_error, forward_width_data_pt, 2.0);


  std::vector< TH1F* > JER015_scale_hist, JER015_FE_scale_hist;

  fill_hist( "SF_SM015", JER015_scale_hist, scales015_classic, scales015_classic_error, asymmetries_width_data_pt, 2.);
  fill_hist( "SF_FE015", JER015_FE_scale_hist, scales015_fe, scales015_fe_error, forward_width_data_pt, 3.0);

  std::vector< TH1F* > JER_correlated_MC_hist,            JER_correlated_data_hist,            JER_correlated_scale_hist;
  std::vector< TH1F* > JER_FE_correlated_MC_hist,         JER_FE_correlated_data_hist,         JER_FE_correlated_scale_hist;
  std::vector< TH1F* > JER_FE_correlated_MC_control_hist, JER_FE_correlated_data_control_hist, JER_FE_correlated_scale_control_hist;

  fill_hist( "MC_JER_correlated_SM",   JER_correlated_MC_hist,    JER_correlated_corrected_MC,         JER_correlated_corrected_MC_error,         asymmetries_width_pt, 0.2);
  fill_hist( "data_JER_correlated_SM", JER_correlated_data_hist,  JER_correlated_corrected_data,       JER_correlated_corrected_data_error,       asymmetries_width_data_pt, 0.2);
  fill_hist( "SF_correlated_SM",       JER_correlated_scale_hist, scales_correlated_SM, scales_correlated_SM_error, asymmetries_width_data_pt, 3.0);

  fill_hist( "MC_JER_correlated_FE",   JER_FE_correlated_MC_hist,    JER_correlated_corrected_FE_MC,   JER_correlated_corrected_FE_MC_error,   forward_width_pt, 0.2);
  fill_hist( "data_JER_correlated_FE", JER_FE_correlated_data_hist,  JER_correlated_corrected_FE_data, JER_correlated_corrected_FE_data_error, forward_width_data_pt, 0.2);
  fill_hist( "SF_correlated_FE",       JER_FE_correlated_scale_hist, scales_correlated_FE,   scales_correlated_FE_error,   forward_width_data_pt, 3.0);

  fill_hist( "MC_JER_correlated_FE_control",   JER_FE_correlated_MC_control_hist,    JER_correlated_corrected_FE_probe_MC,   JER_correlated_corrected_FE_probe_MC_error,   forward_width_pt, 0.2);
  fill_hist( "data_JER_correlated_FE_control", JER_FE_correlated_data_control_hist,  JER_correlated_corrected_FE_probe_data, JER_correlated_corrected_FE_probe_data_error, forward_width_data_pt, 0.2);
  fill_hist( "SF_correlated_FE_control",       JER_FE_correlated_scale_control_hist, scales_correlated_FE_control, scales_correlated_FE_control_error, forward_width_data_pt, 3.0);

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
  for( unsigned int m = 0; m < JER_MC_Truth.size(); m++ ){
    PLOT_4_MCT(JER_MC_Truth,JER_uncorrelated_MC_hist,JER_correlated_MC_hist,JER015_uncorrelated_MC_hist,kMagenta,kRed,kBlue,kGreen+2,m)

    char legTitle[100];
    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
    leg->AddEntry(JER_MC_Truth.at(m),                 "MC Truth",         "lep");
    leg->AddEntry(JER_uncorrelated_MC_hist.at(m),     "MC Uncorrelated",  "lep");
    leg->AddEntry(JER_correlated_MC_hist.at(m),       "MC Correlated",    "lep");
    leg->AddEntry(JER015_uncorrelated_MC_hist.at(m),  "MC 015",           "lep");

    canv->Print("pdfy/MCTruth/"+canvName+".pdf","pdf");
    canv->Print("pdfy/MCTruth/"+canvName+".png","png");
  }

  std::cout << "help" << '\n';
  std::cout << JER_MC_Truth_FE.size() << '\n';
  std::cout << JER_FE_uncorrelated_MC_hist.size() << '\n';
  std::cout << JER_FE_correlated_MC_hist.size() << '\n';
  std::cout << JER015_FE_uncorrelated_MC_hist.size() << '\n';


  for( unsigned int m = 0; m < JER_MC_Truth_FE.size()-1; m++ ){

    PLOT_4_MCT(JER_MC_Truth_FE,JER_FE_uncorrelated_MC_hist,JER_FE_correlated_MC_hist,JER015_FE_uncorrelated_MC_hist,kMagenta,kRed,kBlue,kGreen+2,m)

    char legTitle[100];
    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_control_edge[m], eta_bins_control_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_MC_Truth_FE.at(m),                "MC Truth",         "lep");
    leg->AddEntry(JER_FE_uncorrelated_MC_hist.at(m),    "MC Uncorrelated",  "lep");
    leg->AddEntry(JER_FE_correlated_MC_hist.at(m),      "MC Correlated",    "lep");
    // leg->AddEntry(JER015_FE_uncorrelated_MC_hist.at(m), "MC 015",           "lep");

    canv->Print("pdfy/MCTruth/"+canvName+".pdf","pdf");
    canv->Print("pdfy/MCTruth/"+canvName+".png","png");
  }

  fMCTruth.Close();

/*
  for( unsigned int m = 0; m < asymmetries_all.size(); m++ ){
    for( unsigned int p = 0; p < asymmetries_all.at(m).size(); p++ ){
      for( unsigned int r = 0; r < asymmetries_all.at(m).at(p).size(); r++ ){
        asymmetries_all.at(m).at(p).at(r)      -> Scale(1./asymmetries_all.at(m).at(p).at(r) -> Integral());
        asymmetries_gen_all.at(m).at(p).at(r)  -> Scale(1./asymmetries_gen_all.at(m).at(p).at(r) -> Integral());
        asymmetries_data_all.at(m).at(p).at(r) -> Scale(1./asymmetries_data_all.at(m).at(p).at(r) -> Integral());

        PLOT_3_3(asymmetries_all,asymmetries_gen_all,asymmetries_data_all,kRed,kGreen+2,kBlue,m,p,r)

        char name_mcreco[100];
        char name_mcgen[100];
        char name_data[100];
        char legTitle[100];

        sprintf(name_mcreco,  "MC,   %.4f +- %.4f", asymmetries_width.at(m).at(p).at(r), asymmetries_width_error.at(m).at(p).at(r));
        sprintf(name_mcgen,   "gen,  %.4f +- %.4f", asymmetries_width_gen.at(m).at(p).at(r), asymmetries_width_gen_error.at(m).at(p).at(r));
        sprintf(name_data,    "data, %.4f +- %.4f", asymmetries_width_data.at(m).at(p).at(r), asymmetries_width_data_error.at(m).at(p).at(r));
        sprintf(legTitle,     "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV", eta_bins_edge[m], eta_bins_edge[m+1], p_bins_edge[p], p_bins_edge[p+1]);

        TLegend *leg = tdrLeg(0.5,0.7,0.95,0.9);
        tdrHeader(leg,legTitle);
        sprintf(legTitle,     "#alpha < %.2f", alpha.at(r));
        leg->AddEntry((TObject*)0, legTitle, "");
        leg->AddEntry(asymmetries_data_all.at(m).at(p).at(r),  name_data,"lep");
        leg->AddEntry(asymmetries_all.at(m).at(p).at(r),      name_mcreco,"lep");
        leg->AddEntry(asymmetries_gen_all.at(m).at(p).at(r), name_mcgen,"lep");
        leg->Draw("same");

        canv->Update();
        canv -> Print("output/asymmetries/"+canvName+".pdf","pdf");
        canv -> Print("output/asymmetries/"+canvName+".png","png");
      }
    }
  }

  for( unsigned int m = 0; m < forward_hist.size(); m++ ){
    for( unsigned int p = 0; p < forward_hist.at(m).size(); p++ ){
      for( unsigned int r = 0; r < forward_hist.at(m).at(p).size(); r++ ){
        forward_hist.at(m).at(p).at(r)      -> Scale(1./forward_hist.at(m).at(p).at(r) -> Integral());
        forward_gen_hist.at(m).at(p).at(r)  -> Scale(1./forward_gen_hist.at(m).at(p).at(r) -> Integral());
        forward_data_hist.at(m).at(p).at(r) -> Scale(1./forward_data_hist.at(m).at(p).at(r) -> Integral());

        PLOT_3_3(forward_hist,forward_gen_hist,forward_data_hist,kRed,kGreen+2,kBlue,m,p,r)

        char name_mcreco[100];
        char name_mcgen[100];
        char name_data[100];
        char legTitle[100];

        sprintf(name_mcreco,  "MC,   %.4f+-%.4f", forward_width.at(m).at(p).at(r), forward_width_error.at(m).at(p).at(r));
        sprintf(name_mcgen,   "gen,  %.4f+-%.4f", forward_width_gen.at(m).at(p).at(r), forward_width_gen_error.at(m).at(p).at(r));
        sprintf(name_data,    "data, %.4f+-%.4f", forward_width_data.at(m).at(p).at(r), forward_width_data_error.at(m).at(p).at(r));
        sprintf(legTitle,     "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
        eta_bins_edge[eta_bin + eta_bins_fe - forward_hist.size() + m],
        eta_bins_edge[eta_bin + eta_bins_fe - forward_hist.size() + m+1],
        p_bins_edge_FT[p], p_bins_edge_FT[p+1]);

        TLegend *leg = tdrLeg(0.5,0.7,0.95,0.9);
        tdrHeader(leg,legTitle);
        sprintf(legTitle,     "#alpha < %.2f", alpha.at(r));
        leg->AddEntry((TObject*)0, legTitle, "");
        leg->AddEntry(forward_data_hist.at(m).at(p).at(r), name_data,"lep");
        leg->AddEntry(forward_hist.at(m).at(p).at(r),      name_mcreco,"lep");
        leg->AddEntry(forward_gen_hist.at(m).at(p).at(r),  name_mcgen,"lep");

        canv->Update();
        canv -> Print("output/asymmetries/"+canvName+".pdf","pdf");
        canv -> Print("output/asymmetries/"+canvName+".png","png");

      }
    }
  }

  TFile asyroot("output/asymmetries.root","RECREATE");
  for( unsigned int m = 0; m < asymmetries_all.size(); m++){
    for( unsigned int p = 0; p < asymmetries_all.at(m).size(); p++){
      for( unsigned int r = 0; r < asymmetries_all.at(m).at(p).size(); r++){
        asymmetries_all.at(m).at(p).at(r) -> Write();
        asymmetries_gen_all.at(m).at(p).at(r) -> Write();
        asymmetries_data_all.at(m).at(p).at(r) -> Write();
      }
    }
  }

  for( unsigned int m = 0; m < forward_hist.size(); m++){
    for( unsigned int p = 0; p < forward_hist.at(m).size(); p++){
      for( unsigned int r = 0; r < forward_hist.at(m).at(p).size(); r++){
        forward_hist.at(m).at(p).at(r) -> Write();
        forward_gen_hist.at(m).at(p).at(r) -> Write();
        forward_data_hist.at(m).at(p).at(r) -> Write();
      }
    }
  }

  asyroot.Close();
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Plots with widths(alpha)                                                                                                             //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < widths_hist_all.size(); m++ ){
    for( unsigned int p = 0; p < widths_hist_all.at(m).size(); p++ ){
      PLOT_3_2(widths_hist_all,widths_hist_gen_all,widths_hist_data_all,kRed,kGreen+2,kBlue,m,p)

      if( widths_hist_all.at(m).at(p) -> GetEntries() != 0 )     widths_hist_all.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kRed);
      if( widths_hist_gen_all.at(m).at(p) -> GetEntries() != 0 ) widths_hist_gen_all.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kGreen+2);
      if( widths_hist_data_all.at(m).at(p) -> GetEntries() != 0 )widths_hist_data_all.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kBlue);

      TF1* f;
      char line[100];
      TLegend *legend;

      if (widths_hist_all.at(m).at(p) -> GetFunction("linfit")) {
        f = widths_hist_all.at(m).at(p) -> GetFunction("linfit");
        legend = tdrLeg(0.50,0.70,0.70,0.85);
        legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
        tdrHeader(legend,"MC");
        sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
        legend->Draw("same");
      }

      if (widths_hist_gen_all.at(m).at(p) -> GetFunction("linfit")) {
        f = widths_hist_gen_all.at(m).at(p) -> GetFunction("linfit");
        legend = tdrLeg(0.70,0.70,0.90,0.85);
        legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
        tdrHeader(legend,"gen");
        sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
        legend->Draw("same");
      }

      if (widths_hist_data_all.at(m).at(p) -> GetFunction("linfit")) {
        f = widths_hist_data_all.at(m).at(p) -> GetFunction("linfit");
        legend = tdrLeg(0.30,0.70,0.50,0.85);
        legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
        tdrHeader(legend,"Data");
        sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
        sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
        legend->Draw("same");
      }

      TLegend *leg = tdrLeg(0.35,0.85,0.9,0.89);
      char legTitle[100];
      sprintf(legTitle, "#eta#in [%.3f,%.3f], p_{T}#in [%.0f,%.0f] GeV", eta_bins_edge[m], eta_bins_edge[m+1], p_bins_edge[p], p_bins_edge[p+1]);
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
      PLOT_3_2(widths_hist_FE,widths_hist_gen_FE,widths_hist_data_FE,kRed,kGreen+2,kBlue,m,p)

      if( widths_hist_FE.at(m).at(p)->GetEntries() != 0 ){
        widths_hist_FE.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kRed);
      }
      if( widths_hist_gen_FE.at(m).at(p)->GetEntries() != 0 ){
        widths_hist_gen_FE.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kGreen+2);
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
      tdrHeader(legend,"MC");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      if( widths_hist_gen_FE.at(m).at(p)->GetEntries() != 0 ){
        f = widths_hist_gen_FE.at(m).at(p) -> GetFunction("linfit");
      }
      legend = tdrLeg(0.70,0.70,0.90,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
      tdrHeader(legend,"gen");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      if( widths_hist_data_FE.at(m).at(p)->GetEntries() != 0 ){
        f = widths_hist_data_FE.at(m).at(p) -> GetFunction("linfit");
      }
      legend = tdrLeg(0.30,0.70,0.50,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      tdrHeader(legend,"Data");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      TLegend *leg = tdrLeg(0.35,0.85,0.9,0.89);
      char legTitle[100];
      sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
      eta_bins_edge[eta_bin + eta_bins_fe - widths_hist_FE.size() + m],
      eta_bins_edge[eta_bin + eta_bins_fe - widths_hist_FE.size() + m+1],
      p_bins_edge_FT[p], p_bins_edge_FT[p+1]);
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

  for( unsigned int m = 0; m < soft_widths_hist_all.size(); m++ ){
    for( unsigned int p = 0; p < soft_widths_hist_all.at(m).size(); p++ ){
      PLOT_3_2(soft_widths_hist_all,soft_widths_hist_gen_all,soft_widths_hist_data_all,kRed,kGreen+2,kBlue,m,p)
      canv -> Update();
      canv -> Print("pdfy/widths/allPoints_"+canvName+".png","png");
    }
  }

  for( unsigned int m = 0; m < soft_widths_hist_FE.size(); m++ ){
    for( unsigned int p = 0; p < soft_widths_hist_FE.at(m).size(); p++ ){
      PLOT_3_2(soft_widths_hist_FE,soft_widths_hist_gen_FE,soft_widths_hist_data_FE,kRed,kGreen+2,kBlue,m,p)
      canv -> Update();
      canv -> Print("pdfy/widths/allPoints_"+canvName+".png","png");
    }
  }

  TFile widthroot("output/widths.root","RECREATE");

  for( unsigned int m = 0; m < widths_hist_all.size(); m++ ){
    for( unsigned int p = 0; p < widths_hist_all.at(m).size(); p++ ){
      widths_hist_all.at(m).at(p) -> Write();
      widths_hist_gen_all.at(m).at(p) -> Write();
      widths_hist_data_all.at(m).at(p) -> Write();
    }
  }
  for( unsigned int m = 0; m < widths_hist_FE.size(); m++ ){
    for( unsigned int p = 0; p < widths_hist_FE.at(m).size(); p++ ){
      widths_hist_FE.at(m).at(p) -> Write();
      widths_hist_gen_FE.at(m).at(p) -> Write();
      widths_hist_data_FE.at(m).at(p) -> Write();
    }
  }

  widthroot.Close();

  // ------------------------------
  //       correlation plots
  // ------------------------------

  for( unsigned int m = 0; m < MC_correlated_graphs.size(); m++ ){
    for( unsigned int p = 0; p < MC_correlated_graphs.at(m).size(); p++ ){
      // draw extrapolations data + mc
      TF1* f;
      TF1* Temp = new TF1();
      double range = 0.05;
      if ( p == 0) range = 0.25;
      if ( p == 1) range = 0.2;
      if ( p == 2) range = 0.15;
      if ( p == 3) range = 0.1;
      if ( p == 4) range = 0.1;

      char line[100];
      TLegend *legend;
      // gStyle -> SetOptFit(0000);
      // gStyle -> SetOptStat(0000);
      PLOT_3_2_gr(MC_correlated_graphs,gen_correlated_graphs,data_correlated_graphs,kRed,kGreen+2,kBlue,m,p)

      f = MC_correlated_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kRed); f->SetLineStyle(2);
      Temp = (TF1*) MC_correlated_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.50,0.70,0.70,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
      tdrHeader(legend,"MC");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      f = gen_correlated_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kGreen+2); f->SetLineStyle(2);
      Temp = (TF1*) gen_correlated_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.70,0.70,0.90,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
      tdrHeader(legend,"gen");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      f = data_correlated_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kBlue); f->SetLineStyle(2);
      Temp = (TF1*) data_correlated_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.30,0.70,0.50,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      tdrHeader(legend,"Data");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      TLegend *leg = tdrLeg(0.35,0.85,0.9,0.89);
      char legTitle[100];
      sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV", eta_bins_edge[m], eta_bins_edge[m+1], p_bins_edge[p], p_bins_edge[p+1]);
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

  for( unsigned int m = 0; m < FE_MC_correlated_graphs.size(); m++ ){
    for( unsigned int p = 0; p < FE_MC_correlated_graphs.at(m).size(); p++ ){
      // draw extrapolations data + mc
      TF1* f;
      TF1* Temp = new TF1();
      double range = 0.05;
      if ( p == 0) range = 0.2;
      if ( p == 1) range = 0.15;
      if ( p == 2) range = 0.15;
      if ( p == 3) range = 0.1;
      char line[100];
      TLegend *legend;

      PLOT_3_2_gr(FE_MC_correlated_graphs,FE_gen_correlated_graphs,FE_data_correlated_graphs,kRed,kGreen+2,kBlue,m,p)

      f = FE_MC_correlated_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kRed); f->SetLineStyle(2);
      Temp = (TF1*) FE_MC_correlated_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.50,0.70,0.70,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
      tdrHeader(legend,"MC");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      f = FE_gen_correlated_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kGreen+2); f->SetLineStyle(2);
      Temp = (TF1*) FE_gen_correlated_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.70,0.70,0.90,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
      tdrHeader(legend,"gen");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      f = FE_data_correlated_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kBlue); f->SetLineStyle(2);
      Temp = (TF1*) FE_data_correlated_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
      Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
      legend = tdrLeg(0.30,0.70,0.50,0.85);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      tdrHeader(legend,"Data");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");

      TLegend *leg = tdrLeg(0.35,0.85,0.9,0.89);
      char legTitle[100];
      sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
      eta_bins_edge[eta_bin + eta_bins_fe - FE_MC_correlated_graphs.size() + m],
      eta_bins_edge[eta_bin + eta_bins_fe - FE_MC_correlated_graphs.size() + m+1],
      p_bins_edge_FT[p], p_bins_edge_FT[p+1]);
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

  */
  /////////////////////////////////////////////////////////////////////////////////////////
  // plot with JERs with NSC fit
  /////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_uncorrelated_data_hist.size(); m++ ){
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1000);
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    JER_uncorrelated_MC_hist.at(m) -> Fit("mcFIT", "RMQ+");
    double N = mcFIT -> GetParameter( 0 );
    double S = mcFIT -> GetParameter( 1 );
    double C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])", 50, 1000);
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(0, N);
    dtFIT -> FixParameter(1, S);
    dtFIT -> FixParameter(2, C);
    JER_uncorrelated_data_hist.at(m) -> Fit("dtFIT", "RMQ+");
    double kNS = dtFIT -> GetParameter( 3 );
    double kC = dtFIT -> GetParameter( 4 );
    JER_uncorrelated_data_hist.at(m)->SetStats(kFALSE);
    JER_uncorrelated_MC_hist.at(m)->SetStats(kFALSE);

    PLOT_2(JER_uncorrelated_data_hist,JER_uncorrelated_MC_hist,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_uncorrelated_data_hist.at(m),"data","lep");
    leg->AddEntry(JER_uncorrelated_MC_hist.at(m),"MC","lep");

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

    canv -> Update();
    canvName = canvName(0, canvName.Length()-1);
    canvName += (m+1);
    canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/"+canvName+".png","png");

    PLOT_1_2(JER_uncorrelated_scale_hist,kBlue-4,m)

    TF1 * constfit = new TF1( "constfit", "pol0", 0, 1000 );
    constfit->SetLineColor(kRed+1);
    constfit->SetLineWidth(2);
    JER_uncorrelated_scale_hist.at(m) -> Fit("constfit","RMQ+");

    TF1 *NSC_ratio = new TF1("NSC_ratio","TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])/TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )",0,1000);
    NSC_ratio -> SetParameters(N, S, C, kNS, kC);
    NSC_ratio->SetLineColor(kGreen+2);
    NSC_ratio->SetLineWidth(2);
    NSC_ratio->Draw("same");
    JER_uncorrelated_scale_hist.at(m)->GetListOfFunctions()->Add(NSC_ratio);

    leg = tdrLeg(0.55,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_uncorrelated_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc} uncorrelated","lep");
    leg->AddEntry(constfit,"Constant Fit","l");
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
    leg->AddEntry(NSC_ratio,"Ratio of NSC-Fits","l");

    canv -> Print("pdfy/NSC_SFs/NSC"+canvName+".pdf","pdf");
    canv -> Print("pdfy/NSC_SFs/NSC"+canvName+".png","png");
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // same kind of plot but for correlated results
  /////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_correlated_data_hist.size(); m++ ){ // plot with JERs with NSC fit
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1000);
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    JER_correlated_MC_hist.at(m) -> Fit("mcFIT", "RMQ+");
    double N = mcFIT -> GetParameter( 0 );
    double S = mcFIT -> GetParameter( 1 );
    double C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])", 50, 1000);
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(0, N);
    dtFIT -> FixParameter(1, S);
    dtFIT -> FixParameter(2, C);
    JER_correlated_data_hist.at(m) -> Fit("dtFIT", "RMQ+");
    double kNS = dtFIT -> GetParameter( 3 );
    double kC = dtFIT -> GetParameter( 4 );
    JER_correlated_data_hist.at(m)->SetStats(kFALSE);
    JER_correlated_MC_hist.at(m)->SetStats(kFALSE);

    PLOT_2(JER_correlated_data_hist,JER_correlated_MC_hist,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_correlated_data_hist.at(m),"data","lep");
    leg->AddEntry(JER_correlated_MC_hist.at(m),"MC","lep");

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

    canvName = canvName(0, canvName.Length()-1);
    canvName += (m+1);
    canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/"+canvName+".png","png");


    PLOT_1_2(JER_correlated_scale_hist,kBlue-4,m)

    TF1 * constfit = new TF1( "constfit", "pol0", 0, 1000 );
    constfit->SetLineColor(kRed+1);
    constfit->SetLineWidth(2);
    JER_correlated_scale_hist.at(m) -> Fit("constfit","RMQ+");

    TF1 *NSC_ratio = new TF1("NSC_ratio","TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])/TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )",0,1000);
    NSC_ratio -> SetParameters(N, S, C, kNS, kC);
    NSC_ratio->SetLineColor(kGreen+2);
    NSC_ratio->SetLineWidth(2);
    NSC_ratio->Draw("same");
    JER_correlated_scale_hist.at(m)->GetListOfFunctions()->Add(NSC_ratio);

    leg = tdrLeg(0.55,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_correlated_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc} correlated","lep");
    leg->AddEntry(constfit,"Constant Fit","l");
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
    leg->AddEntry(NSC_ratio,"Ratio of NSC-Fits","l");

    canv -> Print("pdfy/NSC_SFs/NSC_"+canvName+".pdf","pdf");
    canv -> Print("pdfy/NSC_SFs/NSC_"+canvName+".png","png");
  }


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  JER plots for FE with NSC fits
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_FE_uncorrelated_data_hist.size(); m++ ){
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1000);
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    JER_FE_uncorrelated_MC_hist.at(m) -> Fit("mcFIT", "RMQ+");
    double N = mcFIT -> GetParameter( 0 );
    double S = mcFIT -> GetParameter( 1 );
    double C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])", 50, 1000);
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(0, N);
    dtFIT -> FixParameter(1, S);
    dtFIT -> FixParameter(2, C);
    JER_FE_uncorrelated_data_hist.at(m) -> Fit("dtFIT", "RMQ+");
    double kNS = dtFIT -> GetParameter( 3 );
    double kC = dtFIT -> GetParameter( 4 );
    JER_FE_uncorrelated_data_hist.at(m)->SetStats(kFALSE);
    JER_FE_uncorrelated_MC_hist.at(m)->SetStats(kFALSE);

    PLOT_2(JER_FE_uncorrelated_data_hist,JER_FE_uncorrelated_MC_hist,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_control_edge[m], eta_bins_control_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_FE_uncorrelated_data_hist.at(m),"data","lep");
    leg->AddEntry(JER_FE_uncorrelated_MC_hist.at(m),"MC","lep");

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

    canv -> Update();
    canvName = canvName(0, canvName.Length()-1);
    canvName += (m+1);
    canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/"+canvName+".png","png");

    PLOT_1_2(JER_FE_uncorrelated_scale_hist,kBlue-4,m)

    TF1 * constfit = new TF1( "constfit", "pol0", 0, 1000 );
    constfit->SetLineColor(kRed+1);
    constfit->SetLineWidth(2);
    JER_FE_uncorrelated_scale_hist.at(m) -> Fit("constfit","RMQ+");

    TF1 *NSC_ratio = new TF1("NSC_ratio","TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])/TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )",0,1000);
    NSC_ratio -> SetParameters(N, S, C, kNS, kC);
    NSC_ratio->SetLineColor(kGreen+2);
    NSC_ratio->SetLineWidth(2);
    NSC_ratio->Draw("same");
    JER_FE_uncorrelated_scale_hist.at(m)->GetListOfFunctions()->Add(NSC_ratio);

    leg = tdrLeg(0.55,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_control_edge[m], eta_bins_control_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_FE_uncorrelated_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc} uncorrelated","lep");
    leg->AddEntry(constfit,"Constant Fit","l");
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_control_edge[m], eta_bins_control_edge[m+1]);
    leg->AddEntry(NSC_ratio,"Ratio of NSC-Fits","l");

    canv -> Print("pdfy/NSC_SFs/NSC"+canvName+".pdf","pdf");
    canv -> Print("pdfy/NSC_SFs/NSC"+canvName+".png","png");
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // same kind of plot but for correlated results
  /////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_FE_correlated_data_hist.size(); m++ ){ // plot with JERs with NSC fit
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1000);
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    JER_FE_correlated_MC_hist.at(m) -> Fit("mcFIT", "RMQ+");
    double N = mcFIT -> GetParameter( 0 );
    double S = mcFIT -> GetParameter( 1 );
    double C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])", 50, 1000);
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(0, N);
    dtFIT -> FixParameter(1, S);
    dtFIT -> FixParameter(2, C);
    JER_FE_correlated_data_hist.at(m) -> Fit("dtFIT", "RMQ+");
    double kNS = dtFIT -> GetParameter( 3 );
    double kC = dtFIT -> GetParameter( 4 );
    JER_FE_correlated_data_hist.at(m)->SetStats(kFALSE);
    JER_FE_correlated_MC_hist.at(m)->SetStats(kFALSE);

    PLOT_2(JER_FE_correlated_data_hist,JER_FE_correlated_MC_hist,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.55,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_control_edge[m], eta_bins_control_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_correlated_data_hist.at(m),"data","lep");
    leg->AddEntry(JER_correlated_MC_hist.at(m),"MC","lep");

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

    canvName = canvName(0, canvName.Length()-1);
    canvName += (m+1);
    canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/"+canvName+".png","png");


    PLOT_1_2(JER_FE_correlated_scale_hist,kBlue-4,m)

    TF1 * constfit = new TF1( "constfit", "pol0", 0, 1000 );
    constfit->SetLineColor(kRed+1);
    constfit->SetLineWidth(2);
    JER_FE_correlated_scale_hist.at(m) -> Fit("constfit","RMQ+");

    TF1 *NSC_ratio = new TF1("NSC_ratio","TMath::Sqrt( [3]*[3]*[0]*[0]/x*x+[3]*[1]*[1]/x+[4]*[4]*[2]*[2])/TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )",0,1000);
    NSC_ratio -> SetParameters(N, S, C, kNS, kC);
    NSC_ratio->SetLineColor(kGreen+2);
    NSC_ratio->SetLineWidth(2);
    NSC_ratio->Draw("same");
    JER_FE_correlated_scale_hist.at(m)->GetListOfFunctions()->Add(NSC_ratio);

    leg = tdrLeg(0.55,0.7,0.9,0.9);
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_control_edge[m], eta_bins_control_edge[m+1]);
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_FE_correlated_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc} correlated","lep");
    leg->AddEntry(constfit,"Constant Fit","l");
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_control_edge[m], eta_bins_control_edge[m+1]);
    leg->AddEntry(NSC_ratio,"Ratio of NSC-Fits","l");

    canv -> Print("pdfy/NSC_SFs/NSC_"+canvName+".pdf","pdf");
    canv -> Print("pdfy/NSC_SFs/NSC_"+canvName+".png","png");
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TFile JERroot("output/JERs.root","RECREATE");

  for( unsigned int m = 0; m < JER_uncorrelated_MC_hist.size(); m++ ){              JER_uncorrelated_MC_hist.at(m)              -> Write();}
  for( unsigned int m = 0; m < JER_uncorrelated_data_hist.size(); m++ ){            JER_uncorrelated_data_hist.at(m)            -> Write();}
  for( unsigned int m = 0; m < JER_FE_uncorrelated_MC_hist.size(); m++ ){           JER_FE_uncorrelated_MC_hist.at(m)           -> Write();}
  for( unsigned int m = 0; m < JER_FE_uncorrelated_data_hist.size(); m++ ){         JER_FE_uncorrelated_data_hist.at(m)         -> Write();}
  for( unsigned int m = 0; m < JER_FE_uncorrelated_MC_control_hist.size(); m++ ){   JER_FE_uncorrelated_MC_control_hist.at(m)   -> Write();}
  for( unsigned int m = 0; m < JER_FE_uncorrelated_data_control_hist.size(); m++ ){ JER_FE_uncorrelated_data_control_hist.at(m) -> Write();}
  for( unsigned int m = 0; m < JER_correlated_MC_hist.size(); m++ ){                JER_correlated_MC_hist.at(m)                -> Write();}
  for( unsigned int m = 0; m < JER_correlated_data_hist.size(); m++ ){              JER_correlated_data_hist.at(m)              -> Write();}
  for( unsigned int m = 0; m < JER_FE_correlated_MC_hist.size(); m++ ){             JER_FE_correlated_MC_hist.at(m)             -> Write();}
  for( unsigned int m = 0; m < JER_FE_correlated_data_hist.size(); m++ ){           JER_FE_correlated_data_hist.at(m)           -> Write();}
  for( unsigned int m = 0; m < JER_FE_correlated_MC_control_hist.size(); m++ ){     JER_FE_correlated_MC_control_hist.at(m)     -> Write();}
  for( unsigned int m = 0; m < JER_FE_correlated_data_control_hist.size(); m++ ){   JER_FE_correlated_data_control_hist.at(m)   -> Write();}

  JERroot.Close();

  TFile gbis("output/SFs.root","RECREATE");

  // for( unsigned int m = 0; m < JER_FE_uncorrelated_scale_control_hist.size(); m++ ){
  //   TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  //   JER_FE_uncorrelated_scale_control_hist.at(m) -> Fit("constfit","RMQ+");
  //   JER_FE_uncorrelated_scale_control_hist.at(m) -> Write();
  // }
  //
  // for( unsigned int m = 0; m < JER015_scale_hist.size(); m++ ){
  //   TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  //   JER015_scale_hist.at(m) -> Fit("constfit","RMQ+");
  //   JER015_scale_hist.at(m) -> Write();
  // }
  //
  // for( unsigned int m = 0; m < JER015_FE_scale_hist.size(); m++ ){
  //   TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  //   JER015_FE_scale_hist.at(m) -> Fit("constfit","RMQ+");
  //   JER015_FE_scale_hist.at(m) -> Write();
  // }

  std::vector<double> SF_uncorrelated_SM, SF_uncorrelated_SM_error, SF_correlated_SM, SF_correlated_SM_error, SF_uncorrelated_FE, SF_uncorrelated_FE_error, SF_correlated_FE, SF_correlated_FE_error, eta_bin_SM_center, eta_bin_SM_error, eta_bin_FE_center, eta_bin_FE_error;
  std::vector<double> SF_uncorrelated_SM_ptdep_min, SF_correlated_SM_ptdep_min, SF_uncorrelated_FE_ptdep_min, SF_correlated_FE_ptdep_min;
  std::vector<double> SF_uncorrelated_SM_ptdep_max, SF_correlated_SM_ptdep_max, SF_uncorrelated_FE_ptdep_max, SF_correlated_FE_ptdep_max;
  ofstream texfile;
  texfile.open ("output/scalefactors_tex.txt");

  texfile << "standard method\n";
  for( unsigned int m = 0; m < JER_uncorrelated_scale_hist.size(); m++ ){
    TF1 * constfit = JER_uncorrelated_scale_hist.at(m) -> GetFunction("constfit");
    TF1 * NSC_ratio = JER_uncorrelated_scale_hist.at(m) -> GetFunction("NSC_ratio");
    SF_uncorrelated_SM_ptdep_min.push_back(findMinMax(asymmetries_width_data_pt.at(m)[0], NSC_ratio, constfit, 0));
    SF_uncorrelated_SM_ptdep_max.push_back(findMinMax(asymmetries_width_data_pt.at(m)[0], NSC_ratio, constfit, 1));
    JER_uncorrelated_scale_hist.at(m) -> Write();
    eta_bin_SM_center.push_back((eta_bins_edge[m+1]+eta_bins_edge[m]) /2);
    eta_bin_SM_error.push_back((eta_bins_edge[m+1]-eta_bins_edge[m]) /2);
    SF_uncorrelated_SM.push_back(constfit -> GetParameter( 0 ));
    SF_uncorrelated_SM_error.push_back(constfit -> GetParError( 0 ));
    texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
    if(m == JER_uncorrelated_scale_hist.size()-1 ) texfile << "\\\\";
  }

  texfile << "\n forward extension \n";
  for( unsigned int m = 0; m < JER_FE_uncorrelated_scale_hist.size(); m++ ){
    TF1 * constfit = JER_FE_uncorrelated_scale_hist.at(m) -> GetFunction("constfit");
    TF1 * NSC_ratio = JER_FE_uncorrelated_scale_hist.at(m) -> GetFunction("NSC_ratio");
    SF_uncorrelated_FE_ptdep_min.push_back(findMinMax(forward_width_data_pt.at(m)[0], NSC_ratio, constfit, 0));
    SF_uncorrelated_FE_ptdep_max.push_back(findMinMax(forward_width_data_pt.at(m)[0], NSC_ratio, constfit, 1));
    JER_FE_uncorrelated_scale_hist.at(m) -> Write();
    int diff = eta_bin + eta_bins_fe - JER_FE_uncorrelated_scale_hist.size();
    eta_bin_FE_center.push_back((eta_bins_edge[diff+m+1]+eta_bins_edge[diff+m]) /2);
    eta_bin_FE_error.push_back((eta_bins_edge[diff+m+1]-eta_bins_edge[diff+m]) /2);
      SF_uncorrelated_FE.push_back(constfit -> GetParameter( 0 ));
    SF_uncorrelated_FE_error.push_back(constfit -> GetParError( 0 ));
    texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
    if(m == JER_FE_uncorrelated_scale_hist.size()-1 ) texfile << "\\\\";
  }

  texfile << "\n standard method, correlated fit\n";
  for( unsigned int m = 0; m < JER_correlated_scale_hist.size(); m++ ){
    TF1 * constfit = JER_correlated_scale_hist.at(m) -> GetFunction("constfit");
    TF1 * NSC_ratio = JER_correlated_scale_hist.at(m) -> GetFunction("NSC_ratio");
    SF_correlated_SM_ptdep_min.push_back(findMinMax(asymmetries_width_data_pt.at(m)[0], NSC_ratio, constfit, 0));
    SF_correlated_SM_ptdep_max.push_back(findMinMax(asymmetries_width_data_pt.at(m)[0], NSC_ratio, constfit, 1));
    JER_correlated_scale_hist.at(m) -> Write();
    SF_correlated_SM.push_back(constfit -> GetParameter( 0 ));
    SF_correlated_SM_error.push_back(constfit -> GetParError( 0 ));
    texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
    if(m == JER_correlated_scale_hist.size()-1 ) texfile << "\\\\";
  }

  texfile << "\n forward extension, correlated fit \n";
  for( unsigned int m = 0; m < JER_FE_correlated_scale_hist.size(); m++ ){
    TF1 * constfit = JER_FE_correlated_scale_hist.at(m) -> GetFunction("constfit");
    TF1 * NSC_ratio = JER_FE_correlated_scale_hist.at(m) -> GetFunction("NSC_ratio");
    SF_correlated_FE_ptdep_min.push_back(findMinMax(forward_width_data_pt.at(m)[0], NSC_ratio, constfit, 0));
    SF_correlated_FE_ptdep_max.push_back(findMinMax(forward_width_data_pt.at(m)[0], NSC_ratio, constfit, 1));
    JER_FE_correlated_scale_hist.at(m) -> Write();
    SF_correlated_FE.push_back(constfit -> GetParameter( 0 ));
    SF_correlated_FE_error.push_back(constfit -> GetParError( 0 ));
    texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
    if(m == JER_FE_correlated_scale_hist.size()-1 ) texfile << "\\\\";
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
  for (unsigned int i = 0; i < JER_uncorrelated_scale_hist.size(); i++) {
    txt_ST << eta_bin_SM_center[i] << " " << eta_bin_SM_error[i] << " " << SF_uncorrelated_SM[i] << " " << SF_uncorrelated_SM_error[i] << " " << SF_correlated_SM[i] << " " << SF_correlated_SM_error[i] << " " << SF_uncorrelated_SM_ptdep_min[i] << " " << SF_uncorrelated_SM_ptdep_max[i] << " " << SF_correlated_SM_ptdep_min[i] << " " << SF_correlated_SM_ptdep_max[i] << "\n";
    // std::cout << eta_bin_SM_center[i] << " " << eta_bin_SM_error[i] << " " << SF_uncorrelated_SM[i] << " " << SF_uncorrelated_SM_error[i] << " " << SF_correlated_SM[i] << " " << SF_correlated_SM_error[i] << "\n";
  }
  for (unsigned int i = 0; i < JER_FE_uncorrelated_scale_hist.size(); i++) {
    txt_FE << eta_bin_FE_center[i] << " " << eta_bin_FE_error[i] << " " << SF_uncorrelated_FE[i] << " " << SF_uncorrelated_FE_error[i] << " " << SF_correlated_FE[i] << " " << SF_correlated_FE_error[i] << " " << SF_uncorrelated_FE_ptdep_min[i] << " " << SF_uncorrelated_FE_ptdep_max[i] << " " << SF_correlated_FE_ptdep_min[i] << " " << SF_correlated_FE_ptdep_max[i] << "\n";
    // std::cout << eta_bin_FE_center[i] << " " << eta_bin_FE_error[i] << " " << SF_uncorrelated_FE[i] << " " << SF_uncorrelated_FE_error[i] << " " << SF_correlated_FE[i] << " " << SF_correlated_FE_error[i] << "\n";
  }
  txt_ST.close();
  txt_FE.close();

  TCanvas* canv_SF = tdrCanvas("JER SF",eta_bins_edge[0]-0.1, eta_bins_edge[eta_bin + eta_bins_fe]+0.5, 0.8, 3.0, "#eta", "JER SF");
  canv_SF->SetTickx(0);
  canv_SF->SetTicky(0);
  TGraphErrors *gr_st = new TGraphErrors(JER_correlated_scale_hist.size(), &(eta_bin_SM_center[0]), &(SF_correlated_SM[0]), &(eta_bin_SM_error[0]), &(SF_correlated_SM_error[0]));
  TGraphErrors *gr_fe = new TGraphErrors(JER_FE_correlated_scale_hist.size(), &(eta_bin_FE_center[0]), &(SF_correlated_FE[0]), &(eta_bin_FE_error[0]), &(SF_correlated_FE_error[0]));
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

  for( unsigned int m = 0; m < JER_uncorrelated_scale_hist.size(); m++ ){
    TF1* f;
    char line[100];
    TLegend *legend;

    PLOT_3(JER_uncorrelated_scale_hist,JER_correlated_scale_hist,JER015_scale_hist,kBlue,kRed, kGreen+2,m)

    if(JER_uncorrelated_scale_hist.at(m)->GetFunction("constfit")){
      f = JER_uncorrelated_scale_hist.at(m) -> GetFunction("constfit");
      f->SetLineColor(kBlue);
      legend = tdrLeg(0.30,0.70,0.50,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      tdrHeader(legend,"JER", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

    if(JER_correlated_scale_hist.at(m)->GetFunction("constfit")){
      f = JER_correlated_scale_hist.at(m) -> GetFunction("constfit");
      f->SetLineColor(kRed);
      legend = tdrLeg(0.50,0.70,0.70,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
      tdrHeader(legend,"JER cor", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

    if(JER015_scale_hist.at(m)->GetFunction("constfit")){
      f = JER015_scale_hist.at(m) -> GetFunction("constfit");
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
    sprintf(legTitle, "#eta #in [%.3f,%.3f]",eta_bins_edge[m], eta_bins_edge[m+1]);

    tdrHeader(leg,legTitle);
    leg->SetTextFont(42);  leg->SetTextSize(0.04);
    leg->AddEntry(JER_uncorrelated_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{MC}","lep");
    leg->AddEntry(JER_correlated_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{MC} corelated","lep");
    leg->AddEntry(JER015_scale_hist.at(m),"#sigma_{0.15}^{data}/#sigma_{0.15}^{MC}","lep");
    leg->Draw("same");

    canv -> Print("pdfy/SFs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/SFs/"+canvName+".png","png");
  }

  for( unsigned int m = 0; m < JER_FE_uncorrelated_scale_hist.size(); m++){
    TF1* f;
    char line[100];
    TLegend *legend;

    PLOT_3(JER_FE_uncorrelated_scale_hist,JER_FE_correlated_scale_hist,JER015_FE_scale_hist,kBlue,kRed,kGreen+2,m)

    if(JER_FE_uncorrelated_scale_hist.at(m)->GetFunction("constfit")){
      f = JER_FE_uncorrelated_scale_hist.at(m) -> GetFunction("constfit");   f->SetLineColor(kBlue);
      legend = tdrLeg(0.30,0.70,0.50,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);     legend->SetTextColor(kBlue);
      tdrHeader(legend,"JER", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl;
  }

    if(JER_FE_correlated_scale_hist.at(m)->GetFunction("constfit")){
      f = JER_FE_correlated_scale_hist.at(m) -> GetFunction("constfit");   f->SetLineColor(kRed);
      legend = tdrLeg(0.50,0.70,0.70,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);         legend->SetTextColor(kRed);
      tdrHeader(legend,"JER cor", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

    if(JER015_FE_scale_hist.at(m)->GetFunction("constfit")){
      f = JER015_FE_scale_hist.at(m) -> GetFunction("constfit");    f->SetLineColor(kGreen+2);
      legend = tdrLeg(0.70,0.70,0.90,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);         legend->SetTextColor(kGreen+2);
      tdrHeader(legend,"JER 0.15", 22);
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    } else { std::cout << "Fit function at bin " << m << " was not found " << std::endl;}

    char legTitle[100];
    TLegend *leg = tdrLeg(0.6,0.15,0.9,0.35);
    sprintf(legTitle, "#eta #in [%.3f,%.3f]",eta_bins_edge[eta_bin + eta_bins_fe - JER_uncorrelated_scale_hist.size() + m], eta_bins_edge[eta_bin + eta_bins_fe - JER_uncorrelated_scale_hist.size() + m+1]);

    tdrHeader(leg,legTitle);
    leg->SetTextFont(42);  leg->SetTextSize(0.04);
    leg->AddEntry(JER_FE_uncorrelated_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{MC}","lep");
    leg->AddEntry(JER_FE_correlated_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{MC} corelated","lep");
    leg->AddEntry(JER015_FE_scale_hist.at(m),"#sigma_{0.15}^{data}/#sigma_{0.15}^{MC}","lep");
    leg->Draw("same");

    canv -> Print("pdfy/SFs/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/SFs/"+canvName+".png","png");
  }

  SFsoverlayed.Close();

  return true;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


  std::ifstream myfile;
  //    File stream: MC Truth resolutions:
  myfile.open("/nfs/dust/cms/user/niedziem/resolutions_input/Fall17_17Nov2017_V4_MC_PtResolution_ak4pfchsl1l2l3_modified.txt");
  //    myfile.open("/nfs/dust/cms/user/niedziem/resolutions_input/Spring16_25nsV10_MC_SF_AK4PFchs_modified.txt");

  std::string temp;
  std::vector<std::vector<double> > resolutions;

  while (std::getline(myfile, temp)) {
    std::istringstream buffer(temp);
    // Read MC Res line by line (two files opened above)
    // Each line is read word by word into a vector
    std::vector<double> line((std::istream_iterator<double>(buffer)),
    std::istream_iterator<double>());

    resolutions.push_back(line);
  }
  myfile.close();

  std::vector<TF1 *> MCTruth_Chad;
  for( unsigned int m = 0; m < resolutions.size(); m++ ){
    TString name_TF1 = "MCTruth_Chad";
    name_TF1 += m;
    double par_a, par_b, par_c, par_d;
    par_a = resolutions.at(m).at(7);
    par_b = resolutions.at(m).at(8);
    par_c = resolutions.at(m).at(9);
    par_d = resolutions.at(m).at(10);
    TF1 * tempTF1 = new TF1( name_TF1, "TMath::Sqrt([0]*TMath::Abs([0])/(x*x)+[1]*[1]*pow(x,[3])+[2]*[2])",  0, 1000 );
    tempTF1 -> SetParameter( 0, par_a );
    tempTF1 -> SetParameter( 1, par_b );
    tempTF1 -> SetParameter( 2, par_c );
    tempTF1 -> SetParameter( 3, par_d );
    tempTF1->SetLineColor(kBlue+2);  // sets the color to red
    tempTF1->SetLineStyle(1);
    MCTruth_Chad.push_back(tempTF1);
  }

  gStyle -> SetOptFit(0);
  gStyle -> SetOptStat(0);

  TFile test("output/MCTruth_1.root","RECREATE");
  for( unsigned int m = 0; m < JER_MC_Truth.size(); m++ ){

    PLOT_1(JER_MC_Truth,kRed,m)

    // for( unsigned int i = 0; i < 7; i++ ){
    //    MCTruth_Chad.at( 7*m + i ) -> Draw("same");
    //    MCTruth_Chad.at( 7*m + i ) -> SetMarkerColor(29+i);
    //    canv->Update();
    // }
    //         if( m == 1 ){
    //            MCTruth_Chad.at(13) -> Draw("same");
    //			   canv->Update();
    //         }
    //         if( m == 4 ){
    //            MCTruth_Chad.at(17) -> Draw("same");
    //			   canv->Update();
    //         }
    //         if( m == 5 ){
    //            MCTruth_Chad.at(19) -> Draw("same");
    //			   canv->Update();
    //         }

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    leg->AddEntry(JER_MC_Truth   .at(m),"MC Truth"        ,"lep");
    leg->AddEntry(JER_uncorrelated_MC_hist    .at(m),"Normal DiJet"    ,"lep");
    leg->AddEntry(JER_correlated_MC_hist.at(m),"Correlated DiJet","lep");
    // leg->AddEntry(MCTruth_Chad.at(m),   "MCTruth from Chad","l");

    canv->Update();
    canv->Print("pdfy/MCTruth/"+canvName+".pdf","pdf");
    canv->Print("pdfy/MCTruth/"+canvName+".png","png");
    delete canv;
  }

  test.Close();

  {

    // PLOT_3_MCT(JER_MC_Truth_FE,JER_FE_uncorrelated_MC_control_hist,JER_FE_correlated_MC_control_hist,kRed,kBlue,kMagenta,0)

    // for( unsigned int i = 0; i < 14; i++ ){
    //    MCTruth_Chad.at( i ) -> Draw("same");
    // MCTruth_Chad.at( i ) -> SetMarkerColor(29+i);
    // canv->Update();
    // }
    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    leg->AddEntry(JER_MC_Truth_FE      .at(0), "MC Truth", "lep");
    leg->AddEntry(JER_FE_uncorrelated_MC_control_hist    .at(0), "Normal DiJet", "lep");
    leg->AddEntry(JER_FE_correlated_MC_control_hist.at(0), "Correlated DiJet", "lep");
    // leg->AddEntry(MCTruth_Chad           .at(0),"MCTruth from Chad","l");

    // canv->Update();
    // canv -> Print("pdfy/MCTruth/FE_reference.pdf","pdf");
    // canv -> Print("pdfy/MCTruth/FE_reference.png","png");
  }

  return 0;
}
// cout << "trovato " << __LINE__ << endl;

// number of event.
// cout << "calcola " << JER_FE_uncorrelated_scale_control_hist.size()  << " " << widths_hist_FE.at(0).size() << " " << soft_widths_hist_FE.size() << endl;
// aiuto
// cout << "calcola " << widths_hist_all.size()  << " " << widths_hist_all.at(m).size() << endl;
// cout << "calcola_2 " << forward_hist.size() <<  " "  << forward_hist.at(m).size()  <<  " "  << forward_hist.at(m).at(p).size()  << endl;
