#include <iostream>
#include <fstream>
#include <TChain.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLatex.h>
#include <TFile.h>
#include <TLine.h>
#include <TStyle.h>
#include <string>
#include <TMath.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TProfile.h>

#include <TGraphAsymmErrors.h>

#include <TGraphErrors.h>
#include <TMinuit.h>
#include <TMatrixD.h>

// Code by Marek Niedziela
// Based on code by Matthias Schröder, Kristin Goebel

// Smeared MC instead of data:
//bool smeared = true;
TString labeldata = "Data";
//TString labeldata = "Smeared MC";
TString sep = "|";

struct fit_data{
  // x, y values:
  std::vector<double> x_val, y_val;
  // variance-covariance matrix for y values:
  TMatrixD y_cov;
  // inverted cov matrix; calculated by chi2_linear "on demand".
  TMatrixD y_cov_inv;

  void reset(){
    x_val.clear();
    y_val.clear();
    y_cov.ResizeTo(0,0);
    y_cov_inv.ResizeTo(0,0);
  }

  void CheckPoints(){
    std::vector<int> RemovedPoints;
    TMatrixD y_cov_new;
    int j = 0;

    for(unsigned int i = 0; i < y_val.size(); i++) {
      //         std::cout << "i: " << i << "   j: " << j << std::endl;
      if( y_val.at(i) == 0) {
        x_val.erase(x_val.begin()+i);
        y_val.erase(y_val.begin()+i);
        RemovedPoints.push_back(j);
        i = i-1;
      }
      j++;
    }
    //      for(int i = 0; i < x_val.size(); i++) {
    //         std::cout << "x: " << x_val.at(i) << std::endl;
    //      }
    //
    //      std::cout<< "Removed Points: " << RemovedPoints.size() << std::endl;
    //      std::cout<< "Remaining Points: " << x_val.size() << std::endl;

    y_cov_new.ResizeTo(x_val.size(),x_val.size());
    for(unsigned int i=0; i < x_val.size(); i++) {
      for(unsigned int k= 0; k < x_val.size(); k++) {
        y_cov_new(i,k) = y_cov(i+RemovedPoints.size(),k+RemovedPoints.size());
      }
    }
    y_cov.ResizeTo(0,0);
    y_cov.ResizeTo(x_val.size(),x_val.size());
    y_cov = y_cov_new;
  }
};

fit_data data;

//void fitAsym( TH1F &hist, double &width, double &error , double &centre, double &centre_error, double &chisqr, int fitType );
void fitAsym( TH1F &hist, double &width, double &error , double &centre, double &centre_error, double &chisqr, int fitType, double position = 0. , bool fix = false );
void fitConst( TH1F &hist, double &value, double &error );
void fitLin( TH1F &hist, double &width, double &error );
void fitShift( TH1F &hist, double &a, double &errora, double &b, double &errorb );
void fitMCTruth( TH1F &hist, double &width, double &error );
float GetAsymmWidth(TH1F* &htemp, double * xq_IQW, double * yq_IQW, int etabin, int ptbin, int alphabin);



char truncation_info[50];
double truncate_fraction = 0.985;


void histLoadMC( TFile &f, std::vector<TH1F*> &Asymmetry, std::vector<TH1F*> &GenAsymmetry, std::vector<TH1F*> &MCTruth, std::vector<TH1F*> &Alpha, std::vector<TH1F*> &AlphaGen  );
void histLoadFE1( TFile &f, bool data, TString name, std::vector< std::vector< std::vector< TH1F* > > > &forward_hist , std::vector< std::vector< std::vector< TH1F* > > > &forward_gen_hist, int ptbin , int etabin);
void histWidthAsym( std::vector<std::vector<std::vector<TH1F*> > > &Asymmetry , std::vector<std::vector<std::vector<double> > > &Widths, std::vector<std::vector<std::vector<double> > > &WidthsError , bool fill_all );
void histWidthMCTruth( std::vector<std::vector<std::vector<TH1F*> > > &Asymmetry , std::vector<std::vector<std::vector<double> > > &Widths, std::vector<std::vector<std::vector<double> > > &WidthsError );
void def_fill_widths( TString name1, std::vector< std::vector< TH1F* > > &widths_FE1 , std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError);
void histLinFE( std::vector< std::vector< TH1F* > > widths_hist_all , std::vector< std::vector< double > > &Widths, std::vector< std::vector< double > > &WidthsError, int ptbin );
void histLinAsym( std::vector< std::vector< TH1F* > > widths_hist_all , std::vector< std::vector< double > > &Widths, std::vector< std::vector< double > > &WidthsError );


void make_lin_fit(double & slope, double & d_slope, double & offset, double & d_offset);
void chi2_linear(Int_t& npar, Double_t* grad, Double_t& fval, Double_t* p, Int_t status);






void histLoadAsym( TFile &f, bool data, TString name, std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< TH1F* > > > &GenAsymmetry , int ptbin, int etabin ){
  int shift = 0;
  if ( etabin == 10 )	shift = 1;
  if ( etabin == 8 )	shift = 2;
  if ( etabin == 3 )	shift = 10;

  for( int m = 0; m < etabin; m++ ){
    std::vector< std::vector< TH1F* > > temp2;
    std::vector< std::vector< TH1F* > > temp2gen;
    for( int p = 0; p < ptbin; p++ ){
      std::vector< TH1F* > temp1;
      std::vector< TH1F* > temp1gen;
      for( int r = 0; r < 6; r++ ){
        TString name_fw = name;
        TString name_fwgen = "gen_"; name_fwgen += name;
        name_fw += m+shift; name_fw += "_pt"; name_fw += p+1; name_fw += "_alpha"; name_fw += r+1;
        name_fwgen += m+shift; name_fwgen += "_pt"; name_fwgen += p+1; name_fwgen += "_alpha"; name_fwgen += r+1;
        temp1.push_back( ( TH1F* )f.Get( name_fw ) );
        if( data == false ) temp1gen.push_back( ( TH1F* )f.Get( name_fwgen ) );
      }
      temp2.push_back(temp1);
      if( data == false ) temp2gen.push_back(temp1gen);
    }
    Asymmetry.push_back(temp2);
    if( data == false ) GenAsymmetry.push_back(temp2gen);
  }
}


void histLoadFE1( TFile &f, bool data, TString name, std::vector< std::vector< std::vector< TH1F* > > > &forward_hist , std::vector< std::vector< std::vector< TH1F* > > > &forward_gen_hist , int ptbin , int etabin){
  int shift = 0;
  if ( etabin == 8 )	shift = 2;
  if ( etabin == 3 )	shift = 10; // it should be 10, i screwed a bit up with the hist preparation, it;s fixed

  for( int m = 0; m < etabin; m++ ){
    std::vector< std::vector< TH1F* > > temp2;
    std::vector< std::vector< TH1F* > > temp2gen;

    for( int p = 0; p < ptbin; p++ ){
      std::vector< TH1F* > temp1;
      std::vector< TH1F* > temp1gen;
      for( int r = 0; r < 6; r++ ){
        TString name_fw = name;
        TString name_fwgen = "gen_"; name_fwgen += name;
        name_fw += shift+m; 	name_fw += "_pt"; name_fw += p+1; name_fw += "_alpha"; name_fw += r+1;
        name_fwgen += shift+m; 	name_fwgen += "_pt"; name_fwgen += p+1; name_fwgen += "_alpha"; name_fwgen += r+1;
        temp1.push_back( ( TH1F* )f.Get( name_fw ) );
        if( data == false ) temp1gen.push_back( ( TH1F* )f.Get( name_fwgen ) );
      }
      temp2.push_back(temp1);
      if( data == false ) temp2gen.push_back(temp1gen);
    }
    forward_hist.push_back(temp2);
    if( data == false ) forward_gen_hist.push_back(temp2gen);
  }
}


void histMeanPt( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< double > > > &Widths ){
  for( unsigned int m = 0; m < Asymmetry.size(); m++ ){
    std::vector< std::vector< double > > temp1;
    for( unsigned int p = 0; p < Asymmetry.at(m).size(); p++ ){
      std::vector< double > temp2;
      for( unsigned int r = 0; r < Asymmetry.at(m).at(p).size(); r++ ){
        temp2.push_back( (*Asymmetry.at(m).at(p).at(r)).GetMean() );
      }
      temp1.push_back(temp2);
    }
    Widths.push_back(temp1);
  }
}

