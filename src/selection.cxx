#include "UHH2/JERSF/include/selection.h"

#include <iostream>
#include "UHH2/core/include/Jet.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/PrimaryVertex.h"
// #include "UHH2/BaconTrans/baconheaders/TJet.hh"
//#include "UHH2/BaconTrans/baconheaders/TVertex.hh"
#include "UHH2/JERSF/include/constants.hpp"

#include "TVector2.h"
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

using namespace std;
namespace uhh2jersf {

  Selection::Selection(uhh2::Context & ctx) : context(ctx), event(0) {
    tt_gen_pthat = ctx.declare_event_output<float>("gen_pthat");
    tt_gen_weight = ctx.declare_event_output<float>("gen_weight");
    tt_jet1_pt = ctx.declare_event_output<float>("jet1_pt");
    tt_jet2_pt = ctx.declare_event_output<float>("jet2_pt");
    tt_jet3_pt = ctx.declare_event_output<float>("jet3_pt");
    tt_jet1_ptRaw = ctx.declare_event_output<float>("jet1_ptRaw");
    tt_jet2_ptRaw = ctx.declare_event_output<float>("jet2_ptRaw");
    tt_jet3_ptRaw = ctx.declare_event_output<float>("jet3_ptRaw");
    tt_nvertices = ctx.declare_event_output<int>("nvertices");
    tt_probejet_eta = ctx.declare_event_output<float>("probejet_eta");
    tt_probejet_phi = ctx.declare_event_output<float>("probejet_phi");
    tt_probejet_pt = ctx.declare_event_output<float>("probejet_pt");
    tt_probejet_ptRaw = ctx.declare_event_output<float>("probejet_ptRaw");
    tt_barreljet_eta = ctx.declare_event_output<float>("barreljet_eta");
    tt_barreljet_phi = ctx.declare_event_output<float>("barreljet_phi");
    tt_barreljet_pt = ctx.declare_event_output<float>("barreljet_pt");
    tt_barreljet_ptRaw = ctx.declare_event_output<float>("barreljet_ptRaw");
    tt_pt_ave = ctx.declare_event_output<float>("pt_ave");
    tt_alpha = ctx.declare_event_output<float>("alpha");
    tt_rel_r = ctx.declare_event_output<float>("rel_r");
    tt_mpf_r = ctx.declare_event_output<float>("mpf_r");
    tt_asymmetry = ctx.declare_event_output<float>("asymmetry");
    tt_nPU = ctx.declare_event_output<int>("nPU");
    // handle_l1jet_seeds = ctx.declare_event_input< vector< L1Jet>>("L1Jet_seeds");

    dataset_version = ctx.get("dataset_version");

    if (dataset_version.Contains("RunA")) Cut_Dir = ctx.get("Cut_dir") + "hotjets-18runA.root";
    else if (dataset_version.Contains("RunB")) Cut_Dir = ctx.get("Cut_dir") + "hotjets-18runB.root";
    else if (dataset_version.Contains("RunC")) Cut_Dir = ctx.get("Cut_dir") + "hotjets-18runC-ver3.root";
    else if (dataset_version.Contains("RunD")) Cut_Dir = ctx.get("Cut_dir") + "hotjets-18runD.root";
    else Cut_Dir = ctx.get("Cut_dir") + "hotjets-18MC.root";


    cut_map = new TFile(Cut_Dir,"READ");
    h_map = (TH2D*) cut_map->Get("h2hotfilter");
    h_map->SetDirectory(0);
    cut_map->Close();
  }

  void Selection::SetEvent(uhh2::Event& evt) {
    event = &evt;
    assert(event);
  }


  bool Selection::DiJet() {
    assert(event);
    const int njets = event->jets->size();
    if (njets>=2) return true;
    return false;
  }

