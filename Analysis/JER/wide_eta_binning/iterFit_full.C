#include <iostream>
#include <TChain.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TFile.h>
#include <TLine.h>
#include <TStyle.h>
#include <string>
#include <TMath.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TGraphAsymmErrors.h>
#include "functions.C"
#include "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/include/constants.h"

#include "TFrame.h"
// #include "plotting/CMS_lumi.C"
// #include "plotting/tdrstyle_mod15.C"
#include "tdrstyle_all.C"
// #include "plotting/tdrstyle.C"

#include <fstream>
#include <sstream>
// Code by Marek Niedziela
// Based on code by Matthias Schröder, Kristin Goebel

// Smeared MC instead of data:
//bool smeared = true;
// FOR DATA:
//TString label_data = "Data";
//lumi_13TeV = "2.1 fb^{-1}";
//lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
// FOR SMeared MC:
//TString label_data = "Smeared MC";
//TString label_data = "Pythia8"; //"Data";
//TString label_mc = "Herwig++"; //"MC";
//TString label_data = "Data";
//TString label_mc = "MC";
//TString lumi = "804.2 pb^{-1}";
//TString lumi = "[MC 80X]"; //"2.1 fb^{-1}";
TString sqrts = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

// in below function, fitType 0 -> gaussian fit (mean fixed to 0), 1 -> RMS (100% of points), 2 -> shifted gaussian
//void fitAsym( TH1F &hist, double &width, double &error , double &centre, double &centre_error, double &chisqr, int fitType );

void histLoadAsym( TFile &f, bool data, TString name, std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< TH1F* > > > &GenAsymmetry , int ptbin, int etabin );
void histLoadFE1( TFile &f, bool data, TString name, std::vector< std::vector< std::vector< TH1F* > > > &forward_hist , std::vector< std::vector< std::vector< TH1F* > > > &forward_gen_hist, int ptbin , int etabin);
void histMeanFE_Pt( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry ,std::vector< std::vector< std::vector< double > > > &Widths );
void histWidthAsym2( std::vector<std::vector<std::vector<TH1F*> > > &Asymmetry , std::vector<std::vector<std::vector<double> > > &Widths, std::vector<std::vector<std::vector<double> > > &WidthsError , bool fill_all );
void correctPLIbinbybin(std::vector<std::vector<std::vector<double> > > &Output, std::vector<std::vector<std::vector<double> > > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > PLI, std::vector<std::vector<std::vector<double> > > PLIError);
void fill_PLI_hist( TString name1, std::vector< std::vector< TH1F* > > &widths_FE1, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning);
void fit_NSC_PLI( std::vector< std::vector< TF1* > > &functions, std::vector< std::vector< TH1F* > > PLI_hists );
void def_fill_PLI( TString name1, std::vector<std::vector<TH1F*> > &widths_FE1, std::vector<std::vector<TF1*> > functions, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > pt_binning );
void def_fill_widths( TString name1, std::vector< std::vector< TH1F* > > &widths_FE1 , std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError);
void correctForPLI( TString name1, std::vector<std::vector<TH1F*> > widths_FE1, std::vector<std::vector<TH1F*> > &output, std::vector<std::vector<TF1*> > functions, std::vector<std::vector<std::vector<double> > > pt_binning);
void histLinFit( std::vector< std::vector< TH1F* > > widths_hist_all , std::vector< std::vector< double > > &Widths, std::vector< std::vector< double > > &WidthsError );
void fill_widths_pt( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<std::vector<double> > > forward_width_pt );
void KFSR_ratios( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<double> > WidthsTwo, std::vector<std::vector<double> > WidthsTwoError, std::vector<std::vector<std::vector<double> > > forward_width_pt);
void widths_015_ratios( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > WidthsTwo, std::vector<std::vector<std::vector<double> > > WidthsTwoError, std::vector<std::vector<std::vector<double> > > forward_width_pt );
void KNFS_line_fit( TString name1, std::vector<TH1F*> widths, std::vector< TF1* > &functions );
void correctKFSR( std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<TF1*> functions, std::vector<std::vector<std::vector<double> > > forward_width_pt );
void correctJERwithPLI( std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<double> > PLI, std::vector<std::vector<double> > PLIError);
void correctJERwithPLI015(std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > PLI, std::vector<std::vector< std::vector< double > > > PLIError);
void correctForRef( TString name1, std::vector< std::vector< double > > &Output, std::vector< std::vector< double > > &OutputError, std::vector< std::vector< double > > Input, std::vector< std::vector< double > > InputError, std::vector<std::vector<std::vector<double> > > width_pt  );
void makeScales( std::vector< std::vector< double > > &Output, std::vector< std::vector< double > > &OutputError, std::vector< std::vector< double > > Input1, std::vector< std::vector< double > > Input1Error, std::vector< std::vector< double > > Input2, std::vector< std::vector< double > > Input2Error );
// void fill_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< double > > Widths, std::vector< std::vector< double > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range);
void fill_mctruth_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range);




void fitAsym( TH1F &hist, double &width, double &error , double &centre, double &centre_error, double &chisqr, int fitType, double position , bool fix ); // linear fit for general use ( extrapolating to alpha = 0 e.g.)
void fitConst( TH1F &hist, double &value, double &error );
void fitShift( TH1F &hist, double &a, double &errora, double &b, double &errorb ); //gaussian fit, used for MC-Truth
void fitMCTruth( TH1F &hist, double &width, double &error ); // truncated RMS, points from xq_IQW to yq_IQW, rejectes xq_IQW of all points from left, and points above yq_IQW of all points. Both variables from 0 to 1, xq < yq.


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


#define PLOT_3_MCT(h1,h2,h3,c1,c2,c3,m)                                                     \
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
TCanvas* canv = tdrCanvas(canvName, x_min, x_max, y_min, y_max, nameXaxis, nameYaxis);      \
canv->SetTickx(0);                                                                          \
canv->SetTicky(0);                                                                          \
tdrDraw(h1.at(m), "", kFullCircle, c1 );                                                    \
tdrDraw(h2.at(m), "", kFullCircle, c2 );                                                    \
tdrDraw(h3.at(m), "", kFullCircle, c3 );                                                    \

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