void histWidthMCTruth( std::vector<std::vector<std::vector<TH1F*> > > &Asymmetry , std::vector<std::vector<std::vector<double> > > &Widths, std::vector<std::vector<std::vector<double> > > &WidthsError )
{
  double asym;
  double asymerr;

  for( unsigned int m = 0; m < Asymmetry.size(); m++ )
  {
    std::vector< std::vector< double > > temp2;
    std::vector< std::vector< double > > temp_error2;
    for( unsigned int p = 0; p < Asymmetry.at(m).size() ; p++ )
    {
      std::vector< double > temp1;
      std::vector< double > temp_error1;
      for( unsigned int r = 0; r < Asymmetry.at(m).at(p).size(); r++ )
      {
        asym    = ((*Asymmetry.at(m).at(p).at(r)).GetRMS())/TMath::Sqrt(2);
        asymerr = ((*Asymmetry.at(m).at(p).at(r)).GetRMSError())/TMath::Sqrt(2);

        temp1.push_back( asym );
        temp_error1.push_back( asymerr );
        //				std::cout << "eta " << m+1 << " pt " << p+1 << " alpha " << r+1 << ", width " << asym << std::endl << std::endl;
        //				std::cout << "eta " << m+1 << " pt " << p+1 << " alpha " << r+1 << ", error " << asymerr << std::endl << std::endl;
      }
      temp2.push_back(temp1);
      temp_error2.push_back(temp_error1);
    }
    Widths.push_back(temp2);
    WidthsError.push_back(temp_error2);
  }
}



void histWidthAsym( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< double > > > &Widths, std::vector< std::vector< std::vector< double > > > &WidthsError , bool fill_all )
{
  double yq_IQW[2],xq_IQW[2];
  double asym;
  double asymerr;
  // Decide where to truncate
  xq_IQW[0] = 0.0;
  xq_IQW[1] = truncate_fraction;
  std::sprintf(truncation_info, "truncation_info_%f.txt", truncate_fraction);
  //cout << truncation_info << endl;

  //ofstream myfile;
  //myfile.open(truncation_info);
  //myfile << "Truncated at" << truncate_fraction << "\n";
  //myfile.close();

  for( unsigned int m = 0; m < Asymmetry.size(); m++ )
  {
    std::vector< std::vector< double > > temp2;
    std::vector< std::vector< double > > temp_error2;
    for( unsigned int p = 0; p < Asymmetry.at(m).size() ; p++ )
    {
      std::vector< double > temp1;
      std::vector< double > temp_error1;
      for( unsigned int r = 0; r < Asymmetry.at(m).at(p).size(); r++ )
      {
        // std::cout << "eta pt alpha bins " << m << ", " << p << ", " << r << std::endl;
        asym = GetAsymmWidth( Asymmetry.at(m).at(p).at(r) , xq_IQW, yq_IQW, m, p, r);
        asymerr = asym/TMath::Sqrt(2*Asymmetry.at(m).at(p).at(r)->GetEffectiveEntries());

        //				double norm = Asymmetry.at(m).at(p).at(r) -> Integral();
        //				Asymmetry.at(m).at(p).at(r) -> Scale(1/norm, "width");
        //
        //				TF1 * myfit = new TF1( "myfit", "[0]*exp(-0.5*TMath::Power(((x-[1])/[2]),2))", 0, 0.15 );
        //
        //				myfit -> SetParameter( 0, Asymmetry.at(m).at(p).at(r)->GetBinContent(2) );
        //				myfit -> FixParameter( 1, 0 );//I fix the mean value (center of gaus distribution) at 0
        //				myfit -> SetParameter( 2, asym );
        //
        //				myfit -> SetRange( 0, 2*asym );
        //
        //				Asymmetry.at(m).at(p).at(r) -> Fit( "myfit", "+RQ" );
        //
        //				asym		= myfit -> GetParameter( 2 );
        //				asymerr	= myfit -> GetParError( 2 );


        if(!fill_all){
          if (Asymmetry.at(m).size()==9){
            if( p == 0 && r < 4) { asym = 0.; asymerr = 0.; };
            if( p == 1 && r < 3) { asym = 0.; asymerr = 0.; };
            if( p == 2 && r < 2) { asym = 0.; asymerr = 0.; };
            if( p == 3 && r < 1) { asym = 0.; asymerr = 0.; };
            if( p == 4 && r < 1) { asym = 0.; asymerr = 0.; };
          }
          if (Asymmetry.at(m).size()==6){
            if( p == 0 && r < 3) { asym = 0.; asymerr = 0.; };
            if( p == 1 && r < 2) { asym = 0.; asymerr = 0.; };
            if( p == 2 && r < 1) { asym = 0.; asymerr = 0.; };
            if( p == 3 && r < 1) { asym = 0.; asymerr = 0.; };
          }
          if (Asymmetry.size()==10){
            if( m == 6 && p == 8 && r == 0) { asym = 0.; asymerr = 0.; };
            if( m == 7 && p == 8 && r == 0) { asym = 0.; asymerr = 0.; };
            if( m == 8 && p == 5 && r == 0) { asym = 0.; asymerr = 0.; };
            if( m == 8 && p > 5 ) { asym = 0.; asymerr = 0.; };
            if( m == 9 && p > 4 ) { asym = 0.; asymerr = 0.; };
          }
          if (Asymmetry.size()==11){
            if( m == 9 && p == 5 && r == 0) { asym = 0.; asymerr = 0.; };
          }
        }
        temp1.push_back( asym );
        temp_error1.push_back( asymerr );
        //				std::cout << "eta " << m+1 << " pt " << p+1 << " alpha " << r+1 << ", " << asym << std::endl << std::endl;
        //				std::cout << "eta " << m+1 << " pt " << p+1 << " alpha " << r+1 << ", " << asymerr << std::endl << std::endl;

        /*
        TF1 * myfit = new TF1( "myfit", "[0]*exp(-0.5*TMath::Power(((x-[1])/[2]),2))", 0, 0.15 );

        const int nq = 2;
        Asymmetry.at(m).at(p).at(r)->GetQuantiles(nq,yq_IQW,xq_IQW);

        double norm = Asymmetry.at(m).at(p).at(r)->Integral();
        if ( norm != 0 ) Asymmetry.at(m).at(p).at(r)->Scale( 1/norm );

        myfit -> SetParameter( 0, 0.1 );
        myfit -> SetParameter( 1, 0 );
        myfit -> SetParameter( 2, GetAsymmWidth( Asymmetry.at(m).at(p).at(r) , xq_IQW, yq_IQW, m, p, r) );
        myfit -> FixParameter( 2, GetAsymmWidth( Asymmetry.at(m).at(p).at(r) , xq_IQW, yq_IQW, m, p, r) );
        myfit -> FixParameter( 1, 0 );//I fix the mean value (center of gaus distribution) at 0

        myfit -> SetRange( 0, yq_IQW[1]);

        Asymmetry.at(m).at(p).at(r) -> Fit( "myfit", "+RQ" );

        TFile hplaprim("asymmetries.root","UPDATE");

        Asymmetry.at(m).at(p).at(r) -> Write();

        hplaprim.Close();
        */
      }
      temp2.push_back(temp1);
      temp_error2.push_back(temp_error1);
    }
    Widths.push_back(temp2);
    WidthsError.push_back(temp_error2);
  }
}


void correctPLIbinbybin(std::vector< std::vector< std::vector< double > > > &Output, std::vector< std::vector< std::vector< double > > > &OutputError, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > PLI, std::vector< std::vector< std::vector< double > > > PLIError)
{
  double temp, tempError;

  for( unsigned int i = 0; i < Widths.size(); i++ )
  {
    std::vector< std::vector< double > > temp2;
    std::vector< std::vector< double > > temp_error2;
    for( unsigned int j = 0; j < Widths.at(i).size(); j++ )
    {
      std::vector< double > temp1;
      std::vector< double > temp_error1;
      for( unsigned int k = 0; k < Widths.at(i).at(j).size(); k++ )
      {
        temp = TMath::Sqrt( Widths.at(i).at(j).at(k) * Widths.at(i).at(j).at(k) - PLI.at(i).at(j).at(k) * PLI.at(i).at(j).at(k) );
        tempError = ( Widths.at(i).at(j).at(k) * WidthsError.at(i).at(j).at(k) + PLI.at(i).at(j).at(k) * PLIError.at(i).at(j).at(k) )/temp;
        temp1.push_back( temp );
        temp_error1.push_back( tempError );
      }
      temp2.push_back(temp1);
      temp_error2.push_back(temp_error1);
    }
    Output.push_back(temp2);
    OutputError.push_back(temp_error2);
  }
}


void fill_PLI_hist( TString name1, std::vector< std::vector< TH1F* > > &widths_FE1 , std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning)
{
  int shift;

  if (Widths.size() == 4 ) shift = 1;
  if (Widths.size() == 6 ) shift = 2;

  for( unsigned int m = 0; m <  Widths.size(); m++ )
  {
    std::vector< TH1F* > temp1;

    for( unsigned int r = 0; r <  Widths.at(m).at(0).size(); r++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += m+shift; name_width_fe1 += "_alpha"; name_width_fe1 += r+1;
      TH1F *h1 = new TH1F( name_width_fe1, name_width_fe1, 1000, 0, 1000 );
      h1 ->GetYaxis()->SetTitle("#sigma_{A,PLI}");	h1 ->GetXaxis()->SetTitle("p_{T}");	h1 -> Sumw2();

      for( unsigned int p = 0; p < Widths.at(m).size(); p++ )
      {
        if( !(TMath::IsNaN(Widths.at(m).at(p).at(r))) ) h1 -> SetBinContent( h1 -> FindBin( pt_binning.at(m).at(p).at(r) ), Widths.at(m).at(p).at(r) );
        if( !(TMath::IsNaN(WidthsError.at(m).at(p).at(r))) ) h1 -> SetBinError( h1 -> FindBin( pt_binning.at(m).at(p).at(r) ), WidthsError.at(m).at(p).at(r) );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0.8*(h1 -> GetMinimum()), 1.2*(h1 -> GetMaximum()) );
      temp1.push_back(h1);
    }
    widths_FE1.push_back(temp1);
  }
}


