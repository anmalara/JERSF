#include "UHH2/JERSF/include/TestHists.h"
#include "UHH2/JERSF/include/constants.hpp"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Jet.h"

// #include "UHH2/BaconTrans/baconheaders/TJet.hh"
// #include "UHH2/BaconTrans/baconheaders/TEventInfo.hh"
// #include "UHH2/BaconTrans/baconheaders/BaconAnaDefs.hh"
// #include "UHH2/BaconTrans/baconheaders/TVertex.hh"

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <getopt.h>
using namespace std;
using namespace uhh2;
//using namespace baconhep;
TestHists::TestHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  TH1::SetDefaultSumw2();

  book<TH1F>("N_jets", "N_{jets}", 50, -0.5, 49.5);
  book<TH1F>("pt_hat", "p_{T} hat", 150, 0, 6000);
  book<TH1F>("pt","p_{T} all jets; p_{T} (GeV)",100,0,1500);
  book<TH1F>("eta","#eta all jets; #eta",100,-5,5);
  double eta_range[n_eta_bins];
  for(int i=0; i<n_eta_bins; i++) eta_range[i] = eta_bins[i];
  book<TH1F>("eta_binned","|#eta| all jets; |#eta|",n_eta_bins-1, eta_range);
  book<TH1F>("phi","#phi all jets; #phi",50,-M_PI,M_PI);
  book<TH1F>("MET","MET all jets; MET",400,0,400);

  book<TH1F>("nPu","Number of PU events",60,0,60);
  book<TH1F>("N_PV","Number of PVtx",60,0,60);
  book<TH1F>("weight_histo","weight_histo ",20,0,2);

  book<TH1F>("pt_1","p_{T} jet 1",100,0,600);
  book<TH1F>("eta_1","#eta jet 1",100,-5,5);

  book<TH1F>("pt_2","p_{T} jet 2",100,0,600);
  book<TH1F>("eta_2","#eta jet 2",100,-5,5);

  book<TH1F>("pt_3","p_{T} jet 3",100,0,600);
  book<TH1F>("eta_3","#eta jet 3",100,-5,5);

  book<TH1F>("ptRaw_barrel","p^{Raw}_{T} barrel jet; p_{T}^{Raw,barrel} (GeV)",100,0,600);
  book<TH1F>("ptRaw_probe","p^{Raw}_{T} probe jet; p_{T}^{Raw,probe} (GeV)",100,0,600);
  book<TH1F>("pt_barrel","p_{T} barrel jet; p_{T}^{barrel} (GeV)",100,0,600);
  book<TH1F>("pt_probe","p_{T} probe jet; p_{T}^{probe} (GeV)",100,0,600);
  book<TH1F>("eta_barrel","#eta barrel jet; #eta^{barrel}",100,-5,5);
  book<TH1F>("eta_probe","#eta probe jet; #eta^{probe}",100,-5,5);
  book<TH1F>("eta_probe_pos","#eta probe jet >=0; #eta^{probe}",50,0,5);
  book<TH1F>("eta_probe_neg","#eta probe jet <0; |#eta^{probe}|",50,0,5);
  book<TH1F>("pt_ave","p_{T} ave jet; p_{T}^{ave} (GeV)",600,0,600);
  book<TH1F>("pt_ave_pthat","p_{T} ave jet; p_{T}^{ave} - p_{T}^{hat})/p_{T}^{hat}(GeV)",100,0,600);
  book<TH1F>("pt_ave_rebin","p_{T} ave jet; p_{T}^{ave} (GeV)",300,0,3000);

  book<TH1F>("pt_rel","p_{T}^{jet3} / p_{T}^{ave}; #alpha ", 50, 0, 1);

  book<TH1F>("asym","asymmetry jet 1 and jet 2; Asymmetry",150,-1.5,1.5);
  book<TH1F>("mpf","MPF response; MPF response",250,0.,2.5);
  book<TH1F>("r_rel","R_{rel}; R_{rel}; Relative response",250,0.,2.5);


  book<TH2D>("mpf_vs_etaProbe","MPF response vs. #eta probe jet; #eta probe; MPF response",100,-5,5,100,0.,2.);
  book<TH2D>("r_rel_vs_etaProbe","Relative response vs. #eta probe jet; #eta probe; R_{rel}",100,-5,5,100,0.,2.);
  book<TH2D>("pt_ave_vs_etaProbe","pt ave vs #eta probe jet; #eta probe; pT_{ave}, GeV",100,-5.2,5.2,200,0,1000);
  book<TH2D>("dPhi_vs_alpha","#Delta #Phi vs #alpha; #alpha; #Delta #Phi",150,0,3,200,-M_PI,4);



}

void TestHists::fill(const uhh2::Event & ev){
  fill(ev, 0);
}
void TestHists::fill(const uhh2::Event & ev, const int rand){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  // Don't forget to always use the weight when filling.


  double weight = ev.weight;
  const int njets = ev.jets->size();
  hist("N_jets")->Fill(njets, weight);

  for (int i=0; i<njets; i++){
    Jet* jets = &ev.jets->at(i);
    hist("pt")->Fill(jets->pt(), weight);
    hist("eta")->Fill(jets->eta(), weight);
    hist("eta_binned")->Fill(jets->eta(), weight);
    hist("phi")->Fill(jets->phi(), weight);

    hist("MET")->Fill(ev.met->pt(), weight);
    hist("weight_histo")->Fill(weight, 1);
  }



  if (njets>0) {
    Jet* jet1 = &ev.jets->at(0);
    hist("pt_1")->Fill(jet1->pt(), weight);
    hist("eta_1")->Fill(jet1->eta(), weight);
  }
  if (njets>1) {
    Jet* jet2 = &ev.jets->at(1);
    hist("pt_2")->Fill(jet2->pt(), weight);
    hist("eta_2")->Fill(jet2->eta(), weight);
  }
  if (njets > 2){
    Jet* jet3 = &ev.jets->at(2);
    hist("pt_3")->Fill(jet3->pt(), weight);
    hist("eta_3")->Fill(jet3->eta(), weight);

  }
}

TestHists::~TestHists(){}
