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

// Code of Andrea Malara
// Based on code by Marek Niedziela, Matthias Schröder, Kristin Goebel

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

  void reset() {
    x_val.clear();
    y_val.clear();
    y_cov.ResizeTo(0,0);
    y_cov_inv.ResizeTo(0,0);
  }

  void CheckPoints() {
    std::vector<int> RemovedPoints;
    TMatrixD y_cov_new;
    int j = 0;

    for(unsigned int i = 0; i < y_val.size(); i++) {
      //         std::cout << "i: " << i << "   j: " << j << std::endl;
      if ( y_val.at(i) == 0) {
        x_val.erase(x_val.begin()+i);
        y_val.erase(y_val.begin()+i);
        RemovedPoints.push_back(j);
        i = i-1;
      }
      j++;
    }

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

void histLoadAsym( TFile &f, bool data, TString text, std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry, std::vector< std::vector< std::vector< TH1F* > > > &GenAsymmetry, int etaBins, int ptBins, int AlphaBins, int etaShift);
void histMeanPt( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< double > > > &Widths );
void histWidthAsym_old( std::vector<std::vector<std::vector<TH1F*> > > &Asymmetry , std::vector<std::vector<std::vector<double> > > &Widths, std::vector<std::vector<std::vector<double> > > &WidthsError , bool fill_all );
void histWidthAsym( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< double > > > &Widths, std::vector< std::vector< std::vector< double > > > &WidthsError , bool fill_all, double alpha = 0.985, bool isFE = 0);
void histWidthMCTruth( std::vector<std::vector<std::vector<TH1F*> > > &Asymmetry , std::vector<std::vector<std::vector<double> > > &Widths, std::vector<std::vector<std::vector<double> > > &WidthsError );
void fill_widths_hists( TString name1, std::vector< std::vector< TH1F* > > &widths , std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError);
void histLinFit( std::vector< std::vector< TH1F* > > widths_hist_all , std::vector< std::vector< double > > &Widths, std::vector< std::vector< double > > &WidthsError, bool isFE );
void histLinCorFit( std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< TGraphErrors* > > &output_graph, std::vector< std::vector< double > > &output, std::vector< std::vector< double > > &output_error, bool isFE, bool isMC);
void widths_015_ratios( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > WidthsTwo, std::vector<std::vector<std::vector<double> > > WidthsTwoError, std::vector<std::vector<std::vector<double> > > forward_width_pt );
void correctJERwithPLI(std::vector< std::vector< double > > &Output, std::vector< std::vector< double > > &OutputError, std::vector< std::vector< double > > Widths, std::vector< std::vector< double > > WidthsError, std::vector< std::vector< double > > PLI, std::vector< std::vector< double > > PLIError, float shift = 0.0);
void correctJERwithPLI015(std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > PLI, std::vector<std::vector< std::vector< double > > > PLIError, float shift = 0.0);
void correctForRef( TString name1, std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<double> > Input, std::vector<std::vector<double> > InputError, std::vector<std::vector<std::vector<double> > > width_pt, int shift, TString outdir);
void makeScales( std::vector< std::vector< double > > &Output, std::vector< std::vector< double > > &OutputError, std::vector< std::vector< double > > Input1, std::vector< std::vector< double > > Input1Error, std::vector< std::vector< double > > Input2, std::vector< std::vector< double > > Input2Error );
void fill_mctruth_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range);
void fill_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< double > > Widths, std::vector< std::vector< double > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range, int shift2 = 0);
void Fill_Map3D(std::vector< std::vector < std::vector < TH1F* > > > &Asymmetry, std::vector < TH2F* > &Map, std::vector < double > &eta_bins, std::vector < double > &pt_bins );
void make_lin_fit(double & slope, double & d_slope, double & offset, double & d_offset, double min_slope, double max_offset);
void chi2_linear(Int_t& npar, Double_t* grad, Double_t& fval, Double_t* p, Int_t status);
float GetAsymmWidth(TH1F* &htemp, double * xq_IQW, double * yq_IQW, int etabin, int ptbin, int alphabin);
double sumSquare(double a, double b);
double findMinMax(TH1F* JER, std::vector< std::vector< double > > pt_width, TF1* NSC_ratio, TF1* constfit, bool isMin);
void fitLin( TH1F &hist, double &width, double &error );


// Not used
void histLoadFE1( TFile &f, bool data, TString text, std::vector< std::vector< std::vector< TH1F* > > > &forward_hist , std::vector< std::vector< std::vector< TH1F* > > > &forward_gen_hist , int ptbin , int etabin);
void correctPLIbinbybin(std::vector< std::vector< std::vector< double > > > &Output, std::vector< std::vector< std::vector< double > > > &OutputError, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > PLI, std::vector< std::vector< std::vector< double > > > PLIError);
void fill_PLI_hist( TString name1, std::vector< std::vector< TH1F* > > &widths , std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning);
void fit_NSC_PLI( std::vector< std::vector< TF1* > > &functions, std::vector< std::vector< TH1F* > > PLI_hists );
void def_fill_PLI( TString name1, std::vector< std::vector< TH1F* > > &widths , std::vector< std::vector< TF1* > > functions, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning );
void correctForPLI( TString name1, std::vector< std::vector< TH1F* > > widths, std::vector< std::vector< TH1F* > > &output, std::vector< std::vector< TF1* > > functions, std::vector< std::vector< std::vector< double > > > pt_binning);
void fill_widths_pt( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<std::vector<double> > > forward_width_pt );
void KFSR_ratios( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<double> > WidthsTwo, std::vector<std::vector<double> > WidthsTwoError, std::vector<std::vector<std::vector<double> > > forward_width_pt );
void KNFS_line_fit( TString name1, std::vector<TH1F*> widths, std::vector< TF1* > &functions );
void correctKFSR( std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<TF1*> functions, std::vector<std::vector<std::vector<double> > > forward_width_pt );
void fitAsym( TH1F &hist, double &width, double &error , double &centre, double &centre_error, double &chisqr, int fitType, double position , bool fix );
void fitConst( TH1F &hist, double &value, double &error );
void fitMCTruth( TH1F &hist, double &width, double &error );
void fitShift( TH1F &hist, double &a, double &errora, double &b, double &errorb );



double truncate_fraction = 0.985;
char truncation_info[50];


void histLoadAsym( TFile &f, bool data, TString text, std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry, std::vector< std::vector< std::vector< TH1F* > > > &GenAsymmetry, int etaBins, int ptBins, int AlphaBins, int etaShift) {
  for( int m = etaShift; m < etaBins+etaShift; m++ ) {
    std::vector< std::vector< TH1F* > > temp2;
    std::vector< std::vector< TH1F* > > temp2gen;
    for( int p = 0; p < ptBins; p++ ) {
      std::vector< TH1F* > temp1;
      std::vector< TH1F* > temp1gen;
      for( int r = 0; r < AlphaBins; r++ ) {
        TString name = text;                        name    += "_eta"; name     += m+1; name    += "_pt"; name    += p+1; name    += "_alpha"; name     += r+1;
        TString namegen = "gen_"; namegen += text;  namegen += "_eta"; namegen  += m+1; namegen += "_pt"; namegen += p+1; namegen += "_alpha"; namegen  += r+1;
        TH1F* h = (TH1F*)f.Get(name);
        if (h) h->Rebin(2);
        temp1.push_back(h);
        if (( TH1F* )f.Get( name ) == 0) {
          std::cout << "ERROR" << std::endl;
          std::cout << name << std::endl;
        }
        if ( data == false ) {
          TH1F* h_gen = (TH1F*)f.Get(namegen);
					if (h_gen) h_gen->Rebin(2);
          temp1gen.push_back(h_gen);
        }
      }
      temp2.push_back(temp1);
      if ( data == false ) temp2gen.push_back(temp1gen);
    }
    Asymmetry.push_back(temp2);
    if ( data == false ) GenAsymmetry.push_back(temp2gen);
  }
}