void fit_NSC_PLI( std::vector< std::vector< TF1* > > &functions, std::vector< std::vector< TH1F* > > PLI_hists )
{
  for( unsigned int m = 0; m <  PLI_hists.size(); m++ )
  {
    std::vector< TF1* > temp1;

    for( unsigned int r = 0; r <  PLI_hists.at(m).size(); r++ )
    {
      TString name_width_fe1 = "NSCfun";
      name_width_fe1 += m+1; name_width_fe1 += "_alpha"; name_width_fe1 += r+1;
      TF1 * NSCfun = new TF1( name_width_fe1, "TMath::Sqrt(TMath::Power(([0]/x),2)+TMath::Power([1],2)/x + TMath::Power([2],2))", 0, 1000 );
      NSCfun->SetParName(0,"N");	NSCfun->SetParName(1,"S");	NSCfun->SetParName(2,"C");

      PLI_hists.at(m).at(r)-> Fit(name_width_fe1,"");

      NSCfun ->GetYaxis()-> SetRangeUser( 0.8*(NSCfun -> GetMinimum()), 1.2*(NSCfun -> GetMaximum()) );
      temp1.push_back(NSCfun);
    }
    functions.push_back(temp1);
  }
}


void def_fill_PLI( TString name1, std::vector< std::vector< TH1F* > > &widths_FE1 , std::vector< std::vector< TF1* > > functions, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning )
{
  double aMax[] = { 0.05, 0.1, 0.15, 0.2, 0.25, 0.3 };

  for( unsigned int m = 0; m < pt_binning.size(); m++ )
  {
    std::vector< TH1F* > temp1;

    for( unsigned int p = 0; p < pt_binning.at(m).size(); p++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += "_eta"; name_width_fe1 += m+1; name_width_fe1 += "_pt"; name_width_fe1 += p+1;
      TH1F *h1 = new TH1F( name_width_fe1, name_width_fe1, 50, 0, 0.35 );
      h1 ->GetYaxis()->SetTitle("#sigma_{A}");	h1 ->GetXaxis()->SetTitle("#alpha_{max}");
      h1 -> Sumw2();
      for( unsigned int r = 0; r < pt_binning.at(m).at(p).size(); r++ )
      {
        h1 -> SetBinContent( h1 -> FindBin( aMax[ r ] ), functions.at(m).at(r)->Eval( pt_binning.at(m).at(p).at(r) ) );
        h1 -> SetBinError( h1 -> FindBin( aMax[ r ] ), WidthsError.at(m).at(p).at(r) );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0.8*(h1 -> GetMinimum()), 1.2*(h1 -> GetMaximum()) );
      temp1.push_back(h1);
    }
    widths_FE1.push_back(temp1);
  }
}


void def_fill_widths( TString name1, std::vector< std::vector< TH1F* > > &widths_FE1 , std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError)
{
  double aMax[] = { 0.05, 0.1, 0.15, 0.2, 0.25, 0.3 };
  double temp;

  for( unsigned int m = 0; m < Widths.size(); m++ )
  {
    std::vector< TH1F* > temp1;

    for( unsigned int p = 0; p < Widths.at(m).size(); p++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += "_eta";
      if (name1.Contains("_fe")) { name_width_fe1 += m+2; }
      else{ name_width_fe1 += m+1; }
      name_width_fe1 += "_pt"; name_width_fe1 += p+1;
      TH1F *h1 = new TH1F( name_width_fe1, name_width_fe1, 50, 0, 0.35 );
      h1 ->GetYaxis()->SetTitle("#sigma_{A}");	h1 ->GetXaxis()->SetTitle("#alpha_{max}"); h1 ->GetYaxis()->SetTitleOffset(1.);
      h1 -> Sumw2();
      for( unsigned int r = 0; r < Widths.at(m).at(p).size(); r++ )
      {
        temp = Widths.at(m).at(p).at(r);
        if( !(TMath::IsNaN(temp)) && temp != 0) h1 -> SetBinContent( h1 -> FindBin( aMax[ r ] ), Widths.at(m).at(p).at(r) );
        if( !(TMath::IsNaN(temp)) && temp != 0) h1 -> SetBinError( h1 -> FindBin( aMax[ r ] ), WidthsError.at(m).at(p).at(r) );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0., 0.3 );
      temp1.push_back(h1);
    }
    widths_FE1.push_back(temp1);
  }
}


void correctForPLI( TString name1, std::vector< std::vector< TH1F* > > widths_FE1, std::vector< std::vector< TH1F* > > &output, std::vector< std::vector< TF1* > > functions, std::vector< std::vector< std::vector< double > > > pt_binning)
{
  double aMax[] = { 0.05, 0.1, 0.15, 0.2, 0.25, 0.3 };

  double temp, temp_error;
  double Ref, Ext, RefError;

  for( unsigned int m = 0; m < widths_FE1.size(); m++ )
  {
    std::vector< TH1F* > temp1;
    for( unsigned int p = 0; p < widths_FE1.at(m).size(); p++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += "_eta"; name_width_fe1 += m+1; name_width_fe1 += "_pt"; name_width_fe1 += p+1;
      TH1F *h1 = new TH1F( name_width_fe1, name_width_fe1, 50, 0, 0.35 );
      h1 ->GetYaxis()->SetTitle("#sigma_{A}");	h1 ->GetXaxis()->SetTitle("#alpha_{max}");
      h1 -> Sumw2();
      for( unsigned int r = 0 ; r < pt_binning.at(m).at(p).size(); r++ )
      {
        Ref = widths_FE1.at(m).at(p)->GetBinContent( h1 -> FindBin( aMax[ r ] ) );
        Ext = functions.at(m).at(r)->Eval(pt_binning.at(m).at(p).at(r));

        RefError = widths_FE1.at(m).at(p)->GetBinError( h1 -> FindBin( aMax[ r ] ) );

        temp = TMath::Sqrt( Ref*Ref - Ext*Ext );
        temp_error = (2*Ext*RefError + 2*Ref*RefError )/temp;

        //				if (temp != 0) std::cout << "before " << Ref << ", corrected " << temp << std::endl;

        if( !(TMath::IsNaN(temp)) && temp != 0) h1 -> SetBinContent( h1 -> FindBin( aMax[ r ] ) , temp );
        if( !(TMath::IsNaN(temp)) && temp != 0) h1 -> SetBinError( h1 -> FindBin( aMax[ r ] ) , temp_error );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0.8*(h1 -> GetMinimum()), 1.2*(h1 -> GetMaximum()) );
      temp1.push_back(h1);
    }
    output.push_back(temp1);
  }
}


void histLinFit( std::vector< std::vector< TH1F* > > widths_hist_all , std::vector< std::vector< double > > &Widths, std::vector< std::vector< double > > &WidthsError  )
{
  for( unsigned int m = 0; m < widths_hist_all.size(); m++ )
  {
    std::vector<double> temp2;
    std::vector<double> temp_error2;
    for( unsigned int p = 0; p < widths_hist_all.at(m).size(); p++ )
    {
      double value, error;
      if( widths_hist_all.at(m).at(p)->GetEntries() != 0 )
      {
        fitLin( *( widths_hist_all.at(m).at(p) ), value, error );
        if( p==0 ) {value = 0; error = 0;}
        if( m == 10 && p==5 && widths_hist_all.size() == 11) {value = 0; error = 0;}
        temp2.push_back(value);
        temp_error2.push_back(error);
      }
      else
      {
        temp2.push_back(0.);
        temp_error2.push_back(0.);
      }

    }
    Widths.push_back(temp2);
    WidthsError.push_back(temp_error2);
  }
}


