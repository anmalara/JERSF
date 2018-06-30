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

#include "TFrame.h"
#include "plotting/CMS_lumi.C"
#include "plotting/tdrstyle.C"

#include <fstream>
#include <sstream>

#include <TGaxis.h>

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
void histWidthAsym( std::vector<std::vector<std::vector<TH1F*> > > &Asymmetry , std::vector<std::vector<std::vector<double> > > &Widths, std::vector<std::vector<std::vector<double> > > &WidthsError , bool fill_all );
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
void fill_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< double > > Widths, std::vector< std::vector< double > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range, int shift2);
void fill_mctruth_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range);




void fitAsym( TH1F &hist, double &width, double &error , double &centre, double &centre_error, double &chisqr, int fitType, double position , bool fix ); // linear fit for general use ( extrapolating to alpha = 0 e.g.)
void fitConst( TH1F &hist, double &value, double &error );
void fitShift( TH1F &hist, double &a, double &errora, double &b, double &errorb ); //gaussian fit, used for MC-Truth
void fitMCTruth( TH1F &hist, double &width, double &error ); // truncated RMS, points from xq_IQW to yq_IQW, rejectes xq_IQW of all points from left, and points above yq_IQW of all points. Both variables from 0 to 1, xq < yq.




int mainRun( bool data, const char* filename, const char* filename_data, TString lumi, TString label_mc, TString label_data )      // filename is input .root file, data determines if we work on data or mc (no gen level info in data)
{

   //////////////////////////////////////////////////////////////////////////////////////////
   //    I load all histograms I will need                                                 //
   //////////////////////////////////////////////////////////////////////////////////////////

    bool real_data = true; // If false: compare MC to MC:
                           // read out genlevel info from both MC and data.
                           // If true: compare MC to data:
                           // use MC gen info for data as well.
    //bool data = false;     // MC is not data and gets this boolean.

   // bin values only for labels at some plots

   double p_bins_edge [] = { 51, 74, 96, 165, 232, 300, 366, 456, 569, 1500 };
   double p_bins_edge_FT [] = { 72, 95, 118, 188, 257, 354, 1500 };

//   double eta_bins_edge [] = { 0, 0.522, 1.131, 1.740, 2.322, 2.853, 2.964, 3.139, 5.191 };
//   double eta_bins_control_edge [] = { 0, 1.131, 1.740, 2.322, 2.853, 2.964, 3.139, 5.191 };

//   double eta_bins_edge [] = { 0, 0.522, 1.131, 1.305, 1.740, 2.043, 2.5, 2.853, 2.964, 3.139, 5.191 };
//   double eta_bins_control_edge [] = { 0, 1.131, 1.305, 1.740, 2.043, 2.5, 2.853, 2.964, 3.139, 5.191 };

   double eta_bins_edge [] = { 0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191 };
   double eta_bins_control_edge [] = { 0, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191 };

    if(!lumi) lumi = "804.2 pb^{-1}";
    if(!label_data) label_data = "Data";
    if(!label_mc) label_mc = "MC";
    //if ( !real_data && (label_data.std::string::find("ata") != std::string::npos)){
    if ( !real_data && (label_data == "Data" || label_data == "data")){
        std::cout << "WARNING: are you using data?";
        std::cout << "Program has set real_data=" << real_data << std::endl;
        std::cout << "PLI will be corrected with MC gen info only: " << real_data <<std::endl;
    }
   std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all, asymmetries_gen_all, asymmetries_all_data, mc_truth_asymmetries_all;
   std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all_pt, asymmetries_gen_all_pt, asymmetries_all_data_pt;

   std::vector< std::vector< std::vector< TH1F* > > > forward_hist, forward_gen_hist, forward_hist_data, mc_truth_forward_hist;
   std::vector< std::vector< std::vector< TH1F* > > > forward_hist_pt, forward_gen_hist_pt, forward_hist_data_pt;

   std::vector< std::vector< std::vector< TH1F* > > > forward_gen_hist_data, asymmetries_gen_all_data, forward_gen_hist_data_pt, asymmetries_gen_all_data_pt;

   std::vector< std::vector< std::vector< TH1F* > > > dummy_hists;

//   int pt_bins = 9, pt_bins_fe = 6, eta_bins = 5, eta_bins_fe = 3, eta_bins_fec = 4;
//   int pt_bins = 9, pt_bins_fe = 6, eta_bins = 7, eta_bins_fe = 3, eta_bins_fec = 6;
   int pt_bins = 9, pt_bins_fe = 6, eta_bins = 10, eta_bins_fe = 3, eta_bins_fec = 8;

//   int mode = 3;   // mode of the analysis:
                  //      0 -> exclusive, full def            3 -> inclusive, full def
                  //      1 -> exclusive, parallel part         4 -> inclusive, parallel part
                  //      2 -> exclusive, perp. part            5 -> inclusive, perp. part
                  //      actualy it's now just for my information. doesn't make a difference...

   TFile *f, *f_data;

   f = new TFile( filename, "READ");
   f_data = new TFile( filename_data, "READ");

   histLoadAsym( *f_data,    real_data, "asymm_eta", asymmetries_all_data, asymmetries_gen_all_data, pt_bins, eta_bins );   // load asymm from data
   histLoadAsym( *f,    data, "asymm_eta", asymmetries_all, asymmetries_gen_all, pt_bins, eta_bins );      // load asymm from mc (all)

   histLoadFE1( *f_data,    real_data, "forward_control_probe", forward_hist_data , forward_gen_hist_data , pt_bins, eta_bins_fec );
   histLoadFE1( *f,    data, "forward_control_probe", forward_hist , forward_gen_hist, pt_bins, eta_bins_fec );

   histLoadFE1( *f_data,    real_data, "forward_probe", forward_hist_data , forward_gen_hist_data , pt_bins_fe, eta_bins_fe );
   histLoadFE1( *f,    data, "forward_probe", forward_hist, forward_gen_hist, pt_bins_fe, eta_bins_fe );


   histLoadAsym( *f_data,    real_data, "asymmpt_probe", asymmetries_all_data_pt, asymmetries_gen_all_data_pt, pt_bins, eta_bins );   // load asymm pt from data
   histLoadAsym( *f,    data, "asymmpt_probe", asymmetries_all_pt, asymmetries_gen_all_pt, pt_bins, eta_bins );      // load asymm pt from mc (all)

   histLoadFE1( *f_data,    real_data, "forwardpt_control_probe", forward_hist_data_pt , forward_gen_hist_data_pt , pt_bins, eta_bins_fec );
   histLoadFE1( *f,    data, "forwardpt_control_probe", forward_hist_pt , forward_gen_hist_pt, pt_bins, eta_bins_fec );

   histLoadFE1( *f_data,    real_data, "forwardpt_probe", forward_hist_data_pt , forward_gen_hist_data_pt , pt_bins_fe, eta_bins_fe );
   histLoadFE1( *f,    data, "forwardpt_probe", forward_hist_pt , forward_gen_hist_pt, pt_bins_fe, eta_bins_fe );


   histLoadAsym( *f,    data, "mctruth_eta", mc_truth_asymmetries_all, dummy_hists, pt_bins, eta_bins );      // load asymm from mc (all)

   histLoadFE1( *f,    data, "mctruth_forward_control_probe", mc_truth_forward_hist , dummy_hists, pt_bins, eta_bins_fec );
   histLoadFE1( *f,    data, "mctruth_forward_probe",         mc_truth_forward_hist , dummy_hists, pt_bins_fe, eta_bins_fe );

   //////////////////////////////////////////////////////////////////////////////////////////
   //    End of loading histograms                                                         //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I calculate pt_mean for each alpha and pt bin and eta bin.                        //
   //////////////////////////////////////////////////////////////////////////////////////////

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


   //////////////////////////////////////////////////////////////////////////////////////////
   //    End of alpha_mean calc.                                                           //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I calculate width of asymmetry distributions only for                             //
   //    alpha bins above 15 GeV thresholds (too soft contriubtions)                       //
   //    e.g. for bin p_T_ave (55-75) alpha 0.1 corresponds to 57 GeV jets                 //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< std::vector< double > > > forward_width, forward_gen_width, forward_width_data, forward_gen_width_data;
   std::vector< std::vector< std::vector< double > > > forward_width_error, forward_gen_width_error, forward_width_error_data, forward_gen_width_error_data;

   std::vector< std::vector< std::vector< double > > > asymmetries_width, asymmetries_gen_width, asymmetries_width_data, asymmetries_gen_width_data;
   std::vector< std::vector< std::vector< double > > > asymmetries_width_error, asymmetries_gen_width_error, asymmetries_width_error_data, asymmetries_gen_width_error_data;

   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "mc asymmetries" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   histWidthAsym( asymmetries_all , asymmetries_width, asymmetries_width_error, false );
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "mc gen asymmetries" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   histWidthAsym( asymmetries_gen_all , asymmetries_gen_width, asymmetries_gen_width_error, false );
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "data asymmetries" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   histWidthAsym( asymmetries_all_data , asymmetries_width_data, asymmetries_width_error_data, false );
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "data gen ;) asymmetries" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   histWidthAsym( asymmetries_gen_all_data , asymmetries_gen_width_data, asymmetries_gen_width_error_data, false );
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "fw mc asymmetries" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;

   histWidthAsym( forward_hist , forward_width, forward_width_error, false );
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "fw gen mc asymmetries" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   histWidthAsym( forward_gen_hist , forward_gen_width, forward_gen_width_error, false );
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "data fw asymmetries" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   histWidthAsym( forward_hist_data , forward_width_data, forward_width_error_data, false );
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "data fw gen asymmetries" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   histWidthAsym( forward_gen_hist_data , forward_gen_width_data, forward_gen_width_error_data, false );

   //////////////////////////////////////////////////////////////////////////////////////////
   //    End of asymmetry fitting                                                          //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I calculate widths, this time also including                                      //
   //    alpha bins below 15GeV threshold                                                  //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< std::vector< double > > > soft_forward_width, soft_forward_gen_width, soft_forward_width_data, soft_forward_gen_width_data;
   std::vector< std::vector< std::vector< double > > > soft_forward_width_error, soft_forward_gen_width_error, soft_forward_width_error_data, soft_forward_gen_width_error_data;

   std::vector< std::vector< std::vector< double > > > soft_asymmetries_width, soft_asymmetries_gen_width, soft_asymmetries_width_data, soft_asymmetries_gen_width_data;
   std::vector< std::vector< std::vector< double > > > soft_asymmetries_width_error, soft_asymmetries_gen_width_error, soft_asymmetries_width_error_data, soft_asymmetries_gen_width_error_data;

   histWidthAsym( asymmetries_all , soft_asymmetries_width, soft_asymmetries_width_error, true );
   histWidthAsym( asymmetries_gen_all , soft_asymmetries_gen_width, soft_asymmetries_gen_width_error, true );
   histWidthAsym( asymmetries_all_data , soft_asymmetries_width_data, soft_asymmetries_width_error_data, true );
   histWidthAsym( asymmetries_gen_all_data , soft_asymmetries_gen_width_data, soft_asymmetries_gen_width_error_data, true );

   histWidthAsym( forward_hist , soft_forward_width, soft_forward_width_error, true );
   histWidthAsym( forward_gen_hist , soft_forward_gen_width, soft_forward_gen_width_error, true );
   histWidthAsym( forward_hist_data , soft_forward_width_data, soft_forward_width_error_data, true );
   histWidthAsym( forward_gen_hist_data , soft_forward_gen_width_data, soft_forward_gen_width_error_data, true );

   //////////////////////////////////////////////////////////////////////////////////////////
   //    End of widths with no alpha restrictions                                          //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    Calculate mcTruth resolution for cross check with dijet calculation               //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< std::vector< double > > > mcTruth_res, forward_mcTruth_res;
   std::vector< std::vector< std::vector< double > > > mcTruth_res_error, forward_mcTruth_res_error;

   histWidthMCTruth( mc_truth_asymmetries_all, mcTruth_res        , mcTruth_res_error        );
   histWidthMCTruth( mc_truth_forward_hist   , forward_mcTruth_res, forward_mcTruth_res_error);

   //////////////////////////////////////////////////////////////////////////////////////////
   //    Calculate mcTruth resolution                                                      //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I define width(alpha_max) histograms                                              //
   //////////////////////////////////////////////////////////////////////////////////////////

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

   //////////////////////////////////////////////////////////////////////////////////////////
   //    histograms filled                                                                 //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I do same for alpha unconstrained widths                                          //
   //    one needs these plots to prove which points should be rejected!                   //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< TH1F* > > soft_widths_hist_FE, soft_widths_hist_FE_gen, soft_widths_hist_FE_data, soft_widths_hist_FE_gen_data;
   std::vector< std::vector< TH1F* > > soft_widths_hist_all, soft_widths_hist_all_gen, soft_widths_hist_all_data, soft_widths_hist_all_gen_data;

   def_fill_widths( "widths", soft_widths_hist_all, soft_asymmetries_width, soft_asymmetries_width_error );
   def_fill_widths( "widths_gen", soft_widths_hist_all_gen, soft_asymmetries_gen_width, soft_asymmetries_gen_width_error );
   def_fill_widths( "widths_data", soft_widths_hist_all_data, soft_asymmetries_width_data, soft_asymmetries_width_error_data );
   def_fill_widths( "widths_gen_data", soft_widths_hist_all_gen_data, soft_asymmetries_gen_width_data, soft_asymmetries_gen_width_error_data );

   def_fill_widths( "widths_fe", soft_widths_hist_FE, soft_forward_width, soft_forward_width_error );
   def_fill_widths( "widths_gen_fe", soft_widths_hist_FE_gen, soft_forward_gen_width, soft_forward_gen_width_error );
   def_fill_widths( "widths_data_fe", soft_widths_hist_FE_data, soft_forward_width_data, soft_forward_width_error_data );
   def_fill_widths( "widths_gen_data_fe", soft_widths_hist_FE_gen_data, soft_forward_gen_width_data, soft_forward_gen_width_error_data );

   ////////////////////////////////////////////////////////////////////////////
   //    I fit line or const to width(alpha_max)                             //
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

   //////////////////////////////////////////////////////////////////////////////////////////
   //    end of linear fit                                                                 //
   //////////////////////////////////////////////////////////////////////////////////////////
   //      Correlated fit                                                                  //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > widths_cor_all, widths_cor_gen_all, widths_cor_all_data, widths_cor_all_gen_data;
   std::vector< std::vector< double > > widths_cor_all_error, widths_cor_gen_all_error, widths_cor_all_error_data, widths_cor_all_error_gen_data;

   std::vector< std::vector< double > > widths_cor_FE, widths_cor_gen_FE, widths_cor_FE_data, widths_cor_gen_FE_data;
   std::vector< std::vector< double > > widths_cor_FE_error, widths_cor_gen_FE_error, widths_cor_FE_error_data, widths_cor_gen_FE_error_data;

   std::vector< std::vector< TGraphErrors *> > MC_cor_graphs, data_cor_graphs, gen_cor_graphs, gen_data_cor_graphs;
   std::vector< std::vector< TGraphErrors *> > FE_MC_cor_graphs, FE_data_cor_graphs, FE_gen_cor_graphs, FE_gen_data_cor_graphs;

   histLinCorFit(asymmetries_width,
                 asymmetries_width_error,
                 MC_cor_graphs,
                 widths_cor_all,
                 widths_cor_all_error);
   histLinCorFit(asymmetries_width_data,
                 asymmetries_width_error_data,
                 data_cor_graphs,
                 widths_cor_all_data,
                 widths_cor_all_error_data);
   histLinCorFit(asymmetries_gen_width,
                 asymmetries_gen_width_error,
                 gen_cor_graphs,
                 widths_cor_gen_all,
                 widths_cor_gen_all_error);
   histLinCorFit(asymmetries_gen_width_data,
                 asymmetries_gen_width_error_data,
                 gen_data_cor_graphs,
                 widths_cor_all_gen_data,
                 widths_cor_all_error_gen_data);

   histLinCorFit(forward_width,
                 forward_width_error,
                 FE_MC_cor_graphs,
                 widths_cor_FE,
                 widths_cor_FE_error);
   histLinCorFit(forward_width_data,
                 forward_width_error_data,
                 FE_data_cor_graphs,
                 widths_cor_FE_data,
                 widths_cor_FE_error_data);
   histLinCorFit(forward_gen_width,
                 forward_gen_width_error,
                 FE_gen_cor_graphs,
                 widths_cor_gen_FE,
                 widths_cor_gen_FE_error);
   histLinCorFit(forward_gen_width_data,
                 forward_gen_width_error_data,
                 FE_gen_data_cor_graphs,
                 widths_cor_gen_FE_data,
                 widths_cor_gen_FE_error_data);

   //////////////////////////////////////////////////////////////////////////////////////////
   //    End of correlated fit                                                             //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I make histograms ratio of widths(alpha=0.15)                                     //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< TH1F* > widths_015_ratios_all, widths_015_ratios_FE;

   widths_015_ratios( "widths_015_ratios", widths_015_ratios_all, asymmetries_width_data, asymmetries_width_error_data, asymmetries_width, asymmetries_width_error, asymmetries_width_data_pt );
   widths_015_ratios( "widths_015_FE_ratios", widths_015_ratios_FE, forward_width_data, forward_width_error_data, forward_width, forward_width_error, forward_width_data_pt );

   //////////////////////////////////////////////////////////////////////////////////////////
   //    Finished histograms for method check (for Mikkos check)                           //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I correct the forward widths for reference region resolution                      //
   //    before doinfg the PLI correction                                                  //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > widths_refCorrected_FE             , widths_refCorrected_FE_error             ,
                                        widths_refCorrected_gen_FE         , widths_refCorrected_gen_FE_error         ,
                                        widths_refCorrected_FE_data        , widths_refCorrected_FE_error_data        ,
                                        widths_refCorrected_gen_FE_data    , widths_refCorrected_gen_FE_error_data    ,
                                        widths_refCorrected_cor_FE         , widths_refCorrected_cor_FE_error         ,
                                        widths_refCorrected_cor_gen_FE     , widths_refCorrected_cor_gen_FE_error     ,
                                        widths_refCorrected_cor_FE_data    , widths_refCorrected_cor_FE_error_data    ,
                                        widths_refCorrected_cor_gen_FE_data, widths_refCorrected_cor_gen_FE_error_data;

   correctForRef( "FE_refCor"                , widths_refCorrected_FE             , widths_refCorrected_FE_error             , widths_FE             , widths_FE_error             , forward_width_pt  );
   correctForRef( "FE_gen_refCor"            , widths_refCorrected_gen_FE         , widths_refCorrected_gen_FE_error         , widths_gen_FE         , widths_gen_FE_error         , forward_gen_width_pt  );
   correctForRef( "FE_data_refCor"           , widths_refCorrected_FE_data        , widths_refCorrected_FE_error_data        , widths_FE_data        , widths_FE_error_data        , forward_width_data_pt  );
   if( !real_data ){
      correctForRef( "FE_gen_data_refCor"       , widths_refCorrected_gen_FE_data    , widths_refCorrected_gen_FE_error_data    , widths_gen_FE_data    , widths_gen_FE_error_data    , forward_gen_width_data_pt  );
   }

   correctForRef( "FE_correl_refCor"         , widths_refCorrected_cor_FE         , widths_refCorrected_cor_FE_error         , widths_cor_FE         , widths_cor_FE_error         , forward_width_pt  );
   correctForRef( "FE_correl_gen_refCor"     , widths_refCorrected_cor_gen_FE     , widths_refCorrected_cor_gen_FE_error     , widths_cor_gen_FE     , widths_cor_gen_FE_error     , forward_gen_width_pt  );
   correctForRef( "FE_correl_data_refCor"    , widths_refCorrected_cor_FE_data    , widths_refCorrected_cor_FE_error_data    , widths_cor_FE_data    , widths_cor_FE_error_data    , forward_width_data_pt  );
   if( !real_data ){
      correctForRef( "FE_correl_gen_data_refCor", widths_refCorrected_cor_gen_FE_data, widths_refCorrected_cor_gen_FE_error_data, widths_cor_gen_FE_data, widths_cor_gen_FE_error_data, forward_gen_width_data_pt  );
   }

   //////////////////////////////////////////////////////////////////////////////////////////
   //    Ref region corrected                                                              //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    Now I correct for PLI                                                             //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > widths_refCorrected_PLIcor_FE         , widths_refCorrected_PLIcor_FE_error         ,
                                        widths_refCorrected_PLIcor_FE_data    , widths_refCorrected_PLIcor_FE_error_data    ,
                                        widths_refCorrected_PLIcor_cor_FE     , widths_refCorrected_PLIcor_cor_FE_error     ,
                                        widths_refCorrected_PLIcor_cor_FE_data, widths_refCorrected_PLIcor_cor_FE_error_data;

   correctJERwithPLI( widths_refCorrected_PLIcor_FE , widths_refCorrected_PLIcor_FE_error     , widths_refCorrected_FE     , widths_refCorrected_FE_error     , widths_refCorrected_gen_FE     , widths_refCorrected_gen_FE_error);
   if( real_data ){
      correctJERwithPLI( widths_refCorrected_PLIcor_FE_data, widths_refCorrected_PLIcor_FE_error_data, widths_refCorrected_FE_data, widths_refCorrected_FE_error_data, widths_refCorrected_gen_FE     , widths_refCorrected_gen_FE_error);
   } else {
      correctJERwithPLI( widths_refCorrected_PLIcor_FE_data, widths_refCorrected_PLIcor_FE_error_data, widths_refCorrected_FE_data, widths_refCorrected_FE_error_data, widths_refCorrected_gen_FE_data, widths_refCorrected_gen_FE_error_data);
   }

   correctJERwithPLI( widths_refCorrected_PLIcor_cor_FE , widths_refCorrected_PLIcor_cor_FE_error     , widths_refCorrected_cor_FE     , widths_refCorrected_cor_FE_error     , widths_refCorrected_cor_gen_FE     , widths_refCorrected_cor_gen_FE_error);
   if( real_data ){
      correctJERwithPLI( widths_refCorrected_PLIcor_cor_FE_data, widths_refCorrected_PLIcor_cor_FE_error_data, widths_refCorrected_cor_FE_data, widths_refCorrected_cor_FE_error_data, widths_refCorrected_cor_gen_FE     , widths_refCorrected_cor_gen_FE_error);
   } else {
      correctJERwithPLI( widths_refCorrected_PLIcor_cor_FE_data, widths_refCorrected_PLIcor_cor_FE_error_data, widths_refCorrected_cor_FE_data, widths_refCorrected_cor_FE_error_data, widths_refCorrected_cor_gen_FE_data, widths_refCorrected_cor_gen_FE_error_data);
   }

   //////////////////////////////////////////////////////////////////////////////////////////
   //    above: FE resolutions full corrected, first ref then PLI                          //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I correct ST widths for PLI                                                       //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > JER_MC, JER_data;
   std::vector< std::vector< double > > JER_MC_error, JER_data_error;
   correctJERwithPLI( JER_MC, JER_MC_error, widths_all, widths_all_error, widths_gen_all, widths_gen_all_error);
   if( real_data ){
       // correct data with PLI from MC
       correctJERwithPLI( JER_data, JER_data_error, widths_all_data, widths_all_error_data, widths_gen_all, widths_gen_all_error);
   } else {
       // correct 'data' with own PLI
       correctJERwithPLI( JER_data, JER_data_error, widths_all_data, widths_all_error_data, widths_gen_all_data, widths_gen_all_error_data);
   }

   //////////////////////////////////////////////////////////////////////////////////////////
   //    I correct FE widths for PLI (before Ref, alternatively to what is above           //
   //    this is done in case we want to check which is the right one                      //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > JER_uncor_FE_MC, JER_uncor_FE_data;
   std::vector< std::vector< double > > JER_uncor_FE_MC_error, JER_uncor_FE_data_error;

   correctJERwithPLI( JER_uncor_FE_MC, JER_uncor_FE_MC_error, widths_FE, widths_FE_error, widths_gen_FE, widths_gen_FE_error);
   if( real_data ){
       // correct data with PLI from MC
   correctJERwithPLI( JER_uncor_FE_data, JER_uncor_FE_data_error, widths_FE_data, widths_FE_error_data, widths_gen_FE, widths_gen_FE_error);
   } else {
       // correct 'data' with own PLI
   correctJERwithPLI( JER_uncor_FE_data, JER_uncor_FE_data_error, widths_FE_data, widths_FE_error_data, widths_gen_FE_data, widths_gen_FE_error_data);
   }



   //////////////////////////////////////////////////////////////////////////////////////////
   //    PLI corrected using b parameters                                                  //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    Same correction but for correlated fit results                                    //
   //////////////////////////////////////////////////////////////////////////////////////////

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



   //////////////////////////////////////////////////////////////////////////////////////////
   //      I do the same for widths at alpha = 0.15                                        //
   //////////////////////////////////////////////////////////////////////////////////////////

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

   //////////////////////////////////////////////////////////////////////////////////////////
   //      I corrected alpha = 0.15 widhs for PLI correct way                              //
   //////////////////////////////////////////////////////////////////////////////////////////
   //      I correct forward widths for Ref region.                                        //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > JER_FE_MC, JER_FE_data;
   std::vector< std::vector< double > > JER_FE_MC_error, JER_FE_data_error;

   correctForRef( "mccorrected", JER_FE_MC, JER_FE_MC_error, JER_uncor_FE_MC, JER_uncor_FE_MC_error, forward_width_pt  );
   correctForRef( "datacorrect", JER_FE_data, JER_FE_data_error, JER_uncor_FE_data, JER_uncor_FE_data_error, forward_width_data_pt  );

   //////////////////////////////////////////////////////////////////////////////////////////
   //      forward widths corrected for Ref widths!                                        //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    same correction for correlated fit results                                        //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > JER_cor_FE_MC, JER_cor_FE_data;
   std::vector< std::vector< double > > JER_cor_FE_MC_error, JER_cor_FE_data_error;

   correctForRef( "mc_cor_corrected", JER_cor_FE_MC,   JER_cor_FE_MC_error,   JER_cor_uncor_FE_MC,   JER_cor_uncor_FE_MC_error,   forward_width_pt  );
//   correctForRef( "data_cor_correct", JER_cor_FE_data, JER_cor_FE_data_error, JER_cor_uncor_FE_data, JER_uncor_FE_data_error  , forward_width_data_pt  );
   correctForRef( "data_cor_correct", JER_cor_FE_data, JER_cor_FE_data_error, JER_cor_uncor_FE_data, JER_cor_uncor_FE_data_error, forward_width_data_pt  );
//   correctForRef( "datacorrect"     , JER_FE_data    , JER_FE_data_error    , JER_uncor_FE_data    , JER_uncor_FE_data_error    , forward_width_data_pt  );

   //////////////////////////////////////////////////////////////////////////////////////////
   //      ref region corrected for correlated fit                                         //
   //////////////////////////////////////////////////////////////////////////////////////////
   //      and again, Ref region for widths at alpha = 0.15                                //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > JER015_FE_MC, JER015_FE_data;
   std::vector< std::vector< double > > JER015_FE_MC_error, JER015_FE_data_error;

   correctForRef( "mccorrected015", JER015_FE_MC,   JER015_FE_MC_error,   JER015_uncor_FE_MC,   JER015_uncor_FE_MC_error,   forward_width_pt  );
   correctForRef( "datacorrect015", JER015_FE_data, JER015_FE_data_error, JER015_uncor_FE_data, JER015_uncor_FE_data_error, forward_width_data_pt  );

   //////////////////////////////////////////////////////////////////////////////////////////
   //    Ref reg corrected for widths at alpha = 0.15                                      //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I make make vectors with ratios of widths                                         //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< std::vector< double > > scales_classic, scales_fe, scales_fe_control;
   std::vector< std::vector< double > > scales_classic_error, scales_fe_error, scales_fe_control_error;

   makeScales( scales_classic,    scales_classic_error,    JER_data,          JER_data_error,          JER_MC,          JER_MC_error );

   makeScales( scales_fe,         scales_fe_error,         widths_refCorrected_PLIcor_FE_data,       widths_refCorrected_PLIcor_FE_error_data,       widths_refCorrected_PLIcor_FE,       widths_refCorrected_PLIcor_FE_error );
//   makeScales( scales_fe,         scales_fe_error,         JER_FE_data,       JER_FE_data_error,       JER_FE_MC,       JER_FE_MC_error );

   makeScales( scales_fe_control, scales_fe_control_error, JER_uncor_FE_data, JER_uncor_FE_data_error, JER_uncor_FE_MC, JER_uncor_FE_MC_error );   // uncorrected for reference region width, as a cross check. (i think)

// same thing for correlated fit results

   std::vector< std::vector< double > > scales_cor_classic, scales_cor_fe, scales_cor_fe_control;
   std::vector< std::vector< double > > scales_cor_classic_error, scales_cor_fe_error, scales_cor_fe_control_error;

   makeScales( scales_cor_classic,    scales_cor_classic_error,    JER_cor_data,          JER_cor_data_error,          JER_cor_MC,          JER_cor_MC_error );

   makeScales( scales_cor_fe,         scales_cor_fe_error,         widths_refCorrected_PLIcor_cor_FE_data,       widths_refCorrected_PLIcor_cor_FE_error_data,       widths_refCorrected_PLIcor_cor_FE,       widths_refCorrected_PLIcor_cor_FE_error );
//   makeScales( scales_cor_fe,         scales_cor_fe_error,         JER_cor_FE_data,       JER_cor_FE_data_error,       JER_cor_FE_MC,       JER_cor_FE_MC_error );

   makeScales( scales_cor_fe_control, scales_cor_fe_control_error, JER_cor_uncor_FE_data, JER_cor_uncor_FE_data_error, JER_cor_uncor_FE_MC, JER_cor_uncor_FE_MC_error );   // uncorrected for reference region width, as a cross check. (i think)

// and same thing for using-b-parameter calculations

   std::vector< std::vector< double > > scales015_classic, scales015_fe;
   std::vector< std::vector< double > > scales015_classic_error, scales015_fe_error;

   makeScales( scales015_classic, scales015_classic_error, JER015_data,    JER015_data_error,    JER015_MC,    JER015_MC_error );
   makeScales( scales015_fe,      scales015_fe_error,      JER015_FE_data, JER015_FE_data_error, JER015_FE_MC, JER015_FE_MC_error );

   //////////////////////////////////////////////////////////////////////////////////////////
   //    I have vectors with ratios of widths                                              //
   //////////////////////////////////////////////////////////////////////////////////////////
   //    I make plots with Res from dijet and MCTruth, a crosscheck                        //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< TH1F* > JER_MC_Truth, JER_forward_MC_Truth;

   fill_mctruth_hist( "MC_Truth"    , JER_MC_Truth        , mcTruth_res,         mcTruth_res_error,         asymmetries_width_pt, 0.2);
   fill_mctruth_hist( "MC_Truth_Fwd", JER_forward_MC_Truth, forward_mcTruth_res, forward_mcTruth_res_error, forward_width_pt, 0.2);

   //////////////////////////////////////////////////////////////////////////////////////////
   //      I make histograms with  JERs and scale factors                                  //
   //////////////////////////////////////////////////////////////////////////////////////////

   std::vector< TH1F* > JER_MC_hist, JER_data_hist, JER_scale_hist, JER015_MC_hist;
   std::vector< TH1F* > JER_FE_MC_hist, JER_FE_data_hist, JER_FE_scale_hist, JER015_FE_MC_hist;
   std::vector< TH1F* > JER_FE_MC_hist_control, JER_FE_data_hist_control, JER_FE_scale_hist_control;
std::cout << "here1" << std::endl;
   fill_hist( "MC_JER_classic"  , JER_MC_hist   , JER_MC        , JER_MC_error        , asymmetries_width_pt     , 0.2, 0);
std::cout << "here2" << std::endl;
   fill_hist( "data_JER_classtc", JER_data_hist , JER_data      , JER_data_error      , asymmetries_width_data_pt, 0.2, 0);
std::cout << "here3" << std::endl;
   fill_hist( "SF_classic"      , JER_scale_hist, scales_classic, scales_classic_error, asymmetries_width_data_pt, 2., 0);
std::cout << "here4" << std::endl;

   fill_hist( "MC_JER_FE"    ,   JER_FE_MC_hist    ,    widths_refCorrected_PLIcor_FE    ,       widths_refCorrected_PLIcor_FE_error    ,   forward_width_pt, 0.2, 1);
std::cout << "here5" << std::endl;
//   fill_hist( "MC_JER_FE"    ,   JER_FE_MC_hist    ,    JER_FE_MC, JER_FE_MC_error, forward_width_pt, 0.2);

   fill_hist( "MC_JER_FE015", JER015_FE_MC_hist, JER015_FE_MC,   JER015_FE_MC_error, forward_width_pt, 0.2, 1);
std::cout << "here6" << std::endl;
   fill_hist( "MC_JER_015", JER015_MC_hist, JER015_MC,   JER015_MC_error, asymmetries_width_pt, 0.2, 0);
std::cout << "here7" << std::endl;

   fill_hist( "data_JER_FE", JER_FE_data_hist, JER_FE_data, JER_FE_data_error, forward_width_data_pt, 0.2, 1);
std::cout << "here8" << std::endl;
   fill_hist( "SF_FE", JER_FE_scale_hist, scales_fe, scales_fe_error, forward_width_data_pt, 2.0 , 1);
std::cout << "here9" << std::endl;

   fill_hist( "MC_JER_FE_control", JER_FE_MC_hist_control, JER_uncor_FE_MC, JER_uncor_FE_MC_error, forward_width_pt, 0.2);
std::cout << "here10" << std::endl;
   fill_hist( "data_JER_FE_control", JER_FE_data_hist_control, JER_uncor_FE_data, JER_uncor_FE_data_error, forward_width_data_pt, 0.2);
   fill_hist( "SF_FE_control", JER_FE_scale_hist_control, scales_fe_control, scales_fe_control_error, forward_width_data_pt, 2.0);


   std::vector< TH1F* > JER015_scale_hist, JER015_FE_scale_hist;

   fill_hist( "SF_classic015", JER015_scale_hist, scales015_classic, scales015_classic_error, asymmetries_width_data_pt, 2.);
   fill_hist( "SF_FE015", JER015_FE_scale_hist, scales015_fe, scales015_fe_error, forward_width_data_pt, 2.0, 1);


   std::vector< TH1F* > JER_cor_MC_hist,            JER_cor_data_hist,            JER_cor_scale_hist;
   std::vector< TH1F* > JER_cor_FE_MC_hist,         JER_cor_FE_data_hist,         JER_cor_FE_scale_hist;
   std::vector< TH1F* > JER_cor_FE_MC_hist_control, JER_cor_FE_data_hist_control, JER_cor_FE_scale_hist_control;

   fill_hist( "MC_cor_JER_classic",   JER_cor_MC_hist,    JER_cor_MC,         JER_cor_MC_error,         asymmetries_width_pt, 0.2);
   fill_hist( "data_cor_JER_classtc", JER_cor_data_hist,  JER_cor_data,       JER_cor_data_error,       asymmetries_width_data_pt, 0.2);
   fill_hist( "SF_cor_classic",       JER_cor_scale_hist, scales_cor_classic, scales_cor_classic_error, asymmetries_width_data_pt, 2.);

   std::cout << "new and old vector size" << widths_refCorrected_PLIcor_cor_FE.size() << ", " << JER_cor_FE_MC.size() << std::endl;

   fill_hist( "MC_cor_JER_FE",   JER_cor_FE_MC_hist,    widths_refCorrected_PLIcor_cor_FE,       widths_refCorrected_PLIcor_cor_FE_error,   forward_width_pt, 0.2, 1);
//   fill_hist( "MC_cor_JER_FE",   JER_cor_FE_MC_hist,    JER_cor_FE_MC,   JER_cor_FE_MC_error,   forward_width_pt, 0.2);

   fill_hist( "data_cor_JER_FE", JER_cor_FE_data_hist,  JER_cor_FE_data, JER_cor_FE_data_error, forward_width_data_pt, 0.2, 1);
   fill_hist( "SF_cor_FE",       JER_cor_FE_scale_hist, scales_cor_fe,   scales_cor_fe_error,   forward_width_data_pt, 2.0, 1);

   fill_hist( "MC_cor_JER_FE_control",   JER_cor_FE_MC_hist_control,    JER_cor_uncor_FE_MC,   JER_cor_uncor_FE_MC_error,   forward_width_pt, 0.2);
   fill_hist( "data_cor_JER_FE_control", JER_cor_FE_data_hist_control,  JER_cor_uncor_FE_data, JER_cor_uncor_FE_data_error, forward_width_data_pt, 0.2);
   fill_hist( "SF_cor_FE_control",       JER_cor_FE_scale_hist_control, scales_cor_fe_control, scales_cor_fe_control_error, forward_width_data_pt, 2.0);

   //////////////////////////////////////////////////////////////////////////////////////////
   //      I divide the histograms                                                         //
   //////////////////////////////////////////////////////////////////////////////////////////

//   std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all, asymmetries_gen_all, asymmetries_all_data;
//   std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all_pt, asymmetries_gen_all_pt, asymmetries_all_data_pt;
//
//   std::vector< std::vector< std::vector< TH1F* > > > forward_hist, forward_gen_hist, forward_hist_data;
//   std::vector< std::vector< std::vector< TH1F* > > > forward_hist_pt, forward_gen_hist_pt, forward_hist_data_pt;



   //////////////////////////////////////////////////////////////////////////////////////////
   //    resolution cross check with mcTruth                                               //
   //////////////////////////////////////////////////////////////////////////////////////////

   int W = 800;
   int H = 600;

   int H_ref = 600;
   int W_ref = 800;

   // references for T, B, L, R
   float T = 0.08*H_ref;
   float B = 0.12*H_ref;
   float L = 0.12*W_ref;
   float R = 0.04*W_ref;

   int iPeriod = 4;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
   extraText  = "       Preliminary";  // default extra text is "Preliminary"
   lumi_13TeV = lumi; //"2.1 fb^{-1}";
   lumi_sqrtS = sqrts;       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
   //lumi_13TeV = "2.1 fb^{-1}";
   //lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
//   JER_forward_MC_Truth;
//   JER_FE_MC_hist
//   JER_cor_FE_MC_hist


    std::ifstream myfile;
   // File stream: MC Truth resolutions:
    myfile.open("/nfs/dust/cms/user/niedziem/resolutions_input/Fall17_17Nov2017_V4_MC_PtResolution_ak4pfchsl1l2l3.txt");
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
      tempTF1->SetLineColor(kCyan);  // sets the color to red
      tempTF1->SetLineStyle(1);
      MCTruth_Chad.push_back(tempTF1);
   }

   gStyle -> SetOptFit(0);
   gStyle -> SetOptStat(0);