void histMeanPt( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< double > > > &Widths ) {
  for( unsigned int m = 0; m < Asymmetry.size(); m++ ) {
    std::vector< std::vector< double > > temp1;
    for( unsigned int p = 0; p < Asymmetry.at(m).size(); p++ ) {
      std::vector< double > temp2;
      for( unsigned int r = 0; r < Asymmetry.at(m).at(p).size(); r++ ) {
        temp2.push_back( (*Asymmetry.at(m).at(p).at(r)).GetMean() );
      }
      temp1.push_back(temp2);
    }
    Widths.push_back(temp1);
  }
}

void histWidthAsym( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< double > > > &Widths, std::vector< std::vector< std::vector< double > > > &WidthsError , bool fill_all, double alpha, bool isFE) {
  double asym;
  double asymerr;

  for( unsigned int m = 0; m < Asymmetry.size(); m++ ) {
    std::vector< std::vector< double > > temp2;
    std::vector< std::vector< double > > temp_error2;
    for( unsigned int p = 0; p < Asymmetry.at(m).size() ; p++ ) {
      std::vector< double > temp1;
      std::vector< double > temp_error1;
      for( unsigned int r = 0; r < Asymmetry.at(m).at(p).size(); r++ ) {
        // std::cout << "eta pt alpha bins " << m << ", " << p << ", " << r << std::endl;
        TH1F* temp_hist = (TH1F*) Asymmetry.at(m).at(p).at(r)->Clone();
        if (temp_hist->Integral() > 0) {
          temp_hist->ComputeIntegral();
          Double_t xq[2], yq[2];
          xq[0] = std::min(alpha, 1.-alpha);
          xq[1] = std::max(alpha, 1.-alpha);
          temp_hist->GetQuantiles(2, yq, xq);
          temp_hist->GetXaxis()->SetRange(temp_hist->FindBin(yq[0]), temp_hist->FindBin(yq[1]));
          asym = temp_hist->GetRMS();
          asymerr = temp_hist->GetRMSError();
          // if (p == 1 || (m == 7 && p == 10) || (m == 5 && p == 2) || (m == 5 && p == 3) || (m == 8 && p == 2) || (m == 10 && p == 2) ) {
          //   TF1* mygaus = new TF1("mygaus","gaus",-1.0, 1.0);
          //   TH1F* temp_hist2 = (TH1F*) Asymmetry.at(m).at(p).at(r)->Clone();
          //   temp_hist2->Scale(1./temp_hist2->Integral());
          //   mygaus->SetParameters(1, 0., 0.2);
          //   mygaus->SetParLimits(1, -0.2, 0.2);
          //   mygaus->SetParLimits(2, 0.005, 0.3);
          //   temp_hist2->Fit("mygaus", "Q+");
          //   asym = mygaus->GetParameter(2);
          //   asymerr = mygaus->GetParError(2);
          //   Asymmetry.at(m).at(p).at(r)->GetListOfFunctions()->Add(mygaus);
          // }
        } else { asym = 0.; asymerr = 0.; };

        if (!fill_all) {
          if ( p == 0 && r < 4) { asym = 0.; asymerr = 0.; };
          if ( p == 1 && r < 3) { asym = 0.; asymerr = 0.; };
          if ( p == 2 && r < 2) { asym = 0.; asymerr = 0.; };
          if ( p == 3 && r < 1) { asym = 0.; asymerr = 0.; };
          if ( p == 4 && r < 1) { asym = 0.; asymerr = 0.; };
					if ( p == 5 && r < 1) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 6 && p == 2 && r < 3) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 6 && p == 3 && r < 3) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 5 && p == 6 && r < 1) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 6 && p == 6 && r < 2) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 6 && p == 9 && r < 1) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 7 && p == 6 && r < 1) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 7 && p == 7 && r < 1) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 7 && p == 8 && r < 1) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 8 && p == 2 && r < 3) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 8 && p == 6 && r < 3) { asym = 0.; asymerr = 0.; };
          if (!isFE   && m == 9 && p == 3 && r < 2) { asym = 0.; asymerr = 0.; };
          if ( isFE   && m == 9 && p >= 7 && r < 1) { asym = 0.; asymerr = 0.; };
          if ( isFE   && m == 11&& p == 7 && r < 2) { asym = 0.; asymerr = 0.; };
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