void histLinCorFit( std::vector< std::vector< std::vector< double > > > Widths,
  std::vector< std::vector< std::vector< double > > > WidthsError,
  std::vector< std::vector< TGraphErrors* > > &output_graph,
  std::vector< std::vector< double > > &output,
  std::vector< std::vector< double > > &output_error )
  {


    for( unsigned int m = 0; m < Widths.size(); m++ )// eta loop
    {
      std::vector<TGraphErrors*> temp2_graph;
      std::vector<double> temp2;
      std::vector<double> temp_error2;
      for( unsigned int p = 0; p < Widths.at(m).size(); p++ )// p_T loop
      {
        std::vector<float> alpha;
        int no_skip_pts;

        alpha.push_back(0.05);
        alpha.push_back(0.1);
        alpha.push_back(0.15);
        alpha.push_back(0.20);
        alpha.push_back(0.25);
        alpha.push_back(0.3);

        std::vector<double> x,x_e;
        for(int ialpha=0; ialpha < 6; ++ialpha){
          x.push_back(alpha.at(ialpha));
          x_e.push_back(0.);
        }
        TMatrixD y_cov_mc;
        y_cov_mc.ResizeTo(alpha.size(), alpha.size());

        for(unsigned int ialpha=0; ialpha < alpha.size(); ++ialpha){
          for (unsigned int jalpha =0; jalpha < alpha.size(); jalpha++){
            if( ialpha <= jalpha ) {
              double n1_mc = pow(Widths.at(m).at(p).at(ialpha),2)/(2*pow(WidthsError.at(m).at(p).at(ialpha),2));
              double n2_mc = pow(Widths.at(m).at(p).at(jalpha),2)/(2*pow(WidthsError.at(m).at(p).at(jalpha),2));

              y_cov_mc(ialpha, jalpha) = pow(WidthsError.at(m).at(p).at(ialpha),2) * pow((n1_mc/n2_mc),2)*
              (Widths.at(m).at(p).at(ialpha)/Widths.at(m).at(p).at(jalpha));
            }
            else {
              double n1_mc = pow(Widths.at(m).at(p).at(jalpha),2)/(2*pow(WidthsError.at(m).at(p).at(jalpha),2));
              double n2_mc = pow(Widths.at(m).at(p).at(ialpha),2)/(2*pow(WidthsError.at(m).at(p).at(ialpha),2));

              y_cov_mc(ialpha, jalpha) = pow(WidthsError.at(m).at(p).at(jalpha),2) * pow((n1_mc/n2_mc),2)*
              (Widths.at(m).at(p).at(jalpha)/Widths.at(m).at(p).at(ialpha));
            }
          }
        }

        //create TGraphErrors from previously defined vectors
        TGraphErrors* extrapol_MC = new TGraphErrors(alpha.size(),&x[0],&Widths.at(m).at(p).at(0),&x_e[0],&WidthsError.at(m).at(p).at(0));
        TString name_test = "Graph_eta";
        name_test+=m; name_test+="_pt"; name_test+=p;
        extrapol_MC->SetName(name_test);

        // fit linear extrapolation function
        TF1 *lin_extrapol_mc = new TF1("lin_extrapol_mc","[0]+[1]*x",0,alpha.back()+0.05);

        // fit
        data.reset();
        data.x_val = x;
        data.y_val = Widths.at(m).at(p);
        data.y_cov.ResizeTo(alpha.size(), alpha.size());
        data.y_cov = y_cov_mc;
        data.CheckPoints();
        // choose start values for the fit
        double slope = (Widths.at(m).at(p).at(Widths.at(m).at(p).size()-1) - Widths.at(m).at(p).at(Widths.at(m).at(p).size()-3))/(x.at(x.size()-1) - x.at(x.size()-3));
        double d_slope = slope;
        double offset = Widths.at(m).at(p).at(Widths.at(m).at(p).size()-1) - (slope*x.at(x.size()-1));
        double d_offset = offset;


        //         std::cout << "eta: " << m <<  ", p_T: " << p << std::endl;
        //         std::cout << "fit start values: " << "slope: " << slope << " offset: " << offset << std::endl;
        make_lin_fit(slope, d_slope, offset, d_offset);
        //         std::cout << "fit values: " << "slope: " << slope << " offset: " << offset << std::endl;


        lin_extrapol_mc->SetParameter(0, offset);
        lin_extrapol_mc->SetParError(0, d_offset);
        lin_extrapol_mc->SetParameter(1, slope);
        lin_extrapol_mc->SetParError(1, d_slope);
        extrapol_MC->GetListOfFunctions()->Add(lin_extrapol_mc);

        if( p == 0 ){
          temp2.push_back(0.);
          temp_error2.push_back(0.);
          temp2_graph.push_back(extrapol_MC);
        } else{
          temp2.push_back(offset);
          temp_error2.push_back(d_offset);
          temp2_graph.push_back(extrapol_MC);
        }

      }
      output.push_back(temp2);
      output_error.push_back(temp_error2);
      output_graph.push_back(temp2_graph);
    }
  }


  ///////////////////void ComparisonPlots( std::vector< std::vector< double > > &input1,
  ///////////////////                      std::vector< std::vector< double > > &input1_error,
  ///////////////////                      std::vector< std::vector< double > > &input2,
  ///////////////////                      std::vector< std::vector< double > > &input2_error )
  ///////////////////{
  ///////////////////
  ///////////////////
  ///////////////////	for( unsigned int m = 0; m < Widths.size(); m++ ){// eta loop
  ///////////////////      for( unsigned int p = 0; p < Widths.at(m).size(); p++ ){// p_T loop
  ///////////////////         TH1F
  ///////////////////		}
  ///////////////////	}
  ///////////////////}


  // --------------------------------- //
  std::pair <float,float> determineMinMax(TGraphErrors* graph)
  {
    std::pair <float,float> minMaxPair(graph->GetY()[0],graph->GetY()[0]);
    for(Int_t i=0;i<graph->GetN();i++){
      if(graph->GetY()[i]<minMaxPair.first)minMaxPair.first=graph->GetY()[i];
      if(graph->GetY()[i]>minMaxPair.second)minMaxPair.second=graph->GetY()[i];
    }
    return minMaxPair;
  }


  void fill_widths_pt( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<std::vector<double> > > forward_width_pt )
  {
    for( unsigned int m = 0; m < Widths.size(); m++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += "_eta"; name_width_fe1 += m+1;

      TH1F *hist = new TH1F( name_width_fe1, name_width_fe1, 1500, 0, 1500 );
      hist ->GetYaxis()->SetTitle("k_{FSR}");
      hist ->GetXaxis()->SetTitle("p_{T}");
      hist -> GetYaxis()->SetRangeUser( 0, 0.4 );

      for( unsigned int p = 0; p < Widths.at(m).size(); p++ )
      {
        if ( forward_width_pt.at(m).at(p).at(0)!= 0 )
        {
          hist -> SetBinContent( hist -> FindBin( forward_width_pt.at(m).at(p).at(0) ), Widths.at(m).at(p) );
          hist -> SetBinError( hist -> FindBin( forward_width_pt.at(m).at(p).at(0) ), WidthsError.at(m).at(p) );
        }
      }
      hist ->GetYaxis()-> SetRangeUser( 0.8*(hist -> GetMinimum()), 1.2*(hist -> GetMaximum()) );
      widths.push_back(hist);
    }
  }


  void KFSR_ratios( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<double> > WidthsTwo, std::vector<std::vector<double> > WidthsTwoError, std::vector<std::vector<std::vector<double> > > forward_width_pt )
  {
    double temp, tempError;

    for( unsigned int m = 0; m < Widths.size(); m++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += "_eta"; name_width_fe1 += m+1;

      TH1F *hist = new TH1F( name_width_fe1, name_width_fe1, 1500, 0, 1500 );

      hist ->GetYaxis()->SetTitle("k_{FSR}^{" + labeldata + "}/k_{FSR}^{MC}");
      //hist ->GetYaxis()->SetTitle("k_{FSR}^{Smeared MC}/k_{FSR}^{MC}");
      hist ->GetXaxis()->SetTitle("p_{T}");
      hist -> GetYaxis()->SetRangeUser( 0, 2. );

      for( unsigned int p = 0; p < Widths.at(m).size(); p++ )
      {
        if ( forward_width_pt.at(m).at(p).at(0)!= 0 )
        {
          temp = Widths.at(m).at(p)/WidthsTwo.at(m).at(p);
          tempError = WidthsError.at(m).at(p)/WidthsTwo.at(m).at(p) + ( Widths.at(m).at(p) * WidthsTwoError.at(m).at(p) ) / ( WidthsTwo.at(m).at(p) * WidthsTwo.at(m).at(p) ) ;

          hist -> SetBinContent( hist -> FindBin( forward_width_pt.at(m).at(p).at(0) ), temp );
          hist -> SetBinError( hist -> FindBin( forward_width_pt.at(m).at(p).at(0) ), tempError );
        }
      }
      widths.push_back(hist);
    }
  }


  void widths_015_ratios( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > WidthsTwo, std::vector<std::vector<std::vector<double> > > WidthsTwoError, std::vector<std::vector<std::vector<double> > > forward_width_pt )
  {
    double temp, tempError;

    for( unsigned int m = 0; m < Widths.size(); m++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += "_eta"; name_width_fe1 += m+1;

      TH1F *hist = new TH1F( name_width_fe1, name_width_fe1, 1500, 0, 1500 );
      hist ->GetYaxis()->SetTitle();
      hist ->GetXaxis()->SetTitle("p_{T}");
      hist -> GetYaxis()->SetRangeUser( 0, 2. );

      for( unsigned int p = 0; p < Widths.at(m).size(); p++ )
      {
        if ( forward_width_pt.at(m).at(p).at(2)!= 0 )
        {
          temp = Widths.at(m).at(p).at(2)/WidthsTwo.at(m).at(p).at(2);
          tempError = WidthsError.at(m).at(p).at(2)/WidthsTwo.at(m).at(p).at(2) + ( Widths.at(m).at(p).at(2) * WidthsTwoError.at(m).at(p).at(2) ) / ( WidthsTwo.at(m).at(p).at(2) * WidthsTwo.at(m).at(p).at(2) ) ;

          hist -> SetBinContent( hist -> FindBin( forward_width_pt.at(m).at(p).at(2) ), temp );
          hist -> SetBinError( hist -> FindBin( forward_width_pt.at(m).at(p).at(2) ), tempError );
        }
      }
      widths.push_back(hist);
    }
  }


  void KNFS_line_fit( TString name1, std::vector<TH1F*> widths, std::vector< TF1* > &functions )
  {
    for( unsigned int m = 0; m < widths.size(); m++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += "_eta"; name_width_fe1 += m+1;

      TF1 * linfit = new TF1( "linfit", "[0]+x*[1]", 0, 0.25 );

      widths.at(m)-> Fit("linfit","Q");

      functions.push_back(linfit);
    }
  }


  void correctKFSR( std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<TF1*> functions, std::vector<std::vector<std::vector<double> > > forward_width_pt )
  {
    for( unsigned int i = 0; i < Widths.size(); i++ )
    {
      std::vector< double > temp2;
      std::vector< double > temp_error2;

      for( unsigned int j = 0; j < Widths.at(i).size(); j++ )
      {
        double temp1;
        double temp_error1;

        temp1 = TMath::Sqrt(2) * ( Widths.at(i).at(j).at(2) - 0.2 * functions.at(i)->Eval( forward_width_pt.at(i).at(j).at(2) ) );
        temp_error1 = TMath::Sqrt(2) * ( Widths.at(i).at(j).at(2) * WidthsError.at(i).at(j).at(2) +	0.2*functions.at(i)->Eval( forward_width_pt.at(i).at(j).at(2) )*( forward_width_pt.at(i).at(j).at(2)*functions.at(i)->GetParError(1) + functions.at(i)->GetParError(0) ) ) ;

        temp2.push_back(temp1);
        temp_error2.push_back(temp_error1);
      }
      Output.push_back(temp2);
      OutputError.push_back(temp_error2);
    }
  }


  void correctJERwithPLI(std::vector< std::vector< double > > &Output, std::vector< std::vector< double > > &OutputError, std::vector< std::vector< double > > Widths, std::vector< std::vector< double > > WidthsError, std::vector< std::vector< double > > PLI, std::vector< std::vector< double > > PLIError)
  {
    for( unsigned int i = 0; i < Widths.size(); i++ )
    {
      std::vector< double > temp2;
      std::vector< double > temp_error2;

      for( unsigned int j = 0; j < Widths.at(i).size(); j++ )
      {
        double temp;
        double temp_error;

        if(Widths.at(i).at(j) != 0. )
        {
          // Without PLI correction (change also alpha=015):
          //temp = TMath::Sqrt( Widths.at(i).at(j) * Widths.at(i).at(j));
          //temp_error = ( Widths.at(i).at(j) * WidthsError.at(i).at(j))/temp;
          //std::cout << "Warning: not applying (0) PLI correction!" << std::endl;

          // PLI only (change also alpha=015):
          //temp = TMath::Sqrt( PLI.at(i).at(j) * PLI.at(i).at(j) );
          //temp_error = ( PLI.at(i).at(j) * PLIError.at(i).at(j) )/temp;
          //std::cout << "Warning: plotting only (0) PLI correction!" << std::endl;

          // With PLI correction (change also alpha=015):
          temp = TMath::Sqrt( Widths.at(i).at(j) * Widths.at(i).at(j) - PLI.at(i).at(j) * PLI.at(i).at(j) );
          temp_error = ( Widths.at(i).at(j) * WidthsError.at(i).at(j) + PLI.at(i).at(j) * PLIError.at(i).at(j) )/temp;
        }
        else
        {
          temp = 0.;
          temp_error = 0.;
        }

        //			std::cout << "reco " << Widths.at(i).at(j) << ", pli " << PLI.at(i).at(j) << ", corrected width " << temp << std::endl;

        if( TMath::IsNaN(temp) ) { temp = 0. ; temp_error = 0.; }

        temp2.push_back(temp);
        temp_error2.push_back(temp_error);
      }
      Output.push_back(temp2);
      OutputError.push_back(temp_error2);
    }
  }


  void correctJERwithPLI015(std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > PLI, std::vector<std::vector< std::vector< double > > > PLIError)
  {
    for( unsigned int i = 0; i < Widths.size(); i++ )
    {
      std::vector< double > temp2;
      std::vector< double > temp_error2;

      for( unsigned int j = 0; j < Widths.at(i).size(); j++ )
      {
        double temp;
        double temp_error;

        // Without PLI correction (change also alpha=0):
        //temp = TMath::Sqrt( Widths.at(i).at(j).at(2) * Widths.at(i).at(j).at(2));
        //temp_error = ( Widths.at(i).at(j).at(2) * WidthsError.at(i).at(j).at(2))/temp;
        //std::cout << "Warning: not applying (015) PLI correction!" << std::endl;

        // PLI only (change also alpha=0):
        //temp = TMath::Sqrt( PLI.at(i).at(j).at(2) * PLI.at(i).at(j).at(2) );
        //temp_error = ( PLI.at(i).at(j).at(2) * PLIError.at(i).at(j).at(2) )/temp;
        //std::cout << "Warning: plotting only (015) PLI correction!" << std::endl;

        // With PLI correction (change also alpha=0):
        temp = TMath::Sqrt( Widths.at(i).at(j).at(2) * Widths.at(i).at(j).at(2) - PLI.at(i).at(j).at(2) * PLI.at(i).at(j).at(2) );
        temp_error = ( Widths.at(i).at(j).at(2) * WidthsError.at(i).at(j).at(2) + PLI.at(i).at(j).at(2) * PLIError.at(i).at(j).at(2) )/temp;

        //			std::cout << "reco " << Widths.at(i).at(j).at(2) << ", pli " << PLI.at(i).at(j).at(2) << ", corrected width " << temp << std::endl;

        if( TMath::IsNaN(temp) ) { temp = 0 ; temp_error = 0; }

        temp2.push_back(temp);
        temp_error2.push_back(temp_error);
      }
      Output.push_back(temp2);
      OutputError.push_back(temp_error2);
    }
  }


  void correctForRef( TString name1, std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<double> > Input, std::vector<std::vector<double> > InputError, std::vector<std::vector<std::vector<double> > > width_pt  )
  {
    double sqRef , sqProbe;
    double Ref, Probe, RefError, ProbeError;
    double N, S, C, sN, sS, sC, pT;

    //        std::cout << "WARNING size of vectors input         : " << Input.size()      << " x " << Input.at(0).size()      << std::endl;
    //        std::cout << "WARNING size of vectors input error   : " << InputError.size() << " x " << InputError.at(0).size() << std::endl;
    //        std::cout << "WARNING size of vectors input width_pt: " << width_pt.size()   << " x " << width_pt.at(0).size()   << " x " << width_pt.at(0).at(0).size() << std::endl;
    gStyle ->SetOptFit(0);
    TString name_width_fe1 = name1;
    //	TF1 * NSCfun = new TF1( name_width_fe1, "TMath::Sqrt(TMath::Power(([0]/x),2)+TMath::Power([1],2)/x + TMath::Power([2],2))", 0, 1000 );
    TF1 * NSCfun = new TF1( name_width_fe1, "TMath::Sqrt([0]*[0]/(x*x)+[1]*[1]/x + [2]*[2])", 0, 1000 );
    //	NSCfun->SetParName(0,"N");	NSCfun->SetParName(1,"S");	NSCfun->SetParName(2,"C");
    if(name1=="data_cor_correct"){
      NSCfun->SetParameter(0,3);	NSCfun->SetParameter(1,1);	NSCfun->SetParameter(2,0.05);
    }

    TH1F *hist = new TH1F( name1+"_hist", name1+"_hist", 1500, 0, 1500 );
    hist ->GetYaxis()->SetTitle("#sigma_{A}");
    hist ->GetYaxis()->SetRangeUser(0.,1.);
    hist ->GetXaxis()->SetTitle("p_{T}");

    gStyle->SetOptFit(1);

    for( unsigned int p = 0; p < Input.at(0).size(); p++ )
    {
      if ( Input.at(0).at(p) != 0 && !(TMath::IsNaN(Input.at(0).at(p))))
      {
        hist -> SetBinContent( hist -> FindBin( width_pt.at(0).at(p).at(5) ), Input.at(0).at(p) );
        hist -> SetBinError( hist -> FindBin( width_pt.at(0).at(p).at(5) ), InputError.at(0).at(p) );
        //			hist -> SetBinError( hist -> FindBin( width_pt.at(0).at(p).at(5) ), 0.05*Input.at(0).at(p) );
      }
    }

    hist -> Fit(name_width_fe1,"Q");

    TCanvas* canv = new TCanvas("nscplot","nscplot",50,50,600,600);
    hist -> Draw();
    canv -> Print("pdfy/JERs/reference"+name1+".pdf","pdf");
    canv -> Print("pdfy/JERs/reference"+name1+".png","png");

    //	TFile file_name("test.root","UPDATE");
    //	hist -> Write();
    //	file_name.Close();
    for( unsigned int m = 1; m < Input.size() ; m++ )
    {
      std::vector< double > temp2;
      std::vector< double > temp_error2;

      for( unsigned int p = 0; p < Input.at(m).size(); p++ )
      {
        double temp;
        double temp_error;

        if ( Input.at(m).at(p) != 0. )
        {
          pT = width_pt.at(m).at(p).at(5) ;

          N = NSCfun -> GetParameter(0) ;
          sN = NSCfun -> GetParError(0) ;
          S = NSCfun -> GetParameter(1) ;
          sS = NSCfun -> GetParError(1) ;
          C = NSCfun -> GetParameter(2) ;
          sC = NSCfun -> GetParError(2) ;

          Ref = NSCfun->Eval( pT );
          Probe = Input.at(m).at(p);
          ProbeError = InputError.at(m).at(p);
          //			   ProbeError = 0.05*Input.at(m).at(p);
          RefError = ( (N*sN)/(pT*pT) + ( S*sS )/pT + C*sC )/Ref;

          sqRef = Ref*Ref;
          sqProbe = Probe*Probe;

          temp = TMath::Sqrt( 2*sqProbe - 1*sqRef);
          temp_error = TMath::Sqrt(1)*TMath::Sqrt(( 4*Probe*Probe*ProbeError*ProbeError + Ref*Ref*RefError*RefError ))/temp;

          //			   std::cout << "probe " << Probe << ", ref " << Ref << ", corrected width " << temp << std::endl;

          if( !(TMath::IsNaN(temp)) ) temp2.push_back(temp);
          else temp2.push_back(0.);
          if( !(TMath::IsNaN(temp)) ) temp_error2.push_back(temp_error);
          else temp_error2.push_back(0.);
        }
        else { temp2.push_back(0.); temp_error2.push_back(0.); }
      }
      Output.push_back(temp2);
      OutputError.push_back(temp_error2);
    }
    delete NSCfun;
    delete canv;
  }


  void makeScales( std::vector< std::vector< double > > &Output, std::vector< std::vector< double > > &OutputError, std::vector< std::vector< double > > Input1, std::vector< std::vector< double > > Input1Error, std::vector< std::vector< double > > Input2, std::vector< std::vector< double > > Input2Error )
  {
    for( unsigned int i = 0; i < Input1.size(); i++ )
    {
      std::vector< double > temp2;
      std::vector< double > temp_error2;

      for( unsigned int j = 0; j < Input1.at(i).size(); j++ )
      {
        double temp;
        double temp_error;

        // Cut on scale factors not between 0.7 and 2
        //if( (Input1.at(i).at(j)!=0.) && (Input2.at(i).at(j)!=0.) && Input1.at(i).at(j) / Input2.at(i).at(j)>0.7 && Input1.at(i).at(j) / Input2.at(i).at(j)<2. )
        if( (Input1.at(i).at(j)!=0.) && (Input2.at(i).at(j)!=0.) )
        {
          temp = Input1.at(i).at(j) / Input2.at(i).at(j);
          temp_error = Input1Error.at(i).at(j) / Input2.at(i).at(j) + ( Input1.at(i).at(j) * Input2Error.at(i).at(j)) / ( Input2.at(i).at(j) * Input2.at(i).at(j) ) ;
        }
        else{ temp = 0.; temp_error = 0.; }


        if( TMath::IsNaN(temp) ) { temp = 0.; temp_error = 0.; }

        temp2.push_back(temp);
        temp_error2.push_back(temp_error);
      }
      Output.push_back(temp2);
      OutputError.push_back(temp_error2);
    }
  }


  void fill_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< double > > Widths, std::vector< std::vector< double > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range, int shift2 = 0)
  {
    int shift = 0;
    if (name1.Contains("SF_")) {
      // if (true) {
      if (name1.Contains("_FE")) { shift = 3; }
      else{ shift = 1; }
    }
    for( unsigned int m = 0; m <  Widths.size(); m++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += (m+shift);
      TH1F *h1 = new TH1F( name_width_fe1, name_width_fe1, 1000, 0, 1000 );
      //std::cout << name1 << std::endl;;
      if(strncmp(name1, "SF", strlen("SF"))){
        h1 ->GetYaxis()->SetTitle("#sigma_{JER}");	h1 ->GetXaxis()->SetTitle("p_{T}");	h1 -> Sumw2();
        //std::cout << "sigmajer" << std::endl;
      } else {
        h1 ->GetYaxis()->SetTitle("Scale factor");	h1 ->GetXaxis()->SetTitle("p_{T}");	h1 -> Sumw2();
        //std::cout << "scalefactor" << std::endl;
      }


      for( unsigned int p = 0; p <  Widths.at(m).size(); p++ )
      {
        if( ( !(TMath::IsNaN(Widths.at(m).at(p))) ) && Widths.at(m).at(p)!= 0. ) h1 -> SetBinContent( h1 -> FindBin( pt_binning.at(m+shift2).at(p).at(5) ), Widths.at(m).at(p) );
        if( ( !(TMath::IsNaN(WidthsError.at(m).at(p))) ) && Widths.at(m).at(p)!= 0. ) h1 -> SetBinError( h1 -> FindBin( pt_binning.at(m+shift2).at(p).at(5) ), WidthsError.at(m).at(p) );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0., range );
      output.push_back(h1);
    }
  }


  void fill_mctruth_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range)
  {
    //   std::cout << "size of input       = " <<  Widths.size()      << std::endl;;
    //   std::cout << "size of input error = " <<  WidthsError.size() << std::endl;;
    //   std::cout << "size of pt vector   = " <<  pt_binning.size()  << std::endl;;
    for( unsigned int m = 0; m <  Widths.size(); m++ )
    {
      TString name_width_fe1 = name1;
      name_width_fe1 += m;
      TH1F *h1 = new TH1F( name_width_fe1, name_width_fe1, 1000, 0, 1000 );
      //std::cout << name1 << std::endl;;
      //      std::cout << "size of input       = " <<  Widths.at(m).size()      << " for eta bin = " << m << std::endl;;
      //      std::cout << "size of input error = " <<  WidthsError.at(m).size() << " for eta bin = " << m << std::endl;;
      //      std::cout << "size of pt vector   = " <<  pt_binning.at(m).size()  << " for eta bin = " << m << std::endl;;
      h1 ->GetYaxis()->SetTitle("#sigma_{MCTruth}");	h1 ->GetXaxis()->SetTitle("p_{T}");	h1 -> Sumw2();

      for( unsigned int p = 0; p <  Widths.at(m).size(); p++ )
      {
        //         std::cout << "size of input       = " <<  Widths.at(m).at(p).size()      << " for pt bin = " << p << std::endl;;
        //         std::cout << "size of input error = " <<  WidthsError.at(m).at(p).size() << " for pt bin = " << p << std::endl;;
        //         std::cout << "size of pt vector   = " <<  pt_binning.at(m).at(p).size()  << " for pt bin = " << p << std::endl;;
        if( ( !(TMath::IsNaN(Widths.at(m).at(p).at(5))) )      && Widths.at(m).at(p).at(5)!= 0. ) h1 -> SetBinContent( h1 -> FindBin( pt_binning.at(m).at(p).at(5) ), Widths.at(m).at(p).at(5)      );
        if( ( !(TMath::IsNaN(WidthsError.at(m).at(p).at(5))) ) && Widths.at(m).at(p).at(5)!= 0. ) h1 -> SetBinError(   h1 -> FindBin( pt_binning.at(m).at(p).at(5) ), WidthsError.at(m).at(p).at(5) );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0., range );
      output.push_back(h1);
    }
  }



  // /TMath::Sqrt(2)












  // fitType 0 -> gaussian fit, 1 -> RMS (100% of points), 2 -> shifted gaussian
  void fitAsym( TH1F &hist, double &width, double &error , double &centre, double &centre_error, double &chisqr, int fitType, double position , bool fix )
  {

    TF1 * myfit = new TF1( "myfit", "[0]*exp(-0.5*TMath::Power(((x-[1])/[2]),2))", 0, 0.15 );
    //	TF1 * myfit2 = new TF1( "myfit2", "[0]* exp( -( x^2 + [1]^2 )/(2*[2]) ) * ( exp( 2*x*[1]/(2*[2]) ) + exp( -2*x*[1]/([2]*2) ) )	", 0, 0.15 );
    TF1 * myfit2 = new TF1( "myfit2", "[0]*exp( -( TMath::Power(x,2) + TMath::Power([1],2) )/(2*[2]) ) * ( exp( 2*x*[1]/(2*[2]) ) + exp( -2*x*[1]/([2]*2) ) )", 0, 0.15 );

    double norm = hist.Integral();
    if ( norm != 0 ) hist.Scale( 1/norm );

    if ( fitType == 0 )
    {
      double widthAux;
      //		std::cout << hist.GetTitle() << std::endl;
      //		std::cout << "bin content " << hist.GetBinContent(1) << std::endl;
      myfit -> SetParameter( 0, hist.GetBinContent(1) );
      //		std::cout << "set heigth " << myfit.GetParamter(0) << std::endl;
      //		myfit -> SetParameter( 0, 2*hist.GetMean(2) );
      myfit -> SetParameter( 1, 0 );
      myfit -> SetParameter( 2, hist.GetRMS() );
      myfit -> FixParameter( 1, 0 );//I fix the mean value (center of gaus distribution) at 0

      myfit -> SetRange( 0, 0.2);

      hist.Fit( "myfit", "NQLR" );
      widthAux = myfit -> GetParameter(2);
      myfit -> SetRange( 0, 2*TMath::Abs(widthAux));

      hist.Fit( "myfit", "RLQN" );
      widthAux = myfit -> GetParameter(2);
      myfit -> SetRange( 0, 2*TMath::Abs(widthAux));

      hist.Fit( "myfit", "LQR" );
      width = TMath::Abs(myfit -> GetParameter(2));
      error = myfit -> GetParError(2);

      centre = 0;
      centre_error = 0;

      chisqr = ( myfit -> GetChisquare() )/( myfit -> GetNDF() );

      /*		if ( (myfit -> GetParameter(2)) > 1)
      {
      std::cout << "fit not converged, trying log-likelihood fit" << std::endl;
      std::cout << "chi-square fit gave: " << "width = " << width << std::endl;

      hist.Fit( "myfit", "NQLR" );
      widthAux = myfit -> GetParameter(2);
      myfit -> SetRange( 0, 2*TMath::Abs(widthAux));

      hist.Fit( "myfit", "RQNL" );
      widthAux = myfit -> GetParameter(2);
      myfit -> SetRange( 0, 2*TMath::Abs(widthAux));

      hist.Fit( "myfit", "QRL" );
      width = TMath::Abs(myfit -> GetParameter(2));
      error = myfit -> GetParError(2);

      std::cout << "fit gave:          " << "width = " << width << std::endl;
    }*/
  }

  else if ( fitType == 1 )
  {
    width = hist.GetRMS();
    error = hist.GetRMSError();

    centre = 0;
    centre_error = 0;
    chisqr = 0;
  }

  else if ( fitType == 2 )
  {
    hist.Scale(1);

    myfit2 -> SetParameter( 0, 0.05 );
    myfit2 -> SetParLimits( 0, 0.001, 0.2 );
    if ( position != 0. )
    {
      if ( fix == false )
      {
        myfit2 -> SetParameter( 1, position );
        myfit2 -> SetParLimits( 1, 0., 0.2 );
      }
      if ( fix == true ) myfit2 -> FixParameter( 1, position );
    }
    else
    {
      myfit2 -> SetParameter( 1, 0.05 );
      myfit2 -> SetParLimits( 1, 0., 0.2 );
    }
    myfit2 -> SetParameter( 2, hist.GetRMS() );
    myfit2 -> SetParLimits( 2, 0.001, 0.1 );

    myfit2 -> SetRange( 0, 0.25);
    hist.Fit( "myfit2", "QR" );

    //		double new_range = myfit2 -> GetParameter(1) + 2*(myfit2 -> GetParameter(2));

    //		std::cout << "new range up to " << new_range << std::endl;
    //		if ( new_range > 0.05 ) myfit2 -> SetRange( 0, new_range );
    //		hist.Fit( "myfit2", "QR" );

    width = TMath::Abs(myfit2 -> GetParameter(2));
    error = myfit2 -> GetParError(2);

    centre = TMath::Abs(myfit2 -> GetParameter(1));
    centre_error = myfit2 -> GetParError(1);

    chisqr = ( myfit2 -> GetChisquare() )/( myfit2 -> GetNDF() );
    //		std::cout << "chisqr/NDF = " << chisqr << std::endl;
    //		std::cout << "normalization = " << myfit2 -> GetParameter(0) << std::endl;
  }

  else std::cout << "wrong fit type in function argument" << std::endl;

  delete myfit;
  delete myfit2;
}