int mainRun( bool data, const char* filename, const char* filename_data, TString lumi, TString label_mc, TString label_data, std::string Trigger)		// filename is input .root file, data determines if we work on data or mc (no gen level info in data)
{

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
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all, asymmetries_gen_all, asymmetries_all_data, mc_truth_asymmetries_all;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all_pt, asymmetries_gen_all_pt, asymmetries_all_data_pt;

  std::vector< std::vector< std::vector< TH1F* > > > forward_hist, forward_gen_hist, forward_hist_data, mc_truth_forward_hist;
  std::vector< std::vector< std::vector< TH1F* > > > forward_hist_pt, forward_gen_hist_pt, forward_hist_data_pt;

  std::vector< std::vector< std::vector< TH1F* > > > forward_gen_hist_data, asymmetries_gen_all_data, forward_gen_hist_data_pt, asymmetries_gen_all_data_pt;

  std::vector< std::vector< std::vector< TH1F* > > > dummy_hists;

  //
  // int pt_bins, pt_bins_fe, eta_bin, eta_bins_fe, eta_bins_fec;
  //
  // std::vector<double> p_bins_edge;
  // std::vector<double> p_bins_edge_FT;
  //
  // if (Trigger == "Single") {
  //   pt_bins       = n_pt_bins_Si;
  //   pt_bins_fe    = n_pt_bins_Si_HF;
  //   for (int i = 0; i < n_pt_bins_Si; i++) p_bins_edge.push_back(pt_bins_Si[i]);
  //   for (int i = 0; i < n_pt_bins_Si_HF; i++) p_bins_edge_FT.push_back(pt_bins_Si_HF[i]);
  // } else if (Trigger == "DiJet") {
  //   pt_bins       = n_pt_bins_Di;
  //   pt_bins_fe    = n_pt_bins_Di_HF;
  //   for (int i = 0; i < n_pt_bins_Di; i++) p_bins_edge.push_back(pt_bins_Di[i]);
  //   for (int i = 0; i < n_pt_bins_Di_HF; i++) p_bins_edge_FT.push_back(pt_bins_Di_HF[i]);
  // }
  //
  // p_bins_edge.push_back(1500);
  // p_bins_edge_FT.push_back(1500);
  //
  // std::cout << "help " << p_bins_edge.size() << std::endl;
  // std::cout << "help " << p_bins_edge_FT.size() << std::endl;
  // std::cout << "help " << eta_bins_edge.size() << std::endl;
  // std::cout << "help " << eta_bins_control_edge.size() << std::endl;
  // std::cout << "help " << pt_bins << std::endl;
  // std::cout << "help " << pt_bins_fe << std::endl;
  // std::cout << "help " << eta_bin << std::endl;
  // std::cout << "help " << eta_bins_fe << std::endl;
  // std::cout << "help " << eta_bins_fec << std::endl;
  //
  //
  // for (int i = 0; i < p_bins_edge.size(); i++) std::cout << "help " << p_bins_edge.at(i) << std::endl;
  // for (int i = 0; i < p_bins_edge_FT.size(); i++) std::cout << "help " << p_bins_edge_FT.at(i) << std::endl;
  // for (int i = 0; i < eta_bins_edge.size(); i++) std::cout << "help " << eta_bins_edge.at(i) << std::endl;
  // for (int i = 0; i < eta_bins_control_edge.size(); i++) std::cout << "help " << eta_bins_control_edge.at(i) << std::endl;



  // bin values only for labels at some plots

  int pt_bins = 9, pt_bins_fe = 6, eta_bin = 10, eta_bins_fe = 3, eta_bins_fec = 8, alpha_bins;


  std::vector<double> p_bins_edge;
  std::vector<double> p_bins_edge_FT;

  if (Trigger == "Single") {
    pt_bins       = n_pt_bins_Si;
    pt_bins_fe    = n_pt_bins_Si_HF;
    for (int i = 0; i < n_pt_bins_Si; i++) p_bins_edge.push_back(pt_bins_Si[i]);
    for (int i = 0; i < n_pt_bins_Si_HF; i++) p_bins_edge_FT.push_back(pt_bins_Si_HF[i]);
  } else if (Trigger == "DiJet") {
    pt_bins       = n_pt_bins_Di;
    pt_bins_fe    = n_pt_bins_Di_HF;
    for (int i = 0; i < n_pt_bins_Di; i++) p_bins_edge.push_back(pt_bins_Di[i]);
    for (int i = 0; i < n_pt_bins_Di_HF; i++) p_bins_edge_FT.push_back(pt_bins_Di_HF[i]);
  }

  p_bins_edge.push_back(1500);
  p_bins_edge_FT.push_back(1500);


  // double eta_bins_edge [] = { 0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191 };
  // double eta_bins_control_edge [] = { 0, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191 };
  std::vector<double> eta_bins_edge(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_bins_control_edge(eta_bins+3, eta_bins + sizeof(eta_bins)/sizeof(double)-3);
  eta_bins_control_edge.insert(eta_bins_control_edge.begin(), 0.);

  eta_bins_fe   = 3;
  eta_bin      = n_eta_bins - eta_bins_fe - 1;
  eta_bins_fec  = eta_bins_control_edge.size() -1 ;


  std::vector<float> alpha;
  alpha.push_back(0.05); alpha.push_back(0.1);  alpha.push_back(0.15);
  alpha.push_back(0.20); alpha.push_back(0.25); alpha.push_back(0.3);

  TFile *f, *f_data;

  f = new TFile( filename, "READ");
  f_data = new TFile( filename_data, "READ");

  histLoadAsym( *f_data, 	real_data, "asymm_eta", asymmetries_all_data, asymmetries_gen_all_data, pt_bins, eta_bin );	// load asymm from data
  histLoadAsym( *f, 	data, "asymm_eta", asymmetries_all, asymmetries_gen_all, pt_bins, eta_bin );		// load asymm from mc (all)

  histLoadFE1( *f_data, 	real_data, "forward_control_probe", forward_hist_data , forward_gen_hist_data , pt_bins, eta_bins_fec );
  histLoadFE1( *f, 	data, "forward_control_probe", forward_hist , forward_gen_hist, pt_bins, eta_bins_fec );

  histLoadFE1( *f_data, 	real_data, "forward_probe", forward_hist_data , forward_gen_hist_data , pt_bins_fe, eta_bins_fe );
  histLoadFE1( *f, 	data, "forward_probe", forward_hist, forward_gen_hist, pt_bins_fe, eta_bins_fe );

  histLoadAsym( *f_data, 	real_data, "asymmpt_probe", asymmetries_all_data_pt, asymmetries_gen_all_data_pt, pt_bins, eta_bin );	// load asymm pt from data
  histLoadAsym( *f, 	data, "asymmpt_probe", asymmetries_all_pt, asymmetries_gen_all_pt, pt_bins, eta_bin );		// load asymm pt from mc (all)

  histLoadFE1( *f_data, 	real_data, "forwardpt_control_probe", forward_hist_data_pt , forward_gen_hist_data_pt , pt_bins, eta_bins_fec );
  histLoadFE1( *f, 	data, "forwardpt_control_probe", forward_hist_pt , forward_gen_hist_pt, pt_bins, eta_bins_fec );

  histLoadFE1( *f_data, 	real_data, "forwardpt_probe", forward_hist_data_pt , forward_gen_hist_data_pt , pt_bins_fe, eta_bins_fe );
  histLoadFE1( *f, 	data, "forwardpt_probe", forward_hist_pt , forward_gen_hist_pt, pt_bins_fe, eta_bins_fe );

  histLoadAsym( *f, 	data, "mctruth_eta", mc_truth_asymmetries_all, dummy_hists, pt_bins, eta_bin );		// load asymm from mc (all)

  histLoadFE1( *f, 	data, "mctruth_forward_control_probe", mc_truth_forward_hist , dummy_hists, pt_bins, eta_bins_fec );
  histLoadFE1( *f, 	data, "mctruth_forward_probe",         mc_truth_forward_hist , dummy_hists, pt_bins_fe, eta_bins_fe );

  ////////////////////////////////////////////////////////////////////////////
  //		End of loading histograms                                           //
  ////////////////////////////////////////////////////////////////////////////
  //		I calculate pt_mean for each alpha and pt bin and eta bin.          //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > forward_width_pt, forward_gen_width_pt, forward_width_data_pt, forward_gen_width_data_pt;
  std::vector< std::vector< std::vector< double > > > asymmetries_width_pt, asymmetries_gen_width_pt, asymmetries_width_data_pt, asymmetries_gen_width_data_pt;

  histMeanFE_Pt( forward_hist_pt , forward_width_pt );
  histMeanFE_Pt( forward_gen_hist_pt , forward_gen_width_pt );
  histMeanFE_Pt( forward_hist_data_pt , forward_width_data_pt );
  histMeanFE_Pt( forward_gen_hist_data_pt , forward_gen_width_data_pt );

  histMeanFE_Pt( asymmetries_all_pt , asymmetries_width_pt);
  histMeanFE_Pt( asymmetries_gen_all_pt , asymmetries_gen_width_pt);
  histMeanFE_Pt( asymmetries_all_data_pt , asymmetries_width_data_pt);
  histMeanFE_Pt( asymmetries_gen_all_data_pt , asymmetries_gen_width_data_pt);
  // gen_all_data is for running a cross check with smeared MC.

  ////////////////////////////////////////////////////////////////////////////
  //		End of alpha_mean calc.                                             //
  ////////////////////////////////////////////////////////////////////////////
  //		I calculate width of asymmetry distributions only for               //
  //		alpha bins above 10 GeV thresholds (too soft contriubtions)         //
  //		e.g. for bin p_T_ave (55-75) alpha 0.1 corresponds to 57 GeV jets   //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > forward_width, forward_gen_width, forward_width_data, forward_gen_width_data;
  std::vector< std::vector< std::vector< double > > > forward_width_error, forward_gen_width_error, forward_width_error_data, forward_gen_width_error_data;

  std::vector< std::vector< std::vector< double > > > asymmetries_width, asymmetries_gen_width, asymmetries_width_data, asymmetries_gen_width_data;
  std::vector< std::vector< std::vector< double > > > asymmetries_width_error, asymmetries_gen_width_error, asymmetries_width_error_data, asymmetries_gen_width_error_data;

  histWidthAsym2( asymmetries_all , asymmetries_width, asymmetries_width_error, false );
  histWidthAsym2( asymmetries_gen_all , asymmetries_gen_width, asymmetries_gen_width_error, false );
  histWidthAsym2( asymmetries_all_data , asymmetries_width_data, asymmetries_width_error_data, false );
  histWidthAsym2( asymmetries_gen_all_data , asymmetries_gen_width_data, asymmetries_gen_width_error_data, false );

  histWidthAsym2( forward_hist , forward_width, forward_width_error, false );
  histWidthAsym2( forward_gen_hist , forward_gen_width, forward_gen_width_error, false );
  histWidthAsym2( forward_hist_data , forward_width_data, forward_width_error_data, false );
  histWidthAsym2( forward_gen_hist_data , forward_gen_width_data, forward_gen_width_error_data, false );

  ////////////////////////////////////////////////////////////////////////////
  // 	End of asymmetry fitting                                            //
  ////////////////////////////////////////////////////////////////////////////
  //    I calculate widths, this time also including                        //
  //    alpha bins below 10GeV threshold                                    //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > soft_forward_width, soft_forward_gen_width, soft_forward_width_data, soft_forward_gen_width_data;
  std::vector< std::vector< std::vector< double > > > soft_forward_width_error, soft_forward_gen_width_error, soft_forward_width_error_data, soft_forward_gen_width_error_data;

  std::vector< std::vector< std::vector< double > > > soft_asymmetries_width, soft_asymmetries_gen_width, soft_asymmetries_width_data, soft_asymmetries_gen_width_data;
  std::vector< std::vector< std::vector< double > > > soft_asymmetries_width_error, soft_asymmetries_gen_width_error, soft_asymmetries_width_error_data, soft_asymmetries_gen_width_error_data;

  histWidthAsym2( asymmetries_all , soft_asymmetries_width, soft_asymmetries_width_error, true );
  histWidthAsym2( asymmetries_gen_all , soft_asymmetries_gen_width, soft_asymmetries_gen_width_error, true );
  histWidthAsym2( asymmetries_all_data , soft_asymmetries_width_data, soft_asymmetries_width_error_data, true );
  histWidthAsym2( asymmetries_gen_all_data , soft_asymmetries_gen_width_data, soft_asymmetries_gen_width_error_data, true );

  histWidthAsym2( forward_hist , soft_forward_width, soft_forward_width_error, true );
  histWidthAsym2( forward_gen_hist , soft_forward_gen_width, soft_forward_gen_width_error, true );
  histWidthAsym2( forward_hist_data , soft_forward_width_data, soft_forward_width_error_data, true );
  histWidthAsym2( forward_gen_hist_data , soft_forward_gen_width_data, soft_forward_gen_width_error_data, true );

  ////////////////////////////////////////////////////////////////////////////
  //    End of widths with no alpha restrictions                            //
  ////////////////////////////////////////////////////////////////////////////
  //    Calculate mcTruth resolution for cross check with dijet calculation //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< std::vector< double > > > mcTruth_res, forward_mcTruth_res;
  std::vector< std::vector< std::vector< double > > > mcTruth_res_error, forward_mcTruth_res_error;

  histWidthMCTruth( mc_truth_asymmetries_all, mcTruth_res        , mcTruth_res_error        );
  histWidthMCTruth( mc_truth_forward_hist   , forward_mcTruth_res, forward_mcTruth_res_error);

  ////////////////////////////////////////////////////////////////////////////
  //    Calculate mcTruth resolution                                        //
  ////////////////////////////////////////////////////////////////////////////
  //		I define width(alpha_max) histograms                                //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< TH1F* > > widths_hist_FE, widths_hist_FE_gen, widths_hist_FE_data, widths_hist_FE_gen_data;
  std::vector< std::vector< TH1F* > > widths_hist_all, widths_hist_all_gen, widths_hist_all_data, widths_hist_all_gen_data;

  def_fill_widths( "widths", widths_hist_all , asymmetries_width, asymmetries_width_error );
  def_fill_widths( "widths_gen", widths_hist_all_gen , asymmetries_gen_width, asymmetries_gen_width_error );
  def_fill_widths( "widths_data", widths_hist_all_data , asymmetries_width_data, asymmetries_width_error_data );
  def_fill_widths( "widths_gen_data", widths_hist_all_gen_data , asymmetries_gen_width_data, asymmetries_gen_width_error_data );

  def_fill_widths( "widths_fe", widths_hist_FE , forward_width, forward_width_error );
  def_fill_widths( "widths_gen_fe", widths_hist_FE_gen , forward_gen_width, forward_gen_width_error );
  def_fill_widths( "widths_data_fe", widths_hist_FE_data , forward_width_data, forward_width_error_data );
  def_fill_widths( "widths_gen_data_fe", widths_hist_FE_gen_data , forward_gen_width_data, forward_gen_width_error_data );

  ////////////////////////////////////////////////////////////////////////////
  //		histograms filled                                                   //
  ////////////////////////////////////////////////////////////////////////////
  //    I do same for alpha unconstrained widths                            //
  //    one needs these plots to prove which points should be rejected!     //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< TH1F* > > soft_widths_hist_FE, soft_widths_hist_FE_gen, soft_widths_hist_FE_data, soft_widths_hist_FE_gen_data;
  std::vector< std::vector< TH1F* > > soft_widths_hist_all, soft_widths_hist_all_gen, soft_widths_hist_all_data, soft_widths_hist_all_gen_data;

  def_fill_widths( "all_widths", soft_widths_hist_all, soft_asymmetries_width, soft_asymmetries_width_error );
  def_fill_widths( "all_widths_gen", soft_widths_hist_all_gen, soft_asymmetries_gen_width, soft_asymmetries_gen_width_error );
  def_fill_widths( "all_widths_data", soft_widths_hist_all_data, soft_asymmetries_width_data, soft_asymmetries_width_error_data );
  def_fill_widths( "all_widths_gen_data", soft_widths_hist_all_gen_data, soft_asymmetries_gen_width_data, soft_asymmetries_gen_width_error_data );

  def_fill_widths( "all_widths_fe", soft_widths_hist_FE, soft_forward_width, soft_forward_width_error );
  def_fill_widths( "all_widths_gen_fe", soft_widths_hist_FE_gen, soft_forward_gen_width, soft_forward_gen_width_error );
  def_fill_widths( "all_widths_data_fe", soft_widths_hist_FE_data, soft_forward_width_data, soft_forward_width_error_data );
  def_fill_widths( "all_widths_gen_data_fe", soft_widths_hist_FE_gen_data, soft_forward_gen_width_data, soft_forward_gen_width_error_data );

  ////////////////////////////////////////////////////////////////////////////
  //		I fit line or const to width(alpha_max)                             //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > widths_all, widths_gen_all, widths_all_data, widths_gen_all_data;
  std::vector< std::vector< double > > widths_all_error, widths_gen_all_error, widths_all_error_data, widths_gen_all_error_data;

  std::vector< std::vector< double > > widths_FE, widths_gen_FE, widths_FE_data, widths_gen_FE_data;
  std::vector< std::vector< double > > widths_FE_error, widths_gen_FE_error, widths_FE_error_data, widths_gen_FE_error_data;

  histLinFit( widths_hist_all , widths_all, widths_all_error );
  histLinFit( widths_hist_all_gen , widths_gen_all, widths_gen_all_error );
  histLinFit( widths_hist_all_data , widths_all_data, widths_all_error_data );
  histLinFit( widths_hist_all_gen_data , widths_gen_all_data, widths_gen_all_error_data );

  histLinFit( widths_hist_FE , widths_FE, widths_FE_error );
  histLinFit( widths_hist_FE_gen , widths_gen_FE, widths_gen_FE_error );
  histLinFit( widths_hist_FE_data , widths_FE_data, widths_FE_error_data );
  histLinFit( widths_hist_FE_gen_data , widths_gen_FE_data, widths_gen_FE_error_data );

  ////////////////////////////////////////////////////////////////////////////
  //    end of linear fit                                                   //
  ////////////////////////////////////////////////////////////////////////////
  //		Correlated fit                                                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > widths_cor_all, widths_cor_gen_all, widths_cor_all_data, widths_cor_all_gen_data;
  std::vector< std::vector< double > > widths_cor_all_error, widths_cor_gen_all_error, widths_cor_all_error_data, widths_cor_all_error_gen_data;

  std::vector< std::vector< double > > widths_cor_FE, widths_cor_gen_FE, widths_cor_FE_data, widths_cor_gen_FE_data;
  std::vector< std::vector< double > > widths_cor_FE_error, widths_cor_gen_FE_error, widths_cor_FE_error_data, widths_cor_gen_FE_error_data;

  std::vector< std::vector< TGraphErrors *> > MC_cor_graphs, data_cor_graphs, gen_cor_graphs, gen_data_cor_graphs;
  std::vector< std::vector< TGraphErrors *> > FE_MC_cor_graphs, FE_data_cor_graphs, FE_gen_cor_graphs, FE_gen_data_cor_graphs;

  histLinCorFit(asymmetries_width, asymmetries_width_error, MC_cor_graphs, widths_cor_all, widths_cor_all_error);
  histLinCorFit(asymmetries_width_data, asymmetries_width_error_data, data_cor_graphs, widths_cor_all_data, widths_cor_all_error_data);
  histLinCorFit(asymmetries_gen_width, asymmetries_gen_width_error, gen_cor_graphs, widths_cor_gen_all, widths_cor_gen_all_error);
  histLinCorFit(asymmetries_gen_width_data, asymmetries_gen_width_error_data, gen_data_cor_graphs, widths_cor_all_gen_data, widths_cor_all_error_gen_data);

  histLinCorFit(forward_width, forward_width_error, FE_MC_cor_graphs, widths_cor_FE, widths_cor_FE_error);
  histLinCorFit(forward_width_data, forward_width_error_data, FE_data_cor_graphs, widths_cor_FE_data, widths_cor_FE_error_data);
  histLinCorFit(forward_gen_width, forward_gen_width_error, FE_gen_cor_graphs, widths_cor_gen_FE, widths_cor_gen_FE_error);
  histLinCorFit(forward_gen_width_data, forward_gen_width_error_data, FE_gen_data_cor_graphs, widths_cor_gen_FE_data, widths_cor_gen_FE_error_data);

  ////////////////////////////////////////////////////////////////////////////
  //		End of correlated fit                                               //
  ////////////////////////////////////////////////////////////////////////////
  //		I make histograms ratio of widths(alpha=0.15)                       //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< TH1F* > widths_015_ratios_all, widths_015_ratios_FE;

  widths_015_ratios( "widths_015_ratios", widths_015_ratios_all, asymmetries_width_data, asymmetries_width_error_data, asymmetries_width, asymmetries_width_error, asymmetries_width_data_pt );
  widths_015_ratios( "widths_015_FE_ratios", widths_015_ratios_FE, forward_width_data, forward_width_error_data, forward_width, forward_width_error, forward_width_data_pt );

  ////////////////////////////////////////////////////////////////////////////
  //		Finished histograms for method check (for Mikkos check)             //
  ////////////////////////////////////////////////////////////////////////////
  //		I correct for PLI using b parameter from line fit to sigma_A(alpha) //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_MC, JER_data;
  std::vector< std::vector< double > > JER_MC_error, JER_data_error;

  std::vector< std::vector< double > > JER_uncor_FE_MC, JER_uncor_FE_data;
  std::vector< std::vector< double > > JER_uncor_FE_MC_error, JER_uncor_FE_data_error;

  correctJERwithPLI( JER_MC, JER_MC_error, widths_all, widths_all_error, widths_gen_all, widths_gen_all_error);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_data, JER_data_error, widths_all_data, widths_all_error_data, widths_gen_all, widths_gen_all_error);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_data, JER_data_error, widths_all_data, widths_all_error_data, widths_gen_all_data, widths_gen_all_error_data);
  }

  correctJERwithPLI( JER_uncor_FE_MC, JER_uncor_FE_MC_error, widths_FE, widths_FE_error, widths_gen_FE, widths_gen_FE_error);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_uncor_FE_data, JER_uncor_FE_data_error, widths_FE_data, widths_FE_error_data, widths_gen_FE, widths_gen_FE_error);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_uncor_FE_data, JER_uncor_FE_data_error, widths_FE_data, widths_FE_error_data, widths_gen_FE_data, widths_gen_FE_error_data);
  }

  ////////////////////////////////////////////////////////////////////////////
  //		PLI corrected using b parameters                                    //
  ////////////////////////////////////////////////////////////////////////////
  //    Same correction but for correlated fit results                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_cor_MC, JER_cor_data;
  std::vector< std::vector< double > > JER_cor_MC_error, JER_cor_data_error;

  std::vector< std::vector< double > > JER_cor_uncor_FE_MC, JER_cor_uncor_FE_data;               //cor = correlated
  std::vector< std::vector< double > > JER_cor_uncor_FE_MC_error, JER_cor_uncor_FE_data_error;   //uncor = uncorrected fopr reference region

  correctJERwithPLI( JER_cor_MC, JER_cor_MC_error, widths_cor_all, widths_cor_all_error, widths_cor_gen_all, widths_cor_gen_all_error);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_cor_data, JER_cor_data_error, widths_cor_all_data, widths_cor_all_error_data, widths_cor_gen_all, widths_cor_gen_all_error);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_cor_data, JER_cor_data_error, widths_cor_all_data, widths_cor_all_error_data, widths_cor_all_gen_data, widths_cor_all_error_gen_data);
  }

  correctJERwithPLI( JER_cor_uncor_FE_MC, JER_cor_uncor_FE_MC_error, widths_cor_FE, widths_cor_FE_error, widths_cor_gen_FE, widths_cor_gen_FE_error);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI( JER_cor_uncor_FE_data, JER_cor_uncor_FE_data_error, widths_cor_FE_data, widths_cor_FE_error_data, widths_cor_gen_FE, widths_cor_gen_FE_error);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI( JER_cor_uncor_FE_data, JER_cor_uncor_FE_data_error, widths_cor_FE_data, widths_cor_FE_error_data, widths_cor_gen_FE_data, widths_cor_gen_FE_error_data);
  }

  ////////////////////////////////////////////////////////////////////////////
  //		I do the same for widths at alpha = 0.15                            //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER015_MC, JER015_data;
  std::vector< std::vector< double > > JER015_MC_error, JER015_data_error;

  std::vector< std::vector< double > > JER015_uncor_FE_MC, JER015_uncor_FE_data;
  std::vector< std::vector< double > > JER015_uncor_FE_MC_error, JER015_uncor_FE_data_error;

  correctJERwithPLI015(JER015_MC, JER015_MC_error, asymmetries_width, asymmetries_width_error, asymmetries_gen_width, asymmetries_gen_width_error);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI015(JER015_data, JER015_data_error, asymmetries_width_data, asymmetries_width_error_data, asymmetries_gen_width, asymmetries_gen_width_error);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI015(JER015_data, JER015_data_error, asymmetries_width_data, asymmetries_width_error_data, asymmetries_gen_width_data, asymmetries_gen_width_error_data);
  }
  correctJERwithPLI015(JER015_uncor_FE_MC, JER015_uncor_FE_MC_error, forward_width, forward_width_error, forward_gen_width, forward_gen_width_error);
  if( real_data ){
    // correct data with PLI from MC
    correctJERwithPLI015(JER015_uncor_FE_data, JER015_uncor_FE_data_error, forward_width_data, forward_width_error_data, forward_gen_width, forward_gen_width_error);
  } else {
    // correct 'data' with own PLI
    correctJERwithPLI015(JER015_uncor_FE_data, JER015_uncor_FE_data_error, forward_width_data, forward_width_error_data, forward_gen_width_data, forward_gen_width_error_data);
  }

  ////////////////////////////////////////////////////////////////////////////
  //		I corrected alpha = 0.15 widhs for PLI correct way                  //
  ////////////////////////////////////////////////////////////////////////////
  //		I correct forward widths for Ref region.                            //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_FE_MC, JER_FE_data;
  std::vector< std::vector< double > > JER_FE_MC_error, JER_FE_data_error;

  correctForRef( "mccorrected", JER_FE_MC, JER_FE_MC_error, JER_uncor_FE_MC, JER_uncor_FE_MC_error, forward_width_pt  );
  correctForRef( "datacorrect", JER_FE_data, JER_FE_data_error, JER_uncor_FE_data, JER_uncor_FE_data_error, forward_width_data_pt  );

  ////////////////////////////////////////////////////////////////////////////
  //		forward widths corrected for Ref widths!                            //
  ////////////////////////////////////////////////////////////////////////////
  //    same correction for correlated fit results                          //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER_cor_FE_MC, JER_cor_FE_data;
  std::vector< std::vector< double > > JER_cor_FE_MC_error, JER_cor_FE_data_error;

  correctForRef( "mc_cor_corrected", JER_cor_FE_MC,   JER_cor_FE_MC_error,   JER_cor_uncor_FE_MC,   JER_cor_uncor_FE_MC_error,   forward_width_pt  );
  //	correctForRef( "data_cor_correct", JER_cor_FE_data, JER_cor_FE_data_error, JER_cor_uncor_FE_data, JER_uncor_FE_data_error  , forward_width_data_pt  );
  correctForRef( "data_cor_correct", JER_cor_FE_data, JER_cor_FE_data_error, JER_cor_uncor_FE_data, JER_cor_uncor_FE_data_error, forward_width_data_pt  );
  //	correctForRef( "datacorrect"     , JER_FE_data    , JER_FE_data_error    , JER_uncor_FE_data    , JER_uncor_FE_data_error    , forward_width_data_pt  );

  ////////////////////////////////////////////////////////////////////////////
  //		ref region corrected for correlated fit                             //
  ////////////////////////////////////////////////////////////////////////////
  //		and again, Ref region for widths at alpha = 0.15                    //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > JER015_FE_MC, JER015_FE_data;
  std::vector< std::vector< double > > JER015_FE_MC_error, JER015_FE_data_error;

  correctForRef( "mccorrected015", JER015_FE_MC,   JER015_FE_MC_error,   JER015_uncor_FE_MC,   JER015_uncor_FE_MC_error,   forward_width_pt  );
  correctForRef( "datacorrect015", JER015_FE_data, JER015_FE_data_error, JER015_uncor_FE_data, JER015_uncor_FE_data_error, forward_width_data_pt  );

  ////////////////////////////////////////////////////////////////////////////
  //		Ref reg corrected for widths at alpha = 0.15                        //
  ////////////////////////////////////////////////////////////////////////////
  //		I make make vectors with ratios of widths                           //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< std::vector< double > > scales_classic, scales_fe, scales_fe_control;
  std::vector< std::vector< double > > scales_classic_error, scales_fe_error, scales_fe_control_error;

  makeScales( scales_classic,    scales_classic_error,    JER_data,          JER_data_error,          JER_MC,          JER_MC_error );
  makeScales( scales_fe,         scales_fe_error,         JER_FE_data,       JER_FE_data_error,       JER_FE_MC,       JER_FE_MC_error );
  makeScales( scales_fe_control, scales_fe_control_error, JER_uncor_FE_data, JER_uncor_FE_data_error, JER_uncor_FE_MC, JER_uncor_FE_MC_error );	// uncorrected for reference region width, as a cross check. (i think)

  // same thing for correlated fit results

  std::vector< std::vector< double > > scales_cor_classic, scales_cor_fe, scales_cor_fe_control;
  std::vector< std::vector< double > > scales_cor_classic_error, scales_cor_fe_error, scales_cor_fe_control_error;

  makeScales( scales_cor_classic,    scales_cor_classic_error,    JER_cor_data,          JER_cor_data_error,          JER_cor_MC,          JER_cor_MC_error );
  makeScales( scales_cor_fe,         scales_cor_fe_error,         JER_cor_FE_data,       JER_cor_FE_data_error,       JER_cor_FE_MC,       JER_cor_FE_MC_error );
  makeScales( scales_cor_fe_control, scales_cor_fe_control_error, JER_cor_uncor_FE_data, JER_cor_uncor_FE_data_error, JER_cor_uncor_FE_MC, JER_cor_uncor_FE_MC_error );	// uncorrected for reference region width, as a cross check. (i think)

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

  std::vector< TH1F* > JER_MC_Truth, JER_forward_MC_Truth;

  fill_mctruth_hist( "MC_Truth"    , JER_MC_Truth        , mcTruth_res,         mcTruth_res_error,         asymmetries_width_pt, 0.2);
  fill_mctruth_hist( "MC_Truth_Fwd", JER_forward_MC_Truth, forward_mcTruth_res, forward_mcTruth_res_error, forward_width_pt, 0.2);

  ////////////////////////////////////////////////////////////////////////////
  //		I make histograms with  JERs and scale factors                      //
  ////////////////////////////////////////////////////////////////////////////

  std::vector< TH1F* > JER_MC_hist, JER_data_hist, JER_scale_hist;
  std::vector< TH1F* > JER_FE_MC_hist, JER_FE_data_hist, JER_FE_scale_hist, JER015_FE_MC_hist;
  std::vector< TH1F* > JER_FE_MC_hist_control, JER_FE_data_hist_control, JER_FE_scale_hist_control;

  fill_hist( "MC_JER_classic"  , JER_MC_hist   , JER_MC        , JER_MC_error        , asymmetries_width_pt     , 0.2);
  fill_hist( "data_JER_classtc", JER_data_hist , JER_data      , JER_data_error      , asymmetries_width_data_pt, 0.2);
  fill_hist( "SF_classic"      , JER_scale_hist, scales_classic, scales_classic_error, asymmetries_width_data_pt, 2.);

  fill_hist( "MC_JER_FE", JER_FE_MC_hist, JER_FE_MC, JER_FE_MC_error, forward_width_pt, 0.2);
  fill_hist( "MC_JER_FE015", JER015_FE_MC_hist, JER015_FE_MC,   JER015_FE_MC_error, forward_width_pt, 0.2);

  fill_hist( "data_JER_FE", JER_FE_data_hist, JER_FE_data, JER_FE_data_error, forward_width_data_pt, 0.2);
  fill_hist( "SF_FE", JER_FE_scale_hist, scales_fe, scales_fe_error, forward_width_data_pt, 2.0);

  fill_hist( "MC_JER_FE_control", JER_FE_MC_hist_control, JER_uncor_FE_MC, JER_uncor_FE_MC_error, forward_width_pt, 0.2);
  fill_hist( "data_JER_FE_control", JER_FE_data_hist_control, JER_uncor_FE_data, JER_uncor_FE_data_error, forward_width_data_pt, 0.2);
  fill_hist( "SF_FE_control", JER_FE_scale_hist_control, scales_fe_control, scales_fe_control_error, forward_width_data_pt, 2.0);


  std::vector< TH1F* > JER015_scale_hist, JER015_FE_scale_hist;

  fill_hist( "SF_classic015", JER015_scale_hist, scales015_classic, scales015_classic_error, asymmetries_width_data_pt, 2.);
  fill_hist( "SF_FE015", JER015_FE_scale_hist, scales015_fe, scales015_fe_error, forward_width_data_pt, 2.0);

  std::vector< TH1F* > JER_cor_MC_hist,            JER_cor_data_hist,            JER_cor_scale_hist;
  std::vector< TH1F* > JER_cor_FE_MC_hist,         JER_cor_FE_data_hist,         JER_cor_FE_scale_hist;
  std::vector< TH1F* > JER_cor_FE_MC_hist_control, JER_cor_FE_data_hist_control, JER_cor_FE_scale_hist_control;

  fill_hist( "MC_cor_JER_classic",   JER_cor_MC_hist,    JER_cor_MC,         JER_cor_MC_error,         asymmetries_width_pt, 0.2);
  fill_hist( "data_cor_JER_classtc", JER_cor_data_hist,  JER_cor_data,       JER_cor_data_error,       asymmetries_width_data_pt, 0.2);
  fill_hist( "SF_cor_classic",       JER_cor_scale_hist, scales_cor_classic, scales_cor_classic_error, asymmetries_width_data_pt, 2.);

  fill_hist( "MC_cor_JER_FE",   JER_cor_FE_MC_hist,    JER_cor_FE_MC,   JER_cor_FE_MC_error,   forward_width_pt, 0.2);
  fill_hist( "data_cor_JER_FE", JER_cor_FE_data_hist,  JER_cor_FE_data, JER_cor_FE_data_error, forward_width_data_pt, 0.2);
  fill_hist( "SF_cor_FE",       JER_cor_FE_scale_hist, scales_cor_fe,   scales_cor_fe_error,   forward_width_data_pt, 2.0);

  fill_hist( "MC_cor_JER_FE_control",   JER_cor_FE_MC_hist_control,    JER_cor_uncor_FE_MC,   JER_cor_uncor_FE_MC_error,   forward_width_pt, 0.2);
  fill_hist( "data_cor_JER_FE_control", JER_cor_FE_data_hist_control,  JER_cor_uncor_FE_data, JER_cor_uncor_FE_data_error, forward_width_data_pt, 0.2);
  fill_hist( "SF_cor_FE_control",       JER_cor_FE_scale_hist_control, scales_cor_fe_control, scales_cor_fe_control_error, forward_width_data_pt, 2.0);

  //////////////////////////////////////////////////////////////////////////
  //		I divide the histograms
  //////////////////////////////////////////////////////////////////////////

  //	std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all, asymmetries_gen_all, asymmetries_all_data;
  //	std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all_pt, asymmetries_gen_all_pt, asymmetries_all_data_pt;
  //
  //	std::vector< std::vector< std::vector< TH1F* > > > forward_hist, forward_gen_hist, forward_hist_data;
  //	std::vector< std::vector< std::vector< TH1F* > > > forward_hist_pt, forward_gen_hist_pt, forward_hist_data_pt;

  /*
  JER_MC_hist
  JER_data_hist
  JER_scale_hist

  for( unsigned int m = 0; m < JER_MC_hist.size(); m++ ){
  TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*x)**2+[1]**2/x+[2]**2 )", 0, 1500 );
  JER_MC_hist->Fit("mcFIT")
  TF1 * dtFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*x)**2+[1]**2/x+[2]**2 )", 0, 1500 );
}
*/


