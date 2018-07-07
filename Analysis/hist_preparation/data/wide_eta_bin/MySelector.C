#define MySelector_cxx
// The class definition in MySelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("MySelector.C")
// Root > T->Process("MySelector.C","some options")
// Root > T->Process("MySelector.C+")
//

#include "MySelector.h"
#include "../../../../include/constants.h"
#include <TH2.h>
#include <TStyle.h>
#include <algorithm>
#include <iostream>

bool sortFunct( MyJet a, MyJet b) { return ( a.Pt() > b.Pt() ); }

void MySelector::BuildEvent(){
}


void MySelector::Begin(TTree * /*tree*/){


  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

  TotalEvents = 0;

}

void MySelector::SlaveBegin(TTree * /*tree*/){
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

  h_JetAvePt = new TH1F( "JetAvePt" , "Inclusive Jet Ave Pt" , 50 , 0 , 2000 );
  h_JetAvePt -> SetXTitle( "Pt_{ave}[GeV]" );
  h_JetAvePt -> Sumw2();
  // histograms.push_back(h_JetAvePt);

  h_Jet1Pt = new TH1F( "Jet1Pt" , "Inclusive Jet 1 Pt" , 50 , 0 , 2000 );
  h_Jet1Pt -> SetXTitle( "Pt_1[GeV]" );
  h_Jet1Pt -> Sumw2();
  // histograms.push_back(h_Jet1Pt);

  h_Jet2Pt = new TH1F( "Jet2Pt" , "Inclusive Jet 2 Pt" , 50 , 0 , 2000 );
  h_Jet2Pt -> SetXTitle( "Pt_2[GeV]" );
  h_Jet2Pt -> Sumw2();
  // histograms.push_back(h_Jet2Pt);

  h_Jet3Pt = new TH1F( "Jet3Pt" , "Inclusive Jet 3 Pt" , 50 , 0 , 2000 );
  h_Jet3Pt -> SetXTitle( "Pt_3[GeV]" );
  h_Jet3Pt -> Sumw2();
  // histograms.push_back(h_Jet3Pt);

  h_FEJetAvePt = new TH1F( "FEJetAvePt" , "Inclusive FEJet Ave Pt" , 50 , 0 , 2000 );
  h_FEJetAvePt -> SetXTitle( "Pt_{ave}[GeV]" );
  h_FEJetAvePt -> Sumw2();
  // histograms.push_back(h_FEJetAvePt);

  h_FEJet1Pt = new TH1F( "FEJet1Pt" , "Inclusive FEJet 1 Pt" , 50 , 0 , 2000 );
  h_FEJet1Pt -> SetXTitle( "Pt_1[GeV]" );
  h_FEJet1Pt -> Sumw2();
  // histograms.push_back(h_FEJet1Pt);

  h_FEJet2Pt = new TH1F( "FEJet2Pt" , "Inclusive FEJet 2 Pt" , 50 , 0 , 2000 );
  h_FEJet2Pt -> SetXTitle( "Pt_2[GeV]" );
  h_FEJet2Pt -> Sumw2();
  // histograms.push_back(h_FEJet2Pt);

  h_FEJet3Pt = new TH1F( "FEJet3Pt" , "Inclusive FEJet 3 Pt" , 50 , 0 , 2000 );
  h_FEJet3Pt -> SetXTitle( "Pt_3[GeV]" );
  h_FEJet3Pt -> Sumw2();
  // histograms.push_back(h_FEJet3Pt);

  h_PU = new TH1F( "PileUp" , "PU distribution" , 60 , 0 , 60 );
  h_PU -> SetXTitle( "PU" );
  h_PU -> Sumw2();
  // histograms.push_back(h_PU);

  h_alpha_raw = new TH1F( "Alpha_raw" , "#alpha before selection" , 80, 0., 0.8 );
  h_alpha_raw -> SetXTitle( "#alpha_raw" );
  h_alpha_raw -> Sumw2();

  h_alpha_select = new TH1F( "Alpha" , "#alpha after selection" , 80, 0., 0.8 );
  h_alpha_select -> SetXTitle( "#alpha" );
  h_alpha_select -> Sumw2();


  EtaBinsNo = 10;            // st method bins
  EtaForwardBinsNo = 3;     // st method bins with fw triggers ?
  EtaFtControlBinsNo = 8;   // fw method bins with normal triggers
  EtaFtBinsNo = 3;          // fw method bins with fw triggers

  PtBinsNo = n_pt_bins_Si;
  PtFTBinsNo = n_pt_bins_Si_HF;
  AlphaBinsNo = 6;

  // I define histograms for the "normal" JER calculation

  for( int m = 0; m < EtaBinsNo; m++ ){
    std::vector< std::vector< TH1F* > > temp2, temp2pt, temp2rho, temp2pt3;
    std::vector< TH1F* > alpha_temp2;
    for( int p = 0; p < PtBinsNo; p++ ){
      std::vector< TH1F* > temp1, temp1pt, temp1rho, temp1pt3;
      char name_alpha[100];
      sprintf(name_alpha, "alpha_eta%d_pt_%d",m ,p );
      TH1F *h1_alpha = new TH1F( name_alpha, name_alpha, 80, 0., 0.8 );
      h1_alpha ->GetYaxis()->SetTitle("a.u.");    h1_alpha ->GetXaxis()->SetTitle("Alpha");
      h1_alpha -> Sumw2(); alpha_temp2.push_back(h1_alpha);
      for( int r = 0; r < AlphaBinsNo; r++ ){
        TString name_fw = "asymm_"; name_fw += "eta"; name_fw += m+1; name_fw += "_pt"; name_fw += p+1; name_fw += "_alpha"; name_fw += r+1;
        TString name_fw_pt = "asymmpt_"; name_fw_pt += "probe"; name_fw_pt += m+1; name_fw_pt += "_pt"; name_fw_pt += p+1; name_fw_pt += "_alpha"; name_fw_pt += r+1;
        TString name_fw_rho = "asymmrho_"; name_fw_rho += "probe"; name_fw_rho += m+1; name_fw_rho += "_pt"; name_fw_rho += p+1; name_fw_rho += "_alpha"; name_fw_rho += r+1;
        TString name_fw_pt3 = "asymmpt3_"; name_fw_pt3 += "probe"; name_fw_pt3 += m+1; name_fw_pt3 += "_pt"; name_fw_pt3 += p+1; name_fw_pt3 += "_alpha"; name_fw_pt3 += r+1;
        TH1F *h1 = new TH1F( name_fw, name_fw, 160, -0.8, 0.8 );
        h1 ->GetYaxis()->SetTitle("a.u.");    h1 ->GetXaxis()->SetTitle("Asymmetry");
        h1 -> Sumw2(); temp1.push_back(h1);
        TH1F *h2 = new TH1F( name_fw_pt, name_fw_pt, 50, 0, 1500 );
        h2 ->GetYaxis()->SetTitle("a.u.");    h2 ->GetXaxis()->SetTitle("Pt[GeV]");
        h2 -> Sumw2(); temp1pt.push_back(h2);
        TH1F *h3 = new TH1F( name_fw_rho, name_fw_rho, 100, 0, 100 );
        h3 ->GetYaxis()->SetTitle("a.u.");    h3 ->GetXaxis()->SetTitle("rho");
        h3 -> Sumw2(); temp1rho.push_back(h3);
        TH1F *h4 = new TH1F( name_fw_pt3, name_fw_pt3, 50, 0, 1500 );
        h4 ->GetYaxis()->SetTitle("a.u.");    h4 ->GetXaxis()->SetTitle("Pt[GeV]");
        h4 -> Sumw2(); temp1pt3.push_back(h4);
      }
      temp2.push_back(temp1); temp2pt.push_back(temp1pt); temp2rho.push_back(temp1rho);  temp2pt3.push_back(temp1pt3);
    }
    asymmetries_all.push_back(temp2); asymmetries_pt_all.push_back(temp2pt); asymmetries_rho_all.push_back(temp2rho); asymmetries_pt3_all.push_back(temp2pt3);
    alpha_spectrum.push_back(alpha_temp2);
  }

  for( int m = 0; m < EtaForwardBinsNo; m++ ){
    std::vector< std::vector< TH1F* > > temp2, temp2pt, temp2rho, temp2pt3;
    std::vector< TH1F* > alpha_temp2;
    for( int p = 0; p < PtFTBinsNo; p++ ){
      std::vector< TH1F* > temp1, temp1pt, temp1rho, temp1pt3;
      char name_alpha[100];
      sprintf(name_alpha, "alpha_eta%d_pt_%d",m+ EtaBinsNo ,p );
      TH1F *h1_alpha = new TH1F( name_alpha, name_alpha, 80, 0., 0.8 );
      h1_alpha ->GetYaxis()->SetTitle("a.u.");    h1_alpha ->GetXaxis()->SetTitle("Alpha");
      h1_alpha -> Sumw2(); alpha_temp2.push_back(h1_alpha);
      for( int r = 0; r < AlphaBinsNo; r++ ){
        TString name_fw = "asymm_"; name_fw += "eta"; name_fw += m+1+EtaBinsNo; name_fw += "_pt"; name_fw += p+1; name_fw += "_alpha"; name_fw += r+1;
        TString name_fw_pt = "asymmpt_"; name_fw_pt += "probe"; name_fw_pt += m+1+EtaBinsNo; name_fw_pt += "_pt"; name_fw_pt += p+1; name_fw_pt += "_alpha"; name_fw_pt += r+1;
        TString name_fw_rho = "asymmrho_"; name_fw_rho += "probe"; name_fw_rho += m+1+EtaBinsNo; name_fw_rho += "_pt"; name_fw_rho += p+1; name_fw_rho += "_alpha"; name_fw_rho += r+1;
        TString name_fw_pt3 = "asymmpt3_"; name_fw_pt3 += "probe"; name_fw_pt3 += m+1+EtaBinsNo; name_fw_pt3 += "_pt"; name_fw_pt3 += p+1; name_fw_pt3 += "_alpha"; name_fw_pt3 += r+1;
        TH1F *h1 = new TH1F( name_fw, name_fw, 160, -0.8, 0.8 );
        h1 ->GetYaxis()->SetTitle("a.u.");    h1 ->GetXaxis()->SetTitle("Asymmetry");
        h1 -> Sumw2(); temp1.push_back(h1);
        TH1F *h2 = new TH1F( name_fw_pt, name_fw_pt, 50, 0, 1500 );
        h2 ->GetYaxis()->SetTitle("a.u.");    h2 ->GetXaxis()->SetTitle("Pt[GeV]");
        h2 -> Sumw2(); temp1pt.push_back(h2);
        TH1F *h3 = new TH1F( name_fw_rho, name_fw_rho, 100, 0, 100 );
        h3 ->GetYaxis()->SetTitle("a.u.");    h3 ->GetXaxis()->SetTitle("rho");
        h3 -> Sumw2(); temp1rho.push_back(h3);
        TH1F *h4 = new TH1F( name_fw_pt3, name_fw_pt3, 50, 0, 1500 );
        h4 ->GetYaxis()->SetTitle("a.u.");    h4 ->GetXaxis()->SetTitle("Pt[GeV]");
        h4 -> Sumw2(); temp1pt3.push_back(h4);
      }
      temp2.push_back(temp1); temp2pt.push_back(temp1pt); temp2rho.push_back(temp1rho); temp2pt3.push_back(temp1pt3);
    }
    asymmetries_all.push_back(temp2); asymmetries_pt_all.push_back(temp2pt); asymmetries_rho_all.push_back(temp2rho); asymmetries_pt3_all.push_back(temp2pt3);
    alpha_spectrum.push_back(alpha_temp2);
  }

  // I define histograms used in forward extension for reference eta ( 0, s_eta_barr )

  for( int m = 0; m < EtaFtBinsNo; m++ ){
    std::vector< std::vector< TH1F* > > temp2, temp2pt, temp2rho, temp2pt3;
    std::vector< TH1F* > alpha_temp2;
    for( int p = 0; p < PtFTBinsNo; p++ ){
      std::vector< TH1F* > temp1, temp1pt, temp1rho, temp1pt3;
      char name_alpha[100];
      sprintf(name_alpha, "forward_alpha_eta%d_pt_%d",m + 2 + EtaFtControlBinsNo ,p );
      TH1F *h1_alpha = new TH1F( name_alpha, name_alpha, 80, 0., 0.8 );
      h1_alpha ->GetYaxis()->SetTitle("a.u.");    h1_alpha ->GetXaxis()->SetTitle("Alpha");
      h1_alpha -> Sumw2(); alpha_temp2.push_back(h1_alpha);
      for( int r = 0; r < AlphaBinsNo; r++ ){
        TString name_fw = "forward_"; name_fw += "probe"; name_fw += m+2+EtaFtControlBinsNo; name_fw += "_pt"; name_fw += p+1; name_fw += "_alpha"; name_fw += r+1;
        TString name_fw_pt = "forwardpt_"; name_fw_pt += "probe"; name_fw_pt += m+2+EtaFtControlBinsNo; name_fw_pt += "_pt"; name_fw_pt += p+1; name_fw_pt += "_alpha"; name_fw_pt += r+1;
        TString name_fw_rho = "forwardrho_"; name_fw_rho += "probe"; name_fw_rho += m+2+EtaFtControlBinsNo; name_fw_rho += "_pt"; name_fw_rho += p+1; name_fw_rho += "_alpha"; name_fw_rho += r+1;
        TString name_fw_pt3 = "forwardpt3_"; name_fw_pt3 += "probe"; name_fw_pt3 += m+2+EtaFtControlBinsNo; name_fw_pt3 += "_pt"; name_fw_pt3 += p+1; name_fw_pt3 += "_alpha"; name_fw_pt3 += r+1;
        TH1F *h1 = new TH1F( name_fw, name_fw, 160, -0.8, 0.8 );
        h1 ->GetYaxis()->SetTitle("a.u.");    h1 ->GetXaxis()->SetTitle("Asymmetry");
        h1 -> Sumw2(); temp1.push_back(h1);
        TH1F *h2 = new TH1F( name_fw_pt, name_fw_pt, 50, 0, 1500 );
        h2 ->GetYaxis()->SetTitle("a.u.");    h2 ->GetXaxis()->SetTitle("Pt[GeV]");
        h2 -> Sumw2(); temp1pt.push_back(h2);
        TH1F *h3 = new TH1F( name_fw_rho, name_fw_rho, 100, 0, 100 );
        h3 ->GetYaxis()->SetTitle("a.u.");    h3 ->GetXaxis()->SetTitle("rho");
        h3 -> Sumw2(); temp1rho.push_back(h3);
        TH1F *h4 = new TH1F( name_fw_pt3, name_fw_pt3, 50, 0, 1500 );
        h4 ->GetYaxis()->SetTitle("a.u.");    h4 ->GetXaxis()->SetTitle("Pt[GeV]");
        h4 -> Sumw2(); temp1pt3.push_back(h4);
      }
      temp2.push_back(temp1); temp2pt.push_back(temp1pt); temp2rho.push_back(temp1rho); temp2pt3.push_back(temp1pt3);
    }
    forward_hist.push_back(temp2); forward_pt_hist.push_back(temp2pt); forward_rho_hist.push_back(temp2rho); forward_pt3_hist.push_back(temp2pt3);
    forward_alpha_spectrum.push_back(alpha_temp2);
  }

  // I define histograms for the control eta bins for forward method calculation

  for( int m = 0; m < EtaFtControlBinsNo; m++ ){
    std::vector< std::vector< TH1F* > > temp2, temp2pt, temp2rho, temp2pt3;
    std::vector< TH1F* > alpha_temp2;
    for( int p = 0; p < PtBinsNo; p++ ){
      std::vector< TH1F* > temp1, temp1pt, temp1rho, temp1pt3;
      char name_alpha[100];
      sprintf(name_alpha, "forward_alpha_eta%d_pt_%d", m + 2, p );
      TH1F *h1_alpha = new TH1F( name_alpha, name_alpha, 80, 0., 0.8 );
      h1_alpha ->GetYaxis()->SetTitle("a.u.");    h1_alpha ->GetXaxis()->SetTitle("Alpha");
      h1_alpha -> Sumw2(); alpha_temp2.push_back(h1_alpha);
      for( int r = 0; r < AlphaBinsNo; r++ ){
        TString name_fw = "forward_control_"; name_fw += "probe"; name_fw += m+2; name_fw += "_pt"; name_fw += p+1; name_fw += "_alpha"; name_fw += r+1;
        TString name_fw_pt = "forwardpt_control_"; name_fw_pt += "probe"; name_fw_pt += m+2; name_fw_pt += "_pt"; name_fw_pt += p+1; name_fw_pt += "_alpha"; name_fw_pt += r+1;
        TString name_fw_rho = "forwardrho_control_"; name_fw_rho += "probe"; name_fw_rho += m+2; name_fw_rho += "_pt"; name_fw_rho += p+1; name_fw_rho += "_alpha"; name_fw_rho += r+1;
        TString name_fw_pt3 = "forwardpt3_control_"; name_fw_pt3 += "probe"; name_fw_pt3 += m+2; name_fw_pt3 += "_pt"; name_fw_pt3 += p+1; name_fw_pt3 += "_alpha"; name_fw_pt3 += r+1;
        TH1F *h1 = new TH1F( name_fw, name_fw, 160, -0.8, 0.8 );
        h1 ->GetYaxis()->SetTitle("a.u.");    h1 ->GetXaxis()->SetTitle("Asymmetry");
        h1 -> Sumw2(); temp1.push_back(h1);
        TH1F *h2 = new TH1F( name_fw_pt, name_fw_pt, 50, 0, 1500 );
        h2 ->GetYaxis()->SetTitle("a.u.");    h2 ->GetXaxis()->SetTitle("Pt[GeV]");
        h2 -> Sumw2(); temp1pt.push_back(h2);
        TH1F *h3 = new TH1F( name_fw_rho, name_fw_rho, 100, 0, 100 );
        h3 ->GetYaxis()->SetTitle("a.u.");    h3 ->GetXaxis()->SetTitle("rho");
        h3 -> Sumw2(); temp1rho.push_back(h3);
        TH1F *h4 = new TH1F( name_fw_pt3, name_fw_pt3, 50, 0, 1500 );
        h4 ->GetYaxis()->SetTitle("a.u.");    h4 ->GetXaxis()->SetTitle("Pt[GeV]");
        h4 -> Sumw2(); temp1pt3.push_back(h4);
      }
      temp2.push_back(temp1); temp2pt.push_back(temp1pt); temp2rho.push_back(temp1rho); temp2pt3.push_back(temp1pt3);
    }
    forward_hist_dijet.push_back(temp2); forward_pt_hist_dijet.push_back(temp2pt); forward_rho_hist_dijet.push_back(temp2rho); forward_pt3_hist_dijet.push_back(temp2pt3);
    forward_alpha_spectrum_dijet.push_back(alpha_temp2);
  }

}