void fitMCTruth( TH1F &hist, double &width, double &error )
{

  TF1 * myfit = new TF1( "myfit", "[0]*exp(-0.5*((x-[1])/[2])^2)", 0, 0.15 );

  double norm = hist.Integral();
  if ( norm != 0 ) hist.Scale( 1/norm );

  double widthAux, meanAux;
  myfit -> SetParameter( 0, hist.GetMaximum() );
  myfit -> SetParameter( 1, hist.GetMean() );
  myfit -> SetParameter( 2, hist.GetRMS() );

  myfit -> SetRange( 0, 2);

  hist.Fit( "myfit", "NQR" );
  widthAux = TMath::Abs(myfit -> GetParameter(2));
  meanAux = myfit -> GetParameter(1);
  myfit -> SetRange( meanAux - 2*widthAux, meanAux + 2*widthAux);

  hist.Fit( "myfit", "RQN" );
  widthAux = TMath::Abs(myfit -> GetParameter(2));
  meanAux = myfit -> GetParameter(1);
  myfit -> SetRange( meanAux - 2*widthAux, meanAux + 2*widthAux);

  hist.Fit( "myfit", "RQ" );
  width = TMath::Abs(myfit -> GetParameter(2));
  error = myfit -> GetParError(2);

  delete myfit;
}