//   std::cout << "will make plots with mctruth and ratio etc " << std::endl;
   TFile fMCTruth("output/MCTruth.root","RECREATE");
   for( unsigned int m = 0; m < JER_MC_Truth.size(); m++ ) //
   {

         TString canvName = JER_MC_Truth.at(m) -> GetTitle();

//         TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
         TCanvas *canv = new TCanvas("c", "canvas", 800, 800);
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

//       Upper plot will be in pad1
         TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
         pad1->SetBottomMargin(0); // Upper and lower plot are joined
         pad1->SetGridx();         // Vertical grid
         pad1->Draw();             // Draw the upper pad: pad1
         pad1->cd();               // pad1 becomes the current pad

         JER_cor_MC_hist.at(m) -> SetTitle(""); // Remove the ratio title
         JER_cor_MC_hist.at(m) -> GetYaxis() -> SetTitleSize(0.070);
         JER_cor_MC_hist.at(m) -> GetYaxis() -> SetTitleOffset(0.4);
         JER_MC_Truth   .at(m) -> SetMarkerColor(2);
//         JER_MC_hist    .at(m) -> SetMarkerColor(4);
         JER_cor_MC_hist.at(m) -> SetMarkerColor(6);
         JER015_MC_hist .at(m) -> SetMarkerColor(8);

         JER_MC_Truth   .at(m) -> SetMarkerStyle(20);
//         JER_MC_hist    .at(m) -> SetMarkerStyle(20);
         JER_cor_MC_hist.at(m) -> SetMarkerStyle(20);
         JER015_MC_hist .at(m) -> SetMarkerStyle(20);

         JER_cor_MC_hist.at(m) -> SetStats(0);
         JER_cor_MC_hist.at(m) -> Draw();
//         canv->Update();
         for( unsigned int i = 0; i < 7; i++ ){
            MCTruth_Chad.at( 7*m + i ) -> SetMarkerColor(kRed+i);
            MCTruth_Chad.at( 7*m + i ) -> Draw("same");
//            canv->Update();
         }
         JER_cor_MC_hist.at(m) -> Draw("same");
         canv->Update();
         JER015_MC_hist.at(m) -> Draw("same");
         canv->Update();

//
// Do not draw the Y axis label on the upper plot and redraw a small
// axis instead, in order to avoid the first label (0) to be clipped.
         JER_cor_MC_hist.at(m) -> GetYaxis() -> SetLabelSize(0.);
         TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
         axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
         axis->SetLabelSize(15);
         axis->SetTitleSize(30);
         axis->Draw();
         canv->Update();

// lower plot will be in pad
         canv->cd();          // Go back to the main canvas before defining pad2
         TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
         pad2->SetTopMargin(0);
         pad2->SetBottomMargin(0.2);
         pad2->SetGridx(); // vertical grid
         pad2->Draw();
         pad2->cd();       // pad2 becomes the current pad

//         Define the ratio plot
         TH1F *h3 = (TH1F*)JER_cor_MC_hist.at(m)->Clone("h3");
         h3->SetLineColor(kBlack);
         h3->SetMinimum(0.0);  // Define Y ..
         h3->SetMaximum(2.0); // .. range
         h3->GetYaxis()->SetRangeUser(0.5, 1.5);
         h3->Sumw2();
         h3->SetStats(0);      // No statistics on lower plot
         h3->Divide(MCTruth_Chad.at( 7*m ));
         h3->SetMarkerStyle(22);
         h3->SetMarkerSize(1.4);
         h3->SetMarkerColor(9);
         h3->SetTitle(""); // Remove the ratio title
         h3->Draw("ep");       // Draw the ratio plot

         TH1F *h4 = (TH1F*)JER_cor_MC_hist.at(m)->Clone("h4");
         h4->SetLineColor(kBlack);
         h4->GetYaxis()->SetRangeUser(0.5, 1.5);
         h4->SetMinimum(0.0);  // Define Y ..
         h4->SetMaximum(2.0); // .. range
         h4->Sumw2();
         h4->SetStats(0);      // No statistics on lower plot
         h4->Divide(MCTruth_Chad.at( 7*m + 6 ));
         h4->SetMarkerStyle(29);
         h4->SetMarkerSize(1.8);
         h4->SetMarkerColor(2);
         h4->SetTitle(""); // Remove the ratio title
         h4->Draw("same");       // Draw the ratio plot

         TLine *line = new TLine(0,1,1000,1);
         line->SetLineColor(kBlack);
         line->Draw("same");

         // Y axis ratio plot settings
         h3->GetYaxis()->SetTitle("ratio ");
         h3->GetYaxis()->SetNdivisions(505);
         h3->GetYaxis()->SetRangeUser(0.5, 1.5);
         h3->GetYaxis()->SetTitleSize(40);
         h3->GetYaxis()->SetTitleFont(43);
         h3->GetYaxis()->SetTitleOffset(0.6);
         h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
         h3->GetYaxis()->SetLabelSize(15);

         // X axis ratio plot settings
         h3->GetXaxis()->SetTitleSize(20);
         h3->GetXaxis()->SetTitleFont(43);
         h3->GetXaxis()->SetTitleOffset(4.);
         h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
         h3->GetXaxis()->SetLabelSize(15);

         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
//         canv->RedrawAxis();
//         canv->GetFrame()->Draw();
//         canv->Draw();

         TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
//         leg->AddEntry(JER_MC_Truth   .at(m),"MC Truth"        ,"lep");
//         leg->AddEntry(JER_MC_hist    .at(m),"Normal DiJet"    ,"lep");
         leg->AddEntry(JER_cor_MC_hist.at(m),"Correlated DiJet","lep");
         leg->AddEntry(MCTruth_Chad.at(m),   "MCTruth from Chad","l");
         leg->AddEntry(JER015_MC_hist.at(m),"015 DiJet","lep");
         leg->Draw();
         TLegend *leg2 = new TLegend(0.7,0.1,0.9,0.3);
         leg2->AddEntry(h3,"#sigma_{DiJet}/#sigma(#rho_{min})","lep");
         leg2->AddEntry(h4,"#sigma_{DiJet}/#sigma(#rho_{max})","lep");
         leg2->Draw();

         canv -> Write();

         TString outputname = JER_MC_Truth.at(m) -> GetTitle();

         canv -> Print("pdfy/MCTruth/"+outputname+".pdf","pdf");
         canv -> Print("pdfy/MCTruth/"+outputname+".png","png");
   }

   for( unsigned int m = 0; m < (JER_forward_MC_Truth.size()-1); m++ ) //
   {


         TString canvName = JER_forward_MC_Truth.at(m+1) -> GetTitle();

         TCanvas *canv = new TCanvas("c", "canvas", 800, 800);
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

//       Upper plot will be in pad1
         TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
         pad1->SetBottomMargin(0); // Upper and lower plot are joined
         pad1->SetGridx();         // Vertical grid
         pad1->Draw();             // Draw the upper pad: pad1
         pad1->cd();               // pad1 becomes the current pad

         JER_cor_FE_MC_hist.at(m) -> SetTitle(""); // Remove the ratio title
         JER_cor_FE_MC_hist.at(m) -> GetYaxis() -> SetTitleOffset(0.5);
         JER_cor_FE_MC_hist.at(m) -> GetYaxis() -> SetTitleSize(0.060);

//         JER_forward_MC_Truth.at(m+1) -> SetMarkerColor(2);
         JER_FE_MC_hist      .at(m)   -> SetMarkerColor(4);
         JER_cor_FE_MC_hist  .at(m)   -> SetMarkerColor(6);
         JER015_FE_MC_hist   .at(m)   -> SetMarkerColor(8);

//         JER_forward_MC_Truth.at(m+1) -> SetMarkerStyle(20);
         JER_FE_MC_hist      .at(m)   -> SetMarkerStyle(20);
         JER_cor_FE_MC_hist  .at(m)   -> SetMarkerStyle(20);
         JER015_FE_MC_hist   .at(m)   -> SetMarkerStyle(20);

         JER_cor_FE_MC_hist.at(m) -> SetStats(0);
         JER_cor_FE_MC_hist.at(m) -> Draw();
         canv->Update();
         for( unsigned int i = 0; i < 7; i++ ){
//            MCTruth_Chad.at( 7*m + i + 21 ) -> SetMarkerColor(kCyan);
            MCTruth_Chad.at( 7*m + i + 21 ) -> Draw("same");
            canv->Update();
         }
//         JER_forward_MC_Truth.at(m+1) -> Draw("same");
//         canv->Update();
//         JER_FE_MC_hist.at(m) -> Draw("same");
//         canv->Update();
         JER_cor_FE_MC_hist.at(m) -> Draw("same");
         canv->Update();
         JER015_FE_MC_hist.at(m) -> Draw("same");
         canv->Update();

//         if( m == 2 ){
//            MCTruth_Chad.at(17) -> Draw("same");
//            canv->Update();
//         }
//         if( m == 3 ){
//            MCTruth_Chad.at(19) -> Draw("same");
//            canv->Update();
//         }

// Do not draw the Y axis label on the upper plot and redraw a small
// axis instead, in order to avoid the first label (0) to be clipped.
         JER_cor_FE_MC_hist.at(m) -> GetYaxis() -> SetLabelSize(0.);
         TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
         axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
         axis->SetLabelSize(15);
         axis->SetTitleSize(30);
         axis->Draw();

// lower plot will be in pad
         canv->cd();          // Go back to the main canvas before defining pad2
         TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
         pad2->SetTopMargin(0);
         pad2->SetBottomMargin(0.2);
         pad2->SetGridx(); // vertical grid
         pad2->Draw();
         pad2->cd();       // pad2 becomes the current pad

//         Define the ratio plot
         TH1F *h3 = (TH1F*)JER_cor_FE_MC_hist.at(m)->Clone("JER_cor_FE_MC_hist.at(m)");
         h3->SetLineColor(kBlack);
         h3->GetYaxis()->SetRangeUser(0.5, 1.5);
         h3->SetMinimum(0.0);  // Define Y ..
         h3->SetMaximum(2.0); // .. range
         h3->Sumw2();
         h3->SetStats(0);      // No statistics on lower plot
         h3->Divide(MCTruth_Chad.at( 7*m + 21 ));
         h3->SetMarkerStyle(22);
         h3->SetMarkerSize(1.4);
         h3->SetMarkerColor(9);
         h3->Draw("ep");       // Draw the ratio plot

         TH1F *h4 = (TH1F*)JER_cor_FE_MC_hist.at(m)->Clone("JER_cor_FE_MC_hist.at(m)");
         h4->SetLineColor(kBlack);
         h4->GetYaxis()->SetRangeUser(0.5, 1.5);
         h4->SetMinimum(0.0);  // Define Y ..
         h4->SetMaximum(2.0); // .. range
         h4->Sumw2();
         h4->SetStats(0);      // No statistics on lower plot
         h4->Divide(MCTruth_Chad.at( 7*m + 27 ));
         h4->SetMarkerStyle(29);
         h4->SetMarkerSize(1.8);
         h4->SetMarkerColor(2);
         h4->Draw("same");       // Draw the ratio plot
         h3->SetTitle(""); // Remove the ratio title
         h4->SetTitle(""); // Remove the ratio title

         TLine *line = new TLine(0,1,1000,1);
         line->SetLineColor(kBlack);
         line->Draw("same");

         // Y axis ratio plot settings
         h3->GetYaxis()->SetTitle("ratio ");
         h3->GetYaxis()->SetNdivisions(505);
         h3->GetYaxis()->SetRangeUser(0.5, 1.5);
         h3->GetYaxis()->SetTitleSize(40);
         h3->GetYaxis()->SetTitleFont(43);
         h3->GetYaxis()->SetTitleOffset(0.6);
         h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
         h3->GetYaxis()->SetLabelSize(15);

         // X axis ratio plot settings
         h3->GetXaxis()->SetTitleSize(20);
         h4->GetYaxis()->SetRangeUser(0.5, 1.5);
         h3->GetXaxis()->SetTitleFont(43);
         h3->GetXaxis()->SetTitleOffset(4.);
         h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
         h3->GetXaxis()->SetLabelSize(15);
         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
//         leg->AddEntry(JER_forward_MC_Truth   .at(m+1),"MC Truth"        ,"lep");
//         leg->AddEntry(JER_FE_MC_hist    .at(m),"Normal DiJet"    ,"lep");
         leg->AddEntry(JER_cor_FE_MC_hist.at(m),"Correlated DiJet","lep");
         leg->AddEntry(JER015_FE_MC_hist.at(m),"015 DiJet","lep");
         leg->AddEntry(MCTruth_Chad.at(m+2),   "MCTruth from Chad","l");
         leg->Draw();
         TLegend *leg2 = new TLegend(0.7,0.1,0.9,0.3);
         leg2->AddEntry(h3,"#sigma_{DiJet}/#sigma(#rho_{min})","lep");
         leg2->AddEntry(h4,"#sigma_{DiJet}/#sigma(#rho_{max})","lep");
         leg2->Draw();

         canv -> Write();

         TString outputname = JER_forward_MC_Truth.at(m+1) -> GetTitle();

         canv -> Print("pdfy/MCTruth/"+outputname+".pdf","pdf");
         canv -> Print("pdfy/MCTruth/"+outputname+".png","png");

   }

   {

         TString canvName = JER_cor_FE_MC_hist_control.at(0) -> GetTitle();

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

         JER_forward_MC_Truth.at(0)   -> SetMarkerColor(2);
         JER_FE_MC_hist_control     .at(0)   -> SetMarkerColor(4);
         JER_cor_FE_MC_hist_control .at(0)   -> SetMarkerColor(6);

         JER_forward_MC_Truth.at(0)   -> SetMarkerStyle(20);
         JER_FE_MC_hist_control     .at(0)   -> SetMarkerStyle(20);
         JER_cor_FE_MC_hist_control .at(0)   -> SetMarkerStyle(20);

         JER_cor_FE_MC_hist_control.at(0) -> Draw();
         canv->Update();
         for( unsigned int i = 0; i < 14; i++ ){
            MCTruth_Chad.at( i ) -> Draw("same");
            MCTruth_Chad.at( i ) -> SetMarkerColor(kCyan);
            canv->Update();
         }
//         JER_forward_MC_Truth.at(0) -> Draw("same");
//         canv->Update();
//         JER_FE_MC_hist_control     .at(0) -> Draw("same");
//         canv->Update();
         JER_cor_FE_MC_hist_control .at(0) -> Draw("same");
         canv->Update();
         canv->Update();

         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
//         leg->AddEntry(JER_forward_MC_Truth.at(0),"MC Truth"        ,"lep");
//         leg->AddEntry(JER_FE_MC_hist_control     .at(0),"Normal DiJet"    ,"lep");
         leg->AddEntry(JER_cor_FE_MC_hist_control .at(0),"Correlated DiJet","lep");
         leg->AddEntry(MCTruth_Chad        .at(0),"MCTruth from Chad","l");
         leg->Draw();

         canv -> Write();

         canv -> Print("pdfy/MCTruth/FE_reference.pdf","pdf");
         canv -> Print("pdfy/MCTruth/FE_reference.png","png");
   }


   fMCTruth.Close();