void histWidthMCTruth( std::vector<std::vector<std::vector<TH1F*> > > &Asymmetry , std::vector<std::vector<std::vector<double> > > &Widths, std::vector<std::vector<std::vector<double> > > &WidthsError ) {
  double asym;
  double asymerr;

  for( unsigned int m = 0; m < Asymmetry.size(); m++ ) {
    std::vector< std::vector< double > > temp2;
    std::vector< std::vector< double > > temp_error2;
    for( unsigned int p = 0; p < Asymmetry.at(m).size() ; p++ ) {
      std::vector< double > temp1;
      std::vector< double > temp_error1;
      for( unsigned int r = 0; r < Asymmetry.at(m).at(p).size(); r++ ) {
        asym    = ((*Asymmetry.at(m).at(p).at(r)).GetRMS())/TMath::Sqrt(2);
        asymerr = ((*Asymmetry.at(m).at(p).at(r)).GetRMSError())/TMath::Sqrt(2);
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

void fill_widths_hists( TString name1, std::vector< std::vector< TH1F* > > &widths , std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError) {
  double aMax[] = { 0.05, 0.1, 0.15, 0.2, 0.25, 0.3 };
  double temp;
  for( unsigned int m = 0; m < Widths.size(); m++ ) {
    std::vector< TH1F* > temp1;
    for( unsigned int p = 0; p < Widths.at(m).size(); p++ ) {
      TString name_width = name1;
      name_width += "_eta"; name_width += m+1; name_width += "_pt"; name_width += p+1;
      // if (name1.Contains("_fe")) { name_width += m+2; }
      // else { name_width += m+1; }
      TH1F *h1 = new TH1F( name_width, name_width, 50, 0, 0.35 );
      h1 ->GetYaxis()->SetTitle("#sigma_{A}");	h1 ->GetXaxis()->SetTitle("#alpha_{max}"); h1 ->GetYaxis()->SetTitleOffset(1.);
      h1 -> Sumw2();
      for( unsigned int r = 0; r < Widths.at(m).at(p).size(); r++ ) {
        temp = Widths.at(m).at(p).at(r);
        if ( !(TMath::IsNaN(temp)) && temp != 0) h1 -> SetBinContent( h1 -> FindBin( aMax[ r ] ), Widths.at(m).at(p).at(r) );
        if ( !(TMath::IsNaN(temp)) && temp != 0) h1 -> SetBinError( h1 -> FindBin( aMax[ r ] ), WidthsError.at(m).at(p).at(r) );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0., 0.3 );
      temp1.push_back(h1);
    }
    widths.push_back(temp1);
  }
}

void histLinFit( std::vector< std::vector< TH1F* > > widths_hist_all , std::vector< std::vector< double > > &Widths, std::vector< std::vector< double > > &WidthsError, bool isFE ) {
  for( unsigned int m = 0; m < widths_hist_all.size(); m++ ) {
    std::vector<double> temp2;
    std::vector<double> temp_error2;
    for( unsigned int p = 0; p < widths_hist_all.at(m).size(); p++ ) {
      double value, error;
      if ( widths_hist_all.at(m).at(p)->GetEntries() != 0 ) {
        fitLin( *( widths_hist_all.at(m).at(p) ), value, error );

        if ( p==0 ) {value = 0; error = 0;}
        if ( !isFE && m==1  && p==1 ) {value = 0; error = 0;}
        if ( !isFE && m==3  && p==1 ) {value = 0; error = 0;}
        if ( !isFE && m>=6  && p==1 ) {value = 0; error = 0;}
        if ( !isFE && m==5  && p==8 ) {value = 0; error = 0;}
        if ( !isFE && m==6  && p==8 ) {value = 0; error = 0;}
        if ( !isFE && m==7  && p>=8 ) {value = 0; error = 0;}
        if ( !isFE && m==8  && p==1 ) {value = 0; error = 0;}
        if ( !isFE && m==8  && p>=6 ) {value = 0; error = 0;}
        if ( !isFE && m==9  && p>=5 ) {value = 0; error = 0;}
        if (  isFE && m==10 && p==6 ) {value = 0; error = 0;}
        if (  isFE && m==10 && p>=8 ) {value = 0; error = 0;}
        if (  isFE && m==11 && p>=8 ) {value = 0; error = 0;}
        if (  isFE && m==12 && p>=7 ) {value = 0; error = 0;}
        if (  isFE && m>=11 && p==1 ) {value = 0; error = 0;}
        temp2.push_back(value);
        temp_error2.push_back(error);
      } else {
        temp2.push_back(0.);
        temp_error2.push_back(0.);
      }
    }
    Widths.push_back(temp2);
    WidthsError.push_back(temp_error2);
  }
}

void histLinCorFit( std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< TGraphErrors* > > &output_graph, std::vector< std::vector< double > > &output, std::vector< std::vector< double > > &output_error, bool isFE, bool isMC) {
  for( unsigned int m = 0; m < Widths.size(); m++ ) {
    // eta loop
    std::vector<TGraphErrors*> temp2_graph;
    std::vector<double> temp2;
    std::vector<double> temp_error2;
    for( unsigned int p = 0; p < Widths.at(m).size(); p++ ) {
      // p_T loop
      std::vector<float> alpha;
      alpha.push_back(0.05); alpha.push_back(0.1); alpha.push_back(0.15); alpha.push_back(0.20); alpha.push_back(0.25); alpha.push_back(0.3);
      std::vector<double> x,x_e;
      for(int ialpha=0; ialpha < 6; ++ialpha) {
        x.push_back(alpha.at(ialpha));
        x_e.push_back(0.);
      }
      TMatrixD y_cov_mc;
      y_cov_mc.ResizeTo(alpha.size(), alpha.size());
      for(unsigned int ialpha=0; ialpha < alpha.size(); ++ialpha) {
        for (unsigned int jalpha =0; jalpha < alpha.size(); jalpha++) {
          if ( ialpha <= jalpha ) {
            double n1_mc = pow(Widths.at(m).at(p).at(ialpha),2)/(2*pow(WidthsError.at(m).at(p).at(ialpha),2));
            double n2_mc = pow(Widths.at(m).at(p).at(jalpha),2)/(2*pow(WidthsError.at(m).at(p).at(jalpha),2));
            y_cov_mc(ialpha, jalpha) = pow(WidthsError.at(m).at(p).at(ialpha),2) * pow((n1_mc/n2_mc),2)*
            (Widths.at(m).at(p).at(ialpha)/Widths.at(m).at(p).at(jalpha));
          } else {
            double n1_mc = pow(Widths.at(m).at(p).at(jalpha),2)/(2*pow(WidthsError.at(m).at(p).at(jalpha),2));
            double n2_mc = pow(Widths.at(m).at(p).at(ialpha),2)/(2*pow(WidthsError.at(m).at(p).at(ialpha),2));
            y_cov_mc(ialpha, jalpha) = pow(WidthsError.at(m).at(p).at(jalpha),2) * pow((n1_mc/n2_mc),2)*
            (Widths.at(m).at(p).at(jalpha)/Widths.at(m).at(p).at(ialpha));
          }
        }
      }

      //create TGraphErrors from previously defined vectors
      TGraphErrors* extrapol_MC = new TGraphErrors(alpha.size(),&x[0],&Widths.at(m).at(p).at(0),&x_e[0],&WidthsError.at(m).at(p).at(0));
      TString name = "Graph_SM_eta";
      if (isFE) name = "Graph_FE_eta";
      name += m+1; name+="_pt"; name += p+1;
      extrapol_MC->SetName(name);

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
      // double slope = (Widths.at(m).at(p).at(Widths.at(m).at(p).size()-1) - Widths.at(m).at(p).at(Widths.at(m).at(p).size()-3))/(x.at(x.size()-1) - x.at(x.size()-3));
      // double offset = Widths.at(m).at(p).at(Widths.at(m).at(p).size()-1) - (slope*x.at(x.size()-1));
      double slope  = 0.15;
      double offset = 0.05;
      double d_slope = slope;
      double d_offset = offset;
      double min_slope = 0.05;
      double max_offset = 0.15;
      if ( !isFE && m == 7 && p == 2) min_slope  = 0.215;
      if ( !isFE && m == 7 && p == 2) max_offset = 0.07;
      if ( !isFE && m == 8 && p == 6) min_slope  = 0.09;
      if ( !isFE && m == 8 && p == 6) max_offset = 0.1;
      if ( !isFE && m >= 10&& p == 2) max_offset = 0.4;
      if (isFE&&isMC && m == 10&& p == 6) max_offset = 0.06;
      if (isFE&&isMC && m == 10&& p == 6) min_slope  = 0.13;
      //         std::cout << "eta: " << m <<  ", p_T: " << p << std::endl;
      //         std::cout << "fit start values: " << "slope: " << slope << " offset: " << offset << std::endl;
      make_lin_fit(slope, d_slope, offset, d_offset, min_slope, max_offset);
      //         std::cout << "fit values: " << "slope: " << slope << " offset: " << offset << std::endl;
      lin_extrapol_mc->SetParameter(0, offset);
      lin_extrapol_mc->SetParError(0, d_offset);
      lin_extrapol_mc->SetParameter(1, slope);
      lin_extrapol_mc->SetParError(1, d_slope);
      lin_extrapol_mc->SetChisquare(0.);//TODO: set the correct chi/2
      extrapol_MC->GetListOfFunctions()->Add(lin_extrapol_mc);

      if ( p==0 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m==1  && p==1 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m==3  && p==1 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m>=6  && p==1 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m==5  && p==8 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m==6  && p==8 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m==7  && p>=8 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m==8  && p==1 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m==8  && p>=6 ) {offset = 0; d_offset = 0; }
      if ( !isFE && m==9  && p>=5 ) {offset = 0; d_offset = 0; }
      if (  isFE && m==10 && p==6 ) {offset = 0; d_offset = 0; }
      if (  isFE && m==10 && p>=8 ) {offset = 0; d_offset = 0; }
      if (  isFE && m==11 && p>=8 ) {offset = 0; d_offset = 0; }
      if (  isFE && m==12 && p>=7 ) {offset = 0; d_offset = 0; }
      if (  isFE && m>=11 && p==1 ) {offset = 0; d_offset = 0; }

      temp2.push_back(offset);
      temp_error2.push_back(d_offset);
      temp2_graph.push_back(extrapol_MC);
    }
    output.push_back(temp2);
    output_error.push_back(temp_error2);
    output_graph.push_back(temp2_graph);
  }
}

void widths_015_ratios( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > WidthsTwo, std::vector<std::vector<std::vector<double> > > WidthsTwoError, std::vector<std::vector<std::vector<double> > > forward_width_pt ) {
  double temp, tempError;
  for( unsigned int m = 0; m < Widths.size(); m++ ) {
    TString name_width = name1;
    name_width += "_eta"; name_width += m+1;
    TH1F *hist = new TH1F( name_width, name_width, 1100, 0, 1100 );
    hist ->GetYaxis()->SetTitle();
    hist ->GetXaxis()->SetTitle("p_{T} [GeV]");
    hist -> GetYaxis()->SetRangeUser( 0, 2. );
    for( unsigned int p = 0; p < Widths.at(m).size(); p++ ) {
      if (WidthsTwo.at(m).at(p).at(2) != 0.) {
        temp = Widths.at(m).at(p).at(2)/WidthsTwo.at(m).at(p).at(2);
        // tempError = WidthsError.at(m).at(p).at(2)/WidthsTwo.at(m).at(p).at(2) + ( Widths.at(m).at(p).at(2) * WidthsTwoError.at(m).at(p).at(2) ) / ( WidthsTwo.at(m).at(p).at(2) * WidthsTwo.at(m).at(p).at(2) ) ;
        tempError = sumSquare(WidthsError.at(m).at(p).at(2)/WidthsTwo.at(m).at(p).at(2) , ( Widths.at(m).at(p).at(2) * WidthsTwoError.at(m).at(p).at(2) ) / ( WidthsTwo.at(m).at(p).at(2) * WidthsTwo.at(m).at(p).at(2) ) );
        hist -> SetBinContent( hist -> FindBin( forward_width_pt.at(m).at(p).at(2) ), temp );
        hist -> SetBinError( hist -> FindBin( forward_width_pt.at(m).at(p).at(2) ), tempError );
      }
    }
    widths.push_back(hist);
  }
}

void correctJERwithPLI(std::vector< std::vector< double > > &Output, std::vector< std::vector< double > > &OutputError, std::vector< std::vector< double > > Widths, std::vector< std::vector< double > > WidthsError, std::vector< std::vector< double > > PLI, std::vector< std::vector< double > > PLIError, float shift) {
  for( unsigned int i = 0; i < Widths.size(); i++ ) {
    std::vector< double > temp2;
    std::vector< double > temp_error2;
    for( unsigned int j = 0; j < Widths.at(i).size(); j++ ) {
      double temp;
      double temp_error;
      if (Widths.at(i).at(j) != 0. ) {
        // With PLI correction (change also alpha=015):
        temp = TMath::Sqrt(2)*TMath::Sqrt( Widths.at(i).at(j) * Widths.at(i).at(j) - (1.+shift)*PLI.at(i).at(j) * PLI.at(i).at(j) );
        // temp_error = ( Widths.at(i).at(j) * WidthsError.at(i).at(j) + (1.+shift)* PLI.at(i).at(j) * PLIError.at(i).at(j) )/temp;
        temp_error = TMath::Sqrt(2)*sumSquare( Widths.at(i).at(j) * WidthsError.at(i).at(j), (1.+shift)*PLI.at(i).at(j) * PLIError.at(i).at(j) )/temp;
      } else {
        temp = 0.;
        temp_error = 0.;
      }
      if ( TMath::IsNaN(temp) ) { temp = 0. ; temp_error = 0.; }
      temp2.push_back(temp);
      temp_error2.push_back(temp_error);
    }
    Output.push_back(temp2);
    OutputError.push_back(temp_error2);
  }
}

void correctJERwithPLI015(std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<std::vector<std::vector<double> > > PLI, std::vector<std::vector< std::vector< double > > > PLIError, float shift) {
  for( unsigned int i = 0; i < Widths.size(); i++ ) {
    std::vector< double > temp2;
    std::vector< double > temp_error2;
    for( unsigned int j = 0; j < Widths.at(i).size(); j++ ) {
      double temp;
      double temp_error;
      // With PLI correction (change also alpha=0):
      temp = TMath::Sqrt(2)*TMath::Sqrt( Widths.at(i).at(j).at(2) * Widths.at(i).at(j).at(2) - (1.+shift)* PLI.at(i).at(j).at(2) * PLI.at(i).at(j).at(2) );
      // temp_error = ( Widths.at(i).at(j).at(2) * WidthsError.at(i).at(j).at(2) + (1.+shift) * PLI.at(i).at(j).at(2) * PLIError.at(i).at(j).at(2) )/temp;
      temp_error = TMath::Sqrt(2)*sumSquare( Widths.at(i).at(j).at(2) * WidthsError.at(i).at(j).at(2) , (1.+shift) * PLI.at(i).at(j).at(2) * PLIError.at(i).at(j).at(2) )/temp;
      if ( TMath::IsNaN(temp) ) { temp = 0 ; temp_error = 0; }
      temp2.push_back(temp);
      temp_error2.push_back(temp_error);
    }
    Output.push_back(temp2);
    OutputError.push_back(temp_error2);
  }
}

void correctForRef( TString name1, std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<double> > Input, std::vector<std::vector<double> > InputError, std::vector<std::vector<std::vector<double> > > width_pt, int shift, TString outdir) {
  double Ref, Probe, RefError, ProbeError, pT;

  TH1F *hist = new TH1F( name1+"_hist", name1+"_hist", 1100, 0, 1100 );
  hist ->GetYaxis()->SetTitle("#sigma_{A}");
  hist ->GetXaxis()->SetTitle("p_{T}");

  for( unsigned int p = 0; p < Input.at(0).size(); p++ ) {
    double temp = 0;
    double temp_error = 0;
    for (unsigned int i = 0; i < shift; i++) {
      temp += Input.at(i).at(p)/shift;
      temp_error += InputError.at(i).at(p)/shift;
    }
    if ( temp != 0 && !(TMath::IsNaN(temp))) {
      // pT = width_pt.at(0).at(p).at(5) ;
      pT = (double)(*std::max_element(width_pt.at(0).at(p).begin(),width_pt.at(0).at(p).end()));
      hist->SetBinContent(hist->FindBin(pT), temp);
      hist->SetBinError(  hist->FindBin(pT), temp_error );
    }
  }

  TCanvas* canv = new TCanvas("nscplot","nscplot",50,50,800,600);
  hist -> Draw();
  canv -> Print(outdir+"pdfy/JERs/reference"+name1+".pdf","pdf");
  canv -> Print(outdir+"pdfy/JERs/reference"+name1+".png","png");

  for( unsigned int m = 1; m < shift; m++ ) {
    std::vector< double > temp2;
    std::vector< double > temp_error2;

    for( unsigned int p = 0; p < Input.at(m).size(); p++ ) {
      double temp;
      double temp_error;

      if ( Input.at(m).at(p) != 0. ) {
        // pT = width_pt.at(m).at(p).at(5) ;
        pT = (double)(*std::max_element(width_pt.at(0).at(p).begin(),width_pt.at(0).at(p).end()));

        temp = Input.at(m).at(p);
        temp_error = InputError.at(m).at(p);

        if ( !(TMath::IsNaN(temp)) ) temp2.push_back(temp);
        else temp2.push_back(0.);
        if ( !(TMath::IsNaN(temp)) ) temp_error2.push_back(temp_error);
        else temp_error2.push_back(0.);
      }
      else { temp2.push_back(0.); temp_error2.push_back(0.); }
    }
    Output.push_back(temp2);
    OutputError.push_back(temp_error2);
  }


  for( unsigned int m = shift; m < Input.size() ; m++ ) {
    std::vector< double > temp2;
    std::vector< double > temp_error2;

    for( unsigned int p = 0; p < Input.at(m).size(); p++ ) {
      double temp;
      double temp_error;

      if ( Input.at(m).at(p) != 0. ) {
        // pT = width_pt.at(m).at(p).at(5) ;
        pT = (double)(*std::max_element(width_pt.at(0).at(p).begin(),width_pt.at(0).at(p).end()));

        Ref = hist->GetBinContent(hist->FindBin(pT));
        RefError = hist->GetBinError(hist->FindBin(pT));
        Probe = Input.at(m).at(p);
        ProbeError = InputError.at(m).at(p);

        temp = TMath::Sqrt( 2*Probe*Probe - Ref*Ref);
        temp_error = sumSquare(2*Probe*ProbeError, Ref*RefError )/temp;

        if ( !(TMath::IsNaN(temp)) ) temp2.push_back(temp);
        else temp2.push_back(0.);
        if ( !(TMath::IsNaN(temp)) ) temp_error2.push_back(temp_error);
        else temp_error2.push_back(0.);
      }
      else { temp2.push_back(0.); temp_error2.push_back(0.); }
    }
    Output.push_back(temp2);
    OutputError.push_back(temp_error2);
  }
  delete canv;
}



void makeScales( std::vector< std::vector< double > > &Output, std::vector< std::vector< double > > &OutputError, std::vector< std::vector< double > > Input1, std::vector< std::vector< double > > Input1Error, std::vector< std::vector< double > > Input2, std::vector< std::vector< double > > Input2Error ) {
  for( unsigned int i = 0; i < Input1.size(); i++ ) {
    std::vector< double > temp2;
    std::vector< double > temp_error2;
    for( unsigned int j = 0; j < Input1.at(i).size(); j++ ) {
      double temp;
      double temp_error;
      // Cut on scale factors not between 0.7 and 2
      //if ( (Input1.at(i).at(j)!=0.) && (Input2.at(i).at(j)!=0.) && Input1.at(i).at(j) / Input2.at(i).at(j)>0.7 && Input1.at(i).at(j) / Input2.at(i).at(j)<2. )
      if ( (Input1.at(i).at(j)!=0.) && (Input2.at(i).at(j)!=0.) ) {
        temp = Input1.at(i).at(j) / Input2.at(i).at(j);
        // temp_error = Input1Error.at(i).at(j) / Input2.at(i).at(j) + ( Input1.at(i).at(j) * Input2Error.at(i).at(j)) / ( Input2.at(i).at(j) * Input2.at(i).at(j) ) ;
        temp_error = sumSquare( Input1Error.at(i).at(j)/Input2.at(i).at(j) , ( Input1.at(i).at(j) * Input2Error.at(i).at(j)) / ( Input2.at(i).at(j) * Input2.at(i).at(j) ) ) ;
      } else { temp = 0.; temp_error = 0.; }
      if ( TMath::IsNaN(temp) ) { temp = 0.; temp_error = 0.; }
      temp2.push_back(temp);
      temp_error2.push_back(temp_error);
    }
    Output.push_back(temp2);
    OutputError.push_back(temp_error2);
  }
}


void fill_mctruth_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range) {
  for( unsigned int m = 0; m <  Widths.size(); m++ ) {
    TString name_width = name1; name_width += m+1;
    TH1F *h1 = new TH1F( name_width, name_width, 1100, 0, 1100 );
    h1 ->GetYaxis()->SetTitle("#sigma_{MCTruth}");	h1 ->GetXaxis()->SetTitle("p_{T}");	h1 -> Sumw2();

    for( unsigned int p = 0; p <  Widths.at(m).size(); p++ ) {
      double pT = (double)(*std::max_element(pt_binning.at(m).at(p).begin(),pt_binning.at(m).at(p).end()));
      if ( ( !(TMath::IsNaN(Widths.at(m).at(p).at(5))) )      && Widths.at(m).at(p).at(5)!= 0. ) h1 -> SetBinContent( h1 -> FindBin(pT), Widths.at(m).at(p).at(5)      );
      if ( ( !(TMath::IsNaN(WidthsError.at(m).at(p).at(5))) ) && Widths.at(m).at(p).at(5)!= 0. ) h1 -> SetBinError(   h1 -> FindBin(pT), WidthsError.at(m).at(p).at(5) );
    }
    h1 ->GetYaxis()-> SetRangeUser( 0., range );
    output.push_back(h1);
  }
}


void fill_hist( TString name1, std::vector< TH1F* > &output, std::vector< std::vector< double > > Widths, std::vector< std::vector< double > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning, double range, int shift2) {
  // int shift = 0;
  // if (name1.Contains("SF_")) {
  //   shift = 1;
  //   if (Widths.size() > 10) shift = 3;
  // }
  for( unsigned int m = 0; m <  Widths.size(); m++ ) {
    TString name = name1;
    name += m+1;
    TH1F *h1 = new TH1F( name, name, 1100, 0, 1100 );
    //std::cout << name1 << std::endl;;
    if (name1.Contains("SF_")) h1 ->GetYaxis()->SetTitle("#sigma_{JER}");
    else                      h1 ->GetYaxis()->SetTitle("Scale factor");
    h1 ->GetXaxis()->SetTitle("p_{T}");	h1 -> Sumw2();

    for( unsigned int p = 0; p <  Widths.at(m).size(); p++ ) {
      double pT = (double)(*std::max_element(pt_binning.at(m+shift2).at(p).begin(),pt_binning.at(m+shift2).at(p).end()));
      if ( ( !(TMath::IsNaN(Widths.at(m).at(p))) ) && Widths.at(m).at(p)!= 0. )      h1 -> SetBinContent(h1 -> FindBin(pT), Widths.at(m).at(p) );
      if ( ( !(TMath::IsNaN(WidthsError.at(m).at(p))) ) && Widths.at(m).at(p)!= 0. ) h1 -> SetBinError(  h1 -> FindBin(pT), WidthsError.at(m).at(p) );
    }
    h1 ->GetYaxis()-> SetRangeUser( 0., range );
    output.push_back(h1);
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


void make_lin_fit(double & slope, double & d_slope, double & offset, double & d_offset, double min_slope, double max_offset) {
  TMinuit min;
  min.SetPrintLevel(-1);
  //min.SetPrintLevel(0);
  if (slope  < 0.05  || slope  > 0.5) slope  = 0.15;
  if (offset < 0.001 || offset > 0.1) offset = 0.05;
  int err = min.DefineParameter(0, "slope", slope, d_slope, min_slope, 0.5);
  assert(err==0);
  err = min.DefineParameter(1, "offset", offset, d_offset, 0.001, max_offset);
  assert(err==0);
  min.SetFCN(chi2_linear);
  min.mnmigr();
  min.GetParameter(0, slope, d_slope);
  min.GetParameter(1, offset, d_offset);
}


void chi2_linear(Int_t& npar, Double_t* grad, Double_t& fval, Double_t* p, Int_t status) {
  if (data.y_cov_inv.GetNcols()==0) {
    double dummy;
    int ncols = data.y_cov.GetNcols();
    data.y_cov_inv.ResizeTo(ncols, ncols);
    data.y_cov_inv = data.y_cov.Invert(&dummy);
  }
  const size_t ndata = data.x_val.size(); // number of data points in x,y graph to fit to
  std::vector<double> delta_y(ndata);
  for(size_t i=0; i<ndata; ++i) {
    delta_y[i] = data.x_val[i]*p[0] + p[1] - data.y_val[i];
  }
  // now calculate the chi2, i.e.
  //  dy^T * C^{-1} * dy
  // where C is the variance--covariance matrix and dy = (y_data - y_pred)
  // This could probably be implemented in ROOT, but it's so simple, we just do it here:
  fval = 0.0;
  for(size_t i=0; i<ndata; ++i) {
    for(size_t j=0; j<ndata; ++j) {
      fval += delta_y[i] * delta_y[j] * data.y_cov_inv(i,j);
    }
  }
}


// Kristins code, truncated-RMS calculation.
// [ Previously, Matthias did an iterated fit
// for more and more truncated Gaussians ]
float GetAsymmWidth(TH1F* &htemp, double * xq_IQW, double * yq_IQW, int etabin, int ptbin, int alphabin) {
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
  if ( htemp->GetEntries() > 100 ) {
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

    for(int i=1; i <= IQW_high_bin_i; i++) {
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

  } else if ( htemp->GetEntries() == 0 ) {
    // Give a warning when no events are present:
    //std::cout << "pusty histogram, ale czilujemy" << std::endl;
    temp = 0;
  } else {
    // Do not truncate otherwise (for 0 < # events <= 100):
    //		width = htemp -> GetRMS();
    for(int i=1; i <= lastbin; i++) {
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


double sumSquare(double a, double b) {
  return TMath::Sqrt(a*a + b*b);
}


void findExtreme(std::vector<TH1*> vec, double *x_min, double *x_max, double *y_min, double *y_max) {
  std::vector<double> x;
  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetXaxis()->GetXmin());}
  *x_min = *std::min_element(x.begin(), x.end());
  x.clear();

  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetXaxis()->GetXmax());}
  *x_max = *std::max_element(x.begin(), x.end());
  x.clear();

  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetMinimum());}
  *y_min = *std::min_element(x.begin(), x.end());
  x.clear();

  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetMaximum());}
  *y_max = *std::max_element(x.begin(), x.end());
  x.clear();

  if ((*x_min) == (*x_max)) {
    *x_min = (*x_min)*0.9;
    *x_max = (*x_max)*1.2;
  }

  if ((*y_min) == (*y_max)) {
    *y_min = *y_min*0.9;
    *y_max = *y_max*1.2;
  }
}



void findExtreme2(std::vector<TH1*> vec, double *x_min, double *x_max, double *y_min, double *y_max) {
  std::vector<double> x;
  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetXaxis()->GetXmin());}
  *x_min = *std::min_element(x.begin(), x.end());
  x.clear();

  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetXaxis()->GetXmax());}
  *x_max = *std::max_element(x.begin(), x.end());
  x.clear();

  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetMinimum());}
  *y_min = *std::min_element(x.begin(), x.end());
  x.clear();

  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetBinContent(vec.at(i)->GetMaximumBin()));}
  *y_max = *std::max_element(x.begin(), x.end());
  x.clear();

  if ((*x_min) == (*x_max)) {
    *x_min = (*x_min)*0.9;
    *x_max = (*x_max)*1.2;
  }

  if ((*y_min) == (*y_max)) {
    *y_min = *y_min*0.9;
    *y_max = *y_max*1.2;
  }
}