void fitConst( TH1F &hist, double &value, double &error )
{
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );

  constfit -> SetParameter( 0, 0.1 );

  constfit -> SetParLimits( 0, 0., 1. );

  hist.Fit( "constfit", "Q" );
  value = constfit -> GetParameter(0);
  error = constfit -> GetParError(0);

  delete constfit;
}


void fitLin( TH1F &hist, double &width, double &error )
{
  TF1 * linfit = new TF1( "linfit", "[0]+x*[1]", 0, 0.3 );

  linfit -> SetParameter( 0, 0.1 );
  linfit -> SetParameter( 1, 0.1 );

  linfit -> SetParLimits( 0, 0., 1. );

  hist.Fit( "linfit", "QM" );
  width = linfit -> GetParameter(0);
  error = 1. * ( linfit -> GetParError(0) );

  delete linfit;
}


void fitShift( TH1F &hist, double &a, double &errora, double &b, double &errorb )
{
  TF1 * linfit = new TF1( "linfit", "[0]+x*[1]", 0, 0.25 );

  linfit -> FixParameter( 0, 0.0 );
  linfit -> SetParameter( 1, 0.1 );

  //	linfit -> SetParLimits( 0, 0, 1 );

  hist.Fit( "linfit", "Q" );
  a = linfit -> GetParameter(1);
  errora = linfit -> GetParError(1);
  b = linfit -> GetParameter(0);
  errorb = linfit -> GetParError(0);

  delete linfit;
}