//////////////////////////////////////////////////////////////////////////////////////////
//    I write histograms to root and graphic files                                      //
//////////////////////////////////////////////////////////////////////////////////////////


   TFile hprim("output/asymmetries.root","RECREATE");

   for( unsigned int m = 0; m < asymmetries_all.size(); m++ )
   {
      for( unsigned int p = 0; p < asymmetries_all.at(m).size(); p++ )
      {
         for( unsigned int r = 0; r < asymmetries_all.at(m).at(p).size(); r++ )
         {
            asymmetries_all.at(m).at(p).at(r) -> Write();
            asymmetries_gen_all.at(m).at(p).at(r) -> Write();
            asymmetries_all_data.at(m).at(p).at(r) -> Write();
         }
      }
   }

   for( unsigned int m = 0; m < forward_hist.size(); m++ )
   {
      for( unsigned int p = 0; p < forward_hist.at(m).size(); p++ )
      {
         for( unsigned int r = 0; r < forward_hist.at(m).at(p).size(); r++ )
         {
            forward_hist.at(m).at(p).at(r) -> Write();
            forward_gen_hist.at(m).at(p).at(r) -> Write();
            forward_hist_data.at(m).at(p).at(r) -> Write();
         }
      }
   }


   TFile hprimm("output/widths.root","RECREATE");

   for( unsigned int m = 0; m < widths_hist_FE.size(); m++ )
   {
      for( unsigned int p = 0; p < widths_hist_FE.at(m).size(); p++ )
      {
         widths_hist_FE.at(m).at(p) -> Write();
         widths_hist_FE_gen.at(m).at(p) -> Write();
         widths_hist_FE_data.at(m).at(p) -> Write();
      }
   }

   for( unsigned int m = 0; m < widths_hist_all.size(); m++ )
   {
      for( unsigned int p = 0; p < widths_hist_all.at(m).size(); p++ )
      {
         widths_hist_all.at(m).at(p) -> Write();
         widths_hist_all_gen.at(m).at(p) -> Write();
         widths_hist_all_data.at(m).at(p) -> Write();
      }
   }

   hprimm.Close();


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    correlation plots      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   std::vector<float> alpha;
   alpha.push_back(0.05); alpha.push_back(0.1);  alpha.push_back(0.15);
   alpha.push_back(0.20); alpha.push_back(0.25); alpha.push_back(0.3);



   for( unsigned int m = 0; m < MC_cor_graphs.size(); m++ ){
      for( unsigned int p = 0; p < MC_cor_graphs.at(m).size(); p++ ){
         // draw extrapolations data + mc
         TCanvas *c = new TCanvas("c","",600,600);
//         std::pair <float,float> minMaxPair = determineMinMax(&data_cor_graphs.at(m).at(p));
         c->DrawFrame(0,0,0.35,0.35,(";Threshold #alpha_{max};#sigma_{A}"));
         MC_cor_graphs.at(m).at(p)->SetMarkerStyle(20);
         MC_cor_graphs.at(m).at(p)->SetMarkerColor(kRed+1);
         MC_cor_graphs.at(m).at(p)->SetLineColor(kRed+1);
         MC_cor_graphs.at(m).at(p)->Draw("P");
         data_cor_graphs.at(m).at(p)->SetMarkerStyle(20);
         data_cor_graphs.at(m).at(p)->SetMarkerColor(kBlack);
         data_cor_graphs.at(m).at(p)->SetLineColor(kBlack);
         data_cor_graphs.at(m).at(p)->Draw("Psame");
         gen_cor_graphs.at(m).at(p)->SetMarkerStyle(20);
         gen_cor_graphs.at(m).at(p)->SetMarkerColor(kBlue);
         gen_cor_graphs.at(m).at(p)->SetLineColor(kBlue);
         gen_cor_graphs.at(m).at(p)->Draw("Psame");
         TF1* MCTemp = new TF1();
         TF1* DataTemp = new TF1();
         TF1* GenTemp = new TF1();
         MC_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineColor(kRed+1);
         MC_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineStyle(2);
         data_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineColor(kBlack);
         data_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineStyle(2);
         gen_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineColor(kBlue);
         gen_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineStyle(2);
         MCTemp=(TF1*) MC_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
         DataTemp=(TF1*) data_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
         GenTemp=(TF1*) gen_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
         double range = 0.05;
         if ( p == 0) range = 0.2;
         if ( p == 1) range = 0.15;
         if ( p == 2) range = 0.15;
         if ( p == 3) range = 0.1;
         MCTemp->SetRange(range,1);
         MCTemp->SetLineStyle(1);
         MCTemp->Draw("same");
         DataTemp->SetRange(range,1);
         DataTemp->SetLineStyle(1);
         DataTemp->Draw("same");
         GenTemp->SetRange(range,1);
         GenTemp->SetLineStyle(1);
         GenTemp->Draw("same");


         TPaveText *label2 = new TPaveText(0.0,0.27,0.15,0.35);
         label2->AddText("Anti-k_{T} (R=0.4) PFchs Jets");
         label2->AddText( Form("%0.1f #leq |#eta| #leq %0.1f, %3.0f #leq #bar{ p}_{T} [GeV] #leq %3.0f", eta_bins_edge[m], eta_bins_edge[m+1], p_bins_edge[p], p_bins_edge[p+1]) );
         label2->Draw("same");

         TLegend *leg1 = new TLegend(0.6,0.7,0.95,0.95);
         leg1->AddEntry(data_cor_graphs.at(m).at(p),"Extrapolation (data)","LP");
         leg1->AddEntry(MC_cor_graphs.at(m).at(p),"Extrapolation (MC)","LP");
         leg1->AddEntry(gen_cor_graphs.at(m).at(p),"Extrapolation (gen)","LP");

         leg1->Draw();
//         cmsPrel(-1, false , 8);

         TString name;
         name = Form("ClosureTest/Extrapol_Eta%i_pt%i.eps", m+1, p+1);
         name = Form("ClosureTest/Extrapol_Eta%i_pt%i.png", m+1, p+1);
         c->Print(name);
      }
   }

   for( unsigned int m = 0; m < FE_MC_cor_graphs.size(); m++ ){
      for( unsigned int p = 0; p < FE_MC_cor_graphs.at(m).size(); p++ ){
         // draw extrapolations data + mc
         TCanvas *c = new TCanvas("c","",600,600);
//         std::pair <float,float> minMaxPair = determineMinMax(&data_cor_graphs.at(m).at(p));
         c->DrawFrame(0,0,0.35,0.35,(";Threshold #alpha_{max};#sigma_{A}"));
         FE_MC_cor_graphs.at(m).at(p)->SetMarkerStyle(20);
         FE_MC_cor_graphs.at(m).at(p)->SetMarkerColor(kRed+1);
         FE_MC_cor_graphs.at(m).at(p)->SetLineColor(kRed+1);
         FE_MC_cor_graphs.at(m).at(p)->Draw("P");
         FE_data_cor_graphs.at(m).at(p)->SetMarkerStyle(20);
         FE_data_cor_graphs.at(m).at(p)->SetMarkerColor(kBlack);
         FE_data_cor_graphs.at(m).at(p)->SetLineColor(kBlack);
         FE_data_cor_graphs.at(m).at(p)->Draw("Psame");
         FE_gen_cor_graphs.at(m).at(p)->SetMarkerStyle(20);
         FE_gen_cor_graphs.at(m).at(p)->SetMarkerColor(kBlue);
         FE_gen_cor_graphs.at(m).at(p)->SetLineColor(kBlue);
         FE_gen_cor_graphs.at(m).at(p)->Draw("Psame");
         TF1* MCTemp = new TF1();
         TF1* DataTemp = new TF1();
         TF1* GenTemp = new TF1();
         FE_MC_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineColor(kRed+1);
         FE_MC_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineStyle(2);
         FE_data_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineColor(kBlack);
         FE_data_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineStyle(2);
         FE_gen_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineColor(kBlue);
         FE_gen_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->SetLineStyle(2);
         MCTemp=(TF1*) FE_MC_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
         DataTemp=(TF1*) FE_data_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
         GenTemp=(TF1*) FE_gen_cor_graphs.at(m).at(p)->GetFunction("lin_extrapol_mc")->Clone();
         double range = 0.05;
         if ( p == 0) range = 0.2;
         if ( p == 1) range = 0.15;
         if ( p == 2) range = 0.15;
         if ( p == 3) range = 0.1;
         MCTemp->SetRange(range,1);
         MCTemp->SetLineStyle(1);
         MCTemp->Draw("same");
         DataTemp->SetRange(range,1);
         DataTemp->SetLineStyle(1);
         DataTemp->Draw("same");
         GenTemp->SetRange(range,1);
         GenTemp->SetLineStyle(1);
         GenTemp->Draw("same");


         TPaveText *label2 = new TPaveText(0.0,0.27,0.15,0.35);
         label2->AddText("Anti-k_{T} (R=0.4) PFchs Jets");
         label2->AddText( Form("%0.1f #leq |#eta| #leq %0.1f, %3.0f #leq #bar{ p}_{T} [GeV] #leq %3.0f", eta_bins_edge[m], eta_bins_edge[m+1], p_bins_edge[p], p_bins_edge[p+1]) );
         label2->Draw("same");

         TLegend *leg1 = new TLegend(0.6,0.7,0.95,0.95);
         leg1->AddEntry(FE_data_cor_graphs.at(m).at(p),"Extrapolation (data)","LP");
         leg1->AddEntry(FE_MC_cor_graphs.at(m).at(p),"Extrapolation (MC)","LP");
         leg1->AddEntry(FE_gen_cor_graphs.at(m).at(p),"Extrapolation (gen)","LP");

         leg1->Draw();
//         cmsPrel(-1, false , 8);

         TString name;
         name = Form("ClosureTest/ExtrapolFw_Eta%i_pt%i.eps", m+1, p+1);
         name = Form("ClosureTest/ExtrapolFw_Eta%i_pt%i.png", m+1, p+1);
         c->Print(name);
      }
   }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    end of correlation plots      ///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


   TFile gprim("output/JERs.root","RECREATE");

   for( unsigned int m = 0; m < JER_MC_hist.size(); m++ )
   {
      JER_MC_hist.at(m) -> Write();
   }

   for( unsigned int m = 0; m < JER_data_hist.size(); m++ )
   {
      JER_data_hist.at(m) -> Write();
   }

   for( unsigned int m = 0; m < JER_FE_MC_hist.size(); m++ )
   {
      JER_FE_MC_hist.at(m) -> Write();
   }

   for( unsigned int m = 0; m < JER_FE_data_hist.size(); m++ )
   {
      JER_FE_data_hist.at(m) -> Write();
   }

   for( unsigned int m = 0; m < JER_FE_MC_hist_control.size(); m++ )
   {
      JER_FE_MC_hist_control.at(m) -> Write();
   }
   for( unsigned int m = 0; m < JER_FE_data_hist_control.size(); m++ )
   {
      JER_FE_data_hist_control.at(m) -> Write();
   }



   for( unsigned int m = 0; m < JER_cor_MC_hist.size(); m++ )
   {
      JER_cor_MC_hist.at(m) -> Write();
   }

   for( unsigned int m = 0; m < JER_cor_data_hist.size(); m++ )
   {
      JER_cor_data_hist.at(m) -> Write();
   }

   for( unsigned int m = 0; m < JER_cor_FE_MC_hist.size(); m++ )
   {
      JER_cor_FE_MC_hist.at(m) -> Write();
   }

   for( unsigned int m = 0; m < JER_cor_FE_data_hist.size(); m++ )
   {
      JER_cor_FE_data_hist.at(m) -> Write();
   }

   for( unsigned int m = 0; m < JER_cor_FE_MC_hist_control.size(); m++ )
   {
      JER_cor_FE_MC_hist_control.at(m) -> Write();
   }
   for( unsigned int m = 0; m < JER_cor_FE_data_hist_control.size(); m++ )
   {
      JER_cor_FE_data_hist_control.at(m) -> Write();
   }

   gprim.Close();


   TFile gbis("output/SFs.root","RECREATE");
   ofstream mytxtfile;
   mytxtfile.open ("output/scalefactors.txt");
   mytxtfile << "standard method\n";
   mytxtfile << "eta bin & scale factor & stat unc \n" << "{";

   for( unsigned int m = 0; m < JER_scale_hist.size(); m++ )
   {
      TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
      std::cout << "standard scales, eta bin " << m << std::endl;
      JER_scale_hist.at(m) -> Fit("constfit","");
      JER_scale_hist.at(m) -> Write();
      mytxtfile << "{" << constfit -> GetParameter( 0 ) << ", " << constfit -> GetParError( 0 ) << "}";
      if(m != JER_scale_hist.size()-1 ) mytxtfile << ",";
      delete constfit;
      std::cout << std::endl;
      std::cout << std::endl;
   }


   for( unsigned int m = 0; m < JER_FE_scale_hist_control.size(); m++ )
   {
      TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
      std::cout << "FE control scales, eta bin " << m << std::endl;
      JER_FE_scale_hist_control.at(m) -> Fit("constfit","");
      JER_FE_scale_hist_control.at(m) -> Write();
      delete constfit;
      std::cout << std::endl;
      std::cout << std::endl;
   }

   for( unsigned int m = 0; m < JER015_scale_hist.size(); m++ )
   {
      std::cout << "standard with 0.15 scales, eta bin " << m << std::endl;
      JER015_scale_hist.at(m) -> Fit("pol0","");
      JER015_scale_hist.at(m) -> Write();
      std::cout << std::endl;
      std::cout << std::endl;
   }

   for( unsigned int m = 0; m < JER015_FE_scale_hist.size(); m++ )
   {
      std::cout << "FE with 015 scales, eta bin " << m << std::endl;
      JER015_FE_scale_hist.at(m) -> Fit("pol0","");
      JER015_FE_scale_hist.at(m) -> Write();
      std::cout << std::endl;
      std::cout << std::endl;
   }

   mytxtfile << "}; \n forward extension \n";
   mytxtfile << "eta bin & scale factor & stat unc \n" << "{";

   for( unsigned int m = 0; m < JER_FE_scale_hist.size(); m++ )
   {
      TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
      std::cout << "FE scales, eta bin " << m << std::endl;
      JER_FE_scale_hist.at(m) -> Fit("constfit","");
      JER_FE_scale_hist.at(m) -> Write();
      mytxtfile << "{" << constfit -> GetParameter( 0 ) << ", " << constfit -> GetParError( 0 ) << "}";
      if(m != JER_FE_scale_hist.size()-1 ) mytxtfile << ",";
      std::cout << std::endl;
      std::cout << std::endl;
      delete constfit;
   }



   mytxtfile << "}; \n standard method, correlated fit\n";
   mytxtfile << "eta bin & scale factor & stat unc \n" << "{";

   for( unsigned int m = 0; m < JER_cor_scale_hist.size(); m++ )
   {
      TF1 * constfit = new TF1( "constfit", "pol0", 0, 1000 );
      std::cout << "standard scales, eta bin " << m << std::endl;
      JER_cor_scale_hist.at(m) -> Fit("constfit","");
      JER_cor_scale_hist.at(m) -> Write();
      mytxtfile << "{" << constfit -> GetParameter( 0 ) << ", " << constfit -> GetParError( 0 ) << "}";
      if(m != JER_cor_scale_hist.size()-1 ) mytxtfile << ",";
      delete constfit;
      std::cout << std::endl;
      std::cout << std::endl;
   }

   mytxtfile << "}; \n forward extension, correlated fit \n";
   mytxtfile << "eta bin & scale factor & stat unc \n" << "{";

   for( unsigned int m = 0; m < JER_cor_FE_scale_hist.size(); m++ )
   {
      TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
      std::cout << "FE scales, eta bin " << m << std::endl;
      JER_cor_FE_scale_hist.at(m) -> Fit("constfit","");
      JER_cor_FE_scale_hist.at(m) -> Write();
      mytxtfile << "{" << constfit -> GetParameter( 0 ) << ", " << constfit -> GetParError( 0 ) << "}";
      if(m != JER_cor_FE_scale_hist.size()-1 ) mytxtfile << ",";
      std::cout << std::endl;
      std::cout << std::endl;
      delete constfit;
   }


   mytxtfile << "};";
   mytxtfile.close();


   gbis.Close();

   setTDRStyle();