  bool Selection::DiJetAdvanced(uhh2::Event& evt) {
    assert(event);

    const int njets = evt.jets->size();
    if (njets < 2) return false;
    Jet* jet1 = &event->jets->at(0);// leading jet
    Jet* jet2 = &event->jets->at(1);// sub-leading jet

    double deltaPhi = std::abs(TVector2::Phi_mpi_pi(jet1->phi() - jet2->phi()));
    if (deltaPhi < s_delta_phi) return false;

    // // |asymm| < 0.7
    // if (fabs((event->get(tt_jet2_pt) - event->get(tt_jet1_pt)) / (event->get(tt_jet2_pt) + event->get(tt_jet1_pt))) > s_asymm) return false;

    //(pTgen1 < 1.5*pThat || pTreco1 < 1.5* pTgen1)
    if(!event->isRealData){
      if(event->genjets->size() < 1) return false;
      // if (evt.genInfo->binningValues().size()<1) {
      //   if (! (event->jets->at(0).pt() < 2.5*event->genjets->at(0).pt())) return false;
      //   else return true;
      // }
      //if(!(event->genjets->at(0).pt() < 1.5*event->genInfo->qScale() || event->jets->at(0).pt() < 1.5*event->genjets->at(0).pt())) return false;
    }
    return true;
  }

 
  bool Selection::PUpthat(uhh2::Event& evt) {
    assert(event);

    // if (evt.genInfo->binningValues().size()<1) return true;

    double  gen_pthat = event->genInfo->qScale();
    double  PU_pt_hat = event->genInfo->PU_pT_hat_max();
    double Ratio = PU_pt_hat/gen_pthat;

    if(Ratio < 1) return true;

    return false;
  }

  bool Selection::PtaveVsQScale(double cutValue)
  {
    assert(event);
    
    // if(no_genp) return true;
    
    double  pt_hat = event->genInfo->qScale();
    //    double ptave = event->get(tt_pt_ave);
    //    double ptave = event->jets->at(0)->pt()+event->jets->at(1)->pt();
    double ptave = 0.5*(event->jets->at(0).pt()+event->jets->at(1).pt());
    double Ratio = ptave/pt_hat;
    
    if(Ratio < cutValue) return true;
    return false;
  }

  bool Selection::EtaPhiCleaning(uhh2::Event& evt) {
    assert(event);
    if (dataset_version.Contains("RunC") || dataset_version.Contains("RunD")) {

      // None of the first 3 jets should be in that region.
      for (size_t i = 0; i < 3; i++) {
        if (evt.jets->size()<i+1) continue;
        if (-2.964 < evt.jets->at(i).eta() && evt.jets->at(i).eta() < -1.305 && -1.6 < evt.jets->at(i).phi() && evt.jets->at(i).phi() < -0.7) return false;
      }
    }
    return true;

  }


  bool Selection::ApplyHotMap(uhh2::Event& evt) {
    assert(event);
    for(auto jet: *evt.jets) {
      if(h_map->GetBinContent(h_map->GetXaxis()->FindBin(jet.eta()), h_map->GetYaxis()->FindBin(jet.phi())) > 0) return false;
    }
    return true;
  }

  bool Selection::EnergyEtaCut()
  {
    // cut away events with jets containing energy more than sqrt(s)/2
    assert(event);
    //    double probejet_eta = event->get(tt_probejet_eta);
    //    double ptave = event->get(tt_pt_ave);
    double probejet_eta = fabs(event->jets->at(0).eta());
    if(fabs(event->jets->at(1).eta())>probejet_eta) 
      probejet_eta = fabs(event->jets->at(1).eta());
    double ptave = 0.5*(event->jets->at(0).pt()+event->jets->at(1).pt());
    //    cout<<"probejet_eta = "<<probejet_eta<<endl;
    if(ptave*cosh(probejet_eta)>3250) return false; //3250 GeV =sqrt(s)/2 with s=13 TeV    
    return true;
  }

  Selection::~Selection() { }

}