// Kristins code, truncated-RMS calculation.
// [ Previously, Matthias did an iterated fit
// for more and more truncated Gaussians ]
float GetAsymmWidth(TH1F* &htemp, double * xq_IQW, double * yq_IQW, int etabin, int ptbin, int alphabin)
{
  const int nq = 2;

  float width = 0.;
  float temp;

  // For studies of the truncation:
  float truncate_value = -100; // for studies of truncation
  int truncate_bin = -100; // for studies of truncation
  //float events_truncated = -100; // for studies of truncation
  //float events_included = -100; // for studies of truncation
  float events_total = htemp->GetEntries(); // for studies of truncation
  //float events_total = htemp->at(htemp->FindLastBinAbove())->GetEffectiveEntries(); // for studies of truncation
  //cout << "total events eff entr" << events_total << endl;
  int lastbin = htemp->FindLastBinAbove();
  float highest_value = htemp->GetBinContent(lastbin);

  //	std::cout << htemp  << std::endl;

  // Truncate when enough events are present:
  if( htemp->GetEntries() > 100 )
  {
    // For studies of the truncation:
    //events_truncated = 0; // for studies of truncation
    //events_total = 0; // for studies of truncation
    //events_included = 0; // for studies of truncation

    htemp->GetXaxis()->SetRange(0,-1);
    htemp->ComputeIntegral();
    htemp->GetQuantiles(nq,yq_IQW,xq_IQW);
    // Find bin below which there is 0.985 fraction of events:
    // This is done by first finding the value for which this is true,
    // then the bin for that value.
    truncate_value = yq_IQW[1]; // For studies of truncation
    Int_t IQW_high_bin_i = htemp->FindBin(yq_IQW[1]);
    truncate_bin = IQW_high_bin_i; // For studies of truncation


    // cout << "Truncated Integral in %: " << htemp->Integral(1,IQW_high_bin_i)/htemp->Integral() << endl;
    // cout << "Truncated Bin: " << htemp->FindBin(yq_IQW[1]) << endl;

    for(int i=1; i <= IQW_high_bin_i; i++)
    {
      // value of bin is bin center, weights is # events is bin content
      width += htemp->GetBinContent(i) * std::pow(htemp->GetBinCenter(i), 2);

      //events_included += htemp->GetBinContent(i); // For studies of truncation
      //events_included += htemp->at(i)->GetEffectiveEntries(); // For studies of truncation
    }
    // Compute RMS by hand for bins taken into account:
    // Multiply each bin value (center) by its weight (# of events in that bin)
    temp = TMath::Sqrt((1/(htemp->Integral(1, IQW_high_bin_i)))*width);


    //events_truncated = htemp->at(truncate_bin)->GetEffectiveEntries(truncate_bin);
    //cout << "trunc events eff entr" << events_truncated << endl;
    //events_total = htemp->at(lastbin)->GetEffectiveEntries();
    //cout << "total events eff entr" << events_total << endl;
    //events_total = events_included;
    //for(int i=IQW_high_bin_i + 1; i <= lastbin; i++)
    //{
    //    events_total += htemp->at(i)->GetEffectiveEntries(); // For studies of truncation
    //}
    //events_truncated = events_total - events_included; // For studies of truncation
    //cout << "trunc events" << events_truncated << endl;
    //cout << "total events" << events_total << endl;

  }
  // Give a warning when no events are present:
  else if( htemp->GetEntries() == 0 )
  {
    //std::cout << "pusty histogram, ale czilujemy" << std::endl;
    temp = 0;
  }

  // Do not truncate otherwise (for 0 < # events <= 100):
  else
  {
    //		width = htemp -> GetRMS();
    for(int i=1; i <= lastbin; i++)
    {
      width += htemp->GetBinContent(i) * std::pow(htemp->GetBinCenter(i), 2);
    }
    temp = TMath::Sqrt(width/(htemp->Integral()));
    std::cout << "less than 100 events: " << htemp->GetEntries() << std::endl;
  }

  // Remember
  // xq_IQW[0] = 0.0;
  // xq_IQW[1] = 0.985;
  //std::cout << "Truncated low fraction:" << xq_IQW[0] << std::endl;
  //std::cout << "Truncated high fraction:" << xq_IQW[1] << std::endl;

  float truncated_integral = htemp->Integral(1, truncate_bin);
  float total_integral = htemp->Integral();
  float integral_fraction = truncated_integral/total_integral;
  float truncated_fraction = 1 - integral_fraction;
  float asymmetry = temp;
  float asym_error = temp/TMath::Sqrt(2*htemp->GetEffectiveEntries());
  // This is how the asymmetry and width are computed above:
  //temp1.push_back( GetAsymmWidth( Asymmetry.at(m).at(p).at(r) , xq_IQW, yq_IQW, m, p, r) );
  //temp_error1.push_back( GetAsymmWidth( Asymmetry.at(m).at(p).at(r) , xq_IQW, yq_IQW, m, p, r)/TMath::Sqrt(2*Asymmetry.at(m).at(p).at(r)->GetEffectiveEntries()));
  ofstream myfile;
  myfile.open (truncation_info, ios::app);
  myfile << "binETA" << sep << "binPT" << sep << "binALPHA" << sep;
  myfile << "eventstotal" << sep << "trunc_frac" << sep << "integral" << sep;
  myfile << "truncbin" << sep << "truncval" << sep << "binhigh" << sep << "valhigh" << sep << "asymwidth" << sep << "asymerr\n"; // for studies of truncation
  myfile << etabin + 1 << sep << ptbin + 1 << sep << alphabin + 1 << sep;
  //std::cout << events_total << sep << truncated_integral << sep << truncate_bin << sep; // for studies of truncation
  //std::cout << truncate_value << sep << lastbin << sep << highest_value << endl; // for studies of truncation
  myfile << events_total << sep << truncated_fraction << sep << total_integral << sep << truncate_bin << sep; // for studies of truncation
  myfile << truncate_value << sep << lastbin << sep << highest_value << sep << asymmetry << sep << asym_error << std::endl; // for studies of truncation

  myfile.close();
  return temp;
}