//   gStyle -> SetOptFit(1);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plots for forward extension with widths(alpha)                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   TFile h_merged("output/widths_merged.root","RECREATE");

   for( unsigned int m = 0; m < widths_hist_FE.size(); m++ )
   {
      for( unsigned int p = 0; p < widths_hist_FE.at(m).size(); p++ )
      {
         TString canvName = widths_hist_FE.at(m).at(p) -> GetTitle();

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

         if( widths_hist_FE.at(m).at(p) -> GetEntries() != 0 )     widths_hist_FE.at(m).at(p) -> GetFunction("linfit")->SetLineColor(6);
         if( widths_hist_FE_gen.at(m).at(p) -> GetEntries() != 0 ) widths_hist_FE_gen.at(m).at(p) -> GetFunction("linfit")->SetLineColor(2);
         if( widths_hist_FE_data.at(m).at(p) -> GetEntries() != 0 )widths_hist_FE_data.at(m).at(p) -> GetFunction("linfit")->SetLineColor(4);

         widths_hist_FE.at(m).at(p) -> SetMarkerColor(6);
         widths_hist_FE_gen.at(m).at(p) -> SetMarkerColor(2);
         widths_hist_FE_data.at(m).at(p) -> SetMarkerColor(4);

         widths_hist_FE.at(m).at(p) -> SetMarkerStyle(20);
         widths_hist_FE_gen.at(m).at(p) -> SetMarkerStyle(20);
         widths_hist_FE_data.at(m).at(p) -> SetMarkerStyle(20);

         widths_hist_FE.at(m).at(p) -> Draw();

         canv->Update();
         if ((TPaveStats*)canv->GetPrimitive("stats")){
            TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
            stats1->SetName("h1stats2");
            stats1->SetY1NDC(.75);
            stats1->SetY2NDC(.91);
            stats1->SetX1NDC(.71);
            stats1->SetX2NDC(.96);
            stats1->SetTextColor(6);
            TList *list1 = stats1->GetListOfLines();
            TLatex *myt1 = new TLatex(0,0,"MC = ");
            myt1 ->SetTextFont(42);
            myt1 ->SetTextSize(0.04);
            myt1 ->SetTextColor(6);
            list1->AddFirst(myt1);
         }


            canv->Update();
         if ( !((p == 1) && ( m > 3)) )
         {
            widths_hist_FE_data.at(m).at(p) -> Draw();

            canv->Update();

            if ((TPaveStats*)canv->GetPrimitive("stats")){
                TPaveStats *stats3 = (TPaveStats*)canv->GetPrimitive("stats");
                stats3->SetName("h1stats4");
                stats3->SetY1NDC(.75);
                stats3->SetY2NDC(.91);
                stats3->SetX1NDC(.21);
                stats3->SetX2NDC(.46);
                stats3->SetTextColor(4);
                TList *list3 = stats3->GetListOfLines();
                TLatex *myt3 = new TLatex(0,0, label_data + " = ");
                myt3 ->SetTextFont(42);
                myt3 ->SetTextSize(0.04);
                myt3 ->SetTextColor(4);
                list3->AddFirst(myt3);
            }

            canv->Update();
         }

         widths_hist_FE_gen.at(m).at(p) -> Draw();

         canv->Update();
         if ((TPaveStats*)canv->GetPrimitive("stats")){
            TPaveStats *stats2 = (TPaveStats*)canv->GetPrimitive("stats");
            stats2->SetName("h1stats3");
            stats2->SetY1NDC(.75);
            stats2->SetY2NDC(.91);
            stats2->SetX1NDC(.46);
            stats2->SetX2NDC(.71);
            stats2->SetTextColor(2);
            TList *list2 = stats2->GetListOfLines();
            TLatex *myt2 = new TLatex(0,0,"PLI = ");
            myt2 ->SetTextFont(42);
            myt2 ->SetTextSize(0.04);
            myt2 ->SetTextColor(2);
            list2->AddFirst(myt2);
         }


         canv->Update();
//         canv->Update();
         widths_hist_FE_gen.at(m).at(p) -> Draw();
//         canv->Update();
         widths_hist_FE_data.at(m).at(p) -> Draw("same");

//         canv->Update();
         widths_hist_FE.at(m).at(p) -> Draw("same");


         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         canv -> Write();

         TString outputname = widths_hist_FE.at(m).at(p) -> GetTitle();

         canv -> Print("pdfy/widths/"+outputname+".pdf","pdf");
         canv -> Print("pdfy/widths/"+outputname+".png","png");
      }
   }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  same plots with all points (also corresponding to additional jets with < 10 GeV)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   for( unsigned int m = 0; m < soft_widths_hist_FE.size(); m++ )
   {
      for( unsigned int p = 0; p < soft_widths_hist_FE.at(m).size(); p++ )
      {
         TString canvName = soft_widths_hist_FE.at(m).at(p) -> GetTitle();

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

         soft_widths_hist_FE.at(m).at(p) -> SetMarkerColor(6);
         soft_widths_hist_FE_gen.at(m).at(p) -> SetMarkerColor(2);
         soft_widths_hist_FE_data.at(m).at(p) -> SetMarkerColor(4);

         soft_widths_hist_FE.at(m).at(p) -> SetMarkerStyle(20);
         soft_widths_hist_FE_gen.at(m).at(p) -> SetMarkerStyle(20);
         soft_widths_hist_FE_data.at(m).at(p) -> SetMarkerStyle(20);

         canv->Update();
         soft_widths_hist_FE_gen.at(m).at(p) -> Draw();
         soft_widths_hist_FE_data.at(m).at(p) -> Draw("same");
         soft_widths_hist_FE.at(m).at(p) -> Draw("same");


         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         canv -> Write();

         TString outputname = soft_widths_hist_FE.at(m).at(p) -> GetTitle();

         canv -> Print("pdfy/widths/allPoints_"+outputname+".png","png");
      }
   }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plots with widths(alpha)                                                                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   for( unsigned int m = 0; m < widths_hist_all.size(); m++ )
   {
      for( unsigned int p = 0; p < widths_hist_all.at(m).size(); p++ )
      {
         TString canvName = widths_hist_all.at(m).at(p) -> GetTitle();

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

         if( widths_hist_all.at(m).at(p) -> GetEntries() != 0 )     widths_hist_all.at(m).at(p) -> GetFunction("linfit")->SetLineColor(6);
         if( widths_hist_all_gen.at(m).at(p) -> GetEntries() != 0 ) widths_hist_all_gen.at(m).at(p) -> GetFunction("linfit")->SetLineColor(2);
         if( widths_hist_all_data.at(m).at(p) -> GetEntries() != 0 )widths_hist_all_data.at(m).at(p) -> GetFunction("linfit")->SetLineColor(4);

         widths_hist_all.at(m).at(p) -> SetMarkerColor(6);
         widths_hist_all_gen.at(m).at(p) -> SetMarkerColor(2);
         widths_hist_all_data.at(m).at(p) -> SetMarkerColor(4);

         widths_hist_all.at(m).at(p) -> SetMarkerStyle(20);
         widths_hist_all_gen.at(m).at(p) -> SetMarkerStyle(20);
         widths_hist_all_data.at(m).at(p) -> SetMarkerStyle(20);

         widths_hist_all.at(m).at(p) -> Draw();

         canv->Update();
         if( widths_hist_all.at(m).at(p) -> GetEntries() != 0 )
         {
            if ((TPaveStats*)canv->GetPrimitive("stats")){
                TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
                stats1->SetName("h1stats2");
                stats1->SetY1NDC(.75);
                stats1->SetY2NDC(.91);
                stats1->SetX1NDC(.71);
                stats1->SetX2NDC(.96);
                stats1->SetTextColor(6);
                TList *list1 = stats1->GetListOfLines();
                TLatex *myt1 = new TLatex(0,0,"MC = ");
                myt1 ->SetTextFont(42);
                myt1 ->SetTextSize(0.04);
                myt1 ->SetTextColor(6);
                list1->AddFirst(myt1);
            }

         }

         widths_hist_all_gen.at(m).at(p) -> Draw();

         canv->Update();
         if( widths_hist_all_gen.at(m).at(p) -> GetEntries() != 0 )
         {
            if ((TPaveStats*)canv->GetPrimitive("stats")){
                TPaveStats *stats2 = (TPaveStats*)canv->GetPrimitive("stats");
                stats2->SetName("h1stats3");
                stats2->SetY1NDC(.75);
                stats2->SetY2NDC(.91);
                stats2->SetX1NDC(.46);
                stats2->SetX2NDC(.71);
                stats2->SetTextColor(2);

                TList *list2 = stats2->GetListOfLines();
                TLatex *myt2 = new TLatex(0,0,"PLI = ");
                myt2 ->SetTextFont(42);
                myt2 ->SetTextSize(0.04);
                myt2 ->SetTextColor(2);
                list2->AddFirst(myt2);
            }
         }

         widths_hist_all_data.at(m).at(p) -> Draw();

         canv->Update();
         if( widths_hist_all_data.at(m).at(p) -> GetEntries() != 0 )
         {
            if ((TPaveStats*)canv->GetPrimitive("stats")){
                TPaveStats *stats3 = (TPaveStats*)canv->GetPrimitive("stats");
                stats3->SetName("h1stats4");
                stats3->SetY1NDC(.75);
                stats3->SetY2NDC(.91);
                stats3->SetX1NDC(.21);
                stats3->SetX2NDC(.46);
                stats3->SetTextColor(4);

                TList *list3 = stats3->GetListOfLines();
                TLatex *myt3 = new TLatex(0,0,label_data + " = ");
                myt3 ->SetTextFont(42);
                myt3 ->SetTextSize(0.04);
                myt3 ->SetTextColor(4);
                list3->Add(myt3);
            }
         }
         canv->Update();

         widths_hist_all_gen.at(m).at(p) -> Draw();

         canv->Update();

         widths_hist_all_data.at(m).at(p) -> Draw("same");
         widths_hist_all.at(m).at(p) -> Draw("same");


         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

//         canv -> Write();

         TString outputname = widths_hist_all.at(m).at(p) -> GetTitle();

         canv -> Print("pdfy/widths/"+outputname+".pdf","pdf");
         canv -> Print("pdfy/widths/"+outputname+".png","png");
      }
   }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plots with all points of widths(alpha)                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   for( unsigned int m = 0; m < soft_widths_hist_all.size(); m++ )
   {
      for( unsigned int p = 0; p < soft_widths_hist_all.at(m).size(); p++ )
      {
         TString canvName = soft_widths_hist_all.at(m).at(p) -> GetTitle();

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

         soft_widths_hist_all.at(m).at(p) -> SetMarkerColor(6);
         soft_widths_hist_all_gen.at(m).at(p) -> SetMarkerColor(2);
         soft_widths_hist_all_data.at(m).at(p) -> SetMarkerColor(4);

         soft_widths_hist_all.at(m).at(p) -> SetMarkerStyle(20);
         soft_widths_hist_all_gen.at(m).at(p) -> SetMarkerStyle(20);
         soft_widths_hist_all_data.at(m).at(p) -> SetMarkerStyle(20);

         soft_widths_hist_all_gen.at(m).at(p) -> Draw();
         canv->Update();
         soft_widths_hist_all_data.at(m).at(p) -> Draw("same");
         soft_widths_hist_all.at(m).at(p) -> Draw("same");

         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

//         canv -> Write();

         TString outputname = soft_widths_hist_all.at(m).at(p) -> GetTitle();

         canv -> Print("pdfy/widths/allPoints_"+outputname+".png","png");
      }
   }

   h_merged.Close();