//////////////////////////////////////////////////////////////////////////////////////////
//    resolution cross check with mcTruth                                               //
//////////////////////////////////////////////////////////////////////////////////////////


int iPeriod = 4;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
extraText  = "Preliminary";  // default extra text is "Preliminary"
lumi_13TeV = lumi; //"2.1 fb^{-1}";
lumi_sqrtS = sqrts;       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

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
///////
///////
///////    for ( int j = 0; j < resolutions.size(); j++ ){
///////        if( ( Jets.at(i).Eta() > resolutions.at(j).at(0) ) && ( Jets.at(i).Eta() <= resolutions.at(j).at(1) ) && ( rho > resolutions.at(j).at(2) ) && ( rho <= resolutions.at(j).at(3) ) ){
///////            double par_a, par_b, par_c, par_d;
///////            // smearing parameters
///////            par_a = resolutions.at(j).at(7);
///////            par_b = resolutions.at(j).at(8);
///////            par_c = resolutions.at(j).at(9);
///////            par_d = resolutions.at(j).at(10);
///////            // resolution function
///////            JER = TMath::Sqrt(par_a*TMath::Abs(par_a)/(Jets.at(i).Pt()*Jets.at(i).Pt())+par_b*par_b*pow(Jets.at(i).Pt(),par_d)+par_c*par_c) ;
///////            break;
///////        }
///////        if ( j == resolutions.size() ){
///////            std::cout << "out of jer ranges" << std::endl;
///////        }
///////    }

std::vector<TF1 *> MCTruth_Chad;
for( unsigned int m = 0; m < resolutions.size(); m++ ){
  TString name_TF1 = "MCTruth_Chad";
  name_TF1 += m;
  double par_a, par_b, par_c, par_d;
  par_a = resolutions.at(m).at(7);
  par_b = resolutions.at(m).at(8);
  par_c = resolutions.at(m).at(9);
  par_d = resolutions.at(m).at(10);
  //      TF1 * tempTF1 = new TF1( name_TF1, "TMath::Sqrt(par_a*TMath::Abs(par_a)/(x*x)+par_b*par_b*pow(x,par_d)+par_c*par_c)",  0, 1000 );
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
  leg->AddEntry(JER_MC_hist    .at(m),"Normal DiJet"    ,"lep");
  leg->AddEntry(JER_cor_MC_hist.at(m),"Correlated DiJet","lep");
  // leg->AddEntry(MCTruth_Chad.at(m),   "MCTruth from Chad","l");

  canv->Update();
  canv->Write();
  canv->Print("pdfy/MCTruth/"+canvName+".pdf","pdf");
  canv->Print("pdfy/MCTruth/"+canvName+".png","png");
  delete canv;
}

test.Close();