void findExtreme_gr(std::vector<TGraphErrors*> vec, double *x_min, double *x_max, double *y_min, double *y_max) {
  std::vector<double> x;
  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(TMath::MinElement(vec.at(i)->GetN(),vec.at(i)->GetX()));}
  *x_min = *std::min_element(x.begin(), x.end());
  x.clear();

  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(TMath::MaxElement(vec.at(i)->GetN(),vec.at(i)->GetX()));}
  *x_max = *std::max_element(x.begin(), x.end());
  x.clear();

  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(TMath::MinElement(vec.at(i)->GetN(),vec.at(i)->GetY()));}
  *y_min = *std::min_element(x.begin(), x.end());
  x.clear();

  // for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetBinContent(vec.at(i)->GetMaximumBin()));}
  // for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetHistogram()->GetMaximum());}
  // for (int i = 0; i < vec.size(); i++) {x.push_back(vec.at(i)->GetHistogram()->GetBinContent(vec.at(i)->GetHistogram()->GetMaximumBin()));}
  for (unsigned int i = 0; i < vec.size(); i++) {x.push_back(TMath::MaxElement(vec.at(i)->GetN(),vec.at(i)->GetY()));}
  *y_max = *std::max_element(x.begin(), x.end());
  x.clear();

  if ((*x_min) == (*x_max)) {
    *x_min = (*x_min)*0.9;
    *x_max = (*x_max)*1.2;
  }

  if ((*y_min) == (*y_max)) {
    *y_min = *y_min*0.9;
    *y_max = *y_max*1.2;
  }

  if (*x_min >= *x_max ||  *y_min >= *y_max)
  {
    cout << "Aiuto" << endl;
  }
}