//////////////////////////////////////////////////////////////////////////////////////////
// plot with JERs with NSC fit
/////////////////////////////////////////////////////////////////////////////////////////

   gStyle -> SetOptStat(0);
   gStyle -> SetOptFit(0);
   TFile fJERs("output/Jersalltogether.root","RECREATE");
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


         //TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
         TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1500 );
         mcFIT -> SetParameter( 0, 1 );
         mcFIT -> SetParameter( 1, 1 );
         mcFIT -> SetParameter( 2, 0.1);
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

         TLegend *leg = new TLegend(0.6,0.6,0.9,0.8);
         leg->AddEntry(JER_data_hist.at(m),label_data,"lep");
         leg->AddEntry(JER_MC_hist.at(m),"MC","lep");
         leg->Draw();

         canv -> Write();

         TString outputname = JER_data_hist.at(m) -> GetTitle();

         canv -> Print("pdfy/JERs/"+outputname+".pdf","pdf");
         canv -> Print("pdfy/JERs/"+outputname+".png","png");

         TString canvSFName = JER_scale_hist.at(m) -> GetTitle();

         TCanvas* canvSF = new TCanvas(canvName,canvName,50,50,W,H);
         canvSF->SetFillColor(0);
         canvSF->SetBorderMode(0);
         canvSF->SetFrameFillStyle(0);
         canvSF->SetFrameBorderMode(0);
         canvSF->SetLeftMargin( L/W );
         canvSF->SetRightMargin( R/W );
         canvSF->SetTopMargin( T/H );
         canvSF->SetBottomMargin( B/H );
         canvSF->SetTickx(0);
         canvSF->SetTicky(0);

         if(JER_scale_hist.at(m)->GetFunction("constfit")){
            JER_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(6);
//         if(JER_scale_hist.at(m)->GetFunction("pol0")){
//            JER_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
         } else {
             std::cout << "Fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }


         JER_scale_hist.at(m) -> SetMarkerColor(6);

         JER_scale_hist.at(m) -> SetLineColor(6);

         JER_scale_hist.at(m) -> SetMarkerStyle(20);


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

         JER_scale_hist.at(m) -> Draw();

         canvSF->cd();
         CMS_lumi( canvSF, iPeriod, 0 );

         canvSF->Update();
         canvSF->RedrawAxis();
         canvSF->GetFrame()->Draw();
         canvSF->Draw();

         TLegend *legSF = new TLegend(0.6,0.2,0.9,0.4);
         legSF->AddEntry(JER_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");
         legSF->Draw();

         canvSF->Update();
         TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);
         f3->Draw("same");
         canvSF->Update();
         canvSF -> Write();

         TString outputnameSF = JER_scale_hist.at(m) -> GetTitle();

         canvSF -> Print("pdfy/NSC_SFs/NSC"+outputnameSF+".pdf","pdf");
         canvSF -> Print("pdfy/NSC_SFs/NSC"+outputnameSF+".png","png");
   }