TFile fMCTruth("output/MCTruth.root","RECREATE");
for( unsigned int m = 0; m < JER_MC_Truth.size(); m++ ){
  PLOT_3_MCT(JER_MC_Truth,JER_MC_hist,JER_cor_MC_hist,kRed,kBlue,kMagenta,m)

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
  leg->AddEntry(JER_MC_hist    .at(m),"Normal DiJet"    ,"lep");
  leg->AddEntry(JER_cor_MC_hist.at(m),"Correlated DiJet","lep");
  // leg->AddEntry(MCTruth_Chad.at(m),   "MCTruth from Chad","l");

  canv->Update();
  canv->Write();
  canv->Print("pdfy/MCTruth/"+canvName+".pdf","pdf");
  canv->Print("pdfy/MCTruth/"+canvName+".png","png");
}

for( unsigned int m = 0; m < (JER_forward_MC_Truth.size()-1); m++ ){

  PLOT_4(JER_forward_MC_Truth,JER_FE_MC_hist,JER_cor_FE_MC_hist,JER015_FE_MC_hist,kRed,kBlue,kMagenta,kGreen+2,m)

  // for( unsigned int i = 0; i < 7; i++ ){
  //    MCTruth_Chad.at( 7*m + i + 14 ) -> Draw("same");
  //    MCTruth_Chad.at( 7*m + i + 14 ) -> SetMarkerColor(29+i);
  //    canv->Update();
  // }

  //         if( m == 2 ){
  //            MCTruth_Chad.at(17) -> Draw("same");
  //			   canv->Update();
  //         }
  //         if( m == 3 ){
  //            MCTruth_Chad.at(19) -> Draw("same");
  //			   canv->Update();
  //         }

  TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
  leg->AddEntry(JER_forward_MC_Truth.at(m+1), "MC Truth", "lep");
  leg->AddEntry(JER_FE_MC_hist      .at(m), "Normal DiJet", "lep");
  leg->AddEntry(JER_cor_FE_MC_hist  .at(m), "Correlated DiJet", "lep");
  leg->AddEntry(JER015_FE_MC_hist   .at(m), "015 DiJet", "lep");
  // leg->AddEntry(MCTruth_Chad     .at(m+2), "MCTruth from Chad", "l");

  canv->Update();
  canv->Write();
  canv->Print("pdfy/MCTruth/"+canvName+".pdf","pdf");
  canv->Print("pdfy/MCTruth/"+canvName+".png","png");
}

{

  PLOT_3_MCT(JER_forward_MC_Truth,JER_FE_MC_hist_control,JER_cor_FE_MC_hist_control,kRed,kBlue,kMagenta,0)

  // for( unsigned int i = 0; i < 14; i++ ){
  //    MCTruth_Chad.at( i ) -> Draw("same");
  // MCTruth_Chad.at( i ) -> SetMarkerColor(29+i);
  // canv->Update();
  // }
  TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
  leg->AddEntry(JER_forward_MC_Truth      .at(0), "MC Truth", "lep");
  leg->AddEntry(JER_FE_MC_hist_control    .at(0), "Normal DiJet", "lep");
  leg->AddEntry(JER_cor_FE_MC_hist_control.at(0), "Correlated DiJet", "lep");
  // leg->AddEntry(MCTruth_Chad           .at(0),"MCTruth from Chad","l");

  canv->Update();
  canv->Write();
  canv -> Print("pdfy/MCTruth/FE_reference.pdf","pdf");
  canv -> Print("pdfy/MCTruth/FE_reference.png","png");
}
fMCTruth.Close();

//////////////////////////////////////////////////////////////////////////////////////////
//    I write histograms to root and graphic files                                      //
//////////////////////////////////////////////////////////////////////////////////////////


TFile hprim("output/asymmetries.root","RECREATE");
for( unsigned int m = 0; m < asymmetries_all.size(); m++){
  for( unsigned int p = 0; p < asymmetries_all.at(m).size(); p++){
    for( unsigned int r = 0; r < asymmetries_all.at(m).at(p).size(); r++){
      asymmetries_all.at(m).at(p).at(r) -> Write();
      asymmetries_gen_all.at(m).at(p).at(r) -> Write();
      asymmetries_all_data.at(m).at(p).at(r) -> Write();
    }
  }
}

for( unsigned int m = 0; m < forward_hist.size(); m++){
  for( unsigned int p = 0; p < forward_hist.at(m).size(); p++){
    for( unsigned int r = 0; r < forward_hist.at(m).at(p).size(); r++){
      forward_hist.at(m).at(p).at(r) -> Write();
      forward_gen_hist.at(m).at(p).at(r) -> Write();
      forward_hist_data.at(m).at(p).at(r) -> Write();
    }
  }
}

TFile hprimm("output/widths.root","RECREATE");

for( unsigned int m = 0; m < widths_hist_FE.size(); m++ ){
  for( unsigned int p = 0; p < widths_hist_FE.at(m).size(); p++ ){
    widths_hist_FE.at(m).at(p) -> Write();
    widths_hist_FE_gen.at(m).at(p) -> Write();
    widths_hist_FE_data.at(m).at(p) -> Write();
  }
}

for( unsigned int m = 0; m < widths_hist_all.size(); m++ ){
  for( unsigned int p = 0; p < widths_hist_all.at(m).size(); p++ ){
    widths_hist_all.at(m).at(p) -> Write();
    widths_hist_all_gen.at(m).at(p) -> Write();
    widths_hist_all_data.at(m).at(p) -> Write();
  }
}

hprimm.Close();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    correlation plots      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

for( unsigned int m = 0; m < MC_cor_graphs.size(); m++ ){
  for( unsigned int p = 0; p < MC_cor_graphs.at(m).size(); p++ ){
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
    // gStyle -> SetOptFit(0000);
    // gStyle -> SetOptStat(0000);
    PLOT_3_2_gr(MC_cor_graphs,gen_cor_graphs,data_cor_graphs,kRed,kGreen+2,kBlue,m,p)

    f = MC_cor_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kRed); f->SetLineStyle(2);
    Temp = (TF1*) MC_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
    Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
    legend = tdrLeg(0.70,0.70,0.90,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
    // legend->SetHeader("MC", "C");
    tdrHeader(legend,"MC");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    f = gen_cor_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kGreen+2); f->SetLineStyle(2);
    Temp = (TF1*) gen_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
    Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
    legend = tdrLeg(0.50,0.70,0.70,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
    // legend->SetHeader("gen", "C");
    tdrHeader(legend,"gen");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");


    f = data_cor_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kBlue); f->SetLineStyle(2);
    Temp = (TF1*) data_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
    Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
    legend = tdrLeg(0.30,0.70,0.50,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
    // legend->SetHeader("Data", "C");
    tdrHeader(legend,"Data");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    TLegend *leg = tdrLeg(0.42,0.885,0.9,0.92);
    char legTitle[100];
    sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV", eta_bins_edge[m], eta_bins_edge[m+1], p_bins_edge[p], p_bins_edge[p+1]);
    // leg->SetHeader(legTitle,"");
    tdrHeader(leg,"");
    leg->SetTextFont(42);  leg->SetTextSize(0.035);  leg->SetTextColor(kBlack);
    leg->Draw("same");

    TString name;
    name = Form("ClosureTest/Extrapol_Eta%i_pt%i.png", m+1, p+1);
    canv->Print(name);
    name = Form("ClosureTest/Extrapol_Eta%i_pt%i.pdf", m+1, p+1);
    canv->Print(name);
  }
}