double findMinMax(TH1F* JER, std::vector< std::vector< double > > pt_width, TF1* NSC_ratio, TF1* constfit, bool isMin) {
	double min = 10000;
  double max = 0;
	for (unsigned int p = 2; p < pt_width.size(); p++) {// TODO It's set to 2 just because in the following steps pt>2"bin are used
    double pT = (double)(*std::max_element(pt_width.at(p).begin(),pt_width.at(p).end()));
    if (JER->GetBinContent(JER->FindBin(pT))== 0.) continue;
		min = std::min(min, TMath::Abs(NSC_ratio->Eval(pT) - constfit->Eval(pT)));
    max = std::max(min, TMath::Abs(NSC_ratio->Eval(pT) - constfit->Eval(pT)));
	}
  if (isMin) return min;
  else return max;
}

void fitLin( TH1F &hist, double &width, double &error ) {
  TF1 * linfit = new TF1( "linfit", "[0]+x*[1]", 0, 0.3 );
  linfit -> SetParameter( 0, 0.1 );
  linfit -> SetParameter( 1, 0.1 );
  linfit -> SetParLimits( 0, 0., 1. );
  hist.Fit( "linfit", "QM+" );
  width = linfit -> GetParameter(0);
  error = 1. * ( linfit -> GetParError(0) );
  delete linfit;
}




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Not used
void histLoadFE1( TFile &f, bool data, TString text, std::vector< std::vector< std::vector< TH1F* > > > &forward_hist , std::vector< std::vector< std::vector< TH1F* > > > &forward_gen_hist , int ptbin , int etabin) {
  int shift = 0;
  if ( etabin == 8 )	shift = 2;
  if ( etabin == 3 )	shift = 10; // it should be 10, i screwed a bit up with the hist preparation, it;s fixed

  for( int m = 0; m < etabin; m++ ) {
    std::vector< std::vector< TH1F* > > temp2;
    std::vector< std::vector< TH1F* > > temp2gen;

    for( int p = 0; p < ptbin; p++ ) {
      std::vector< TH1F* > temp1;
      std::vector< TH1F* > temp1gen;
      for( int r = 0; r < 6; r++ ) {
        TString name = text;
        TString namegen = "gen_"; namegen += name;
        name += shift+m; 	name += "_pt"; name += p+1; name += "_alpha"; name += r+1;
        namegen += shift+m; 	namegen += "_pt"; namegen += p+1; namegen += "_alpha"; namegen += r+1;
        temp1.push_back( ( TH1F* )f.Get( name ) );
        if ( data == false ) temp1gen.push_back( ( TH1F* )f.Get( namegen ) );
      }
      temp2.push_back(temp1);
      if ( data == false ) temp2gen.push_back(temp1gen);
    }
    forward_hist.push_back(temp2);
    if ( data == false ) forward_gen_hist.push_back(temp2gen);
  }
}