//////////////////////////////////////////////////////////////////////////////////////////
// same kind of plot but for correlated results
/////////////////////////////////////////////////////////////////////////////////////////

   for( unsigned int m = 0; m < JER_cor_data_hist.size(); m++ ) // plot with JERs with NSC fit
   {

         TString canvName = JER_cor_data_hist.at(m) -> GetTitle();

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

         JER_cor_data_hist.at(m) -> SetMarkerColor(4);
         JER_cor_MC_hist.at(m) -> SetMarkerColor(2);

         JER_cor_data_hist.at(m) -> SetMarkerStyle(20);
         JER_cor_MC_hist.at(m) -> SetMarkerStyle(20);


         //TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
         TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]*[0]/(x*x))+[1]*[1]/x+[2]*[2] )", 50, 1500 );
         mcFIT -> SetParameter( 0, 1 );
         mcFIT -> SetParameter( 1, 1 );
         mcFIT -> SetParameter( 2, 0.1 );
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

         JER_cor_data_hist.at(m) -> Draw();

         canv->Update();

         JER_cor_MC_hist.at(m) -> Draw();

         canv->Update();
         JER_cor_data_hist.at(m) -> Draw("same");


         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         TLegend *leg = new TLegend(0.6,0.6,0.9,0.8);
         leg->AddEntry(JER_cor_data_hist.at(m),label_data,"lep");
         leg->AddEntry(JER_cor_MC_hist.at(m),"MC","lep");
         leg->Draw();

         canv -> Write();

         TString outputname = JER_cor_data_hist.at(m) -> GetTitle();

         canv -> Print("pdfy/JERs/cor_"+outputname+".pdf","pdf");
         canv -> Print("pdfy/JERs/cor_"+outputname+".png","png");

         TString canvSFName = JER_cor_scale_hist.at(m) -> GetTitle();

         TCanvas* canvSF = new TCanvas(canvName,canvName,50,50,W,H);
         canvSF->SetFillColor(0);
         canvSF->SetBorderMode(0);
         canvSF->SetFrameFillStyle(0);
         canvSF->SetFrameBorderMode(0);
         canvSF->SetLeftMargin( L/W );
         canvSF->SetRightMargin( R/W );
         canvSF->SetTopMargin( T/H );
         canvSF->SetBottomMargin( B/H );
         canvSF->SetTickx(0);
         canvSF->SetTicky(0);

         if(JER_cor_scale_hist.at(m)->GetFunction("constfit")){
            JER_cor_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(6);
//         if(JER_cor_scale_hist.at(m)->GetFunction("pol0")){
//            JER_cor_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
//asymmetries_width_data_pt
         } else {
             std::cout << "Fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }
std::cout << __LINE__ << std::endl;
std::cout << m << std::endl;

         JER_cor_scale_hist.at(m) -> SetMarkerColor(6);

         JER_cor_scale_hist.at(m) -> SetLineColor(6);

         JER_cor_scale_hist.at(m) -> SetMarkerStyle(20);

         std::cout << __LINE__ << std::endl;
         std::cout << m << std::endl;

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

         JER_cor_scale_hist.at(m) -> Draw();

         canvSF->cd();
         CMS_lumi( canvSF, iPeriod, 0 );

         canvSF->Update();
         canvSF->RedrawAxis();
         canvSF->GetFrame()->Draw();
         canvSF->Draw();
         std::cout << __LINE__ << std::endl;
         std::cout << m << std::endl;
         TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);

         TLegend *legSF = new TLegend(0.6,0.2,0.95,0.4);
         legSF->AddEntry(JER_cor_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");
         std::cout << __LINE__ << std::endl;
         std::cout << m << std::endl;
         std::cout << asymmetries_width_data_pt.size() << std::endl;
         std::cout << asymmetries_width_data_pt.at(m).size() << std::endl;
         std::cout << asymmetries_width_data_pt.at(m).at(1).size() << std::endl;
         std::cout << asymmetries_width_data_pt.at(m).at(1).(4) << std::endl;
         std::cout << f3->Eval(asymmetries_width_data_pt.at(m).at(1).at(5)) << std::endl;
         std::cout << (JER_cor_scale_hist.at(m) -> GetFunction("constfit"))->Eval(asymmetries_width_data_pt.at(m).at(1).at(5)) << std::endl;
         double valueratio = f3->Eval(asymmetries_width_data_pt.at(m).at(1).at(5)) - (JER_cor_scale_hist.at(m) -> GetFunction("constfit"))->Eval(asymmetries_width_data_pt.at(m).at(1).at(5));
         std::cout << __LINE__ << std::endl;
         std::cout << m << std::endl;
         char name_leg[100];
         sprintf(name_leg, "diff %.6f", valueratio);
//         double valueratio = asymmetries_width_data_pt.at(m).at(1).at(5);
         legSF->AddEntry((TObject*)0,name_leg , "");
         legSF->Draw();

         canvSF->Update();
         f3->Draw("same");
         canvSF->Update();
         canvSF -> Write();

         TString outputnameSF = JER_cor_scale_hist.at(m) -> GetTitle();
         std::cout << "pt dependance difference for cor_NSC" << outputnameSF << " is: " << valueratio << std::endl;

         canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+outputnameSF+".pdf","pdf");
         canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+outputnameSF+".png","png");
   }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  JER plots for FE with NSC fits
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   for( unsigned int m = 0; m < JER_FE_MC_hist.size(); m++ )
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


         TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
         mcFIT->SetLineColor(kRed+2);  // sets the color to red
         mcFIT -> SetParameter( 0, 1 );
         mcFIT -> SetParameter( 1, 1 );
         mcFIT -> SetParameter( 2, 0.1 );
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

         TLegend *leg = new TLegend(0.6,0.6,0.9,0.8);
         leg->AddEntry(JER_FE_data_hist.at(m),label_data,"lep");
         leg->AddEntry(JER_FE_MC_hist.at(m),"MC","lep");
         leg->Draw();

         canv -> Write();

         TString outputname = JER_FE_data_hist.at(m) -> GetTitle();

         canv -> Print("pdfy/JERs/"+outputname+".pdf","pdf");
         canv -> Print("pdfy/JERs/"+outputname+".png","png");

         TString canvSFName = JER_FE_scale_hist.at(m) -> GetTitle();

         TCanvas* canvSF = new TCanvas(canvName,canvName,50,50,W,H);
         canvSF->SetFillColor(0);
         canvSF->SetBorderMode(0);
         canvSF->SetFrameFillStyle(0);
         canvSF->SetFrameBorderMode(0);
         canvSF->SetLeftMargin( L/W );
         canvSF->SetRightMargin( R/W );
         canvSF->SetTopMargin( T/H );
         canvSF->SetBottomMargin( B/H );
         canvSF->SetTickx(0);
         canvSF->SetTicky(0);

         if(JER_FE_scale_hist.at(m)->GetFunction("constfit")){
            JER_FE_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(6);
//         if(JER_FE_scale_hist.at(m)->GetFunction("pol0")){
//            JER_FE_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
         } else {
             std::cout << "Fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }


         JER_FE_scale_hist.at(m) -> SetMarkerColor(6);

         JER_FE_scale_hist.at(m) -> SetLineColor(6);

         JER_FE_scale_hist.at(m) -> SetMarkerStyle(20);


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

         JER_FE_scale_hist.at(m) -> Draw();

         canvSF->cd();
         CMS_lumi( canvSF, iPeriod, 0 );

         canvSF->Update();
         canvSF->RedrawAxis();
         canvSF->GetFrame()->Draw();
         canvSF->Draw();

         TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);

         TLegend *legSF = new TLegend(0.6,0.2,0.9,0.4);
         legSF->AddEntry(JER_FE_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");
         legSF->Draw();

         canvSF->Update();
         f3->Draw("same");
         canvSF->Update();
         canvSF -> Write();

         TString outputnameSF = JER_FE_scale_hist.at(m) -> GetTitle();

         canvSF -> Print("pdfy/NSC_SFs/NSC"+outputnameSF+".pdf","pdf");
         canvSF -> Print("pdfy/NSC_SFs/NSC"+outputnameSF+".png","png");
   }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  correlated results JER plots for FE with NSC fits
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


   for( unsigned int m = 0; m < JER_cor_FE_MC_hist.size(); m++ )
   {

         TString canvName = JER_cor_FE_data_hist.at(m) -> GetTitle();

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

         JER_cor_FE_data_hist.at(m) -> SetMarkerColor(4);
         JER_cor_FE_MC_hist.at(m) -> SetMarkerColor(2);

         JER_cor_FE_data_hist.at(m) -> SetMarkerStyle(20);
         JER_cor_FE_MC_hist.at(m) -> SetMarkerStyle(20);


         TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
         mcFIT -> SetParameter( 0, 0.05 );
         mcFIT -> SetParameter( 1, 1 );
         mcFIT -> SetParameter( 2, 0.05 );
         mcFIT->SetLineColor(kRed+2);  // sets the color to red
         JER_cor_FE_MC_hist.at(m) -> Fit("mcFIT", "ML");

         double N, S, C;

         N = mcFIT -> GetParameter( 0 );
         S = mcFIT -> GetParameter( 1 );
         C = mcFIT -> GetParameter( 2 );

         TF1 * dtFIT = new TF1( "dtFIT", "TMath::Sqrt( [2]*(([0]/x)**2)+[3]*([0]**2)/x+[1]**2 )", 50, 1500 );
         dtFIT->SetLineColor(kBlue+2);  // sets the color to red
         dtFIT -> FixParameter(2, N*N);
         dtFIT -> FixParameter(3, S*S);
         JER_cor_FE_data_hist.at(m) -> Fit("dtFIT", "M");

         JER_cor_FE_data_hist.at(m) -> Draw();

         canv->Update();

         JER_cor_FE_MC_hist.at(m) -> Draw();

         canv->Update();
         JER_cor_FE_data_hist.at(m) -> Draw("same");


         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         TLegend *leg = new TLegend(0.6,0.6,0.9,0.8);
         leg->AddEntry(JER_cor_FE_data_hist.at(m),label_data,"lep");
         leg->AddEntry(JER_cor_FE_MC_hist.at(m),"MC","lep");
         leg->Draw();

         canv -> Write();

         TString outputname = JER_cor_FE_data_hist.at(m) -> GetTitle();

         canv -> Print("pdfy/JERs/cor_"+outputname+".pdf","pdf");
         canv -> Print("pdfy/JERs/cor_"+outputname+".png","png");

         TString canvSFName = JER_cor_FE_scale_hist.at(m) -> GetTitle();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  correlated results JER plots for FE with NSC fits
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


   for( unsigned int m = 0; m < JER_cor_FE_MC_hist.size(); m++ )
   {

         TString canvName = JER_cor_FE_data_hist.at(m) -> GetTitle();

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

         JER_cor_FE_data_hist.at(m) -> SetMarkerColor(4);
         JER_cor_FE_MC_hist.at(m) -> SetMarkerColor(2);

         JER_cor_FE_data_hist.at(m) -> SetMarkerStyle(20);
         JER_cor_FE_MC_hist.at(m) -> SetMarkerStyle(20);


         TF1 * mcFIT = new TF1( "mcFIT", "TMath::Sqrt( ([0]/x)**2+[1]**2/x+[2]**2 )", 50, 1500 );
         mcFIT -> SetParameter( 0, 1 );
         mcFIT -> SetParameter( 1, 1 );
         mcFIT -> SetParameter( 1, 0.1 );
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

         JER_cor_FE_data_hist.at(m) -> Draw();

         canv->Update();

         JER_cor_FE_MC_hist.at(m) -> Draw();

         canv->Update();
         JER_cor_FE_data_hist.at(m) -> Draw("same");


         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         TLegend *leg = new TLegend(0.6,0.6,0.9,0.8);
         leg->AddEntry(JER_cor_FE_data_hist.at(m),label_data,"lep");
         leg->AddEntry(JER_cor_FE_MC_hist.at(m),"MC","lep");
         leg->Draw();

         canv -> Write();

         TString outputname = JER_cor_FE_data_hist.at(m) -> GetTitle();

         canv -> Print("pdfy/JERs/cor_"+outputname+".pdf","pdf");
         canv -> Print("pdfy/JERs/cor_"+outputname+".png","png");

         TString canvSFName = JER_cor_FE_scale_hist.at(m) -> GetTitle();

         TCanvas* canvSF = new TCanvas(canvName,canvName,50,50,W,H);
         canvSF->SetFillColor(0);
         canvSF->SetBorderMode(0);
         canvSF->SetFrameFillStyle(0);
         canvSF->SetFrameBorderMode(0);
         canvSF->SetLeftMargin( L/W );
         canvSF->SetRightMargin( R/W );
         canvSF->SetTopMargin( T/H );
         canvSF->SetBottomMargin( B/H );
         canvSF->SetTickx(0);
         canvSF->SetTicky(0);

         if(JER_cor_FE_scale_hist.at(m)->GetFunction("pol0")){
            JER_cor_FE_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
         } else {
             std::cout << "Fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }


         JER_cor_FE_scale_hist.at(m) -> SetMarkerColor(6);

         JER_cor_FE_scale_hist.at(m) -> SetLineColor(6);

         JER_cor_FE_scale_hist.at(m) -> SetMarkerStyle(20);


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

         JER_cor_FE_scale_hist.at(m) -> Draw();

         canvSF->cd();
         CMS_lumi( canvSF, iPeriod, 0 );

         canvSF->Update();
         canvSF->RedrawAxis();
         canvSF->GetFrame()->Draw();
         canvSF->Draw();


         TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);

         TLegend *legSF = new TLegend(0.6,0.2,0.9,0.4);
         legSF->AddEntry(JER_cor_FE_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");
         double valueratio = f3->Eval(forward_width_data_pt.at(m+1).at(1).at(5)) - (JER_cor_FE_scale_hist.at(m) -> GetFunction("constfit"))->Eval(forward_width_data_pt.at(m+1).at(1).at(5));
         char name_leg[100];
         sprintf(name_leg, "diff %.6f", valueratio);
//         sprintf(name_leg, "difference %.4f", valueratio);
//         double valueratio = asymmetries_width_data_pt.at(m).at(1).at(5);
         legSF->AddEntry((TObject*)0,name_leg , "");
         legSF->Draw();

         canvSF->Update();
         f3->Draw("same");
         canvSF->Update();
         canvSF -> Write();

         TString outputnameSF = JER_cor_FE_scale_hist.at(m) -> GetTitle();
         std::cout << "pt dependance difference for cor_NSC" << outputnameSF << " is: " << valueratio << std::endl;

         canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+outputnameSF+".pdf","pdf");
         canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+outputnameSF+".png","png");
   }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         TCanvas* canvSF = new TCanvas(canvName,canvName,50,50,W,H);
         canvSF->SetFillColor(0);
         canvSF->SetBorderMode(0);
         canvSF->SetFrameFillStyle(0);
         canvSF->SetFrameBorderMode(0);
         canvSF->SetLeftMargin( L/W );
         canvSF->SetRightMargin( R/W );
         canvSF->SetTopMargin( T/H );
         canvSF->SetBottomMargin( B/H );
         canvSF->SetTickx(0);
         canvSF->SetTicky(0);

         if(JER_cor_FE_scale_hist.at(m)->GetFunction("constfit")){
            JER_cor_FE_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(6);
//         if(JER_cor_FE_scale_hist.at(m)->GetFunction("pol0")){
//            JER_cor_FE_scale_hist.at(m) -> GetFunction("pol0")->SetLineColor(6);
         } else {
             std::cout << "Fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }


         JER_cor_FE_scale_hist.at(m) -> SetMarkerColor(6);

         JER_cor_FE_scale_hist.at(m) -> SetLineColor(6);

         JER_cor_FE_scale_hist.at(m) -> SetMarkerStyle(20);


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

         JER_cor_FE_scale_hist.at(m) -> Draw();

         canvSF->cd();
         CMS_lumi( canvSF, iPeriod, 0 );

         canvSF->Update();
         canvSF->RedrawAxis();
         canvSF->GetFrame()->Draw();
         canvSF->Draw();

         TLegend *legSF = new TLegend(0.6,0.2,0.9,0.4);
         legSF->AddEntry(JER_cor_FE_scale_hist.at(m),"#sigma_{JER}^{data}/#sigma_{JER}^{mc}","lep");
         legSF->Draw();

         canvSF->Update();
         TF1 *f3 = new TF1("f3","dtFIT/mcFIT",0,1500);
         f3->Draw("same");
         canvSF->Update();
         canvSF -> Write();

         TString outputnameSF = JER_cor_FE_scale_hist.at(m) -> GetTitle();

         canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+outputnameSF+".pdf","pdf");
         canvSF -> Print("pdfy/NSC_SFs/cor_NSC"+outputnameSF+".png","png");
   }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
         leg->AddEntry(JER_FE_MC_hist.at(m),label_mc,"lep");
         leg->Draw();

         canv -> Write();

         TString outputname = JER_FE_data_hist.at(m) -> GetTitle();