for( unsigned int m = 0; m < FE_MC_cor_graphs.size(); m++ ){
  for( unsigned int p = 0; p < FE_MC_cor_graphs.at(m).size(); p++ ){
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

    PLOT_3_2_gr(FE_MC_cor_graphs,FE_gen_cor_graphs,FE_data_cor_graphs,kRed,kGreen+2,kBlue,m,p)

    f = FE_MC_cor_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kRed); f->SetLineStyle(2);
    Temp = (TF1*) FE_MC_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone(); gStyle -> SetOptFit(0000);
    Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
    legend = tdrLeg(0.70,0.70,0.90,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
    // legend->SetHeader("MC", "C");
    tdrHeader(legend,"MC");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    f = FE_gen_cor_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kGreen+2); f->SetLineStyle(2);
    Temp = (TF1*) FE_gen_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
    Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
    legend = tdrLeg(0.50,0.70,0.70,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
    // legend->SetHeader("gen", "C");
    tdrHeader(legend,"gen");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    f = FE_data_cor_graphs.at(m).at(p) -> GetFunction("lin_extrapol_mc"); f->SetLineColor(kBlue); f->SetLineStyle(2);
    Temp = (TF1*) FE_data_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
    Temp->SetRange(range,1); Temp->SetLineStyle(1); Temp->Draw("same");
    legend = tdrLeg(0.30,0.70,0.50,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
    // legend->SetHeader("Data", "C");
    tdrHeader(legend,"Data");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    TLegend *leg = tdrLeg(0.42,0.885,0.9,0.92);
    char legTitle[100];
    sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
    eta_bins_edge[eta_bin + eta_bins_fe - FE_MC_cor_graphs.size() + m],
    eta_bins_edge[eta_bin + eta_bins_fe - FE_MC_cor_graphs.size() + m+1],
    p_bins_edge_FT[p], p_bins_edge_FT[p+1]);
    // leg->SetHeader(legTitle,"");
    tdrHeader(leg,"");
    leg->SetTextFont(42);  leg->SetTextSize(0.035);  leg->SetTextColor(kBlack);
    leg->Draw("same");

    TString name;
    name = Form("ClosureTest/ExtrapolFw_Eta%i_pt%i.png", m+1, p+1);
    canv->Print(name);
    name = Form("ClosureTest/ExtrapolFw_Eta%i_pt%i.pdf", m+1, p+1);
    canv->Print(name);
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    end of correlation plots      ///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


TFile gprim("output/JERs.root","RECREATE");

for( unsigned int m = 0; m < JER_MC_hist.size(); m++ ){                   JER_MC_hist.at(m)                   -> Write();}
for( unsigned int m = 0; m < JER_data_hist.size(); m++ ){                 JER_data_hist.at(m)                 -> Write();}
for( unsigned int m = 0; m < JER_FE_MC_hist.size(); m++ ){                JER_FE_MC_hist.at(m)                -> Write();}
for( unsigned int m = 0; m < JER_FE_data_hist.size(); m++ ){              JER_FE_data_hist.at(m)              -> Write();}
for( unsigned int m = 0; m < JER_FE_MC_hist_control.size(); m++ ){        JER_FE_MC_hist_control.at(m)        -> Write();}
for( unsigned int m = 0; m < JER_FE_data_hist_control.size(); m++ ){      JER_FE_data_hist_control.at(m)      -> Write();}
for( unsigned int m = 0; m < JER_cor_MC_hist.size(); m++ ){               JER_cor_MC_hist.at(m)               -> Write();}
for( unsigned int m = 0; m < JER_cor_data_hist.size(); m++ ){             JER_cor_data_hist.at(m)             -> Write();}
for( unsigned int m = 0; m < JER_cor_FE_MC_hist.size(); m++ ){            JER_cor_FE_MC_hist.at(m)            -> Write();}
for( unsigned int m = 0; m < JER_cor_FE_data_hist.size(); m++ ){          JER_cor_FE_data_hist.at(m)          -> Write();}
for( unsigned int m = 0; m < JER_cor_FE_MC_hist_control.size(); m++ ){    JER_cor_FE_MC_hist_control.at(m)    -> Write();}
for( unsigned int m = 0; m < JER_cor_FE_data_hist_control.size(); m++ ){  JER_cor_FE_data_hist_control.at(m)  -> Write();}

gprim.Close();


TFile gbis("output/SFs.root","RECREATE");
ofstream texfile;
texfile.open ("output/scalefactors_tex.txt");
texfile << "standard method\n";

// ofstream mytxtfile2;
// mytxtfile2.open ("output/scalefactors_short.txt");
// mytxtfile2 << "standard method\n";
// char text_tex[100];


std::vector<double> st, st_err, st_cor, st_err_cor, fe, fe_err, fe_cor, fe_err_cor, eta_bin_center, eta_bin_err, eta_bin_center_fe, eta_bin_err_fe;

for( unsigned int m = 0; m < JER_scale_hist.size(); m++ ){
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  // std::cout << "standard scales, eta bin " << m << std::endl;
  JER_scale_hist.at(m) -> Fit("constfit","Q");
  JER_scale_hist.at(m) -> Write();
  eta_bin_center.push_back((eta_bins_edge[m+1]+eta_bins_edge[m]) /2);
  eta_bin_err.push_back((eta_bins_edge[m+1]-eta_bins_edge[m]) /2);
  st.push_back(constfit -> GetParameter( 0 ));
  st_err.push_back(constfit -> GetParError( 0 ));
  texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
  if(m == JER_scale_hist.size()-1 ) texfile << "\\\\";
  // sprintf(text_tex, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
  delete constfit;
  // std::cout << std::endl;
  // std::cout << std::endl;
}

for( unsigned int m = 0; m < JER_FE_scale_hist_control.size(); m++ ){
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  // std::cout << "FE control scales, eta bin " << m << std::endl;
  JER_FE_scale_hist_control.at(m) -> Fit("constfit","Q");
  JER_FE_scale_hist_control.at(m) -> Write();
  delete constfit;
  // std::cout << std::endl;
  // std::cout << std::endl;
}

for( unsigned int m = 0; m < JER015_scale_hist.size(); m++ ){
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  // std::cout << "standard with 0.15 scales, eta bin " << m << std::endl;
  JER015_scale_hist.at(m) -> Fit("constfit","Q");
  JER015_scale_hist.at(m) -> Write();
  delete constfit;
  // std::cout << std::endl;
  // std::cout << std::endl;
}

for( unsigned int m = 0; m < JER015_FE_scale_hist.size(); m++ ){
  // std::cout << "FE with 015 scales, eta bin " << m << std::endl;
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  JER015_FE_scale_hist.at(m) -> Fit("constfit","Q");
  JER015_FE_scale_hist.at(m) -> Write();
  delete constfit;
  // std::cout << std::endl;
  // std::cout << std::endl;
}

texfile << "\n forward extension \n";
for( unsigned int m = 0; m < JER_FE_scale_hist.size(); m++ ){
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  // std::cout << "FE scales, eta bin " << m << std::endl;
  JER_FE_scale_hist.at(m) -> Fit("constfit","Q");
  JER_FE_scale_hist.at(m) -> Write();
  int diff = eta_bin + eta_bins_fe - JER_FE_scale_hist.size();
  eta_bin_center_fe.push_back((eta_bins_edge[diff+m+1]+eta_bins_edge[diff+m]) /2);
  eta_bin_err_fe.push_back((eta_bins_edge[diff+m+1]-eta_bins_edge[diff+m]) /2);
  fe.push_back(constfit -> GetParameter( 0 ));
  fe_err.push_back(constfit -> GetParError( 0 ));
  texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
  if(m == JER_FE_scale_hist.size()-1 ) texfile << "\\\\";
  delete constfit;
}

texfile << "\n standard method, correlated fit\n";
for( unsigned int m = 0; m < JER_cor_scale_hist.size(); m++ ){
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  // std::cout << "standard scales, eta bin " << m << std::endl;
  JER_cor_scale_hist.at(m) -> Fit("constfit","Q");
  JER_cor_scale_hist.at(m) -> Write();
  st_cor.push_back(constfit -> GetParameter( 0 ));
  st_err_cor.push_back(constfit -> GetParError( 0 ));
  texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
  if(m == JER_cor_scale_hist.size()-1 ) texfile << "\\\\";
  delete constfit;
  // std::cout << std::endl;
  // std::cout << std::endl;
}

texfile << "\n forward extension, correlated fit \n";
for( unsigned int m = 0; m < JER_cor_FE_scale_hist.size(); m++ ){
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  // std::cout << "FE scales, eta bin " << m << std::endl;
  JER_cor_FE_scale_hist.at(m) -> Fit("constfit","Q");
  JER_cor_FE_scale_hist.at(m) -> Write();
  fe_cor.push_back(constfit -> GetParameter( 0 ));
  fe_err_cor.push_back(constfit -> GetParError( 0 ));
  texfile << constfit -> GetParameter( 0 ) << " \\pm " << constfit -> GetParError( 0 ) << " & ";
  if(m == JER_cor_FE_scale_hist.size()-1 ) texfile << "\\\\";
  // std::cout << std::endl;
  // std::cout << std::endl;
  delete constfit;
}
texfile << "\n";
texfile.close();

gbis.Close();
ofstream txt_ST, txt_FE;
txt_ST.open ("output/scalefactors_ST.txt");
txt_FE.open ("output/scalefactors_FE.txt");
std::cout << "#eta_bin_center" << "\t" << "eta_bin_err" << "\t" << "st" << "\t" << "st_err" << "\t" << "st_cor" << "\t" << "st_err_cor" << "\n";
txt_ST << "#eta_bin_center" << "\t" << "eta_bin_err" << "\t" << "st" << "\t" << "st_err" << "\t" << "st_cor" << "\t" << "st_err_cor" << "\n";
txt_FE << "#eta_bin_center_fe" << "\t" << "eta_bin_err_fe" << "\t" << "fe" << "\t" << "fe_err" << "\t" << "fe_cor" << "\t" << "fe_err_cor" << "\n";
for (unsigned int i = 0; i < JER_scale_hist.size(); i++) {
  txt_ST << eta_bin_center[i] << " " << eta_bin_err[i] << " " << st[i] << " " << st_err[i] << " " << st_cor[i] << " " << st_err_cor[i] << "\n";
  std::cout << eta_bin_center[i] << " " << eta_bin_err[i] << " " << st[i] << " " << st_err[i] << " " << st_cor[i] << " " << st_err_cor[i] << "\n";
}
for (unsigned int i = 0; i < JER_FE_scale_hist.size(); i++) {
  txt_FE << eta_bin_center_fe[i] << " \t " << eta_bin_err_fe[i] << " \t " << fe[i] << " \t " << fe_err[i] << " \t " << fe_cor[i] << " \t " << fe_err_cor[i] << "\n";
  std::cout << eta_bin_center_fe[i] << "\t" << eta_bin_err_fe[i] << "\t" << fe[i] << "\t" << fe_err[i] << "\t" << fe_cor[i] << "\t" << fe_err_cor[i] << "\n";
}
txt_ST.close();
txt_FE.close();
// return 0;
TCanvas* canv_SF = tdrCanvas("JER SF",eta_bins_edge[0]-0.1, eta_bins_edge[eta_bin + eta_bins_fe]+0.5, 0.8, 3.0, "#eta", "JER SF");
canv_SF->SetTickx(0);
canv_SF->SetTicky(0);
TGraphErrors *gr_st = new TGraphErrors(JER_scale_hist.size(), &(eta_bin_center[0]), &(st[0]), &(eta_bin_err[0]), &(st_err[0]));
TGraphErrors *gr_fe = new TGraphErrors(JER_FE_scale_hist.size(), &(eta_bin_center_fe[0]), &(fe[0]), &(eta_bin_err_fe[0]), &(fe_err[0]));
tdrDraw(gr_st, "P", kFullDotLarge, kRed);
// gr_st->SetMarkerStyle(kFullCircle);
// gr_st->SetMarkerColor(kRed);
// gr_st->SetLineStyle(kSolid);
// gr_st->SetLineColor(kRed);
// gr_st->SetFillStyle(1001);
// gr_st->SetFillColor(kRed);
// gr_st->Draw();
tdrDraw(gr_fe, "P", kFullDotLarge, kBlue);

TLegend *leg = tdrLeg(0.70,0.75,0.9,0.9);
leg->AddEntry(gr_st, "Standard","lep");
leg->AddEntry(gr_fe, "Forward ext", "lep");
leg->Draw("same");

canv_SF->SaveAs("output/SF.png");
canv_SF->SaveAs("output/SF.pdf");
canv_SF->SaveAs("output/SF.root");

gStyle -> SetOptFit(1);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plots for forward extension with widths(alpha)                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TFile h_merged("output/widths_merged.root","RECREATE");

for( unsigned int m = 0; m < widths_hist_FE.size(); m++ ){
  for( unsigned int p = 0; p < widths_hist_FE.at(m).size(); p++ ){
    std::cout << "help " << m << " " << eta_bins_edge.at(m) << " " << p << " " << p_bins_edge.at(p) << std::endl;
    PLOT_3_2(widths_hist_FE,widths_hist_FE_gen,widths_hist_FE_data,kRed,kGreen+2,kBlue,m,p)

    if( widths_hist_FE.at(m).at(p)->GetEntries() != 0 ){
      widths_hist_FE.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kRed);
    }
    if( widths_hist_FE_gen.at(m).at(p)->GetEntries() != 0 ){
      widths_hist_FE_gen.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kGreen+2);
    }
    if( widths_hist_FE_data.at(m).at(p)->GetEntries() != 0 ){
      widths_hist_FE_data.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kBlue);
    }

    TF1* f;
    char line[100];
    TLegend *legend;

    if( widths_hist_FE.at(m).at(p)->GetEntries() != 0 ){
      f = widths_hist_FE.at(m).at(p) -> GetFunction("linfit");
    }
    legend = tdrLeg(0.50,0.70,0.70,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
    // legend->SetHeader("MC", "C");
    tdrHeader(legend,"MC");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    if( widths_hist_FE_gen.at(m).at(p)->GetEntries() != 0 ){
      f = widths_hist_FE_gen.at(m).at(p) -> GetFunction("linfit");
    }
    legend = tdrLeg(0.70,0.70,0.90,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
    // legend->SetHeader("gen", "C");
    tdrHeader(legend,"gen");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    if( widths_hist_FE_data.at(m).at(p)->GetEntries() != 0 ){
      f = widths_hist_FE_data.at(m).at(p) -> GetFunction("linfit");
    }
    legend = tdrLeg(0.30,0.70,0.50,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
    // legend->SetHeader("Data", "C");
    tdrHeader(legend,"Data");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");

    TLegend *leg = tdrLeg(0.42,0.885,0.9,0.92);
    char legTitle[100];
    sprintf(legTitle, "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
    eta_bins_edge[eta_bin + eta_bins_fe - widths_hist_FE.size() + m],
    eta_bins_edge[eta_bin + eta_bins_fe - widths_hist_FE.size() + m+1],
    p_bins_edge_FT[p], p_bins_edge_FT[p+1]);
    // leg->SetHeader(legTitle,"");
    tdrHeader(leg,legTitle);
    leg->SetTextFont(42);  leg->SetTextSize(0.035);  leg->SetTextColor(kBlack);
    leg->Draw("same");

    canv -> Write();
    canv -> Print("pdfy/widths/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/widths/"+canvName+".png","png");
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  same plots with all points (also corresponding to additional jets with < 10 GeV)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
for( unsigned int m = 0; m < soft_widths_hist_FE.size(); m++ ){
  for( unsigned int p = 0; p < soft_widths_hist_FE.at(m).size(); p++ ){
    PLOT_3_2(soft_widths_hist_FE,soft_widths_hist_FE_gen,soft_widths_hist_FE_data,kRed,kGreen+2,kBlue,m,p)
    canv -> Write();
    canv -> Print("pdfy/widths/allPoints_"+canvName+".png","png");
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plots with widths(alpha)                                                                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
for( unsigned int m = 0; m < widths_hist_all.size(); m++ ){
  for( unsigned int p = 0; p < widths_hist_all.at(m).size(); p++ ){
    PLOT_3_2(widths_hist_all,widths_hist_all_gen,widths_hist_all_data,kRed,kGreen+2,kBlue,m,p)

    if( widths_hist_all.at(m).at(p) -> GetEntries() != 0 )     widths_hist_all.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kRed);
    if( widths_hist_all_gen.at(m).at(p) -> GetEntries() != 0 ) widths_hist_all_gen.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kGreen+2);
    if( widths_hist_all_data.at(m).at(p) -> GetEntries() != 0 )widths_hist_all_data.at(m).at(p) -> GetFunction("linfit")->SetLineColor(kBlue);

    TF1* f;
    char line[100];
    TLegend *legend;

    if (widths_hist_all.at(m).at(p) -> GetFunction("linfit")) {
      f = widths_hist_all.at(m).at(p) -> GetFunction("linfit");
      legend = tdrLeg(0.50,0.70,0.70,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
      // legend->SetHeader("MC", "C");
      tdrHeader(legend,"MC");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    }

    if (widths_hist_all_gen.at(m).at(p) -> GetFunction("linfit")) {
      f = widths_hist_all_gen.at(m).at(p) -> GetFunction("linfit");
      legend = tdrLeg(0.70,0.70,0.90,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
      // legend->SetHeader("gen", "C");
      tdrHeader(legend,"gen");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    }

    if (widths_hist_all_data.at(m).at(p) -> GetFunction("linfit")) {
      f = widths_hist_all_data.at(m).at(p) -> GetFunction("linfit");
      legend = tdrLeg(0.30,0.70,0.50,0.90);
      legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
      // legend->SetHeader("Data", "C");
      tdrHeader(legend,"Data");
      sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
      sprintf(line, "p1 = %.5f #pm %.5f", f->GetParameter(1), f->GetParError(1));    legend->AddEntry((TObject*)0, line, "");
      legend->Draw("same");
    }

    TLegend *leg = tdrLeg(0.35,0.885,0.9,0.92);
    char legTitle[100];
    sprintf(legTitle, "#eta#in [%.3f,%.3f], p_{T}#in [%.0f,%.0f] GeV", eta_bins_edge[m], eta_bins_edge[m+1], p_bins_edge[p], p_bins_edge[p+1]);
    // leg->SetHeader(legTitle,"");
    tdrHeader(leg,legTitle);
    leg->SetTextFont(42);  leg->SetTextSize(0.035);  leg->SetTextColor(kBlack);
    leg->Draw("same");


    canv -> Write();
    canv -> Print("pdfy/widths/"+canvName+".pdf","pdf");
    canv -> Print("pdfy/widths/"+canvName+".png","png");
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plots with all points of widths(alpha)                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

for( unsigned int m = 0; m < soft_widths_hist_all.size(); m++ ){
  for( unsigned int p = 0; p < soft_widths_hist_all.at(m).size(); p++ ){
    PLOT_3_2(soft_widths_hist_all,soft_widths_hist_all_gen,soft_widths_hist_all_data,kRed,kGreen+2,kBlue,m,p)

    canv -> Update();
    canv -> Write();
    canv -> Print("pdfy/widths/allPoints_"+canvName+".png","png");
  }
}

h_merged.Close();

//////////////////////////////////////////////////////////////////////////////////////////
// plot with JERs with NSC fit
/////////////////////////////////////////////////////////////////////////////////////////

gStyle -> SetOptStat(0);
TFile fJERs("output/Jersalltogether.root","RECREATE");
for( unsigned int m = 0; m < JER_data_hist.size(); m++ ){
  //TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
  TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1500 );
  mcFIT->SetLineColor(kRed+2);  // sets the color to red
  JER_MC_hist.at(m) -> Fit("mcFIT", "M");

  double N, S, C;
  N = mcFIT -> GetParameter( 0 );
  S = mcFIT -> GetParameter( 1 );
  C = mcFIT -> GetParameter( 2 );

  TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [2]*(([0]/x)**2)+[3]*([0]**2)/x+[1]**2 )", 50, 1500 );
  dtFIT->SetLineColor(kBlue+2);  // sets the color to red
  dtFIT -> FixParameter(2, N*N);
  dtFIT -> FixParameter(3, S*S);
  JER_data_hist.at(m) -> Fit("dtFIT", "M");

  PLOT_2(JER_data_hist,JER_MC_hist,kBlue,kRed,m)

  TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
  char legTitle[100];
  sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
  // leg->SetHeader(legTitle,"");
  tdrHeader(leg,legTitle);
  leg->AddEntry(JER_data_hist.at(m),label_data,"lep");
  leg->AddEntry(JER_MC_hist.at(m),"MC","lep");

  canv -> Write();
  canvName = canvName(0, canvName.Length()-1);
  canvName += (m+1);
  canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
  canv -> Print("pdfy/JERs/"+canvName+".png","png");

  TString canvSFName = JER_scale_hist.at(m) -> GetTitle();
  TString nameXaxisSF = JER_scale_hist.at(m)->GetXaxis()->GetTitle();
  TString nameYaxisSF = JER_scale_hist.at(m)->GetYaxis()->GetTitle();

  vec.clear();
  vec.push_back(JER_data_hist.at(m));
  vec.push_back(JER_MC_hist.at(m));
  findExtreme(vec, &x_min, &x_max, &y_min, &y_max);

  TCanvas* canvSF = tdrCanvas(canvSFName, x_min, x_max, y_min, y_max, nameXaxisSF, nameYaxisSF);
  canvSF->SetTickx(0);
  canvSF->SetTicky(0);

  tdrDraw(JER_scale_hist.at(m), "", kFullCircle, kBlue);

  if(JER_scale_hist.at(m)->GetFunction("constfit")){
    JER_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(kBlue);
    //         if(JER_scale_hist.at(m)->GetFunction("pol0")){
    //			   JER_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
  } else {
    std::cout << "Fit function at bin " << m;
    std::cout << " was not found" << std::endl;
  }

  if ((TPaveStats*)canvSF->GetPrimitive("stats")){
    TPaveStats *stats2 = (TPaveStats*)canvSF->GetPrimitive("stats");
    stats2->SetName("h1stats3");
    stats2->SetY1NDC(.64);
    stats2->SetY2NDC(.77);
    stats2->SetX1NDC(.66);
    stats2->SetX2NDC(.91);
    stats2->SetTextColor(4);
  }

  TLegend *legSF = tdrLeg(0.6,0.2,0.9,0.4);
  legSF->AddEntry(JER_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");

  TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);
  f3->Draw("same");
  canvSF->Update();
  canvSF -> Write();

  canvSF -> Print("pdfy/NSC_SFs/NSC"+canvSFName+".pdf","pdf");
  canvSF -> Print("pdfy/NSC_SFs/NSC"+canvSFName+".png","png");
}
//////////////////////////////////////////////////////////////////////////////////////////
// same kind of plot but for correlated results
/////////////////////////////////////////////////////////////////////////////////////////

for( unsigned int m = 0; m < JER_cor_data_hist.size(); m++ ){ // plot with JERs with NSC fit
  //TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
  TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1500 );
  mcFIT->SetLineColor(kRed+2);  // sets the color to red
  JER_cor_MC_hist.at(m) -> Fit("mcFIT", "M");

  double N, S, C;
  N = mcFIT -> GetParameter( 0 );
  S = mcFIT -> GetParameter( 1 );
  C = mcFIT -> GetParameter( 2 );

  TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [2]*(([0]/x)**2)+[3]*([0]**2)/x+[1]**2 )", 50, 1500 );
  dtFIT->SetLineColor(kBlue+2);  // sets the color to red
  dtFIT -> FixParameter(2, N*N);
  dtFIT -> FixParameter(3, S*S);
  JER_cor_data_hist.at(m) -> Fit("dtFIT", "M");

  PLOT_2(JER_cor_data_hist,JER_cor_MC_hist,kBlue,kRed,m)

  TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
  char legTitle[100];
  sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[m], eta_bins_edge[m+1]);
  // leg->SetHeader(legTitle,"");
  tdrHeader(leg,legTitle);
  leg->AddEntry(JER_cor_data_hist.at(m),label_data,"lep");
  leg->AddEntry(JER_cor_MC_hist.at(m),"MC","lep");
  canv -> Write();
  canvName = canvName(0, canvName.Length()-1);
  canvName += (m+1);
  canv -> Print("pdfy/JERs/cor_"+canvName+".pdf","pdf");
  canv -> Print("pdfy/JERs/cor_"+canvName+".png","png");

  if(JER_cor_scale_hist.at(m)->GetFunction("constfit")){
    JER_cor_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(kBlue);
    //         if(JER_cor_scale_hist.at(m)->GetFunction("pol0")){
    //			   JER_cor_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
  } else {
    std::cout << "Fit function at bin " << m;
    std::cout << " was not found" << std::endl;
  }

  PLOT_1SF(JER_cor_scale_hist,kBlue,m)

  if ((TPaveStats*)canvSF->GetPrimitive("stats")){
    TPaveStats *stats2 = (TPaveStats*)canvSF->GetPrimitive("stats");
    stats2->SetName("h1stats3");
    stats2->SetY1NDC(.64);
    stats2->SetY2NDC(.77);
    stats2->SetX1NDC(.66);
    stats2->SetX2NDC(.91);
    stats2->SetTextColor(4);
  }

  TLegend *legSF = tdrLeg(0.6,0.2,0.9,0.4);
  legSF->AddEntry(JER_cor_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");

  TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);
  f3->Draw("same");

  canvSF -> Write();
  canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+canvSFName+".pdf","pdf");
  canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+canvSFName+".png","png");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  JER plots for FE with NSC fits
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

for( unsigned int m = 0; m < JER_FE_MC_hist.size(); m++ ){
  TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
  mcFIT->SetLineColor(kRed+2);  // sets the color to red
  JER_FE_MC_hist.at(m) -> Fit("mcFIT", "M");

  double N, S, C;
  N = mcFIT -> GetParameter( 0 );
  S = mcFIT -> GetParameter( 1 );
  C = mcFIT -> GetParameter( 2 );

  TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [2]*(([0]/x)**2)+[3]*([0]**2)/x+[1]**2 )", 50, 1500 );
  dtFIT->SetLineColor(kBlue+2);  // sets the color to red
  dtFIT -> FixParameter(2, N*N);
  dtFIT -> FixParameter(3, S*S);
  JER_FE_data_hist.at(m) -> Fit("dtFIT", "M");

  PLOT_2(JER_FE_data_hist,JER_FE_MC_hist,kBlue,kRed,m)

  TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
  char legTitle[100];
  sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[eta_bin + eta_bins_fe - JER_FE_MC_hist.size() + m], eta_bins_edge[eta_bin + eta_bins_fe - JER_FE_MC_hist.size() + m+1]);
  // leg->SetHeader(legTitle,"");
  tdrHeader(leg,legTitle);
  leg->AddEntry(JER_FE_data_hist.at(m),label_data,"lep");
  leg->AddEntry(JER_FE_MC_hist.at(m),"MC","lep");

  canv -> Write();
  canvName = canvName(0, canvName.Length()-1);
  canvName += (m+1);
  canv -> Print("pdfy/JERs/"+canvName+".pdf","pdf");
  canv -> Print("pdfy/JERs/"+canvName+".png","png");

  TString canvSFName = JER_FE_scale_hist.at(m) -> GetTitle();
  nameXaxis = JER_FE_scale_hist.at(m)->GetXaxis()->GetTitle();
  nameYaxis = JER_FE_scale_hist.at(m)->GetYaxis()->GetTitle();

  vec.clear();
  vec.push_back(JER_FE_scale_hist.at(m));
  findExtreme(vec, &x_min, &x_max, &y_min, &y_max);

  TCanvas* canvSF = tdrCanvas(canvSFName, x_min, x_max, y_min, y_max, nameXaxis, nameYaxis);
  canvSF->SetTickx(0);
  canvSF->SetTicky(0);

  tdrDraw(JER_FE_scale_hist.at(m), "", kFullCircle, kBlue);

  if(JER_FE_scale_hist.at(m)->GetFunction("constfit")){
    JER_FE_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(kBlue);
    //         if(JER_FE_scale_hist.at(m)->GetFunction("pol0")){
    //			   JER_FE_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
  } else {
    std::cout << "Fit function at bin " << m;
    std::cout << " was not found" << std::endl;
  }

  canvSF->Update();
  if ((TPaveStats*)canvSF->GetPrimitive("stats")){
    TPaveStats *stats2 = (TPaveStats*)canvSF->GetPrimitive("stats");
    stats2->SetName("h1stats3");
    stats2->SetY1NDC(.64);
    stats2->SetY2NDC(.77);
    stats2->SetX1NDC(.66);
    stats2->SetX2NDC(.91);
    stats2->SetTextColor(4);
  }

  TLegend *legSF = tdrLeg(0.6,0.2,0.9,0.4);
  legSF->AddEntry(JER_FE_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");

  TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);
  f3->Draw("same");

  canvSF -> Write();
  canvSF -> Print("pdfy/NSC_SFs/NSC"+canvSFName+".pdf","pdf");
  canvSF -> Print("pdfy/NSC_SFs/NSC"+canvSFName+".png","png");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  correlated results JER plots for FE with NSC fits
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

for( unsigned int m = 0; m < JER_cor_FE_MC_hist.size(); m++ ){
  TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
  mcFIT->SetLineColor(kRed+2);  // sets the color to red
  JER_cor_FE_MC_hist.at(m) -> Fit("mcFIT", "M");

  double N, S, C;
  N = mcFIT -> GetParameter( 0 );
  S = mcFIT -> GetParameter( 1 );
  C = mcFIT -> GetParameter( 2 );

  TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [2]*(([0]/x)**2)+[3]*([0]**2)/x+[1]**2 )", 50, 1500 );
  dtFIT->SetLineColor(kBlue+2);  // sets the color to red
  dtFIT -> FixParameter(2, N*N);
  dtFIT -> FixParameter(3, S*S);
  JER_cor_FE_data_hist.at(m) -> Fit("dtFIT", "M");

  PLOT_2(JER_cor_FE_data_hist,JER_cor_FE_MC_hist,kBlue,kRed,m)

  TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
  char legTitle[100];
  sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[eta_bin + eta_bins_fe - JER_cor_FE_data_hist.size() + m], eta_bins_edge[eta_bin + eta_bins_fe - JER_cor_FE_data_hist.size() + m+1]);
  // leg->SetHeader(legTitle,"");
  tdrHeader(leg,legTitle);
  leg->AddEntry(JER_cor_FE_data_hist.at(m),label_data,"lep");
  leg->AddEntry(JER_cor_FE_MC_hist.at(m),"MC","lep");

  canv -> Write();
  canvName = canvName(0, canvName.Length()-1);
  canvName += (m+1);
  canv -> Print("pdfy/JERs/cor_"+canvName+".pdf","pdf");
  canv -> Print("pdfy/JERs/cor_"+canvName+".png","png");

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  correlated results JER plots for FE with NSC fits
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for( unsigned int m = 0; m < JER_cor_FE_MC_hist.size(); m++ ){
    TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
    mcFIT->SetLineColor(kRed+2);  // sets the color to red
    JER_cor_FE_MC_hist.at(m) -> Fit("mcFIT", "M");

    double N, S, C;
    N = mcFIT -> GetParameter( 0 );
    S = mcFIT -> GetParameter( 1 );
    C = mcFIT -> GetParameter( 2 );

    TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [2]*(([0]/x)**2)+[3]*([0]**2)/x+[1]**2 )", 50, 1500 );
    dtFIT->SetLineColor(kBlue+2);  // sets the color to red
    dtFIT -> FixParameter(2, N*N);
    dtFIT -> FixParameter(3, S*S);
    JER_cor_FE_data_hist.at(m) -> Fit("dtFIT", "M");

    PLOT_2(JER_cor_FE_data_hist,JER_cor_FE_MC_hist,kBlue,kRed,m)

    TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
    char legTitle[100];
    sprintf(legTitle,     "#eta #in [%.3f,%.3f]", eta_bins_edge[eta_bin + eta_bins_fe - JER_cor_FE_data_hist.size() + m], eta_bins_edge[eta_bin + eta_bins_fe - JER_cor_FE_data_hist.size() + m+1]);
    // leg->SetHeader(legTitle,"");
    tdrHeader(leg,legTitle);
    leg->AddEntry(JER_cor_FE_data_hist.at(m),label_data,"lep");
    leg->AddEntry(JER_cor_FE_MC_hist.at(m),"MC","lep");

    canv -> Write();
    canvName = canvName(0, canvName.Length()-1);
    canvName += (m+1);
    canv -> Print("pdfy/JERs/cor_"+canvName+".pdf","pdf");
    canv -> Print("pdfy/JERs/cor_"+canvName+".png","png");

    if(JER_cor_FE_scale_hist.at(m)->GetFunction("pol0")){
      JER_cor_FE_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(kBlue);
    } else {
      std::cout << "Fit function at bin " << m;
      std::cout << " was not found" << std::endl;
    }

    PLOT_1SF(JER_cor_FE_scale_hist,kBlue,m)

    if ((TPaveStats*)canvSF->GetPrimitive("stats")){
      TPaveStats *stats2 = (TPaveStats*)canvSF->GetPrimitive("stats");
      stats2->SetName("h1stats3");
      stats2->SetY1NDC(.64);
      stats2->SetY2NDC(.77);
      stats2->SetX1NDC(.66);
      stats2->SetX2NDC(.91);
      stats2->SetTextColor(4);
    }

    TLegend *legSF = new TLegend(0.6,0.2,0.9,0.4);
    legSF->AddEntry(JER_cor_FE_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");
    legSF->Draw();

    canvSF->Update();
    TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);
    f3->Draw("same");

    canvSF -> Write();
    canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+canvSFName+".pdf","pdf");
    canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+canvSFName+".png","png");
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if(JER_cor_FE_scale_hist.at(m)->GetFunction("constfit")){
    JER_cor_FE_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(kBlue);
    //         if(JER_cor_FE_scale_hist.at(m)->GetFunction("pol0")){
    //			   JER_cor_FE_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
  } else {
    std::cout << "Fit function at bin " << m;
    std::cout << " was not found" << std::endl;
  }

  PLOT_1SF(JER_cor_FE_scale_hist,kBlue,m)

  if ((TPaveStats*)canvSF->GetPrimitive("stats")){
    TPaveStats *stats2 = (TPaveStats*)canvSF->GetPrimitive("stats");
    stats2->SetName("h1stats3");
    stats2->SetY1NDC(.64);
    stats2->SetY2NDC(.77);
    stats2->SetX1NDC(.66);
    stats2->SetX2NDC(.91);
    stats2->SetTextColor(4);
  }

  TLegend *legSF = tdrLeg(0.6,0.2,0.9,0.4);
  legSF->AddEntry(JER_cor_FE_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");

  canvSF->Update();
  TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);
  f3->Draw("same");


  canvSF -> Write();
  canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+canvSFName+".pdf","pdf");
  canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+canvSFName+".png","png");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

