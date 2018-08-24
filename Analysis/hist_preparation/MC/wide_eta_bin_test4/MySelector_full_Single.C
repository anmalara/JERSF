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

#include <iostream>
#include <algorithm>
#include <TH2.h>
#include <TStyle.h>
#include <TLorentzVector.h>
#include <TRandom3.h>
#include "MySelector.h"
#include "MyJet.h"
#include "../../../../include/constants.hpp"

bool sortFunct( MyJet a, MyJet b) { return ( a.Pt() > b.Pt() ); }

void MySelector::BuildEvent(){
}

void MySelector::MakeWeight(){

  std::string filename;

  filename = std::string(fChain->GetDirectory()->GetPath());

  // event weight
  if (! Weight(filename) ){
    weight = weight;
  } else {
    weight *= Weight(filename);
  }

  if(weight <= 0 || weight > 1e11){
    std::cout << "WARNING: weight was very small/large " << weight << std::endl;
    std::cout << "Filename is " << filename << std::endl;
    std::cout << "leading jet eta is " << probejet_eta << std::endl;
    weight = 0;
  }
}


void MySelector::Begin(TTree * /*tree*/){
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

  TotalEvents = 0;
  unmatchegGenJets = 0;
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
  histograms.push_back(h_FEJet1Pt);

  h_FEJet2Pt = new TH1F( "FEJet2Pt" , "Inclusive FEJet 2 Pt" , 50 , 0 , 2000 );
  h_FEJet2Pt -> SetXTitle( "Pt_2[GeV]" );
  h_FEJet2Pt -> Sumw2();
  histograms.push_back(h_FEJet2Pt);

  h_FEJet3Pt = new TH1F( "FEJet3Pt" , "Inclusive FEJet 3 Pt" , 50 , 0 , 2000 );
  h_FEJet3Pt -> SetXTitle( "Pt_3[GeV]" );
  h_FEJet3Pt -> Sumw2();
  // histograms.push_back(h_FEJet3Pt);

  h_PU = new TH1F( "PileUp" , "PU distribution" , 60 , 0 , 60 );
  h_PU -> SetXTitle( "PU" );
  h_PU -> Sumw2();
  // histograms.push_back(h_PU);


  h_rho = new TH1F( "Rho" , "Rho distribution" , 60 , 0 , 30 );
  h_rho -> SetXTitle( "Rho" );
  h_rho -> Sumw2();
  histograms.push_back(h_rho);


  h_rhoFWD = new TH1F( "RhoFWD" , "RhoFWD distribution" , 60 , 0 , 30 );
  h_rhoFWD -> SetXTitle( "RhoFWD" );
  h_rhoFWD -> Sumw2();
  histograms.push_back(h_rhoFWD);





  EtaBinsNo = 10;            // st method bins
  EtaForwardBinsNo = 3;     // st method bins with fw triggers ?
  EtaFtControlBinsNo = 8;   // fw method bins with normal triggers
  EtaFtBinsNo = 3;          // fw method bins with fw triggers

  PtBinsNo = n_pt_bins_Si;
  PtFTBinsNo = n_pt_bins_Si_HF;
  AlphaBinsNo = 6;

  // I define histograms for the "normal" JER calculation

  for( int m = 0; m < EtaBinsNo; m++ ){
    std::vector< std::vector< TH1F* > > temp2, temp2pt, temp2rho, temp2pt3, gen_temp2rho, gen_temp2pt3, gen_temp2, gen_temp2pt, temp2_MCTruth;
    std::vector< std::vector< TH2F* > > temp2_dR, gen_temp2_dR;
    std::vector< TH1F* > alpha_temp2;
    for( int p = 0; p < PtBinsNo; p++ ){
      std::vector< TH1F* > temp1, temp1pt, temp1rho, temp1pt3, gen_temp1rho, gen_temp1pt3, gen_temp1, gen_temp1pt, temp1_MCTruth;
      std::vector< TH2F* > temp1_dR, gen_temp1_dR;;
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
        TString gen_name_fw = "gen_asymm_"; gen_name_fw += "eta"; gen_name_fw += m+1; gen_name_fw += "_pt"; gen_name_fw += p+1; gen_name_fw += "_alpha"; gen_name_fw += r+1;
        TString gen_name_fw_pt = "gen_asymmpt_"; gen_name_fw_pt += "probe"; gen_name_fw_pt += m+1; gen_name_fw_pt += "_pt"; gen_name_fw_pt += p+1; gen_name_fw_pt += "_alpha"; gen_name_fw_pt += r+1;
        TString gen_name_fw_rho = "gen_asymmrho_"; gen_name_fw_rho += "probe"; gen_name_fw_rho += m+1; gen_name_fw_rho += "_pt"; gen_name_fw_rho += p+1; gen_name_fw_rho += "_alpha"; gen_name_fw_rho += r+1;
        TString gen_name_fw_pt3 = "gen_asymmpt3_"; gen_name_fw_pt3 += "probe"; gen_name_fw_pt3 += m+1; gen_name_fw_pt3 += "_pt"; gen_name_fw_pt3 += p+1; gen_name_fw_pt3 += "_alpha"; gen_name_fw_pt3 += r+1;
        TString name_MCTruth = "mctruth_"; name_MCTruth += "eta"; name_MCTruth += (m+1); name_MCTruth += "_pt"; name_MCTruth += (p+1); name_MCTruth += "_alpha"; name_MCTruth += (r+1);
        TString name_dR = "dR_"; name_dR += "eta"; name_dR += m+1; name_dR += "_pt"; name_dR += p+1; name_dR += "_alpha"; name_dR += r+1;
        TString gen_name_dR = "gen_dR_"; gen_name_dR += "eta"; gen_name_dR += m+1; gen_name_dR += "_pt"; gen_name_dR += p+1; gen_name_dR += "_alpha"; gen_name_dR += r+1;
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
        TH1F *gen_h1 = new TH1F( gen_name_fw, gen_name_fw, 160, -0.8, 0.8 );
        gen_h1 ->GetYaxis()->SetTitle("a.u.");    gen_h1 ->GetXaxis()->SetTitle("Asymmetry");
        gen_h1 -> Sumw2(); gen_temp1.push_back(gen_h1);
        TH1F *gen_h2 = new TH1F( gen_name_fw_pt, gen_name_fw_pt, 50, 0, 1500 );
        gen_h2 ->GetYaxis()->SetTitle("a.u.");    gen_h2 ->GetXaxis()->SetTitle("Pt[GeV]");
        gen_h2 -> Sumw2(); gen_temp1pt.push_back(gen_h2);
        TH1F *gen_h3 = new TH1F( gen_name_fw_rho, gen_name_fw_rho, 100, 0, 100 );
        gen_h3 ->GetYaxis()->SetTitle("a.u.");    gen_h3 ->GetXaxis()->SetTitle("rho");
        gen_h3 -> Sumw2(); gen_temp1rho.push_back(gen_h3);
        TH1F *gen_h4 = new TH1F( gen_name_fw_pt3, gen_name_fw_pt3, 50, 0, 1500 );
        gen_h4 ->GetYaxis()->SetTitle("a.u.");    gen_h4 ->GetXaxis()->SetTitle("Pt[GeV]");
        gen_h4 -> Sumw2(); gen_temp1pt3.push_back(gen_h4);
        TH1F *h1_MCTruth = new TH1F( name_MCTruth, name_MCTruth, 200, 0, 2.0 );
        h1_MCTruth ->GetYaxis()->SetTitle("a.u.");    h1_MCTruth ->GetXaxis()->SetTitle("Response");
        h1_MCTruth -> Sumw2(); temp1_MCTruth.push_back(h1_MCTruth);
        TH2F *h2_dR = new TH2F( name_dR, name_dR, 70, 0, 7.0, 70, 0, 7.0 );
        h2_dR ->GetXaxis()->SetTitle("#Delta R (jet_{barrel}, jet_{3})");    h2_dR ->GetYaxis()->SetTitle("#Delta R (jet_{probe}, jet_{3})");
        h2_dR -> Sumw2(); temp1_dR.push_back(h2_dR);
        TH2F *gen_h2_dR = new TH2F( gen_name_dR, gen_name_dR, 70, 0, 7.0, 70, 0, 7.0 );
        gen_h2_dR ->GetXaxis()->SetTitle("#Delta R (jet_{barrel}, jet_{3})");    gen_h2_dR ->GetYaxis()->SetTitle("#Delta R (jet_{probe}, jet_{3})");
        gen_h2_dR -> Sumw2(); gen_temp1_dR.push_back(gen_h2_dR);
      }
      temp2.push_back(temp1); temp2pt.push_back(temp1pt); temp2rho.push_back(temp1rho);  temp2pt3.push_back(temp1pt3);
      gen_temp2.push_back(gen_temp1); gen_temp2pt.push_back(gen_temp1pt); gen_temp2rho.push_back(gen_temp1rho); gen_temp2pt3.push_back(gen_temp1pt3);
      temp2_MCTruth.push_back(temp1_MCTruth); temp2_dR.push_back(temp1_dR); gen_temp2_dR.push_back(gen_temp1_dR);
    }
    asymmetries_all.push_back(temp2); asymmetries_pt_all.push_back(temp2pt); asymmetries_rho_all.push_back(temp2rho); asymmetries_pt3_all.push_back(temp2pt3);
    asymmetries_gen_all.push_back(gen_temp2); asymmetries_gen_pt_all.push_back(gen_temp2pt); asymmetries_gen_rho_all.push_back(gen_temp2rho); asymmetries_gen_pt3_all.push_back(gen_temp2pt3);
    MC_Truth_asymmetries_all.push_back(temp2_MCTruth); dR_all.push_back(temp2_dR); gen_dR_all.push_back(gen_temp2_dR);
    alpha_spectrum.push_back(alpha_temp2);
  }

  for( int m = 0; m < EtaForwardBinsNo; m++ ){
    std::vector< std::vector< TH1F* > > temp2, temp2pt, temp2rho, temp2pt3, gen_temp2rho, gen_temp2pt3, gen_temp2, gen_temp2pt, temp2_MCTruth;
    std::vector< std::vector< TH2F* > > temp2_dR, gen_temp2_dR;
    std::vector< TH1F* > alpha_temp2;
    for( int p = 0; p < PtFTBinsNo; p++ ){
      std::vector< TH1F* > temp1, temp1pt, temp1rho, temp1pt3, gen_temp1rho, gen_temp1pt3, gen_temp1, gen_temp1pt, temp1_MCTruth;
      std::vector< TH2F* > temp1_dR, gen_temp1_dR;
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
        TString gen_name_fw = "gen_asymm_"; gen_name_fw += "eta"; gen_name_fw += ( m + 1 + EtaBinsNo ); gen_name_fw += "_pt"; gen_name_fw += (p+1); gen_name_fw += "_alpha"; gen_name_fw += (r+1);
        TString gen_name_fw_pt = "gen_asymmpt_"; gen_name_fw_pt += "probe"; gen_name_fw_pt += ( m + 1 + EtaBinsNo ); gen_name_fw_pt += "_pt"; gen_name_fw_pt += (p+1); gen_name_fw_pt += "_alpha"; gen_name_fw_pt += (r+1);
        TString gen_name_fw_rho = "gen_asymmrho_"; gen_name_fw_rho += "probe"; gen_name_fw_rho += m+1+EtaBinsNo; gen_name_fw_rho += "_pt"; gen_name_fw_rho += p+1; gen_name_fw_rho += "_alpha"; gen_name_fw_rho += r+1;
        TString gen_name_fw_pt3 = "gen_asymmpt3_"; gen_name_fw_pt3 += "probe"; gen_name_fw_pt3 += m+1+EtaBinsNo; gen_name_fw_pt3 += "_pt"; gen_name_fw_pt3 += p+1; gen_name_fw_pt3 += "_alpha"; gen_name_fw_pt3 += r+1;
        TString name_MCTruth = "mctruth_"; name_MCTruth += "eta"; name_MCTruth += ( m + 1 + EtaBinsNo ); name_MCTruth += "_pt"; name_MCTruth += (p+1); name_MCTruth += "_alpha"; name_MCTruth += (r+1);
        TString name_dR = "dR_"; name_dR += "eta"; name_dR += ( m + 1 + EtaBinsNo ); name_dR += "_pt"; name_dR += (p+1); name_dR += "_alpha"; name_dR += (r+1);
        TString gen_name_dR = "gen_dR_"; gen_name_dR += "eta"; gen_name_dR += ( m + 1 + EtaBinsNo ); gen_name_dR += "_pt"; gen_name_dR += (p+1); gen_name_dR += "_alpha"; gen_name_dR += (r+1);
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
        TH1F *gen_h1 = new TH1F( gen_name_fw, gen_name_fw, 160, -0.8, 0.8 );
        gen_h1 ->GetYaxis()->SetTitle("a.u.");    gen_h1 ->GetXaxis()->SetTitle("Asymmetry");
        gen_h1 -> Sumw2(); gen_temp1.push_back(gen_h1);
        TH1F *gen_h2 = new TH1F( gen_name_fw_pt, gen_name_fw_pt, 50, 0, 1500 );
        gen_h2 ->GetYaxis()->SetTitle("a.u.");    gen_h2 ->GetXaxis()->SetTitle("Pt[GeV]");
        gen_h2 -> Sumw2(); gen_temp1pt.push_back(gen_h2);
        TH1F *gen_h3 = new TH1F( gen_name_fw_rho, gen_name_fw_rho, 100, 0, 100 );
        gen_h3 ->GetYaxis()->SetTitle("a.u.");    gen_h3 ->GetXaxis()->SetTitle("rho");
        gen_h3 -> Sumw2(); gen_temp1rho.push_back(gen_h3);
        TH1F *gen_h4 = new TH1F( gen_name_fw_pt3, gen_name_fw_pt3, 50, 0, 1500 );
        gen_h4 ->GetYaxis()->SetTitle("a.u.");    gen_h4 ->GetXaxis()->SetTitle("Pt[GeV]");
        gen_h4 -> Sumw2(); gen_temp1pt3.push_back(gen_h4);
        TH1F *h1_MCTruth = new TH1F( name_MCTruth, name_MCTruth, 200, 0, 2.0 );
        h1_MCTruth ->GetYaxis()->SetTitle("a.u.");    h1_MCTruth ->GetXaxis()->SetTitle("Response");
        h1_MCTruth -> Sumw2(); temp1_MCTruth.push_back(h1_MCTruth);
        TH2F *h2_dR = new TH2F( name_dR, name_dR, 70, 0, 7.0, 70, 0, 7.0 );
        h2_dR ->GetXaxis()->SetTitle("#Delta R (jet_{barrel}, jet_{3})");    h2_dR ->GetYaxis()->SetTitle("#Delta R (jet_{probe}, jet_{3})");
        h2_dR -> Sumw2(); temp1_dR.push_back(h2_dR);
        TH2F *gen_h2_dR = new TH2F( gen_name_dR, gen_name_dR, 70, 0, 7.0, 70, 0, 7.0 );
        gen_h2_dR ->GetXaxis()->SetTitle("#Delta R (jet_{barrel}, jet_{3})");    gen_h2_dR ->GetYaxis()->SetTitle("#Delta R (jet_{probe}, jet_{3})");
        gen_h2_dR -> Sumw2(); gen_temp1_dR.push_back(gen_h2_dR);
      }
      temp2.push_back(temp1); temp2pt.push_back(temp1pt); temp2rho.push_back(temp1rho);  temp2pt3.push_back(temp1pt3);
      gen_temp2.push_back(gen_temp1); gen_temp2pt.push_back(gen_temp1pt); gen_temp2rho.push_back(gen_temp1rho); gen_temp2pt3.push_back(gen_temp1pt3);
      temp2_MCTruth.push_back(temp1_MCTruth); temp2_dR.push_back(temp1_dR); gen_temp2_dR.push_back(gen_temp1_dR);
    }
    asymmetries_all.push_back(temp2); asymmetries_pt_all.push_back(temp2pt); asymmetries_rho_all.push_back(temp2rho); asymmetries_pt3_all.push_back(temp2pt3);
    asymmetries_gen_all.push_back(gen_temp2); asymmetries_gen_pt_all.push_back(gen_temp2pt); asymmetries_gen_rho_all.push_back(gen_temp2rho); asymmetries_gen_pt3_all.push_back(gen_temp2pt3);
    MC_Truth_asymmetries_all.push_back(temp2_MCTruth); dR_all.push_back(temp2_dR); gen_dR_all.push_back(gen_temp2_dR);
    alpha_spectrum.push_back(alpha_temp2);
  }

  // I define histograms used in forward extension for reference eta ( 0, s_eta_barr )

  for( int m = 0; m < EtaFtBinsNo; m++ ){
    std::vector< std::vector< TH1F* > > temp2, temp2pt, temp2rho, temp2pt3, gen_temp2rho, gen_temp2pt3, gen_temp2, gen_temp2pt, temp2_MCTruth;
    std::vector< std::vector< TH2F* > > temp2_dR, gen_temp2_dR;
    std::vector< TH1F* > alpha_temp2;
    for( int p = 0; p < PtFTBinsNo; p++ ){
      std::vector< TH1F* > temp1, temp1pt, temp1rho, temp1pt3, gen_temp1rho, gen_temp1pt3, gen_temp1, gen_temp1pt, temp1_MCTruth;
      std::vector< TH2F* > temp1_dR, gen_temp1_dR;
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
        TString gen_name_fw = "gen_forward_"; gen_name_fw += "probe"; gen_name_fw += m+2+EtaFtControlBinsNo; gen_name_fw += "_pt"; gen_name_fw += p+1; gen_name_fw += "_alpha"; gen_name_fw += r+1;
        TString gen_name_fw_pt = "gen_forwardpt_"; gen_name_fw_pt += "probe"; gen_name_fw_pt += m+2+EtaFtControlBinsNo; gen_name_fw_pt += "_pt"; gen_name_fw_pt += p+1; gen_name_fw_pt += "_alpha"; gen_name_fw_pt += r+1;
        TString gen_name_fw_rho = "gen_forwardrho_"; gen_name_fw_rho += "probe"; gen_name_fw_rho += m+2+EtaFtControlBinsNo; gen_name_fw_rho += "_pt"; gen_name_fw_rho += p+1; gen_name_fw_rho += "_alpha"; gen_name_fw_rho += r+1;
        TString gen_name_fw_pt3 = "gen_forwardpt3_"; gen_name_fw_pt3 += "probe"; gen_name_fw_pt3 += m+2+EtaFtControlBinsNo; gen_name_fw_pt3 += "_pt"; gen_name_fw_pt3 += p+1; gen_name_fw_pt3 += "_alpha"; gen_name_fw_pt3 += r+1;
        TString name_MCTruth = "mctruth_forward_"; name_MCTruth += "probe"; name_MCTruth += m+2+EtaFtControlBinsNo ; name_MCTruth += "_pt"; name_MCTruth += p+1; name_MCTruth += "_alpha"; name_MCTruth += r+1;
        TString name_dR = "dR_forward_"; name_dR += "probe"; name_dR += m+2+EtaFtControlBinsNo ; name_dR += "_pt"; name_dR += p+1; name_dR += "_alpha"; name_dR += r+1;
        TString gen_name_dR = "gen_dR_forward_"; gen_name_dR += "probe"; gen_name_dR += m+2+EtaFtControlBinsNo ; gen_name_dR += "_pt"; gen_name_dR += p+1; gen_name_dR += "_alpha"; gen_name_dR += r+1;
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
        TH1F *gen_h1 = new TH1F( gen_name_fw, gen_name_fw, 160, -0.8, 0.8 );
        gen_h1 ->GetYaxis()->SetTitle("a.u.");    gen_h1 ->GetXaxis()->SetTitle("Asymmetry");
        gen_h1 -> Sumw2(); gen_temp1.push_back(gen_h1);
        TH1F *gen_h2 = new TH1F( gen_name_fw_pt, gen_name_fw_pt, 50, 0, 1500 );
        gen_h2 ->GetYaxis()->SetTitle("a.u.");    gen_h2 ->GetXaxis()->SetTitle("Pt[GeV]");
        gen_h2 -> Sumw2(); gen_temp1pt.push_back(gen_h2);
        TH1F *gen_h3 = new TH1F( gen_name_fw_rho, gen_name_fw_rho, 100, 0, 100 );
        gen_h3 ->GetYaxis()->SetTitle("a.u.");    gen_h3 ->GetXaxis()->SetTitle("rho");
        gen_h3 -> Sumw2(); gen_temp1rho.push_back(gen_h3);
        TH1F *gen_h4 = new TH1F( gen_name_fw_pt3, gen_name_fw_pt3, 50, 0, 1500 );
        gen_h4 ->GetYaxis()->SetTitle("a.u.");    gen_h4 ->GetXaxis()->SetTitle("Pt[GeV]");
        gen_h4 -> Sumw2(); gen_temp1pt3.push_back(gen_h4);
        TH1F *h1_MCTruth = new TH1F( name_MCTruth, name_MCTruth, 200, 0, 2.0 );
        h1_MCTruth ->GetYaxis()->SetTitle("a.u.");    h1_MCTruth ->GetXaxis()->SetTitle("Response");
        h1_MCTruth -> Sumw2(); temp1_MCTruth.push_back(h1_MCTruth);
        TH2F *h2_dR = new TH2F( name_dR, name_dR, 70, 0, 7.0, 70, 0, 7.0 );
        h2_dR ->GetXaxis()->SetTitle("#Delta R (jet_{barrel}, jet_{3})");    h2_dR ->GetYaxis()->SetTitle("#Delta R (jet_{probe}, jet_{3})");
        h2_dR -> Sumw2(); temp1_dR.push_back(h2_dR);
        TH2F *gen_h2_dR = new TH2F( gen_name_dR, gen_name_dR, 70, 0, 7.0, 70, 0, 7.0 );
        gen_h2_dR ->GetXaxis()->SetTitle("#Delta R (jet_{barrel}, jet_{3})");    gen_h2_dR ->GetYaxis()->SetTitle("#Delta R (jet_{probe}, jet_{3})");
        gen_h2_dR -> Sumw2(); gen_temp1_dR.push_back(gen_h2_dR);
      }
      temp2.push_back(temp1); temp2pt.push_back(temp1pt); temp2rho.push_back(temp1rho);  temp2pt3.push_back(temp1pt3);
      gen_temp2.push_back(gen_temp1); gen_temp2pt.push_back(gen_temp1pt); gen_temp2rho.push_back(gen_temp1rho); gen_temp2pt3.push_back(gen_temp1pt3);
      temp2_MCTruth.push_back(temp1_MCTruth); temp2_dR.push_back(temp1_dR); gen_temp2_dR.push_back(gen_temp1_dR);
    }
    forward_hist.push_back(temp2); forward_pt_hist.push_back(temp2pt); forward_rho_hist.push_back(temp2rho); forward_pt3_hist.push_back(temp2pt3);
    forward_gen_hist.push_back(gen_temp2); forward_gen_pt_hist.push_back(gen_temp2pt); forward_gen_rho_hist.push_back(gen_temp2rho); forward_gen_pt3_hist.push_back(gen_temp2pt3);
    MC_Truth_forward_hist.push_back(temp2_MCTruth); dR_forward_hist.push_back(temp2_dR); gen_dR_forward_hist.push_back(gen_temp2_dR);
    forward_alpha_spectrum.push_back(alpha_temp2);
  }

  // I define histograms for the control eta bins for forward method calculation

  for( int m = 0; m < EtaFtControlBinsNo; m++ ){
    std::vector< std::vector< TH1F* > > temp2, temp2pt, temp2rho, temp2pt3, gen_temp2rho, gen_temp2pt3, gen_temp2, gen_temp2pt, temp2_MCTruth;
    std::vector< std::vector< TH2F* > > temp2_dR, gen_temp2_dR;
    std::vector< TH1F* > alpha_temp2;
    for( int p = 0; p < PtBinsNo; p++ ){
      std::vector< TH1F* > temp1, temp1pt, temp1rho, temp1pt3, gen_temp1rho, gen_temp1pt3, gen_temp1, gen_temp1pt, temp1_MCTruth;
      std::vector< TH2F* > temp1_dR, gen_temp1_dR;
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
        TString gen_name_fw = "gen_forward_control_"; gen_name_fw += "probe"; gen_name_fw += m+2; gen_name_fw += "_pt"; gen_name_fw += p+1; gen_name_fw += "_alpha"; gen_name_fw += r+1;
        TString gen_name_fw_pt = "gen_forwardpt_control_"; gen_name_fw_pt += "probe"; gen_name_fw_pt += m+2; gen_name_fw_pt += "_pt"; gen_name_fw_pt += p+1; gen_name_fw_pt += "_alpha"; gen_name_fw_pt += r+1;
        TString gen_name_fw_rho = "gen_forwardrho_control_"; gen_name_fw_rho += "probe"; gen_name_fw_rho += m+2; gen_name_fw_rho += "_pt"; gen_name_fw_rho += p+1; gen_name_fw_rho += "_alpha"; gen_name_fw_rho += r+1;
        TString gen_name_fw_pt3 = "gen_forwardpt3_control_"; gen_name_fw_pt3 += "probe"; gen_name_fw_pt3 += m+2; gen_name_fw_pt3 += "_pt"; gen_name_fw_pt3 += p+1; gen_name_fw_pt3 += "_alpha"; gen_name_fw_pt3 += r+1;
        TString name_MCTruth = "mctruth_forward_control_"; name_MCTruth += "probe"; name_MCTruth += m+2; name_MCTruth += "_pt"; name_MCTruth += p+1; name_MCTruth += "_alpha"; name_MCTruth += r+1;
        TString name_dR = "dR_forward_control_"; name_dR += "probe"; name_dR += m+2; name_dR += "_pt"; name_dR += p+1; name_dR += "_alpha"; name_dR += r+1;
        TString gen_name_dR = "gen_dR_forward_control_"; gen_name_dR += "probe"; gen_name_dR += m+2; gen_name_dR += "_pt"; gen_name_dR += p+1; gen_name_dR += "_alpha"; gen_name_dR += r+1;
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
        TH1F *gen_h1 = new TH1F( gen_name_fw, gen_name_fw, 160, -0.8, 0.8 );
        gen_h1 ->GetYaxis()->SetTitle("a.u.");    gen_h1 ->GetXaxis()->SetTitle("Asymmetry");
        gen_h1 -> Sumw2(); gen_temp1.push_back(gen_h1);
        TH1F *gen_h2 = new TH1F( gen_name_fw_pt, gen_name_fw_pt, 50, 0, 1500 );
        gen_h2 ->GetYaxis()->SetTitle("a.u.");    gen_h2 ->GetXaxis()->SetTitle("Pt[GeV]");
        gen_h2 -> Sumw2(); gen_temp1pt.push_back(gen_h2);
        TH1F *gen_h3 = new TH1F( gen_name_fw_rho, gen_name_fw_rho, 100, 0, 100 );
        gen_h3 ->GetYaxis()->SetTitle("a.u.");    gen_h3 ->GetXaxis()->SetTitle("rho");
        gen_h3 -> Sumw2(); gen_temp1rho.push_back(gen_h3);
        TH1F *gen_h4 = new TH1F( gen_name_fw_pt3, gen_name_fw_pt3, 50, 0, 1500 );
        gen_h4 ->GetYaxis()->SetTitle("a.u.");    gen_h4 ->GetXaxis()->SetTitle("Pt[GeV]");
        gen_h4 -> Sumw2(); gen_temp1pt3.push_back(gen_h4);
        TH1F *h1_MCTruth = new TH1F( name_MCTruth, name_MCTruth, 200, 0, 2.0 );
        h1_MCTruth ->GetYaxis()->SetTitle("a.u.");    h1_MCTruth ->GetXaxis()->SetTitle("Response");
        h1_MCTruth -> Sumw2(); temp1_MCTruth.push_back(h1_MCTruth);
        TH2F *h2_dR = new TH2F( name_dR, name_dR, 70, 0, 7.0, 70, 0, 7.0 );
        h2_dR ->GetXaxis()->SetTitle("#Delta R (jet_{barrel}, jet_{3})");    h2_dR ->GetYaxis()->SetTitle("#Delta R (jet_{probe}, jet_{3})");
        h2_dR -> Sumw2(); temp1_dR.push_back(h2_dR);
        TH2F *gen_h2_dR = new TH2F( gen_name_dR, gen_name_dR, 70, 0, 7.0, 70, 0, 7.0 );
        gen_h2_dR ->GetXaxis()->SetTitle("#Delta R (jet_{barrel}, jet_{3})");    gen_h2_dR ->GetYaxis()->SetTitle("#Delta R (jet_{probe}, jet_{3})");
        gen_h2_dR -> Sumw2(); gen_temp1_dR.push_back(gen_h2_dR);
      }
      temp2.push_back(temp1); temp2pt.push_back(temp1pt); temp2rho.push_back(temp1rho);  temp2pt3.push_back(temp1pt3);
      gen_temp2.push_back(gen_temp1); gen_temp2pt.push_back(gen_temp1pt); gen_temp2rho.push_back(gen_temp1rho); gen_temp2pt3.push_back(gen_temp1pt3);
      temp2_MCTruth.push_back(temp1_MCTruth); temp2_dR.push_back(temp1_dR); gen_temp2_dR.push_back(gen_temp1_dR);
    }
    forward_hist_dijet.push_back(temp2); forward_pt_hist_dijet.push_back(temp2pt); forward_rho_hist_dijet.push_back(temp2rho); forward_pt3_hist_dijet.push_back(temp2pt3);
    forward_gen_hist_dijet.push_back(gen_temp2); forward_pt_gen_hist_dijet.push_back(gen_temp2pt); forward_gen_rho_hist_dijet.push_back(gen_temp2rho); forward_gen_pt3_hist_dijet.push_back(gen_temp2pt3);
    MC_Truth_forward_hist_dijet.push_back(temp2_MCTruth); dR_forward_hist_dijet.push_back(temp2_dR); gen_dR_forward_hist_dijet.push_back(gen_temp2_dR);
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

  MakeWeight();
  //2017
  std::vector<int> p_bins(pt_bins_Si, pt_bins_Si + sizeof(pt_bins_Si)/sizeof(double));
  std::vector<int> p_bins_FT(pt_bins_Si_HF, pt_bins_Si_HF + sizeof(pt_bins_Si_HF)/sizeof(double));
  std::vector<double> eta_bins_all(eta_bins, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_ref_down(eta_bins+10, eta_bins + sizeof(eta_bins)/sizeof(double)-1);
  std::vector<double> eta_ref_up(eta_bins+11, eta_bins + sizeof(eta_bins)/sizeof(double));
  std::vector<double> eta_bins_control(eta_bins+3, eta_bins + sizeof(eta_bins)/sizeof(double)-3);
  eta_bins_control.insert(eta_bins_control.begin(), 0.);
  p_bins.push_back(1500);
  p_bins_FT.push_back(1500);
  double alpha_bins [] = { 0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3 };

  if( TotalEvents%100000 == 0 ){  std::cout << "            Analyzing event #" << TotalEvents << std::endl; }
  if( TotalEvents == 1 ){ std::cout << "asymmetry length " << asymmetries_all.size() << std::endl;  }

  double gen_threshold = 10;
  double jet_threshold = 15;

  double alpha_raw;

  if ( jet3_pt > jet_threshold ){
    alpha_raw =  2 * jet3_pt/( jet1_pt + jet2_pt );
  }

  h_PU -> Fill( npuIT, 1 ); //weight );

  double parallel, perpendicular, complete, alpha;
  double parallelGen, perpendicularGen, completeGen, alphaGen;

  // Below I choose what kind of asymmetries I want to study!

  //    bool excl_bin = true;  // exclusive
  bool excl_bin = false; // inclusive

  int flag1 = 0; // 0 -> complete_alpha
  // 1 -> parallel
  // 2 -> perpendicular

  if ( jet2_pt > jet_threshold &&  (njet > 1) ){

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

    double DPhi1, DPhi2, DEta1, DEta2, Resp1, Resp2;
    double ProbeBarrelGen, ProbeProbeGen, BarrelBarrelGen, BarrelProbeGen;

    ProbeProbeGen   = TMath::Abs( TVector2::Phi_mpi_pi(probejet_phi - probegenjet_phi) );
    ProbeBarrelGen  = TMath::Abs( TVector2::Phi_mpi_pi(probejet_phi - barrelgenjet_phi) );
    BarrelBarrelGen = TMath::Abs( TVector2::Phi_mpi_pi(barreljet_phi - barrelgenjet_phi) );
    BarrelProbeGen  = TMath::Abs( TVector2::Phi_mpi_pi(barreljet_phi - probegenjet_phi) );

    if( ( ProbeProbeGen < ProbeBarrelGen ) &&
    ( BarrelBarrelGen < BarrelProbeGen ) ){
      DPhi1 = TMath::Abs(TVector2::Phi_mpi_pi( probejet_phi - probegenjet_phi));
      DPhi2 = TMath::Abs(TVector2::Phi_mpi_pi( barreljet_phi - barrelgenjet_phi));
      DEta1 = TMath::Abs( probejet_eta - probegenjet_eta);
      DEta2 = TMath::Abs( barreljet_eta - barrelgenjet_eta);
      Resp1 = probejet_pt / probegenjet_pt;
      Resp2 = barreljet_pt / barrelgenjet_pt;
    }else if( ( ProbeBarrelGen < ProbeProbeGen ) &&
    ( BarrelProbeGen < BarrelBarrelGen ) ){
      DPhi1 = TMath::Abs(TVector2::Phi_mpi_pi( probejet_phi - barrelgenjet_phi));
      DPhi2 = TMath::Abs(TVector2::Phi_mpi_pi( barreljet_phi - probegenjet_phi));
      DEta1 = TMath::Abs( probejet_eta - barrelgenjet_eta);
      DEta2 = TMath::Abs( barreljet_eta - probegenjet_eta);
      Resp1 = probejet_pt / barrelgenjet_pt;
      Resp2 = barreljet_pt / probegenjet_pt;
    }

    double DR1, DR2;
    DR1 = TMath::Sqrt( TMath::Power( DPhi1, 2 ) + TMath::Power( DEta1, 2) );
    DR2 = TMath::Sqrt( TMath::Power( DPhi2, 2 ) + TMath::Power( DEta2, 2) );

    if ( TMath::Abs(TVector2::Phi_mpi_pi((probejet_phi - barreljet_phi)))  > 2.7 ){ // I fill alpha_max histograms
      for ( int k = 0 ; k < PtBinsNo ; k++ ){

        if ((0.5*(jet1_pt+jet2_pt) > p_bins[k]) &&
        (0.5*(jet1_pt+jet2_pt) < p_bins[k+1]) ){
          for ( int r = 0 ; r < EtaBinsNo ; r++ ){
            if (TMath::Abs( probejet_eta ) > eta_bins_all[r] &&
            TMath::Abs( probejet_eta ) < eta_bins_all[r+1] &&
            TMath::Abs( barreljet_eta) > eta_bins_all[r] &&
            TMath::Abs( barreljet_eta) < eta_bins_all[r+1] ){
              for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                if ( alpha < alpha_bins[ m+1 ] ){
                  double asy = asymmetry;
                  double Delta_R_radiation_barrel = TMath::Sqrt(TMath::Power( barreljet_eta - jet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( barreljet_phi - jet3_phi),2));
                  double Delta_R_radiation_probe = TMath::Sqrt(TMath::Power( probejet_eta - jet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( probejet_phi - jet3_phi),2));
                  if (((rand()%2)+1)==1 ) {
                    asy = - asy;
                    std::swap(Delta_R_radiation_barrel, Delta_R_radiation_probe);
                  }
                  asymmetries_all.at(r).at(k).at(m) -> Fill( asy , weight );
                  asymmetries_pt_all.at(r).at(k).at(m) -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                  asymmetries_rho_all.at(r).at(k).at(m) -> Fill( rho, weight );
                  asymmetries_pt3_all.at(r).at(k).at(m) -> Fill( jet3_pt, weight );
                  dR_all.at(r).at(k).at(m) -> Fill( Delta_R_radiation_barrel, Delta_R_radiation_probe, weight );
                  if(DR1 < s_delta_R) MC_Truth_asymmetries_all.at(r).at(k).at(m) -> Fill( Resp1, weight ); //?
                  if(DR2 < s_delta_R) MC_Truth_asymmetries_all.at(r).at(k).at(m) -> Fill( Resp2, weight ); //?
                  if ( m == AlphaBinsNo-1 ){
                    h_JetAvePt -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                    h_Jet1Pt -> Fill( jet1_pt, weight );
                    h_Jet2Pt -> Fill( jet2_pt, weight );
                    h_Jet3Pt -> Fill( jet3_pt, weight );
                    h_rho -> Fill( rho, weight );
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
                  double Delta_R_radiation_barrel = TMath::Sqrt(TMath::Power( barreljet_eta - jet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( barreljet_phi - jet3_phi),2));
                  double Delta_R_radiation_probe = TMath::Sqrt(TMath::Power( probejet_eta - jet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( probejet_phi - jet3_phi),2));
                  if ((TMath::Abs( probejet_eta ) > 0. && TMath::Abs( probejet_eta ) < s_eta_barr && TMath::Abs( barreljet_eta ) > eta_bins_control[r] && TMath::Abs( barreljet_eta ) < eta_bins_control[r+1])) {
                    asy = - asymmetry;
                    std::swap(Delta_R_radiation_barrel, Delta_R_radiation_probe);
                  }
                  if (Delta_R_radiation_barrel < Delta_R_radiation_probe) continue;
                  if ( (((rand()%2)+1)==1) && r ==0 ) { asy = - asy;}
                  forward_hist_dijet.at(r).at(k).at(m) -> Fill( asy , weight );
                  forward_pt_hist_dijet.at(r).at(k).at(m) -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                  forward_rho_hist_dijet.at(r).at(k).at(m) -> Fill( rho, weight );
                  forward_pt3_hist_dijet.at(r).at(k).at(m) -> Fill( jet3_pt, weight );
                  dR_forward_hist_dijet.at(r).at(k).at(m) -> Fill( Delta_R_radiation_barrel, Delta_R_radiation_probe, weight );
                  if( (DR1< s_delta_R) && (probejet_phi >s_eta_barr) ) MC_Truth_forward_hist_dijet.at(r).at(k).at(m) -> Fill( Resp1, weight ); //?
                  if( (DR2< s_delta_R) && (barreljet_phi>s_eta_barr) ) MC_Truth_forward_hist_dijet.at(r).at(k).at(m) -> Fill( Resp2, weight ); //?
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
      for ( int k = 0 ; k < PtFTBinsNo ; k++ ){
        if ((0.5*(jet1_pt+jet2_pt) > p_bins_FT[k]) &&
        (0.5*(jet1_pt+jet2_pt) < p_bins_FT[k+1])){
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
                  double Delta_R_radiation_barrel = TMath::Sqrt(TMath::Power( barreljet_eta - jet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( barreljet_phi - jet3_phi),2));
                  double Delta_R_radiation_probe = TMath::Sqrt(TMath::Power( probejet_eta - jet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( probejet_phi - jet3_phi),2));
                  if ((TMath::Abs( probejet_eta ) > 0. && TMath::Abs( probejet_eta ) < s_eta_barr && TMath::Abs( barreljet_eta) > eta_ref_down[r] && TMath::Abs( barreljet_eta) < eta_ref_up[r])) {
                    asy = - asymmetry;
                    std::swap(Delta_R_radiation_barrel, Delta_R_radiation_probe);
                  }
                  if (Delta_R_radiation_barrel < Delta_R_radiation_probe) continue;
                  forward_hist.at(r).at(k).at(m) -> Fill( asy , weight );
                  forward_pt_hist.at(r).at(k).at(m) -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                  forward_rho_hist.at(r).at(k).at(m) -> Fill( rho, weight );
                  forward_pt3_hist.at(r).at(k).at(m) -> Fill( jet3_pt, weight );
                  dR_forward_hist.at(r).at(k).at(m) -> Fill( Delta_R_radiation_barrel, Delta_R_radiation_probe, weight );
                  if( (DR1< s_delta_R) && (probejet_phi >s_eta_barr) ) MC_Truth_forward_hist.at(r).at(k).at(m) -> Fill( Resp1, weight );//?
                  if( (DR2< s_delta_R) && (barreljet_phi>s_eta_barr) ) MC_Truth_forward_hist.at(r).at(k).at(m) -> Fill( Resp2, weight );//?
                  if ( m == AlphaBinsNo-1 ){
                    h_FEJetAvePt -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                    h_FEJet1Pt -> Fill( jet1_pt, weight );
                    h_FEJet2Pt -> Fill( jet2_pt, weight );
                    h_FEJet3Pt -> Fill( jet3_pt, weight );
                    h_rhoFWD -> Fill( rho, weight );
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
              for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                if ( alpha < alpha_bins[ m+1 ] ){
                  double asy = asymmetry;
                  double Delta_R_radiation_barrel = TMath::Sqrt(TMath::Power( barreljet_eta - jet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( barreljet_phi - jet3_phi),2));
                  double Delta_R_radiation_probe = TMath::Sqrt(TMath::Power( probejet_eta - jet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( probejet_phi - jet3_phi),2));
                  if (((rand()%2)+1)==1 ) {
                    asy = - asy;
                    std::swap(Delta_R_radiation_barrel, Delta_R_radiation_probe);
                  }
                  asymmetries_all.at(r+EtaBinsNo).at(k).at(m) -> Fill( asy , weight );
                  asymmetries_pt_all.at(r+EtaBinsNo).at(k).at(m) -> Fill( 0.5 * ( jet1_pt + jet2_pt ), weight );
                  asymmetries_rho_all.at(r).at(k).at(m) -> Fill( rho, weight );
                  asymmetries_pt3_all.at(r).at(k).at(m) -> Fill( jet3_pt, weight );
                  dR_all.at(r).at(k).at(m) -> Fill( Delta_R_radiation_barrel, Delta_R_radiation_probe, weight );
                  if(DR1< s_delta_R) MC_Truth_asymmetries_all.at(r).at(k).at(m) -> Fill( Resp1, weight );//?
                  if(DR2< s_delta_R) MC_Truth_asymmetries_all.at(r).at(k).at(m) -> Fill( Resp2, weight );//?
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

  //    same part for GenJets

  double gen_alpha_raw;
  if ( genjet3_pt > gen_threshold ){
    //gen_alpha_raw =  2 * genjet3_pt/( genjet1_pt + genjet2_pt );
    gen_alpha_raw = gen_alpha;
  }
  if ( genjet2_pt > gen_threshold){
    if ( ngenjet > 2 && genjet3_pt > gen_threshold ){//?
      completeGen =  gen_alpha_raw; //2 * jet3_pt/( jet1_pt + jet2_pt );
      parallelGen = gen_alpha_raw; //(2*Jets[2]*(Jets[0]-Jets[1]))/((Jets[0]-Jets[1]).Pt()*( jet1_pt + jet2_pt ));
      perpendicularGen = gen_alpha_raw; //TMath::Sqrt( TMath::Power( complete, 2 ) - TMath::Power( parallel, 2) );
    } else {
      completeGen = 0 ;
      parallelGen = 0 ;
      perpendicularGen = 0 ;
    }

    if (flag1 == 0 ) alphaGen = TMath::Abs(completeGen);
    if (flag1 == 1 ) alphaGen = TMath::Abs(parallelGen);
    if (flag1 == 2 ) alphaGen = TMath::Abs(perpendicularGen);
    //std::cout << "your genalpha is: " << alphaGen << " while alpha is " << alpha << std::endl;
    //std::cout << "your genasym is: " << gen_asymmetry << " while asym is " << asymmetry << std::endl;
    double jet1_phi, jet2_phi, genjet1_phi, genjet2_phi;
    double jet1_eta, jet2_eta, genjet1_eta, genjet2_eta;

    // select the 1st jet as the one with largest pt
    if( (jet1_pt == probejet_pt) && (jet2_pt == barreljet_pt) ){
      jet1_phi = probejet_phi;
      jet2_phi = barreljet_phi;
      jet1_eta = probejet_eta;
      jet2_eta = barreljet_eta;
    } else if( (jet2_pt == probejet_pt) && (jet1_pt == barreljet_pt) ){
      jet1_phi = barreljet_phi;
      jet2_phi = probejet_phi;
      jet1_eta = barreljet_eta;
      jet2_eta = probejet_eta;
    } else{
      std::cout << "did not get the jets properly..." << std::endl;
    }

    // select the 1st jet as the one with largest pt
    if( (genjet1_pt == probegenjet_pt) && (genjet2_pt == barrelgenjet_pt) ){
      genjet1_phi = probegenjet_phi;
      genjet2_phi = barrelgenjet_phi;
      genjet1_eta = probegenjet_eta;
      genjet2_eta = barrelgenjet_eta;
    } else if( (genjet2_pt == probegenjet_pt) && (genjet1_pt == barrelgenjet_pt) ){
      genjet1_phi = barrelgenjet_phi;
      genjet2_phi = probegenjet_phi;
      genjet1_eta = barrelgenjet_eta;
      genjet2_eta = probegenjet_eta;
    } else{
      std::cout << "did not get the gen jets properly..." << std::endl;
    }

    if((TMath::Sqrt( TMath::Power( jet1_phi - genjet1_phi ,2) + TMath::Power( jet1_eta - genjet1_eta ,2) )< 0.4) &&
    (TMath::Sqrt( TMath::Power( jet2_phi - genjet2_phi ,2) + TMath::Power( jet2_eta - genjet2_eta ,2) )< 0.4)   ){
      gen_asymmetry = ( genjet1_pt - genjet2_pt )/( genjet1_pt + genjet2_pt );
    }else if((TMath::Sqrt( TMath::Power( jet1_phi - genjet2_phi ,2) + TMath::Power( jet1_eta - genjet2_eta ,2) )< 0.4) &&
    (TMath::Sqrt( TMath::Power( jet2_phi - genjet1_phi ,2) + TMath::Power( jet2_eta - genjet1_eta ,2) )< 0.4)   ){
      gen_asymmetry = ( genjet2_pt - genjet1_pt )/( genjet1_pt + genjet2_pt );
      std::swap(genjet1_eta, genjet2_eta);
      std::swap(genjet1_phi, genjet2_phi);
    }else{
      gen_asymmetry = 10.0;
      unmatchegGenJets++;
    }
    if ( TMath::Abs(TVector2::Phi_mpi_pi((genjet1_phi - genjet2_phi)))  > 2.7 ){ // I fill alpha_max histograms
      for ( int k = 0 ; k < PtBinsNo ; k++ ){
        if ((0.5*(genjet1_pt+genjet2_pt) > p_bins[k]) &&
        (0.5*(genjet1_pt+genjet2_pt) < p_bins[k+1])){
          for ( int r = 0 ; r < EtaBinsNo ; r++ ){
            if (TMath::Abs( genjet1_eta ) > eta_bins_all[r] &&
            TMath::Abs( genjet1_eta ) < eta_bins_all[r+1] &&
            TMath::Abs( genjet2_eta) > eta_bins_all[r] &&
            TMath::Abs( genjet2_eta) < eta_bins_all[r+1] ){
              for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                if ( alphaGen < alpha_bins[ m+1 ] ){
                  double gen_asy = gen_asymmetry;
                  double gen_Delta_R_radiation_barrel = TMath::Sqrt(TMath::Power( genjet1_eta - genjet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( genjet1_phi - genjet3_phi),2));
                  double gen_Delta_R_radiation_probe = TMath::Sqrt(TMath::Power( genjet2_eta - genjet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( genjet2_phi - genjet3_phi),2));
                  if (((rand()%2)+1)==1 ) {
                    gen_asy = - gen_asy;
                    std::swap(gen_Delta_R_radiation_barrel, gen_Delta_R_radiation_probe);
                  }
                  if(TMath::Abs(gen_asy) < 5){
                    asymmetries_gen_all.at(r).at(k).at(m) -> Fill( gen_asy , weight );
                    gen_dR_all.at(r).at(k).at(m) -> Fill( gen_Delta_R_radiation_barrel, gen_Delta_R_radiation_probe, weight );
                  }
                  asymmetries_gen_pt_all.at(r).at(k).at(m) -> Fill( 0.5 * ( genjet1_pt + genjet2_pt ), weight );
                  asymmetries_gen_pt3_all.at(r).at(k).at(m) -> Fill( genjet3_pt, weight );
                  if ( excl_bin ) break;
                }
              }
            }
          }
          for ( int r = 0 ; r < EtaFtControlBinsNo ; r++ ){
            if (( TMath::Abs( genjet1_eta ) > 0. &&
            TMath::Abs( genjet1_eta ) < s_eta_barr &&
            TMath::Abs( genjet2_eta ) > eta_bins_control[r] &&
            TMath::Abs( genjet2_eta ) < eta_bins_control[r+1]) ||
            (TMath::Abs( genjet2_eta ) > 0. &&
            TMath::Abs( genjet2_eta ) < s_eta_barr &&
            TMath::Abs( genjet1_eta ) > eta_bins_control[r] &&
            TMath::Abs( genjet1_eta ) < eta_bins_control[r+1]) ){
              for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                if ( alpha < alpha_bins[ m+1 ] ){
                  double gen_asy = gen_asymmetry;
                  double gen_Delta_R_radiation_barrel = TMath::Sqrt(TMath::Power( genjet1_eta - genjet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( genjet1_phi - genjet3_phi),2));
                  double gen_Delta_R_radiation_probe = TMath::Sqrt(TMath::Power( genjet2_eta - genjet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( genjet2_phi - genjet3_phi),2));
                  if ((TMath::Abs( genjet2_eta ) > 0. && TMath::Abs( genjet2_eta ) < s_eta_barr && TMath::Abs( genjet1_eta ) > eta_bins_control[r] && TMath::Abs( genjet1_eta ) < eta_bins_control[r+1])) {
                    gen_asy = - gen_asymmetry;
                    std::swap(gen_Delta_R_radiation_barrel, gen_Delta_R_radiation_probe);
                  }
                  if (gen_Delta_R_radiation_barrel < gen_Delta_R_radiation_probe) continue;
                  if(TMath::Abs(gen_asy) < 5){
                    forward_gen_hist_dijet.at(r).at(k).at(m) -> Fill( gen_asy , weight );
                    gen_dR_forward_hist_dijet.at(r).at(k).at(m) -> Fill( gen_Delta_R_radiation_barrel, gen_Delta_R_radiation_probe, weight );
                  }
                  forward_pt_gen_hist_dijet.at(r).at(k).at(m) -> Fill( 0.5 * ( genjet1_pt + genjet2_pt ), weight );
                  forward_pt3_hist_dijet.at(r).at(k).at(m) -> Fill( genjet3_pt, weight );
                  if ( excl_bin ) break;
                }
              }
            }
          }
          break;
        }
      }
      for ( int k = 0 ; k < PtFTBinsNo ; k++ ){
        if ((0.5*(genjet1_pt+genjet2_pt) > p_bins_FT[k]) &&
        (0.5*(genjet1_pt+genjet2_pt) < p_bins_FT[k+1])){
          for ( int r = 0 ; r < EtaFtBinsNo ; r++ ){
            if (( TMath::Abs( genjet1_eta ) > 0. &&
            TMath::Abs( genjet1_eta ) < s_eta_barr &&
            TMath::Abs( genjet2_eta ) > eta_ref_down[r] &&
            TMath::Abs( genjet2_eta ) < eta_ref_up[r]) ||
            (TMath::Abs( genjet2_eta ) > 0. &&
            TMath::Abs( genjet2_eta ) < s_eta_barr &&
            TMath::Abs( genjet1_eta ) > eta_ref_down[r] &&
            TMath::Abs( genjet1_eta ) < eta_ref_up[r]) ){
              for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                if ( alpha < alpha_bins[ m+1 ] ){
                  double gen_asy = gen_asymmetry;
                  double gen_Delta_R_radiation_barrel = TMath::Sqrt(TMath::Power( genjet1_eta - genjet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( genjet1_phi - genjet3_phi),2));
                  double gen_Delta_R_radiation_probe = TMath::Sqrt(TMath::Power( genjet2_eta - genjet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( genjet2_phi - genjet3_phi),2));
                  if ((TMath::Abs( genjet2_eta ) > 0. && TMath::Abs( genjet2_eta ) < s_eta_barr && TMath::Abs( genjet1_eta ) > eta_ref_down[r] && TMath::Abs( genjet1_eta ) < eta_ref_up[r])) {
                    gen_asy = - gen_asymmetry;
                    std::swap(gen_Delta_R_radiation_barrel, gen_Delta_R_radiation_probe);
                  }
                  if (gen_Delta_R_radiation_barrel < gen_Delta_R_radiation_probe) continue;
                  if(TMath::Abs(gen_asy) < 5){
                    forward_gen_hist.at(r).at(k).at(m) -> Fill( gen_asy , weight );
                    gen_dR_forward_hist.at(r).at(k).at(m) -> Fill( gen_Delta_R_radiation_barrel, gen_Delta_R_radiation_probe, weight );
                  }
                  forward_gen_pt_hist.at(r).at(k).at(m) -> Fill( 0.5 * ( genjet1_pt + genjet2_pt ), weight );
                  forward_gen_pt3_hist.at(r).at(k).at(m) -> Fill( genjet3_pt, weight );
                  if ( excl_bin ) break;
                }
              }
            }
          }
          for ( int r = 0 ; r < EtaForwardBinsNo ; r++ ){
            if (TMath::Abs( genjet1_eta ) > eta_bins_all[r+EtaBinsNo] &&
            TMath::Abs( genjet1_eta ) < eta_bins_all[r+1+EtaBinsNo] &&
            TMath::Abs( genjet2_eta ) > eta_bins_all[r+EtaBinsNo] &&
            TMath::Abs( genjet2_eta ) < eta_bins_all[r+1+EtaBinsNo] ){
              for ( int m = 0 ; m < AlphaBinsNo ; m++ ){
                if ( alphaGen < alpha_bins[ m+1 ] ){
                  double gen_asy = gen_asymmetry;
                  double gen_Delta_R_radiation_barrel = TMath::Sqrt(TMath::Power( genjet1_eta - genjet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( genjet1_phi - genjet3_phi),2));
                  double gen_Delta_R_radiation_probe = TMath::Sqrt(TMath::Power( genjet2_eta - genjet3_eta,2) + TMath::Power(TVector2::Phi_mpi_pi( genjet2_phi - genjet3_phi),2));
                  if (((rand()%2)+1)==1 ) {
                    gen_asy = - gen_asymmetry;
                    std::swap(gen_Delta_R_radiation_barrel, gen_Delta_R_radiation_probe);
                  }
                  if(TMath::Abs(gen_asy) < 5){
                    asymmetries_gen_all.at(r+EtaBinsNo).at(k).at(m) -> Fill( gen_asy , weight );
                    gen_dR_all.at(r).at(k).at(m) -> Fill( gen_Delta_R_radiation_barrel, gen_Delta_R_radiation_probe, weight );
                  }
                  asymmetries_gen_pt_all.at(r+EtaBinsNo).at(k).at(m) -> Fill( 0.5 * ( genjet1_pt + genjet2_pt ), weight );
                  asymmetries_gen_pt3_all.at(r).at(k).at(m) -> Fill( genjet3_pt, weight );
                  if ( excl_bin ) break;
                }
              }
            }
          }
          break;
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
  std::cout <<"            unmatchegGenJets events #" <<  unmatchegGenJets << std::endl;



  std::ofstream mytxtfile;
  mytxtfile.open ("counts.txt");
  mytxtfile << "Analyzed events #" <<  TotalEvents << "\n";
  mytxtfile << "unmatchegGenJets events #" <<  unmatchegGenJets << "\n";
  mytxtfile.close();

  TFile *fpt = new TFile("pt_mc_incl_full.root","RECREATE"); ;
  fpt->cd();
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

  TFile *f = new TFile("histograms_mc_incl_full.root","RECREATE");
  TFile *f1 = new TFile("histograms_mc_incl_full_control.root","RECREATE");

  for( int m = 0; m < EtaBinsNo; m++ ){
    for( int p = 0; p < PtBinsNo; p++ ){
      for( int r = 0; r < AlphaBinsNo; r++ ){
        f->cd();
        asymmetries_all.at(m).at(p).at(r) -> Write();
        asymmetries_pt_all.at(m).at(p).at(r) -> Write();
        asymmetries_gen_all.at(m).at(p).at(r) -> Write();
        asymmetries_gen_pt_all.at(m).at(p).at(r) -> Write();
        MC_Truth_asymmetries_all.at(m).at(p).at(r) -> Write();
        f1->cd();
        asymmetries_rho_all.at(m).at(p).at(r) -> Write();
        asymmetries_pt3_all.at(m).at(p).at(r) -> Write();
        asymmetries_gen_pt3_all.at(m).at(p).at(r) -> Write();
        dR_all.at(m).at(p).at(r) -> Write();
        gen_dR_all.at(m).at(p).at(r) -> Write();
      }
    }
  }

  for( int m = 0; m < EtaForwardBinsNo; m++ ){
    for( int p = 0; p < PtFTBinsNo; p++ ){
      for( int r = 0; r < AlphaBinsNo; r++ ){
        f->cd();
        asymmetries_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        asymmetries_pt_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        asymmetries_gen_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        asymmetries_gen_pt_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        MC_Truth_asymmetries_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        f1->cd();
        asymmetries_rho_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        asymmetries_pt3_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        asymmetries_gen_pt3_all.at(m+EtaBinsNo).at(p).at(r) -> Write();
        dR_all.at(m).at(p).at(r) -> Write();
        gen_dR_all.at(m).at(p).at(r) -> Write();
      }
    }
  }

  for( int m = 0; m < EtaFtBinsNo; m++ ){
    for( int p = 0; p < PtFTBinsNo; p++ ){
      for( int r = 0; r < AlphaBinsNo; r++ ){
        f->cd();
        forward_hist.at(m).at(p).at(r) -> Write();
        forward_pt_hist.at(m).at(p).at(r) -> Write();
        forward_gen_hist.at(m).at(p).at(r) -> Write();
        forward_gen_pt_hist.at(m).at(p).at(r) -> Write();
        MC_Truth_forward_hist.at(m).at(p).at(r) -> Write();
        f1->cd();
        forward_rho_hist.at(m).at(p).at(r) -> Write();
        forward_pt3_hist.at(m).at(p).at(r) -> Write();
        forward_gen_pt3_hist.at(m).at(p).at(r) -> Write();
        dR_forward_hist.at(m).at(p).at(r) -> Write();
        gen_dR_forward_hist.at(m).at(p).at(r) -> Write();
      }
    }
  }

  for( int m = 0; m < EtaFtControlBinsNo; m++ ){
    for( int p = 0; p < PtBinsNo; p++ ){
      for( int r = 0; r < AlphaBinsNo; r++ ){
        f->cd();
        forward_hist_dijet.at(m).at(p).at(r) -> Write();
        forward_pt_hist_dijet.at(m).at(p).at(r) -> Write();
        forward_gen_hist_dijet.at(m).at(p).at(r) -> Write();
        forward_pt_gen_hist_dijet.at(m).at(p).at(r) -> Write();
        MC_Truth_forward_hist_dijet.at(m).at(p).at(r) -> Write();
        f1->cd();
        forward_rho_hist_dijet.at(m).at(p).at(r) -> Write();
        forward_pt3_hist_dijet.at(m).at(p).at(r) -> Write();
        forward_gen_pt3_hist_dijet.at(m).at(p).at(r) -> Write();
        dR_forward_hist_dijet.at(m).at(p).at(r) -> Write();
        gen_dR_forward_hist_dijet.at(m).at(p).at(r) -> Write();
      }
    }
  }

  f->cd();
  h_rho -> Write();
  h_rhoFWD -> Write();
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
  if ( filename.std::string::find("QCDPt15to30")      != std::string::npos )  return 1837410000.0000  /19777211;
  if ( filename.std::string::find("QCDPt30to50")      != std::string::npos )  return 140932000.0000   /19693825;
  if ( filename.std::string::find("QCDPt50to80")      != std::string::npos )  return 19204300.0000    /19191105;
  if ( filename.std::string::find("QCDPt80to120")     != std::string::npos )  return 2762530.0000     /28780800;
  if ( filename.std::string::find("QCDPt120to170")    != std::string::npos )  return 471100.0000      /26840644;
  if ( filename.std::string::find("QCDPt170to300")    != std::string::npos )  return 117276.0000      /29725594;
  if ( filename.std::string::find("QCDPt300to470")    != std::string::npos )  return 7823.0000        /53285022;
  if ( filename.std::string::find("QCDPt470to600")    != std::string::npos )  return 648.2000         /26439242;
  if ( filename.std::string::find("QCDPt600to800")    != std::string::npos )  return 186.9000         /63545546;
  if ( filename.std::string::find("QCDPt800to1000")   != std::string::npos )  return 32.2930          /37632352;
  if ( filename.std::string::find("QCDPt1000to1400")  != std::string::npos )  return 9.4183           /19552294;
  if ( filename.std::string::find("QCDPt1400to1800")  != std::string::npos )  return 0.8427           /5669140;
  if ( filename.std::string::find("QCDPt1800to2400")  != std::string::npos )  return 0.1149           /2923941;
  if ( filename.std::string::find("QCDPt2400to3200")  != std::string::npos )  return 0.0068           /1910526;
  if ( filename.std::string::find("QCDPt3200toInf")   != std::string::npos )  return 0.0002           /770558;

  else
  {
    //std::cout << "failed to get pt_hat weight" << std::endl;
    std::cout << "failed to get QCD HT sample weight" << std::endl;
    std::cout << "This is for filename " << filename << std::endl;
    return 0;
    //return 1e+12;
  }
}