//         canv -> Print("pdfy/"+outputname+".pdf","pdf");
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
         leg->AddEntry(JER_FE_MC_hist_control.at(m),label_mc,"lep");
         leg->Draw();

         canv -> Write();

         TString outputname = JER_FE_data_hist_control.at(m) -> GetTitle();

//         canv -> Print("pdfy/"+outputname+".pdf","pdf");
   }
   fJERs.Close();

//////////////////////////////////////////////////////////////////////////////////////////
//    SF plots overlayed with ...                                                       //
//////////////////////////////////////////////////////////////////////////////////////////
   TFile gsfoverlayed("output/SFsOver.root","RECREATE");

//   gStyle -> SetOptFit(1);

   for( unsigned int m = 0; m < JER_scale_hist.size(); m++ )
   {
         TString canvName = JER_scale_hist.at(m) -> GetTitle();

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

         if(JER_scale_hist.at(m)->GetFunction("constfit")){
            JER_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(6);
         } else {
             std::cout << "Fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }

         if(JER_cor_scale_hist.at(m)->GetFunction("constfit")){
            JER_cor_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(kGreen+2);
         } else {
             std::cout << "Fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }

         if(JER015_scale_hist.at(m)->GetFunction("constfit")){
            JER015_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(4);
         } else {
             std::cout << "Fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }


         JER_scale_hist.at(m) -> SetMarkerColor(6);
         JER_cor_scale_hist.at(m) -> SetMarkerColor(kGreen+2);
         JER015_scale_hist.at(m) -> SetMarkerColor(4);

         JER_scale_hist.at(m) -> SetLineColor(6);
         JER_cor_scale_hist.at(m) -> SetLineColor(kGreen+2);
         JER015_scale_hist.at(m) -> SetLineColor(4);

         JER_scale_hist.at(m) -> SetMarkerStyle(20);
         JER_cor_scale_hist.at(m) -> SetMarkerStyle(20);
         JER015_scale_hist.at(m) -> SetMarkerStyle(20);

         JER015_scale_hist.at(m) -> Draw();

         canv->Update();
         if ((TPaveStats*)canv->GetPrimitive("stats")){
            TPaveStats *stats2 = (TPaveStats*)canv->GetPrimitive("stats");
            stats2->SetName("h1stats3");
            stats2->SetY1NDC(.64);
            stats2->SetY2NDC(.77);
            stats2->SetX1NDC(.66);
            stats2->SetX2NDC(.91);
            stats2->SetTextColor(4);
         }

         JER_cor_scale_hist.at(m) -> Draw();

         canv->Update();
         if ((TPaveStats*)canv->GetPrimitive("stats")){
            TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
            stats1->SetName("h1stats2");
            stats1->SetY1NDC(.77);
            stats1->SetY2NDC(.90);
            stats1->SetX1NDC(.41);
            stats1->SetX2NDC(.66);
            stats1->SetTextColor(kGreen+2);
         }

         JER_scale_hist.at(m) -> Draw();

         canv->Update();
         if ((TPaveStats*)canv->GetPrimitive("stats")){
            TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
            stats1->SetName("h1stats2");
            stats1->SetY1NDC(.77);
            stats1->SetY2NDC(.90);
            stats1->SetX1NDC(.66);
            stats1->SetX2NDC(.91);
            stats1->SetTextColor(6);
         }

         JER015_scale_hist.at(m) -> Draw();
         JER_cor_scale_hist.at(m) -> Draw("same");
         JER_scale_hist.at(m) -> Draw("same");

         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         TLegend *leg = new TLegend(0.6,0.2,0.9,0.4);
         leg->AddEntry(JER_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{" + label_mc + "}","lep");
         leg->AddEntry(JER_cor_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{" + label_mc + "} corelated","lep");
         leg->AddEntry(JER015_scale_hist.at(m),"#sigma_{0.15}^{" + label_data + "}/#sigma_{0.15}^{" + label_mc + "}","lep");
         leg->Draw();
         canv -> Write();


         TString outputname = JER_scale_hist.at(m) -> GetTitle();

         canv -> Print("pdfy/SFs/"+outputname+".pdf","pdf");
         canv -> Print("pdfy/SFs/"+outputname+".png","png");
   }
   for( unsigned int m = 0; m < JER_FE_scale_hist.size(); m++ )
   {
         TString canvName = JER_FE_scale_hist.at(m) -> GetTitle();

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

         if(JER_FE_scale_hist.at(m)->GetFunction("constfit")){
            JER_FE_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(6);
         } else {
             std::cout << "Forward fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }

         if(JER_cor_FE_scale_hist.at(m)->GetFunction("constfit")){
            JER_cor_FE_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(kGreen+2);
         } else {
             std::cout << "Forward fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }

         if(JER015_FE_scale_hist.at(m)->GetFunction("constfit")){
            JER015_FE_scale_hist.at(m) -> GetFunction("constfit")->SetLineColor(4);
         } else {
             std::cout << "Forward fit function at bin " << m;
             std::cout << " was not found" << std::endl;
         }

         JER_FE_scale_hist.at(m) -> SetMarkerColor(6);
         JER_cor_FE_scale_hist.at(m) -> SetMarkerColor(kGreen+2);
         JER015_FE_scale_hist.at(m) -> SetMarkerColor(4);

         JER_FE_scale_hist.at(m) -> SetLineColor(6);
         JER_cor_FE_scale_hist.at(m) -> SetLineColor(kGreen+2);
         JER015_FE_scale_hist.at(m) -> SetLineColor(4);

         JER_FE_scale_hist.at(m) -> SetMarkerStyle(20);
         JER_cor_FE_scale_hist.at(m) -> SetMarkerStyle(20);
         JER015_FE_scale_hist.at(m) -> SetMarkerStyle(20);


         JER015_FE_scale_hist.at(m) -> Draw();

         canv->Update();
         if ((TPaveStats*)canv->GetPrimitive("stats")){
            TPaveStats *stats2 = (TPaveStats*)canv->GetPrimitive("stats");
            stats2->SetName("h1stats3");
            stats2->SetY1NDC(.64);
            stats2->SetY2NDC(.77);
            stats2->SetX1NDC(.66);
            stats2->SetX2NDC(.91);
            stats2->SetTextColor(4);
         }

         JER_cor_FE_scale_hist.at(m) -> Draw();

         canv->Update();
         if ((TPaveStats*)canv->GetPrimitive("stats")){
            TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
            stats1->SetName("h1stats2");
            stats1->SetY1NDC(.77);
            stats1->SetY2NDC(.90);
            stats1->SetX1NDC(.41);
            stats1->SetX2NDC(.66);
            stats1->SetTextColor(kGreen+2);
         }

         JER_FE_scale_hist.at(m) -> Draw();

         canv->Update();
         if ((TPaveStats*)canv->GetPrimitive("stats")){
            TPaveStats *stats1 = (TPaveStats*)canv->GetPrimitive("stats");
            stats1->SetName("h1stats2");
            stats1->SetY1NDC(.77);
            stats1->SetY2NDC(.90);
            stats1->SetX1NDC(.66);
            stats1->SetX2NDC(.91);
            stats1->SetTextColor(6);
         }

         JER015_FE_scale_hist.at(m) -> Draw();
         JER_cor_FE_scale_hist.at(m) -> Draw("same");
         JER_FE_scale_hist.at(m) -> Draw("same");

         canv->cd();
         CMS_lumi( canv, iPeriod, 0 );

         canv->Update();
         canv->RedrawAxis();
         canv->GetFrame()->Draw();
         canv->Draw();

         TLegend *leg = new TLegend(0.6,0.2,0.9,0.4);
         leg->AddEntry(JER_FE_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{" + label_mc + "}","lep");
         leg->AddEntry(JER_cor_FE_scale_hist.at(m),"#sigma_{JER}^{" + label_data + "}/#sigma_{JER}^{" + label_mc + "} corelated","lep");
         leg->AddEntry(JER015_FE_scale_hist.at(m),"#sigma_{0.15}^{" + label_data + "}/#sigma_{0.15}^{" + label_mc + "}","lep");
         leg->Draw();

         canv -> Write();

         TString outputname = JER_FE_scale_hist.at(m) -> GetTitle();

         canv -> Print("pdfy/SFs/"+outputname+".pdf","pdf");
         canv -> Print("pdfy/SFs/"+outputname+".png","png");
   }



   gsfoverlayed.Close();

   gStyle->SetOptTitle(0);

   for( unsigned int m = 0; m < asymmetries_all.size(); m++ )
   {
      for( unsigned int p = 0; p < asymmetries_all.at(m).size(); p++ )
      {
         TString cName = asymmetries_all.at(m).at(p).at(0) -> GetTitle();

         TCanvas* canv = new TCanvas(cName,cName,50,50,1200,800);

         canv->Divide(4,2);

         for( unsigned int r = 0; r < asymmetries_all.at(m).at(p).size(); r++ )
         {
            canv -> cd(r+1);
//            canv->SetLogy();
            gPad->SetLogy();

            asymmetries_all.at(m).at(p).at(r)      -> SetMarkerColor(4);
            asymmetries_gen_all.at(m).at(p).at(r)  -> SetMarkerColor(2);
            asymmetries_all_data.at(m).at(p).at(r) -> SetMarkerColor(3);

            asymmetries_all.at(m).at(p).at(r)      -> SetLineColor(4);
            asymmetries_gen_all.at(m).at(p).at(r)  -> SetLineColor(2);
            asymmetries_all_data.at(m).at(p).at(r) -> SetLineColor(3);

            asymmetries_all.at(m).at(p).at(r)      -> SetMarkerStyle(20);
            asymmetries_gen_all.at(m).at(p).at(r)  -> SetMarkerStyle(20);
            asymmetries_all_data.at(m).at(p).at(r) -> SetMarkerStyle(20);

//            double normMCRe = asymmetries_all.at(m).at(p).at(r) -> Integral();
//            double normMCGe = asymmetries_gen_all.at(m).at(p).at(r) -> Integral();
//            double normData = asymmetries_all_data.at(m).at(p).at(r) -> Integral();
//
            double normMCRe = asymmetries_all.at(m).at(p).at(r) -> GetBinContent(1);
            double normMCGe = asymmetries_gen_all.at(m).at(p).at(r) -> GetBinContent(1);
            double normData = asymmetries_all_data.at(m).at(p).at(r) -> GetBinContent(1);

            asymmetries_all.at(m).at(p).at(r)      -> Scale(1/normMCRe, "width");
            asymmetries_gen_all.at(m).at(p).at(r)  -> Scale(1/normMCGe, "width");
            asymmetries_all_data.at(m).at(p).at(r) -> Scale(1/normData, "width");


            asymmetries_all.at(m).at(p).at(r) -> Draw();
            asymmetries_gen_all.at(m).at(p).at(r) -> Draw("same");
            asymmetries_all_data.at(m).at(p).at(r) -> Draw("same");

            TString legTitle = "Alpha bin";legTitle += m+1;

            char name_mcreco[100];
            char name_mcgen[100];
            char name_data[100];
            sprintf(name_mcreco, "MC, %.4f+-%.4f", asymmetries_width.at(m).at(p).at(r), asymmetries_width_error.at(m).at(p).at(r));
            sprintf(name_mcgen, "gen, %.4f+-%.4f", asymmetries_gen_width.at(m).at(p).at(r), asymmetries_gen_width_error.at(m).at(p).at(r));
            sprintf(name_data, "data, %.4f+-%.4f", asymmetries_width_data.at(m).at(p).at(r), asymmetries_width_error_data.at(m).at(p).at(r));

            TLegend *leg = new TLegend(0.5,0.6,0.95,0.8);
            leg->AddEntry(legTitle);
            leg->AddEntry(asymmetries_all.at(m).at(p).at(r),    name_mcreco,"lep");
            leg->AddEntry(asymmetries_all_data.at(m).at(p).at(r), name_data,"lep");
            leg->AddEntry(asymmetries_gen_all.at(m).at(p).at(r), name_mcgen,"lep");
            leg->Draw();


         }
         canv -> cd(8);

         TPaveText *pt = new TPaveText(.05,.1,.95,.8);

         char name_eta_label[100];

         sprintf(name_eta_label, "Eta range %.2f-%.2f",eta_bins_edge[m] ,eta_bins_edge[m+1] );

         //TString eta_label = "Eta range"; eta_label += eta_bins_edge[m]; eta_label += "-"; eta_label += eta_bins_edge[m+1];
         TString pt_label = "Pt range "; pt_label += p_bins_edge[p]; pt_label += "-"; pt_label += p_bins_edge[p+1];

         //pt->AddText(eta_label);
         pt->AddText(name_eta_label);
         pt->AddText(pt_label);

         pt->Draw();

         TString outputname = asymmetries_all.at(m).at(p).at(0) -> GetTitle();
//         canv->SaveAs("output/asymmetries/"+outputname+".pdf");
//         canv->SaveAs("output/asymmetries/"+outputname+".png");
      }
   }

   for( unsigned int m = 0; m < forward_hist.size(); m++ )
   {
      for( unsigned int p = 0; p < forward_hist.at(m).size(); p++ )
      {
         TString cName = forward_hist.at(m).at(p).at(0) -> GetTitle();

         TCanvas* canv = new TCanvas(cName,cName,50,50,1200,800);

         canv->Divide(3,2);

         for( unsigned int r = 0; r < forward_hist.at(m).at(p).size(); r++ )
         {
            canv -> cd(r+1);

            forward_hist.at(m).at(p).at(r)      -> SetMarkerColor(4);
            forward_gen_hist.at(m).at(p).at(r)  -> SetMarkerColor(2);
            forward_hist_data.at(m).at(p).at(r) -> SetMarkerColor(3);

            forward_hist.at(m).at(p).at(r)      -> SetLineColor(4);
            forward_gen_hist.at(m).at(p).at(r)  -> SetLineColor(2);
            forward_hist_data.at(m).at(p).at(r) -> SetLineColor(3);

            forward_hist.at(m).at(p).at(r)      -> SetMarkerStyle(20);
            forward_gen_hist.at(m).at(p).at(r)  -> SetMarkerStyle(20);
            forward_hist_data.at(m).at(p).at(r) -> SetMarkerStyle(20);

            double normMCRe = forward_hist.at(m).at(p).at(r) -> Integral();
            double normMCGe = forward_gen_hist.at(m).at(p).at(r) -> Integral();
            double normData = forward_hist_data.at(m).at(p).at(r) -> Integral();

            forward_hist.at(m).at(p).at(r)      -> Scale(1/normMCRe, "width");
            forward_gen_hist.at(m).at(p).at(r)  -> Scale(1/normMCGe, "width");
            forward_hist_data.at(m).at(p).at(r) -> Scale(1/normData, "width");


            forward_gen_hist.at(m).at(p).at(r) -> Draw();
            forward_hist.at(m).at(p).at(r) -> Draw("same");
            forward_hist_data.at(m).at(p).at(r) -> Draw("same");

            TString legTitle = "Alpha bin";legTitle += m+1;

            char name_mcreco[100];
            char name_mcgen[100];
            char name_data[100];
            sprintf(name_mcreco, "MC, %.4f+-%.4f", forward_width.at(m).at(p).at(r), forward_width_error.at(m).at(p).at(r));
            sprintf(name_mcgen, "gen, %.4f+-%.4f", forward_gen_width.at(m).at(p).at(r), forward_gen_width_error.at(m).at(p).at(r));
            sprintf(name_data, "data, %.4f+-%.4f", forward_width_data.at(m).at(p).at(r), forward_width_error_data.at(m).at(p).at(r));

            TLegend *leg = new TLegend(0.5,0.6,0.95,0.8);
            leg->AddEntry(legTitle);
            leg->AddEntry(forward_hist.at(m).at(p).at(r),      name_mcreco,"lep");
            leg->AddEntry(forward_hist_data.at(m).at(p).at(r), name_data,"lep");
            leg->AddEntry(forward_gen_hist.at(m).at(p).at(r),  name_mcgen,"lep");
            leg->Draw();


         }
         canv -> cd(6);

         TPaveText *pt = new TPaveText(.05,.1,.95,.8);

         char name_eta_label[100];
         sprintf(name_eta_label, "Eta range %.2f-%.2f",eta_bins_edge[m] ,eta_bins_edge[m+1] );

         //TString eta_label = "Eta range"; eta_label += eta_bins_edge[m]; eta_label += "-"; eta_label += eta_bins_edge[m+1];
         TString pt_label = "Pt range "; pt_label += p_bins_edge_FT[p]; pt_label += "-"; pt_label += p_bins_edge_FT[p+1];

         pt->AddText(name_eta_label);
         pt->AddText(pt_label);

         pt->Draw();

         TString outputname = forward_hist.at(m).at(p).at(0) -> GetTitle();
         canv->SaveAs("output/asymmetries/"+outputname+".pdf");
//         canv->SaveAs("output/asymmetries/"+outputname+".png");
      }
   }
/*
//   for( unsigned int m = 0; m < forward_hist.size(); m++ )
//   {
//      for( unsigned int p = 0; p < forward_hist.at(m).size(); p++ )
//      {
//         for( unsigned int r = 0; r < forward_hist.at(m).at(p).size(); r++ )
//         {
//            forward_hist.at(m).at(p).at(r) -> Write();
//            forward_gen_hist.at(m).at(p).at(r) -> Write();
//            forward_hist_data.at(m).at(p).at(r) -> Write();
//         }
//      }
//   }

*/

/*
   for( unsigned int m = 0; m < forward_width_data_pt.size(); m++ )
   {
      for( unsigned int p = 0; p < forward_width_data_pt.at(m).size(); p++ )
      {
//         std::cout << m << ", " << p << ", " << forward_width_data_pt.at(m).at(p).at(4) << std::endl;
      }
   }

   for( unsigned int m = 0; m < scales_fe.size(); m++ )
   {
      for( unsigned int p = 0; p < scales_fe.at(m).size(); p++ )
      {
//         std::cout << m << ", " << p << ", " << scales_fe.at(m).at(p) << std::endl;
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





//      JERb_scale_hist.at(m) -> Fit("pol0","");
//      JERb_FE_scale_hist.at(m) -> Fit("pol0","");

   TFile gbisb("SFsb.root","RECREATE");

//   std::cout << "now i'm fitting scale factors for b-par method" << std::endl;

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
   //      CMS_lumi( canv, iPeriod, 0 );

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
//         CMS_lumi( canv, iPeriod, 0 );

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



//   std::cout << " tutej jestsems --------------------------- jestem tutej --------------------------- jestem tutej " << std::endl;


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