for( unsigned int m = 0; m < JER_FE_data_hist.size(); m++ ){
  PLOT_2(JER_FE_data_hist,JER_FE_MC_hist,kBlue,kRed,m)

  TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
  leg->AddEntry(JER_FE_data_hist.at(m),label_data,"lep");
  leg->AddEntry(JER_FE_MC_hist.at(m),label_mc,"lep");

  canv -> Write();
  // canv -> Print("pdfy/"+canvName+".pdf","pdf");
}

for( unsigned int m = 0; m < JER_FE_data_hist_control.size(); m++ ){
  PLOT_2(JER_FE_data_hist_control,JER_FE_MC_hist_control,kBlue,kRed,m)

  TLegend *leg = tdrLeg(0.6,0.7,0.9,0.9);
  leg->AddEntry(JER_FE_data_hist_control.at(m),label_data,"lep");
  leg->AddEntry(JER_FE_MC_hist_control.at(m),label_mc,"lep");

  canv -> Write();
  // canv -> Print("pdfy/"+canvName+".pdf","pdf");
}
fJERs.Close();

//////////////////////////////////////////////////////////////////////////////////////////
//    SF plots overlayed with ...                                                       //
//////////////////////////////////////////////////////////////////////////////////////////
TFile gsfoverlayed("output/SFsOver.root","RECREATE");

gStyle -> SetOptFit(1);