Bool_t MySelector::Process(Long64_t entry){
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either MySelector::GetEntry() or TBranch::GetEntry()
  // to read either all or the required parts of the data. When processing
  // keyed objects with PROOF, the object is already loaded and is available
  // via the fObject pointer.
  //
  // This function should contain the "body" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  ++TotalEvents;

  GetEntry( entry );

  BuildEvent();


  //2017
  std::vector<int> p_bins(pt_bins_Si, pt_bins_Si + sizeof(pt_bins_Si)/sizeof(int));
  std::vector<int> p_bins_FT(pt_bins_Si_HF, pt_bins_Si_HF + sizeof(pt_bins_Si_HF)/sizeof(int));
  std::vector<double> eta_bins_all(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_ref_down(eta_bins+10, eta_bins + sizeof(eta_bins)/sizeof(double)-1);
  std::vector<double> eta_ref_up(eta_bins+11, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_bins_control(eta_bins+3, eta_bins + sizeof(eta_bins)/sizeof(double)-3);
  eta_bins_control.insert(eta_bins_control.begin(), 0.);
  double alpha_bins [] = { 0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3 };


  // Triggers are called by index of this list



  bool trigger[PtBinsNo];
  bool ftrigger[PtFTBinsNo];

  std::fill_n(trigger, PtBinsNo, false);
  std::fill_n(ftrigger, PtFTBinsNo, false);

  if(pass_trigger_bl){
    for( int i = 0; i < PtBinsNo; i++ ){
      if(p_bins[i] <= pt_ave && p_bins[i + 1] >= pt_ave){
        trigger[i] = true;
      }
    }
  }
  if(!pass_trigger_hf){ // FIXME this is only a temporal solution
    for( int i = 0; i < PtFTBinsNo; i++ ){
      if(p_bins_FT[i] <= pt_ave && p_bins_FT[i + 1] >= pt_ave){
        ftrigger[i] = true;
      }
    }
  }

  double weight;

  weight = 1.0;

  if( TotalEvents%100000 == 0 ){  std::cout << "            Analyzing event #" << TotalEvents << std::endl; }

  h_PU -> Fill( npuIT, 1 );

  double jet_threshold = 15;

  double alpha_raw;

  if ( jet3_pt > jet_threshold ){
    alpha_raw =  2 * jet3_pt/( jet1_pt + jet2_pt );
  }

  h_alpha_raw -> Fill( alpha_raw, 1 );

  double parallel, perpendicular, complete, alpha;

  // Below I choose what kind of asymmetries I want to study!

  //    bool excl_bin = true;  // exclusive
  bool excl_bin = false; // inclusive

  int flag1 = 0; // 0 -> complete_alpha
  // 1 -> parallel
  // 2 -> perpendicular
  bool pass_trigger = false;

  if ( jet2_pt > jet_threshold ){
    if ( jet3_pt > jet_threshold ){
      complete =  alpha_raw; //2 * jet3_pt/( jet1_pt + jet2_pt );
      parallel = alpha_raw; //(2*Jets[2]*(Jets[0]-Jets[1]))/((Jets[0]-Jets[1]).Pt()*( jet1_pt + jet2_pt ));
      perpendicular = alpha_raw; //TMath::Sqrt( TMath::Power( complete, 2 ) - TMath::Power( parallel, 2) );
    } else {
      complete = 0 ;
      parallel = 0 ;
      perpendicular = 0 ;
    }

    if (flag1 == 0 ) alpha = TMath::Abs(complete);
    if (flag1 == 1 ) alpha = TMath::Abs(parallel);
    if (flag1 == 2 ) alpha = TMath::Abs(perpendicular);

    if ( TMath::Abs(TVector2::Phi_mpi_pi((probejet_phi - barreljet_phi)))  > 2.7 ){ // I fill alpha_max histograms
      for ( int k = 0 ; k < PtBinsNo ; k++ ){
        if(trigger[k]){
          if ((0.5*(jet1_pt+jet2_pt) > p_bins[k]) &&
          (0.5*(jet1_pt+jet2_pt) < p_bins[k+1]) ){
            pass_trigger = true;
            for ( int r = 0 ; r < EtaBinsNo ; r++ ){
              if (TMath::Abs( probejet_eta ) > eta_bins_all[r] &&
              TMath::Abs( probejet_eta ) < eta_bins_all[r+1] &&
              TMath::Abs( barreljet_eta) > eta_bins_all[r] &&
              TMath::Abs( barreljet_eta) < eta_bins_all[r+1] ){

                h_alpha_select -> Fill( alpha, 1 );
                for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                  if ( alpha < alpha_bins[ m+1 ] ){
                    double asy = asymmetry;
                    if (((rand()%2)+1)==1 ) { asy = - asy;}
                    asymmetries_all.at(r).at(k).at(m) -> Fill( asy , weight );
                    asymmetries_pt_all.at(r).at(k).at(m) -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                    asymmetries_rho_all.at(r).at(k).at(m) -> Fill( rho, weight );
                    asymmetries_pt3_all.at(r).at(k).at(m) -> Fill( jet3_pt, weight );
                    if ( m == AlphaBinsNo-1 ){
                      h_JetAvePt -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                      h_Jet1Pt -> Fill( jet1_pt, weight );
                      h_Jet2Pt -> Fill( jet2_pt, weight );
                      h_Jet3Pt -> Fill( jet3_pt, weight );
                    }
                    if ( excl_bin ) break;
                  }
                }
                alpha_spectrum.at(r).at(k) -> Fill(alpha, weight);
              }
            }
            for ( int r = 0 ; r < EtaFtControlBinsNo ; r++ ){
              if (( TMath::Abs( probejet_eta ) > 0. &&
              TMath::Abs( probejet_eta ) < s_eta_barr &&
              TMath::Abs( barreljet_eta ) > eta_bins_control[r] &&
              TMath::Abs( barreljet_eta ) < eta_bins_control[r+1]) ||
              (TMath::Abs( barreljet_eta ) > 0. &&
              TMath::Abs( barreljet_eta ) < s_eta_barr &&
              TMath::Abs( probejet_eta ) > eta_bins_control[r] &&
              TMath::Abs( probejet_eta ) < eta_bins_control[r+1]) ){
                for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                  if ( alpha < alpha_bins[ m+1 ] ){
                    double asy = asymmetry;
                    if ((TMath::Abs( probejet_eta ) > 0. && TMath::Abs( probejet_eta ) < s_eta_barr && TMath::Abs( barreljet_eta ) > eta_bins_control[r] && TMath::Abs( barreljet_eta ) < eta_bins_control[r+1])) { asy = - asymmetry;}
                    if ( (((rand()%2)+1)==1) && r ==0 ) { asy = - asy;}
                    forward_hist_dijet.at(r).at(k).at(m) -> Fill( asy , weight );
                    forward_pt_hist_dijet.at(r).at(k).at(m) -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                    forward_rho_hist_dijet.at(r).at(k).at(m) -> Fill( rho, weight );
                    forward_pt3_hist_dijet.at(r).at(k).at(m) -> Fill( jet3_pt, weight );
                    if ( m == AlphaBinsNo-1 ){
                      h_FEJetAvePt -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                      h_FEJet1Pt -> Fill( jet1_pt, weight );
                      h_FEJet2Pt -> Fill( jet2_pt, weight );
                      h_FEJet3Pt -> Fill( jet3_pt, weight );
                    }
                    if ( excl_bin ) break;
                  }
                }
                forward_alpha_spectrum_dijet.at(r).at(k) -> Fill(alpha, weight);
              }
            }
            break;
          }
        }
      }
      for ( int k = 0 ; k < PtFTBinsNo ; k++ ){
        if(ftrigger[k]){
          if ((0.5*(jet1_pt+jet2_pt) > p_bins_FT[k]) &&
          (0.5*(jet1_pt+jet2_pt) < p_bins_FT[k+1])){
            pass_trigger = true;
            for ( int r = 0 ; r < EtaFtBinsNo ; r++ ){
              if (( TMath::Abs( probejet_eta ) > 0. &&
              TMath::Abs( probejet_eta ) < s_eta_barr &&
              TMath::Abs( barreljet_eta ) > eta_ref_down[r] &&
              TMath::Abs( barreljet_eta ) < eta_ref_up[r]) ||
              (TMath::Abs( barreljet_eta ) > 0. &&
              TMath::Abs( barreljet_eta ) < s_eta_barr &&
              TMath::Abs( probejet_eta ) > eta_ref_down[r] &&
              TMath::Abs( probejet_eta ) < eta_ref_up[r]) ){
                for ( int m = 0 ; m < AlphaBinsNo; m++ ){
                  if ( alpha < alpha_bins[ m+1 ] ){
                    double asy = asymmetry;
                    if ((TMath::Abs( probejet_eta ) > 0. && TMath::Abs( probejet_eta ) < s_eta_barr && TMath::Abs( barreljet_eta ) > eta_ref_down[r] && TMath::Abs( barreljet_eta ) < eta_ref_up[r])) { asy = - asymmetry;}
                    // std::cout << "help " << ftrigger[k] << " " << asy << std::endl;
                    forward_hist.at(r).at(k).at(m) -> Fill( asy , weight );
                    forward_pt_hist.at(r).at(k).at(m) -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                    forward_rho_hist.at(r).at(k).at(m) -> Fill( rho, weight );
                    forward_pt3_hist.at(r).at(k).at(m) -> Fill( jet3_pt, weight );
                    if ( m == AlphaBinsNo-1 ){
                      h_FEJetAvePt -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                      h_FEJet1Pt -> Fill( jet1_pt, weight );
                      h_FEJet2Pt -> Fill( jet2_pt, weight );
                      h_FEJet3Pt -> Fill( jet3_pt, weight );
                    }
                    if ( excl_bin ) break;
                  }
                }
                forward_alpha_spectrum.at(r).at(k) -> Fill(alpha, weight);
              }
            }
            for ( int r = 0 ; r < EtaForwardBinsNo; r++ ){
              if (TMath::Abs( probejet_eta ) > eta_bins_all[r+EtaBinsNo] &&
              TMath::Abs( probejet_eta ) < eta_bins_all[r+1+EtaBinsNo] &&
              TMath::Abs( barreljet_eta ) > eta_bins_all[r+EtaBinsNo] &&
              TMath::Abs( barreljet_eta ) < eta_bins_all[r+1+EtaBinsNo] ){
                h_alpha_select -> Fill( alpha, 1 );
                for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                  if ( alpha < alpha_bins[ m+1 ] ){
                    double asy = asymmetry;
                    if (((rand()%2)+1)==1 ) { asy = - asymmetry;}
                    asymmetries_all.at(r+EtaBinsNo).at(k).at(m) -> Fill( asy , weight );
                    asymmetries_pt_all.at(r+EtaBinsNo).at(k).at(m) -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                    asymmetries_rho_all.at(r).at(k).at(m) -> Fill( rho, weight );
                    asymmetries_pt3_all.at(r).at(k).at(m) -> Fill( jet3_pt, weight );
                    if ( m == AlphaBinsNo-1 ){
                      h_JetAvePt -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                      h_Jet1Pt -> Fill( jet1_pt, weight );
                      h_Jet2Pt -> Fill( jet2_pt, weight );
                      h_Jet3Pt -> Fill( jet3_pt, weight );
                    }
                    if ( excl_bin ) break;
                  }
                }
                alpha_spectrum.at(r).at(k) -> Fill(alpha, weight);
              }
            }
            break;
          }
        }
      }
    }
  }
  return kTRUE;
}