void make_lin_fit(double & slope, double & d_slope, double & offset, double & d_offset){
  TMinuit min;
  min.SetPrintLevel(-1);
  //min.SetPrintLevel(0);
  int err = min.DefineParameter(0, "slope", slope, d_slope, 0.05, 1.0);
  assert(err==0);
  err = min.DefineParameter(1, "offset", offset, d_offset, 0.001, 0.2);
  assert(err==0);
  min.SetFCN(chi2_linear);
  min.mnmigr();
  min.GetParameter(0, slope, d_slope);
  min.GetParameter(1, offset, d_offset);
}

void chi2_linear(Int_t& npar, Double_t* grad, Double_t& fval, Double_t* p, Int_t status){
  if(data.y_cov_inv.GetNcols()==0){
    double dummy;
    int ncols = data.y_cov.GetNcols();
    data.y_cov_inv.ResizeTo(ncols, ncols);
    data.y_cov_inv = data.y_cov.Invert(&dummy);
  }
  const size_t ndata = data.x_val.size(); // number of data points in x,y graph to fit to
  std::vector<double> delta_y(ndata);
  for(size_t i=0; i<ndata; ++i){
    delta_y[i] = data.x_val[i]*p[0] + p[1] - data.y_val[i];
  }
  // now calculate the chi2, i.e.
  //  dy^T * C^{-1} * dy
  // where C is the variance--covariance matrix and dy = (y_data - y_pred)
  // This could probably be implemented in ROOT, but it's so simple, we just do it here:
  fval = 0.0;
  for(size_t i=0; i<ndata; ++i){
    for(size_t j=0; j<ndata; ++j){
      fval += delta_y[i] * delta_y[j] * data.y_cov_inv(i,j);
    }
  }
}




void histWidthAsym2( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< double > > > &Widths, std::vector< std::vector< std::vector< double > > > &WidthsError , bool fill_all ){
  double asym;
  double asymerr;

  for( unsigned int m = 0; m < Asymmetry.size(); m++ )
  {
    std::vector< std::vector< double > > temp2;
    std::vector< std::vector< double > > temp_error2;
    for( unsigned int p = 0; p < Asymmetry.at(m).size() ; p++ )
    {
      std::vector< double > temp1;
      std::vector< double > temp_error1;
      for( unsigned int r = 0; r < Asymmetry.at(m).at(p).size(); r++ )
      {
        // std::cout << "eta pt alpha bins " << m << ", " << p << ", " << r << std::endl;
        asym = Asymmetry.at(m).at(p).at(r)->GetRMS();
        asymerr = Asymmetry.at(m).at(p).at(r)->GetRMSError();

        const int n_pt_bins_Si = 10;
        const double pt_bins_Si[n_pt_bins_Si]       = { 40, 72,  95, 160, 226, 283, 344, 443, 577, 606};
        /* //SingleJet HF triggers highest ------------- */
        const int n_pt_bins_Si_HF = 10;
        const double pt_bins_Si_HF[n_pt_bins_Si_HF] = { 40, 72,  95, 160, 226, 283, 344, 443, 577, 606};

        /* //DiJet central triggers highest ------------- */
        const int n_pt_bins_Di = 9;
        const double pt_bins_Di[n_pt_bins_Di]       = { 73, 85,  97, 179, 307, 370, 434, 520, 649};
        /* //DiJet HF triggers highest ------------- */
        const int n_pt_bins_Di_HF = 6;
        const double pt_bins_Di_HF[n_pt_bins_Di_HF] = { 73, 93, 113, 176, 239, 318 };



        if(!fill_all){
          if( p == 0 && r < 4) { asym = 0.; asymerr = 0.; };
          if( p == 1 && r < 3) { asym = 0.; asymerr = 0.; };
          if( p == 2 && r < 2) { asym = 0.; asymerr = 0.; };
          if( p == 3 && r < 1) { asym = 0.; asymerr = 0.; };
          if( p == 4 && r < 1) { asym = 0.; asymerr = 0.; };

        }
        temp1.push_back( asym );
        temp_error1.push_back( asymerr );
      }
      temp2.push_back(temp1);
      temp_error2.push_back(temp_error1);
    }
    Widths.push_back(temp2);
    WidthsError.push_back(temp_error2);
  }
}


void Fill_Map3D(std::vector< std::vector < std::vector < TH1F* > > > &Asymmetry, std::vector < TH2F* > &Map, std::vector < double > &eta_bins, std::vector < double > &pt_bins ) {
  for (unsigned int r = 0; r < 6; r++) {
    TString name = "Map_mean_"; name += r;
    TH2F* temp = new TH2F(name,name, pt_bins.size(), pt_bins.at(0), pt_bins.at(pt_bins.size()-1), eta_bins.size(), eta_bins.at(0), eta_bins.at(eta_bins.size()-1));
    // TH2F* temp = new TH2F(name,name, eta_bins.size(), &eta_bins[0], pt_bins.size(), &pt_bins[0]);
    Map.push_back(temp);
  }

  for (unsigned int m = 0; m < Asymmetry.size(); m++) {
    for (unsigned int p = 0; p < Asymmetry.at(m).size(); p++) {
      for (unsigned int r = 0; r < Asymmetry.at(m).at(p).size(); r++) {
        Map.at(r) -> SetBinContent( Map.at(r) -> FindBin(pt_bins[p]), Map.at(r) -> FindBin(eta_bins[m]), Asymmetry.at(m).at(p).at(r)->GetMean());
      }
    }
  }
}