for( unsigned int m = 0; m < JER_scale_hist.size(); m++ ){
  TF1* f;
  char line[100];
  TLegend *legend;

  PLOT_3(JER_scale_hist,JER_cor_scale_hist,JER015_scale_hist,kBlue,kRed, kGreen+2,m)

  if(JER_scale_hist.at(m)->GetFunction("constfit")){
    f = JER_scale_hist.at(m) -> GetFunction("constfit");
    f->SetLineColor(kBlue);
    legend = tdrLeg(0.30,0.70,0.50,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kBlue);
    // legend->SetHeader("JER", "C");
    tdrHeader(legend,"JER");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");
  } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

  if(JER_cor_scale_hist.at(m)->GetFunction("constfit")){
    f = JER_cor_scale_hist.at(m) -> GetFunction("constfit");
    f->SetLineColor(kRed);
    legend = tdrLeg(0.50,0.70,0.70,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kRed);
    // legend->SetHeader("JER cor", "C");
    tdrHeader(legend,"JER cor");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");
  } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

  if(JER015_scale_hist.at(m)->GetFunction("constfit")){
    f = JER015_scale_hist.at(m) -> GetFunction("constfit");
    f->SetLineColor(kGreen+2);
    legend = tdrLeg(0.70,0.70,0.90,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);  legend->SetTextColor(kGreen+2);
    // legend->SetHeader("JER 0.15", "C");
    tdrHeader(legend,"JER 0.15");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");
  } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

  char legTitle[100];
  TLegend *leg = tdrLeg(0.6,0.15,0.9,0.35);
  sprintf(legTitle, "#eta #in [%.3f,%.3f]",eta_bins_edge[m], eta_bins_edge[m+1]);
  // leg->SetHeader(legTitle,"");
  tdrHeader(leg,legTitle);
  leg->AddEntry(JER_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{" + label_mc + "}","lep");
  leg->AddEntry(JER_cor_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{" + label_mc + "} corelated","lep");
  leg->AddEntry(JER015_scale_hist.at(m),"#sigma_{0.15}^{" + label_data + "}/#sigma_{0.15}^{" + label_mc + "}","lep");
  leg->Draw("same");

  canv -> Write();
  canv -> Print("pdfy/SFs/"+canvName+".pdf","pdf");
  canv -> Print("pdfy/SFs/"+canvName+".png","png");
}

for( unsigned int m = 0; m < JER_FE_scale_hist.size(); m++ ){
  TF1* f;
  char line[100];
  TLegend *legend;

  PLOT_3(JER_FE_scale_hist,JER_cor_FE_scale_hist,JER015_FE_scale_hist,kBlue,kRed,kGreen+2,m)

  if(JER_FE_scale_hist.at(m)->GetFunction("constfit")){
    f = JER_FE_scale_hist.at(m) -> GetFunction("constfit");   f->SetLineColor(kBlue);
    legend = tdrLeg(0.30,0.70,0.50,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);     legend->SetTextColor(kBlue);
    // legend->SetHeader("JER", "C");
    tdrHeader(legend,"JER");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");
  } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

  if(JER_cor_FE_scale_hist.at(m)->GetFunction("constfit")){
    f = JER_cor_FE_scale_hist.at(m) -> GetFunction("constfit");   f->SetLineColor(kRed);
    legend = tdrLeg(0.50,0.70,0.70,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);         legend->SetTextColor(kRed);
    // legend->SetHeader("JER cor", "C");
    tdrHeader(legend,"JER cor");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");
  } else { std::cout << "Fit function at bin " << m << " was not found" << std::endl; }

  if(JER015_FE_scale_hist.at(m)->GetFunction("constfit")){
    f = JER015_FE_scale_hist.at(m) -> GetFunction("constfit");    f->SetLineColor(kGreen+2);
    legend = tdrLeg(0.70,0.70,0.90,0.90);
    legend->SetTextFont(42);  legend->SetTextSize(0.025);         legend->SetTextColor(kGreen+2);
    // legend->SetHeader("JER 0.15", "C");
    tdrHeader(legend,"JER 0.15");
    sprintf(line, "#chi^{2}/ndf = %.2f/%d", f->GetChisquare(), f->GetNDF());      legend->AddEntry((TObject*)0, line, "");
    sprintf(line, "p0 = %.5f #pm %.5f", f->GetParameter(0), f->GetParError(0));   legend->AddEntry((TObject*)0, line, "");
    legend->Draw("same");
    JER015_FE_scale_hist.at(m)->GetListOfFunctions();
  } else { std::cout << "Fit function at bin " << m << " was not found " << std::endl;}

  char legTitle[100];
  TLegend *leg = tdrLeg(0.6,0.15,0.9,0.35);
  sprintf(legTitle, "#eta #in [%.3f,%.3f]",eta_bins_edge[eta_bin + eta_bins_fe - JER_scale_hist.size() + m], eta_bins_edge[eta_bin + eta_bins_fe - JER_scale_hist.size() + m+1]);
  // leg->SetHeader(legTitle,"");
  tdrHeader(leg,legTitle);
  leg->SetTextFont(42);  leg->SetTextSize(0.05);
  leg->AddEntry(JER_FE_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{" + label_mc + "}","lep");
  leg->AddEntry(JER_cor_FE_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{" + label_mc + "}","lep");
  leg->AddEntry(JER015_FE_scale_hist.at(m),"#sigma_{0.15}^{" + label_data + "}/#sigma_{0.15}^{" + label_mc + "}","lep");
  leg->Draw("same");

  canv -> Write();
  canv -> Print("pdfy/SFs/"+canvName+".pdf","pdf");
  canv -> Print("pdfy/SFs/"+canvName+".png","png");
}

gsfoverlayed.Close();

gStyle->SetOptTitle(0);

for( unsigned int m = 0; m < asymmetries_all.size(); m++ ){
  for( unsigned int p = 0; p < asymmetries_all.at(m).size(); p++ ){
    for( unsigned int r = 0; r < asymmetries_all.at(m).at(p).size(); r++ ){
      asymmetries_all.at(m).at(p).at(r)      -> Scale(1./asymmetries_all.at(m).at(p).at(r) -> Integral(), "width");
      asymmetries_gen_all.at(m).at(p).at(r)  -> Scale(1./asymmetries_gen_all.at(m).at(p).at(r) -> Integral(), "width");
      asymmetries_all_data.at(m).at(p).at(r) -> Scale(1./asymmetries_all_data.at(m).at(p).at(r) -> Integral(), "width");

      PLOT_3_3(asymmetries_all,asymmetries_gen_all,asymmetries_all_data,kRed,kGreen+2,kBlue,m,p,r)

      char name_mcreco[100];
      char name_mcgen[100];
      char name_data[100];
      char legTitle[100];

      sprintf(name_mcreco,  "MC,   %.4f +- %.4f", asymmetries_width.at(m).at(p).at(r), asymmetries_width_error.at(m).at(p).at(r));
      sprintf(name_mcgen,   "gen,  %.4f +- %.4f", asymmetries_gen_width.at(m).at(p).at(r), asymmetries_gen_width_error.at(m).at(p).at(r));
      sprintf(name_data,    "data, %.4f +- %.4f", asymmetries_width_data.at(m).at(p).at(r), asymmetries_width_error_data.at(m).at(p).at(r));
      sprintf(legTitle,     "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV", eta_bins_edge[m], eta_bins_edge[m+1], p_bins_edge[p], p_bins_edge[p+1]);

      TLegend *leg = tdrLeg(0.5,0.7,0.95,0.9);
      // leg->SetHeader(legTitle,"");
      tdrHeader(leg,legTitle);
      sprintf(legTitle,     "#alpha < %.2f", alpha.at(r));
      leg->AddEntry((TObject*)0, legTitle, "");
      leg->AddEntry(asymmetries_all_data.at(m).at(p).at(r),  name_data,"lep");
      leg->AddEntry(asymmetries_all.at(m).at(p).at(r),      name_mcreco,"lep");
      leg->AddEntry(asymmetries_gen_all.at(m).at(p).at(r), name_mcgen,"lep");

      canv->Update();
      canv -> Print("output/asymmetries/"+canvName+".pdf","pdf");
      canv -> Print("output/asymmetries/"+canvName+".png","png");
    }
  }
}

for( unsigned int m = 0; m < forward_hist.size(); m++ ){
  for( unsigned int p = 0; p < forward_hist.at(m).size(); p++ ){
    for( unsigned int r = 0; r < forward_hist.at(m).at(p).size(); r++ ){
      forward_hist.at(m).at(p).at(r)      -> Scale(1./forward_hist.at(m).at(p).at(r) -> Integral(), "width");
      forward_gen_hist.at(m).at(p).at(r)  -> Scale(1./forward_gen_hist.at(m).at(p).at(r) -> Integral(), "width");
      forward_hist_data.at(m).at(p).at(r) -> Scale(1./forward_hist_data.at(m).at(p).at(r) -> Integral(), "width");

      PLOT_3_3(forward_hist,forward_gen_hist,forward_hist_data,kRed,kGreen+2,kBlue,m,p,r)

      char name_mcreco[100];
      char name_mcgen[100];
      char name_data[100];
      char legTitle[100];

      sprintf(name_mcreco,  "MC,   %.4f+-%.4f", forward_width.at(m).at(p).at(r), forward_width_error.at(m).at(p).at(r));
      sprintf(name_mcgen,   "gen,  %.4f+-%.4f", forward_gen_width.at(m).at(p).at(r), forward_gen_width_error.at(m).at(p).at(r));
      sprintf(name_data,    "data, %.4f+-%.4f", forward_width_data.at(m).at(p).at(r), forward_width_error_data.at(m).at(p).at(r));
      sprintf(legTitle,     "#eta #in [%.3f,%.3f], p_{T} #in [%.0f,%.0f] GeV",
      eta_bins_edge[eta_bin + eta_bins_fe - forward_hist.size() + m],
      eta_bins_edge[eta_bin + eta_bins_fe - forward_hist.size() + m+1],
      p_bins_edge_FT[p], p_bins_edge_FT[p+1]);

      TLegend *leg = tdrLeg(0.5,0.7,0.95,0.9);
      tdrHeader(leg,legTitle);
      sprintf(legTitle,     "#alpha < %.2f", alpha.at(r));
      leg->AddEntry((TObject*)0, legTitle, "");
      leg->AddEntry(forward_hist_data.at(m).at(p).at(r), name_data,"lep");
      leg->AddEntry(forward_hist.at(m).at(p).at(r),      name_mcreco,"lep");
      leg->AddEntry(forward_gen_hist.at(m).at(p).at(r),  name_mcgen,"lep");

      // canv -> cd(6);
      //
      // TPaveText *pt = new TPaveText(.05,.1,.95,.8);
      //
      // char name_eta_label[100];
      // sprintf(name_eta_label, "Eta range %.2f-%.2f",eta_bins_edge[m] ,eta_bins_edge[m+1] );

      // //TString eta_label = "Eta range"; eta_label += eta_bins_edge[m]; eta_label += "-"; eta_label += eta_bins_edge[m+1];
      // TString pt_label = "Pt range "; pt_label += p_bins_edge_FT[p]; pt_label += "-"; pt_label += p_bins_edge_FT[p+1];
      //
      // pt->AddText(name_eta_label);
      // pt->AddText(pt_label);
      //
      // pt->Draw();
      canv->Update();

      canv -> Print("output/asymmetries/"+canvName+".pdf","pdf");
      canv -> Print("output/asymmetries/"+canvName+".png","png");

    }
  }
}

//	for( unsigned int m = 0; m < forward_hist.size(); m++ )
//	{
//		for( unsigned int p = 0; p < forward_hist.at(m).size(); p++ )
//		{
//			for( unsigned int r = 0; r < forward_hist.at(m).at(p).size(); r++ )
//			{
//				forward_hist.at(m).at(p).at(r) -> Write();
//				forward_gen_hist.at(m).at(p).at(r) -> Write();
//				forward_hist_data.at(m).at(p).at(r) -> Write();
//			}
//		}
//	}



/*
for( unsigned int m = 0; m < forward_width_data_pt.size(); m++ )
{
for( unsigned int p = 0; p < forward_width_data_pt.at(m).size(); p++ )
{
//			std::cout << m << ", " << p << ", " << forward_width_data_pt.at(m).at(p).at(4) << std::endl;
}
}

for( unsigned int m = 0; m < scales_fe.size(); m++ )
{
for( unsigned int p = 0; p < scales_fe.at(m).size(); p++ )
{
//			std::cout << m << ", " << p << ", " << scales_fe.at(m).at(p) << std::endl;
}
}





TFile g("KFSR_vs_pt.root","RECREATE");

for( unsigned int m = 0; m < KFSR_vs_pt_all.size(); m++ )
{
KFSR_vs_pt_all.at(m) -> Write();
}

for( unsigned int m = 0; m < KFSR_vs_pt_gen_all.size(); m++ )
{
KFSR_vs_pt_gen_all.at(m) -> Write();
}

for( unsigned int m = 0; m < KFSR_vs_pt_all_data.size(); m++ )
{
KFSR_vs_pt_all_data.at(m) -> Write();
}

for( unsigned int m = 0; m < KFSR_vs_pt_FE.size(); m++ )
{
KFSR_vs_pt_FE.at(m) -> Write();
}

for( unsigned int m = 0; m < KFSR_vs_pt_gen_FE.size(); m++ )
{
KFSR_vs_pt_gen_FE.at(m) -> Write();
}

for( unsigned int m = 0; m < KFSR_vs_pt_FE_data.size(); m++ )
{
KFSR_vs_pt_FE_data.at(m) -> Write();
}
g.Close();





//		JERb_scale_hist.at(m) -> Fit("pol0","");
//		JERb_FE_scale_hist.at(m) -> Fit("pol0","");

TFile gbisb("SFsb.root","RECREATE");

//	std::cout << "now i'm fitting scale factors for b-par method" << std::endl;

for( unsigned int m = 0; m < JERb_scale_hist.size(); m++ )
{
JERb_scale_hist.at(m) -> Fit("pol0","");
JERb_scale_hist.at(m) -> Write();
}

for( unsigned int m = 0; m < JERb_FE_scale_hist.size(); m++ )
{
JERb_FE_scale_hist.at(m) -> Fit("pol0","");
JERb_FE_scale_hist.at(m) -> Write();
}

for( unsigned int m = 0; m < JERb_FE_scale_hist_control.size(); m++ )
{
JERb_FE_scale_hist_control.at(m) -> Fit("pol0","");
JERb_FE_scale_hist_control.at(m) -> Write();
}

gbisb.Close();

TFile gsfboverlayed("SFbsOver.root","RECREATE");


for( unsigned int m = 0; m < JERb_scale_hist.size(); m++ )
{
TString canvName = JERb_scale_hist.at(m) -> GetTitle();
TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

JERb_scale_hist.at(m) -> SetMarkerColor(2);

JERb_scale_hist.at(m) -> SetMarkerStyle(20);

JERb_scale_hist.at(m) -> Draw();

canv->Update();

JERb_scale_hist.at(m) -> Draw("same");


canv->cd();
//		CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

canv -> Write();

TString outputname = JERb_scale_hist.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}


for( unsigned int m = 0; m < JERb_FE_scale_hist.size(); m++ )
{
TString canvName = JERb_FE_scale_hist.at(m) -> GetTitle();

TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

JERb_FE_scale_hist.at(m) -> SetMarkerColor(2);

JERb_FE_scale_hist.at(m) -> SetMarkerStyle(20);

JERb_FE_scale_hist.at(m) -> Draw();

canv->Update();


canv->Update();
JERb_FE_scale_hist.at(m) -> Draw("same");


canv->cd();
//			CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

canv -> Write();

TString outputname = JERb_FE_scale_hist.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}
gsfboverlayed.Close();






setTDRStyle();

writeExtraText = true;       // if extra text
extraText  = "       Preliminary";  // default extra text is "Preliminary"
lumi_13TeV = "1.28 fb^{-1}";
lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)

int iPeriod = 4;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)


gStyle -> SetOptFit(1);





























TFile hkfsr_merged("KFSR_merged.root","RECREATE");

for( unsigned int m = 0; m < KFSR_vs_pt_all.size(); m++ )
{
TString canvName = KFSR_vs_pt_all.at(m) -> GetTitle();

TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

if( KFSR_vs_pt_all.at(m) -> GetEntries() != 0 )     KFSR_vs_pt_all.at(m) -> GetFunction("linfit")->SetLineColor(6);
if( KFSR_vs_pt_gen_all.at(m) -> GetEntries() != 0 ) KFSR_vs_pt_gen_all.at(m) -> GetFunction("linfit")->SetLineColor(2);
if( KFSR_vs_pt_all_data.at(m) -> GetEntries() != 0 )KFSR_vs_pt_all_data.at(m) -> GetFunction("linfit")->SetLineColor(4);

KFSR_vs_pt_all.at(m) -> SetMarkerColor(6);
KFSR_vs_pt_gen_all.at(m) -> SetMarkerColor(2);
KFSR_vs_pt_all_data.at(m) -> SetMarkerColor(4);

KFSR_vs_pt_all.at(m) -> SetMarkerStyle(20);
KFSR_vs_pt_gen_all.at(m) -> SetMarkerStyle(20);
KFSR_vs_pt_all_data.at(m) -> SetMarkerStyle(20);

KFSR_vs_pt_all.at(m) -> Draw();

canv->Update();
TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
stats1->SetName("h1stats2");
stats1->SetY1NDC(.74);
stats1->SetY2NDC(.90);
stats1->SetX1NDC(.71);
stats1->SetX2NDC(.96);
stats1->SetTextColor(6);

TList *list1 = stats1->GetListOfLines();
TLatex *myt1 = new TLatex(0,0,"MC = ");
myt1 ->SetTextFont(42);
myt1 ->SetTextSize(0.04);
myt1 ->SetTextColor(6);
list1->AddFirst(myt1);

KFSR_vs_pt_gen_all.at(m) -> Draw();

canv->Update();
TPaveStats *stats2 = (TPaveStats*)canv->GetPrimitive("stats");
stats2->SetName("h1stats3");
stats2->SetY1NDC(.74);
stats2->SetY2NDC(.90);
stats2->SetX1NDC(.46);
stats2->SetX2NDC(.71);
stats2->SetTextColor(2);

TList *list2 = stats2->GetListOfLines();
TLatex *myt2 = new TLatex(0,0,"PLI = ");
myt2 ->SetTextFont(42);
myt2 ->SetTextSize(0.04);
myt2 ->SetTextColor(2);
list2->AddFirst(myt2);

KFSR_vs_pt_all_data.at(m) -> Draw();

canv->Update();
TPaveStats *stats3 = (TPaveStats*)canv->GetPrimitive("stats");
stats3->SetName("h1stats4");
stats3->SetY1NDC(.74);
stats3->SetY2NDC(.60);
stats3->SetX1NDC(.71);
stats3->SetX2NDC(.96);
stats3->SetTextColor(4);

TList *list3 = stats3->GetListOfLines();
TLatex *myt3 = new TLatex(0,0,"" + label_data + " = ");
myt3 ->SetTextFont(42);
myt3 ->SetTextSize(0.04);
myt3 ->SetTextColor(4);
list3->AddFirst(myt3);

KFSR_vs_pt_all_data.at(m) -> Draw();

canv->Update();
KFSR_vs_pt_gen_all.at(m) -> Draw("same");
KFSR_vs_pt_all.at(m) -> Draw("same");


canv->cd();
CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

canv -> Write();

TString outputname = KFSR_vs_pt_all.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}



for( unsigned int m = 0; m < KFSR_vs_pt_FE.size(); m++ )
{
TString canvName = KFSR_vs_pt_FE.at(m) -> GetTitle();

TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

if( KFSR_vs_pt_FE.at(m) -> GetEntries() != 0 )     KFSR_vs_pt_FE.at(m) -> GetFunction("linfit")->SetLineColor(6);
if( KFSR_vs_pt_gen_FE.at(m) -> GetEntries() != 0 ) KFSR_vs_pt_gen_FE.at(m) -> GetFunction("linfit")->SetLineColor(2);
if( KFSR_vs_pt_FE_data.at(m) -> GetEntries() != 0 )KFSR_vs_pt_FE_data.at(m) -> GetFunction("linfit")->SetLineColor(4);

KFSR_vs_pt_FE.at(m) -> SetMarkerColor(6);
KFSR_vs_pt_gen_FE.at(m) -> SetMarkerColor(2);
KFSR_vs_pt_FE_data.at(m) -> SetMarkerColor(4);

KFSR_vs_pt_FE.at(m) -> SetMarkerStyle(20);
KFSR_vs_pt_gen_FE.at(m) -> SetMarkerStyle(20);
KFSR_vs_pt_FE_data.at(m) -> SetMarkerStyle(20);

KFSR_vs_pt_FE.at(m) -> Draw();

canv->Update();
TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
stats1->SetName("h1stats2");
stats1->SetY1NDC(.74);
stats1->SetY2NDC(.90);
stats1->SetX1NDC(.71);
stats1->SetX2NDC(.96);
stats1->SetTextColor(6);

TList *list1 = stats1->GetListOfLines();
TLatex *myt1 = new TLatex(0,0,"MC = ");
myt1 ->SetTextFont(42);
myt1 ->SetTextSize(0.04);
myt1 ->SetTextColor(6);
list1->AddFirst(myt1);

KFSR_vs_pt_gen_FE.at(m) -> Draw();

canv->Update();
TPaveStats *stats2 = (TPaveStats*)canv->GetPrimitive("stats");
stats2->SetName("h1stats3");
stats2->SetY1NDC(.74);
stats2->SetY2NDC(.90);
stats2->SetX1NDC(.46);
stats2->SetX2NDC(.71);
stats2->SetTextColor(2);

TList *list2 = stats2->GetListOfLines();
TLatex *myt2 = new TLatex(0,0,"PLI = ");
myt2 ->SetTextFont(42);
myt2 ->SetTextSize(0.04);
myt2 ->SetTextColor(2);
list2->AddFirst(myt2);

KFSR_vs_pt_FE_data.at(m) -> Draw();

canv->Update();
TPaveStats *stats3 = (TPaveStats*)canv->GetPrimitive("stats");
stats3->SetName("h1stats4");
stats3->SetY1NDC(.74);
stats3->SetY2NDC(.60);
stats3->SetX1NDC(.71);
stats3->SetX2NDC(.96);
stats3->SetTextColor(4);

TList *list3 = stats3->GetListOfLines();
TLatex *myt3 = new TLatex(0,0,"" + label_data + " = ");
myt3 ->SetTextFont(42);
myt3 ->SetTextSize(0.04);
myt3 ->SetTextColor(4);
list3->AddFirst(myt3);

KFSR_vs_pt_FE_data.at(m) -> Draw();

canv->Update();
KFSR_vs_pt_gen_FE.at(m) -> Draw("same");
KFSR_vs_pt_FE.at(m) -> Draw("same");


canv->cd();
CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

canv -> Write();

TString outputname = KFSR_vs_pt_FE.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}
hkfsr_merged.Close();





TFile fJERs("Jersalltogether.root","RECREATE");
for( unsigned int m = 0; m < JER_data_hist.size(); m++ )
{
TString canvName = JER_data_hist.at(m) -> GetTitle();

TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

JER_data_hist.at(m) -> SetMarkerColor(4);
JER_MC_hist.at(m) -> SetMarkerColor(2);

JER_data_hist.at(m) -> SetMarkerStyle(20);
JER_MC_hist.at(m) -> SetMarkerStyle(20);

JER_data_hist.at(m) -> Draw();

canv->Update();

JER_MC_hist.at(m) -> Draw();

canv->Update();
JER_data_hist.at(m) -> Draw("same");


canv->cd();
CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
leg->AddEntry(JER_data_hist.at(m),label_data,"lep");
leg->AddEntry(JER_MC_hist.at(m),"MC","lep");
leg->Draw();

canv -> Write();

TString outputname = JER_data_hist.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}



for( unsigned int m = 0; m < JER_FE_data_hist.size(); m++ )
{
TString canvName = JER_FE_data_hist.at(m) -> GetTitle();

TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

JER_FE_data_hist.at(m) -> SetMarkerColor(4);
JER_FE_MC_hist.at(m) -> SetMarkerColor(2);

JER_FE_data_hist.at(m) -> SetMarkerStyle(20);
JER_FE_MC_hist.at(m) -> SetMarkerStyle(20);

JER_FE_data_hist.at(m) -> Draw();

canv->Update();
JER_FE_MC_hist.at(m) -> Draw();

canv->Update();
JER_FE_data_hist.at(m) -> Draw("same");


canv->cd();
CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
leg->AddEntry(JER_FE_data_hist.at(m),label_data,"lep");
leg->AddEntry(JER_FE_MC_hist.at(m),"MC","lep");
leg->Draw();

canv -> Write();

TString outputname = JER_FE_data_hist.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}

for( unsigned int m = 0; m < JER_FE_data_hist_control.size(); m++ )
{
TString canvName = JER_FE_data_hist_control.at(m) -> GetTitle();

TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

JER_FE_data_hist_control.at(m) -> SetMarkerColor(4);
JER_FE_MC_hist_control.at(m) -> SetMarkerColor(2);

JER_FE_data_hist_control.at(m) -> SetMarkerStyle(20);
JER_FE_MC_hist_control.at(m) -> SetMarkerStyle(20);

JER_FE_data_hist_control.at(m) -> Draw();

canv->Update();

JER_FE_MC_hist_control.at(m) -> Draw();

canv->Update();
JER_FE_data_hist_control.at(m) -> Draw("same");


canv->cd();
CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
leg->AddEntry(JER_FE_data_hist_control.at(m),label_data,"lep");
leg->AddEntry(JER_FE_MC_hist_control.at(m),"MC","lep");
leg->Draw();

canv -> Write();

TString outputname = JER_FE_data_hist_control.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}
fJERs.Close();



//	std::cout << " tutej jestsems --------------------------- jestem tutej --------------------------- jestem tutej " << std::endl;


TFile gsfoverlayed("SFsOver.root","RECREATE");


for( unsigned int m = 0; m < JERb_scale_hist.size(); m++ )
{
TString canvName = JERb_scale_hist.at(m) -> GetTitle();

TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

JERb_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(3);
JER015_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(4);

JERb_scale_hist.at(m) -> SetMarkerColor(3);
JER015_scale_hist.at(m) -> SetMarkerColor(4);

JERb_scale_hist.at(m) -> SetLineColor(3);
JER015_scale_hist.at(m) -> SetLineColor(4);

JERb_scale_hist.at(m) -> SetMarkerStyle(20);
JER015_scale_hist.at(m) -> SetMarkerStyle(20);


JER015_scale_hist.at(m) -> Draw();

canv->Update();
TPaveStats *stats2 = (TPaveStats*)canv->GetPrimitive("stats");
stats2->SetName("h1stats3");
stats2->SetY1NDC(.64);
stats2->SetY2NDC(.77);
stats2->SetX1NDC(.66);
stats2->SetX2NDC(.91);
stats2->SetTextColor(4);

JERb_scale_hist.at(m) -> Draw();

canv->Update();
TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
stats1->SetName("h1stats2");
stats1->SetY1NDC(.77);
stats1->SetY2NDC(.90);
stats1->SetX1NDC(.66);
stats1->SetX2NDC(.91);
stats1->SetTextColor(3);

JER015_scale_hist.at(m) -> Draw();
JERb_scale_hist.at(m) -> Draw("same");

canv->cd();
CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

TLegend *leg = new TLegend(0.6,0.2,0.9,0.4);
leg->AddEntry(JERb_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{MC}","lep");
leg->AddEntry(JER015_scale_hist.at(m),"#sigma_{0.15}^{" + label_data + "}/#sigma_{0.15}^{MC}","lep");
leg->Draw();
canv -> Write();

TString outputname = JERb_scale_hist.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}
for( unsigned int m = 0; m < JERb_FE_scale_hist.size(); m++ )
{
TString canvName = JERb_FE_scale_hist.at(m) -> GetTitle();

TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
canv->SetFillColor(0);
canv->SetBorderMode(0);
canv->SetFrameFillStyle(0);
canv->SetFrameBorderMode(0);
canv->SetLeftMargin( L/W );
canv->SetRightMargin( R/W );
canv->SetTopMargin( T/H );
canv->SetBottomMargin( B/H );
canv->SetTickx(0);
canv->SetTicky(0);

JERb_FE_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(3);
JER015_FE_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(4);

JERb_FE_scale_hist.at(m) -> SetMarkerColor(3);
JER015_FE_scale_hist.at(m) -> SetMarkerColor(4);

JERb_FE_scale_hist.at(m) -> SetLineColor(3);
JER015_FE_scale_hist.at(m) -> SetLineColor(4);

JERb_FE_scale_hist.at(m) -> SetMarkerStyle(20);
JER015_FE_scale_hist.at(m) -> SetMarkerStyle(20);


JER015_FE_scale_hist.at(m) -> Draw();

canv->Update();
TPaveStats *stats2 = (TPaveStats*)canv->GetPrimitive("stats");
stats2->SetName("h1stats3");
stats2->SetY1NDC(.64);
stats2->SetY2NDC(.77);
stats2->SetX1NDC(.66);
stats2->SetX2NDC(.91);
stats2->SetTextColor(4);

JERb_FE_scale_hist.at(m) -> Draw();
canv->Update();
TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
stats1->SetName("h1stats2");
stats1->SetY1NDC(.77);
stats1->SetY2NDC(.90);
stats1->SetX1NDC(.66);
stats1->SetX2NDC(.91);
stats1->SetTextColor(3);

JER015_FE_scale_hist.at(m) -> Draw();
JERb_FE_scale_hist.at(m) -> Draw("same");
JER015_FE_scale_hist.at(m) -> Draw("same");

canv->cd();
CMS_lumi( canv, iPeriod, 0 );

canv->Update();
canv->RedrawAxis();
canv->GetFrame()->Draw();
canv->Draw();

TLegend *leg = new TLegend(0.6,0.2,0.9,0.4);
leg->AddEntry(JERb_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{MC}","lep");
leg->AddEntry(JER015_scale_hist.at(m),"#sigma_{0.15}^{" + label_data + "}/#sigma_{0.15}^{MC}","lep");
leg->Draw();

canv -> Write();

TString outputname = JERb_FE_scale_hist.at(m) -> GetTitle();

canv -> Print("pdfy/"+outputname+".pdf","pdf");
}




gsfoverlayed.Close();

TFile checksformikko("CHECKs.root","RECREATE");

for( unsigned int m = 0; m < KFSR_ratios_all.size(); m++ )
{
KFSR_ratios_all.at(m) -> Fit("pol0","");
KFSR_ratios_all.at(m) -> Write();
}
for( unsigned int m = 0; m < KFSR_ratios_FE.size(); m++ )
{
KFSR_ratios_FE.at(m) -> Fit("pol0","");
KFSR_ratios_FE.at(m) -> Write();
}
for( unsigned int m = 0; m < widths_015_ratios_all.size(); m++ )
{
widths_015_ratios_all.at(m) -> Fit("pol0","");
widths_015_ratios_all.at(m) -> Write();
}
for( unsigned int m = 0; m < widths_015_ratios_FE.size(); m++ )
{
widths_015_ratios_FE.at(m) -> Fit("pol0","");
widths_015_ratios_FE.at(m) -> Write();
}

checksformikko.Close();
*/

return 0;
}
// cout << "trovato " << __LINE__ << endl;

// number of event.
// cout << "calcola " << JER_FE_scale_hist_control.size()  << " " << widths_hist_FE.at(0).size() << " " << soft_widths_hist_FE.size() << endl;
// aiuto
// cout << "calcola " << widths_hist_all.size()  << " " << widths_hist_all.at(m).size() << endl;
// cout << "calcola_2 " << forward_hist.size() <<  " "  << forward_hist.at(m).size()  <<  " "  << forward_hist.at(m).at(p).size()  << endl;