void MySelector::SlaveTerminate(){
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

  std::cout <<"            Analyzed events #" <<  TotalEvents << std::endl;



  std::ofstream mytxtfile;
  mytxtfile.open ("counts.txt");
  mytxtfile << "Analyzed events #" <<  TotalEvents << "\n";
  mytxtfile.close();

  TFile *fpt = new TFile("pt_data_incl_full.root","RECREATE"); ;
  fpt->cd();
  h_alpha_raw -> Write();
  h_alpha_select -> Write();
  h_FEJetAvePt -> Write();
  h_FEJet1Pt -> Write();
  h_FEJet2Pt -> Write();
  h_FEJet3Pt -> Write();
  h_JetAvePt -> Write();
  h_Jet1Pt -> Write();
  h_Jet2Pt -> Write();
  h_Jet3Pt -> Write();
  fpt->Close();


  TFile *fprim = new TFile("PU_incl_full.root","RECREATE"); ;
  fprim->cd();
  h_PU -> Write();
  fprim -> Close();

  TFile *f = new TFile("histograms_data_incl_full.root","RECREATE");
  TFile *f1 = new TFile("histograms_data_incl_full_control.root","RECREATE");

  for( int m = 0; m < EtaBinsNo; m++ ){
    for( int p = 0; p < PtBinsNo; p++ ){
      for( int r = 0; r < AlphaBinsNo; r++ ){
        f->cd();
        asymmetries_all.at(m).at(p).at(r) -> Write();
        asymmetries_pt_all.at(m).at(p).at(r) -> Write();
        f1->cd();
        asymmetries_rho_all.at(m).at(p).at(r) -> Write();
        asymmetries_pt3_all.at(m).at(p).at(r) -> Write();
      }
    }
  }

  for( int m = 0; m < EtaForwardBinsNo; m++ ){
    for( int p = 0; p < PtFTBinsNo; p++ ){
      for( int r = 0; r < AlphaBinsNo; r++ ){
        f->cd();
        asymmetries_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        asymmetries_pt_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        f1->cd();
        asymmetries_rho_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        asymmetries_pt3_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
      }
    }
  }

  for( int m = 0; m < EtaFtBinsNo; m++ ){
    for( int p = 0; p < PtFTBinsNo; p++ ){
      for( int r = 0; r < AlphaBinsNo; r++ ){
        f->cd();
        forward_hist.at(m).at(p).at(r) -> Write();
        forward_pt_hist.at(m).at(p).at(r) -> Write();
        f1->cd();
        forward_rho_hist.at(m).at(p).at(r) -> Write();
        forward_pt3_hist.at(m).at(p).at(r) -> Write();
      }
    }
  }

  for( int m = 0; m < EtaFtControlBinsNo; m++ ){
    for( int p = 0; p < PtBinsNo; p++ ){
      for( int r = 0; r < AlphaBinsNo; r++ ){
        f->cd();
        forward_hist_dijet.at(m).at(p).at(r) -> Write();
        forward_pt_hist_dijet.at(m).at(p).at(r) -> Write();
        f1->cd();
        forward_rho_hist_dijet.at(m).at(p).at(r) -> Write();
        forward_pt3_hist_dijet.at(m).at(p).at(r) -> Write();
      }
    }
  }

  f -> Close();
  f1-> Close();


  TFile *f_alpha = new TFile("alpha_spectrum.root","RECREATE"); ;

  for( int m = 0; m < asymmetries_all.size() ; m++ ){
    for( int p = 0; p < asymmetries_all.at(m).size(); p++ ){
      alpha_spectrum.at(m).at(p) -> Write();
    }
  }

  for( int m = 0; m < forward_alpha_spectrum.size(); m++ ){
    for( int p = 0; p < forward_alpha_spectrum.at(m).size(); p++ ){
      forward_alpha_spectrum.at(m).at(p) -> Write();
    }
  }

  for( int m = 0; m < forward_alpha_spectrum_dijet.size(); m++ ){
    for( int p = 0; p < forward_alpha_spectrum_dijet.at(m).size(); p++ ){
      forward_alpha_spectrum_dijet.at(m).at(p) -> Write();
    }
  }

  f_alpha -> Close();

}