void correctPLIbinbybin(std::vector< std::vector< std::vector< double > > > &Output, std::vector< std::vector< std::vector< double > > > &OutputError, std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > PLI, std::vector< std::vector< std::vector< double > > > PLIError) {
  double temp, tempError;
  for( unsigned int i = 0; i < Widths.size(); i++ ) {
    std::vector< std::vector< double > > temp2;
    std::vector< std::vector< double > > temp_error2;
    for( unsigned int j = 0; j < Widths.at(i).size(); j++ ) {
      std::vector< double > temp1;
      std::vector< double > temp_error1;
      for( unsigned int k = 0; k < Widths.at(i).at(j).size(); k++ ) {
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


void fill_PLI_hist( TString name1, std::vector< std::vector< TH1F* > > &widths , std::vector< std::vector< std::vector< double > > > Widths, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning) {
  int shift;
  if (Widths.size() == 4 ) shift = 1;
  if (Widths.size() == 6 ) shift = 2;

  for( unsigned int m = 0; m <  Widths.size(); m++ ) {
    std::vector< TH1F* > temp1;
    for( unsigned int r = 0; r <  Widths.at(m).at(0).size(); r++ ) {
      TString name_width = name1;
      name_width += m+shift; name_width += "_alpha"; name_width += r+1;
      TH1F *h1 = new TH1F( name_width, name_width, 1100, 0, 1100 );
      h1 ->GetYaxis()->SetTitle("#sigma_{A,PLI}");	h1 ->GetXaxis()->SetTitle("p_{T}");	h1 -> Sumw2();
      for( unsigned int p = 0; p < Widths.at(m).size(); p++ ) {
        if ( !(TMath::IsNaN(Widths.at(m).at(p).at(r))) ) h1 -> SetBinContent( h1 -> FindBin( pt_binning.at(m).at(p).at(r) ), Widths.at(m).at(p).at(r) );
        if ( !(TMath::IsNaN(WidthsError.at(m).at(p).at(r))) ) h1 -> SetBinError( h1 -> FindBin( pt_binning.at(m).at(p).at(r) ), WidthsError.at(m).at(p).at(r) );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0.8*(h1 -> GetMinimum()), 1.2*(h1 -> GetMaximum()) );
      temp1.push_back(h1);
    }
    widths.push_back(temp1);
  }
}


void fit_NSC_PLI( std::vector< std::vector< TF1* > > &functions, std::vector< std::vector< TH1F* > > PLI_hists ) {
  for( unsigned int m = 0; m <  PLI_hists.size(); m++ ) {
    std::vector< TF1* > temp1;
    for( unsigned int r = 0; r <  PLI_hists.at(m).size(); r++ ) {
      TString name_width = "NSCfun";
      name_width += m+1; name_width += "_alpha"; name_width += r+1;
      TF1 * NSCfun = new TF1( name_width, "TMath::Sqrt(TMath::Power(([0]/x),2)+TMath::Power([1],2)/x + TMath::Power([2],2))", 0, 1100 );
      NSCfun->SetParName(0,"N");	NSCfun->SetParName(1,"S");	NSCfun->SetParName(2,"C");
      PLI_hists.at(m).at(r)-> Fit(name_width,"");
      NSCfun ->GetYaxis()-> SetRangeUser( 0.8*(NSCfun -> GetMinimum()), 1.2*(NSCfun -> GetMaximum()) );
      temp1.push_back(NSCfun);
    }
    functions.push_back(temp1);
  }
}


void def_fill_PLI( TString name1, std::vector< std::vector< TH1F* > > &widths , std::vector< std::vector< TF1* > > functions, std::vector< std::vector< std::vector< double > > > WidthsError, std::vector< std::vector< std::vector< double > > > pt_binning ) {
  double aMax[] = { 0.05, 0.1, 0.15, 0.2, 0.25, 0.3 };
  for( unsigned int m = 0; m < pt_binning.size(); m++ ) {
    std::vector< TH1F* > temp1;
    for( unsigned int p = 0; p < pt_binning.at(m).size(); p++ ) {
      TString name_width = name1;
      name_width += "_eta"; name_width += m+1; name_width += "_pt"; name_width += p+1;
      TH1F *h1 = new TH1F( name_width, name_width, 50, 0, 0.35 );
      h1 ->GetYaxis()->SetTitle("#sigma_{A}");	h1 ->GetXaxis()->SetTitle("#alpha_{max}");
      h1 -> Sumw2();
      for( unsigned int r = 0; r < pt_binning.at(m).at(p).size(); r++ ) {
        h1 -> SetBinContent( h1 -> FindBin( aMax[ r ] ), functions.at(m).at(r)->Eval( pt_binning.at(m).at(p).at(r) ) );
        h1 -> SetBinError( h1 -> FindBin( aMax[ r ] ), WidthsError.at(m).at(p).at(r) );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0.8*(h1 -> GetMinimum()), 1.2*(h1 -> GetMaximum()) );
      temp1.push_back(h1);
    }
    widths.push_back(temp1);
  }
}


void correctForPLI( TString name1, std::vector< std::vector< TH1F* > > widths, std::vector< std::vector< TH1F* > > &output, std::vector< std::vector< TF1* > > functions, std::vector< std::vector< std::vector< double > > > pt_binning) {
  double aMax[] = { 0.05, 0.1, 0.15, 0.2, 0.25, 0.3 };
  double temp, temp_error;
  double Ref, Ext, RefError;
  for( unsigned int m = 0; m < widths.size(); m++ ) {
    std::vector< TH1F* > temp1;
    for( unsigned int p = 0; p < widths.at(m).size(); p++ ) {
      TString name_width = name1;
      name_width += "_eta"; name_width += m+1; name_width += "_pt"; name_width += p+1;
      TH1F *h1 = new TH1F( name_width, name_width, 50, 0, 0.35 );
      h1 ->GetYaxis()->SetTitle("#sigma_{A}");	h1 ->GetXaxis()->SetTitle("#alpha_{max}");
      h1 -> Sumw2();
      for( unsigned int r = 0 ; r < pt_binning.at(m).at(p).size(); r++ ) {
        Ref = widths.at(m).at(p)->GetBinContent( h1 -> FindBin( aMax[ r ] ) );
        Ext = functions.at(m).at(r)->Eval(pt_binning.at(m).at(p).at(r));
        RefError = widths.at(m).at(p)->GetBinError( h1 -> FindBin( aMax[ r ] ) );
        temp = TMath::Sqrt( Ref*Ref - Ext*Ext );
        temp_error = (2*Ext*RefError + 2*Ref*RefError )/temp;
        //				if (temp != 0) std::cout << "before " << Ref << ", corrected " << temp << std::endl;
        if ( !(TMath::IsNaN(temp)) && temp != 0) h1 -> SetBinContent( h1 -> FindBin( aMax[ r ] ) , temp );
        if ( !(TMath::IsNaN(temp)) && temp != 0) h1 -> SetBinError( h1 -> FindBin( aMax[ r ] ) , temp_error );
      }
      h1 ->GetYaxis()-> SetRangeUser( 0.8*(h1 -> GetMinimum()), 1.2*(h1 -> GetMaximum()) );
      temp1.push_back(h1);
    }
    output.push_back(temp1);
  }
}


void fill_widths_pt( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<std::vector<double> > > forward_width_pt ) {
  for( unsigned int m = 0; m < Widths.size(); m++ ) {
    TString name_width = name1;
    name_width += "_eta"; name_width += m+1;
    TH1F *hist = new TH1F( name_width, name_width, 1100, 0, 1100 );
    hist ->GetYaxis()->SetTitle("k_{FSR}");
    hist ->GetXaxis()->SetTitle("p_{T}");
    hist -> GetYaxis()->SetRangeUser( 0, 0.4 );
    for( unsigned int p = 0; p < Widths.at(m).size(); p++ ) {
      if ( forward_width_pt.at(m).at(p).at(0)!= 0 ) {
        hist -> SetBinContent( hist -> FindBin( forward_width_pt.at(m).at(p).at(0) ), Widths.at(m).at(p) );
        hist -> SetBinError( hist -> FindBin( forward_width_pt.at(m).at(p).at(0) ), WidthsError.at(m).at(p) );
      }
    }
    hist ->GetYaxis()-> SetRangeUser( 0.8*(hist -> GetMinimum()), 1.2*(hist -> GetMaximum()) );
    widths.push_back(hist);
  }
}


void KFSR_ratios( TString name1, std::vector<TH1F*> &widths, std::vector<std::vector<double> > Widths, std::vector<std::vector<double> > WidthsError, std::vector<std::vector<double> > WidthsTwo, std::vector<std::vector<double> > WidthsTwoError, std::vector<std::vector<std::vector<double> > > forward_width_pt ) {
  double temp, tempError;
  for( unsigned int m = 0; m < Widths.size(); m++ ) {
    TString name_width = name1;
    name_width += "_eta"; name_width += m+1;
    TH1F *hist = new TH1F( name_width, name_width, 1100, 0, 1100 );
    hist ->GetYaxis()->SetTitle("k_{FSR}^{" + labeldata + "}/k_{FSR}^{MC}");
    //hist ->GetYaxis()->SetTitle("k_{FSR}^{Smeared MC}/k_{FSR}^{MC}");
    hist ->GetXaxis()->SetTitle("p_{T}");
    hist -> GetYaxis()->SetRangeUser( 0, 2. );
    for( unsigned int p = 0; p < Widths.at(m).size(); p++ ) {
      if ( forward_width_pt.at(m).at(p).at(0)!= 0 ) {
        temp = Widths.at(m).at(p)/WidthsTwo.at(m).at(p);
        tempError = WidthsError.at(m).at(p)/WidthsTwo.at(m).at(p) + ( Widths.at(m).at(p) * WidthsTwoError.at(m).at(p) ) / ( WidthsTwo.at(m).at(p) * WidthsTwo.at(m).at(p) ) ;
        hist -> SetBinContent( hist -> FindBin( forward_width_pt.at(m).at(p).at(0) ), temp );
        hist -> SetBinError( hist -> FindBin( forward_width_pt.at(m).at(p).at(0) ), tempError );
      }
    }
    widths.push_back(hist);
  }
}


void KNFS_line_fit( TString name1, std::vector<TH1F*> widths, std::vector< TF1* > &functions ) {
  for( unsigned int m = 0; m < widths.size(); m++ ) {
    TString name_width = name1;
    name_width += "_eta"; name_width += m+1;
    TF1 * linfit = new TF1( "linfit", "[0]+x*[1]", 0, 0.25 );
    widths.at(m)-> Fit("linfit","Q");
    functions.push_back(linfit);
  }
}


void correctKFSR( std::vector<std::vector<double> > &Output, std::vector<std::vector<double> > &OutputError, std::vector<std::vector<std::vector<double> > > Widths, std::vector<std::vector<std::vector<double> > > WidthsError, std::vector<TF1*> functions, std::vector<std::vector<std::vector<double> > > forward_width_pt ) {
  for( unsigned int i = 0; i < Widths.size(); i++ ) {
    std::vector< double > temp2;
    std::vector< double > temp_error2;
    for( unsigned int j = 0; j < Widths.at(i).size(); j++ ) {
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

// fitType 0 -> gaussian fit, 1 -> RMS (100% of points), 2 -> shifted gaussian
void fitAsym( TH1F &hist, double &width, double &error , double &centre, double &centre_error, double &chisqr, int fitType, double position , bool fix ) {
  TF1 * myfit = new TF1( "myfit", "[0]*exp(-0.5*TMath::Power(((x-[1])/[2]),2))", 0, 0.15 );
  //	TF1 * myfit2 = new TF1( "myfit2", "[0]* exp( -( x^2 + [1]^2 )/(2*[2]) ) * ( exp( 2*x*[1]/(2*[2]) ) + exp( -2*x*[1]/([2]*2) ) )	", 0, 0.15 );
  TF1 * myfit2 = new TF1( "myfit2", "[0]*exp( -( TMath::Power(x,2) + TMath::Power([1],2) )/(2*[2]) ) * ( exp( 2*x*[1]/(2*[2]) ) + exp( -2*x*[1]/([2]*2) ) )", 0, 0.15 );
  double norm = hist.Integral();
  if ( norm != 0 ) hist.Scale( 1/norm );
  if ( fitType == 0 ) {
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

    std::cout << "fit gave:          " << "width = " << width << std::endl; }*/
  } else if ( fitType == 1 ) {
    width = hist.GetRMS();
    error = hist.GetRMSError();

    centre = 0;
    centre_error = 0;
    chisqr = 0;
  } else if ( fitType == 2 ) {
    hist.Scale(1);

    myfit2 -> SetParameter( 0, 0.05 );
    myfit2 -> SetParLimits( 0, 0.001, 0.2 );
    if ( position != 0. ) {
      if ( fix == false ) {
        myfit2 -> SetParameter( 1, position );
        myfit2 -> SetParLimits( 1, 0., 0.2 );
      }
      if ( fix == true ) myfit2 -> FixParameter( 1, position );
    } else {
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


void fitConst( TH1F &hist, double &value, double &error ) {
  TF1 * constfit = new TF1( "constfit", "pol0", 0, 0.25 );
  constfit -> SetParameter( 0, 0.1 );
  constfit -> SetParLimits( 0, 0., 1. );
  hist.Fit( "constfit", "Q" );
  value = constfit -> GetParameter(0);
  error = constfit -> GetParError(0);
  delete constfit;
}


void fitMCTruth( TH1F &hist, double &width, double &error ) {
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


void fitShift( TH1F &hist, double &a, double &errora, double &b, double &errorb ) {
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

void histWidthAsym_old( std::vector< std::vector< std::vector< TH1F* > > > &Asymmetry , std::vector< std::vector< std::vector< double > > > &Widths, std::vector< std::vector< std::vector< double > > > &WidthsError , bool fill_all ) {
  std::sprintf(truncation_info, "truncation_info_%f.txt", truncate_fraction);
  double yq_IQW[2],xq_IQW[2];
  double asym;
  double asymerr;
  // Decide where to truncate
  xq_IQW[0] = 0.0;
  xq_IQW[1] = truncate_fraction;
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


        if (!fill_all) {
          if (Asymmetry.at(m).size()==9) {
            if ( p == 0 && r < 4) { asym = 0.; asymerr = 0.; };
            if ( p == 1 && r < 3) { asym = 0.; asymerr = 0.; };
            if ( p == 2 && r < 2) { asym = 0.; asymerr = 0.; };
            if ( p == 3 && r < 1) { asym = 0.; asymerr = 0.; };
            if ( p == 4 && r < 1) { asym = 0.; asymerr = 0.; };
          }
          if (Asymmetry.at(m).size()==6) {
            if ( p == 0 && r < 3) { asym = 0.; asymerr = 0.; };
            if ( p == 1 && r < 2) { asym = 0.; asymerr = 0.; };
            if ( p == 2 && r < 1) { asym = 0.; asymerr = 0.; };
            if ( p == 3 && r < 1) { asym = 0.; asymerr = 0.; };
          }
          if (Asymmetry.size()==10) {
            if ( m == 6 && p == 8 && r == 0) { asym = 0.; asymerr = 0.; };
            if ( m == 7 && p == 8 && r == 0) { asym = 0.; asymerr = 0.; };
            if ( m == 8 && p == 5 && r == 0) { asym = 0.; asymerr = 0.; };
            if ( m == 8 && p > 5 ) { asym = 0.; asymerr = 0.; };
            if ( m == 9 && p > 4 ) { asym = 0.; asymerr = 0.; };
          }
          if (Asymmetry.size()==11) {
            if ( m == 9 && p == 5 && r == 0) { asym = 0.; asymerr = 0.; };
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