void MySelector::Terminate(){
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

}


double Weight( std::string filename ){
  if ( filename.std::string::find("Pt_5to10") != std::string::npos )      return 6.102e+10/3422691;
  if ( filename.std::string::find("Pt_10to15") != std::string::npos )     return 5.888e+09/3490055;
  if ( filename.std::string::find("Pt_15to30") != std::string::npos )     return 1.837e+09/4936496;
  if ( filename.std::string::find("Pt_30to50") != std::string::npos )     return 1.409e+08/4899251;
  if ( filename.std::string::find("Pt_50to80") != std::string::npos )     return 1.92e+07/4966990;
  if ( filename.std::string::find("Pt_80to120") != std::string::npos )    return 2.763e+06/3465021;
  if ( filename.std::string::find("Pt_120to170") != std::string::npos )   return 4.711e+05/3446207;
  if ( filename.std::string::find("Pt_170to300") != std::string::npos )   return 1.173e+05/3438066;
  if ( filename.std::string::find("Pt_300to470") != std::string::npos )   return 7823./2930578;
  if ( filename.std::string::find("Pt_470to600") != std::string::npos )   return 648.2/1939229;
  if ( filename.std::string::find("Pt_600to800") != std::string::npos )   return 186.9/1890256;
  if ( filename.std::string::find("Pt_800to1000") != std::string::npos )  return 32.29/1911296;
  if ( filename.std::string::find("Pt_1000to1400") != std::string::npos ) return 9.418/1461216;
  if ( filename.std::string::find("Pt_1400to1800") != std::string::npos ) return 0.8426/197959;
  if ( filename.std::string::find("Pt_1800to2400") != std::string::npos ) return 0.1149/194924;
  if ( filename.std::string::find("Pt_2400to3200") != std::string::npos ) return 0.00683/198383;
  if ( filename.std::string::find("Pt_3200toInf") != std::string::npos )  return 0.0001654/188696;
  else {
    std::cout << "fail" << std::endl;
    return 0.0;
  }
}
