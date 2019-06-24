//#include <iostream>

#include "TClonesArray.h"

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/FlavorParticle.h"
#include "UHH2/core/include/Jet.h"

#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
//#include "UHH2/BaconTrans/baconheaders/TEventInfo.hh"

namespace uhh2jersf {

  class Selection {

  private:
    uhh2::Context& context;
    uhh2::Event* event;

    /* uhh2::Event::Handle<TClonesArray> h_jets; */
    /* uhh2::Event::Handle<baconhep::TEventInfo> h_eventInfo; */
    /* uhh2::Event::Handle<TClonesArray> h_pv; */

    //Additional vars in Event, specific for dijet
    uhh2::Event::Handle<float> tt_gen_pthat; uhh2::Event::Handle<float> tt_gen_weight;
    uhh2::Event::Handle<float> tt_jet1_pt;     uhh2::Event::Handle<float> tt_jet2_pt;     uhh2::Event::Handle<float> tt_jet3_pt;
    uhh2::Event::Handle<float> tt_jet1_ptRaw;  uhh2::Event::Handle<float> tt_jet2_ptRaw;  uhh2::Event::Handle<float> tt_jet3_ptRaw;
    uhh2::Event::Handle<int> tt_nvertices;
    uhh2::Event::Handle<float> tt_probejet_eta;  uhh2::Event::Handle<float> tt_probejet_phi; uhh2::Event::Handle<float> tt_probejet_pt; uhh2::Event::Handle<float> tt_probejet_ptRaw;
    uhh2::Event::Handle<float> tt_barreljet_eta;  uhh2::Event::Handle<float> tt_barreljet_phi; uhh2::Event::Handle<float> tt_barreljet_pt; uhh2::Event::Handle<float> tt_barreljet_ptRaw;
    uhh2::Event::Handle<float> tt_pt_ave;
    uhh2::Event::Handle<float> tt_alpha;
    uhh2::Event::Handle<float> tt_rel_r; uhh2::Event::Handle<float> tt_mpf_r; uhh2::Event::Handle<float> tt_asymmetry; uhh2::Event::Handle<int> tt_nPU;

    uhh2::GenericEvent::Handle<std::vector<L1Jet>> handle_l1jet_seeds;

    TString Cut_Dir;
    TString dataset_version;

    TFile* cut_map;
    TH2D* h_map;

    bool diJetTrg;
    bool central;
    bool fwd;

    bool no_genp;

    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger40;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger60;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger80;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger140;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger200;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger260;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger320;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger400;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger450;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger500;

    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger60_HF;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger80_HF;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger100_HF;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger160_HF;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger220_HF;
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_trigger300_HF;

  public:
    Selection(uhh2::Context & ctx);
    ~Selection();

    void SetEvent(uhh2::Event& evt);
    bool DiJet();
    bool DiJetAdvanced(uhh2::Event& evt);

    bool PUpthat(uhh2::Event& evt);
    bool PtaveVsQScale(double cutValue=2.);

    bool EtaPhiCleaning(uhh2::Event& evt);
    bool ApplyHotMap(uhh2::Event& evt);
    bool EnergyEtaCut();// cut away events with jets containing energy more than sqrt(s)/2

    //  bool FullSelection();
    // bool PtMC(uhh2::Event& evt); //apply lowest Pt cut on MC
    // bool triggerFired(float bin1, float bin2);
    // bool Trigger(uhh2::Event& evt);
    // int goodPVertex();
    // bool L1JetBXclean(Jet& jet, bool usePtRatioFilter = false);
    // bool L1JetBXcleanFull();
    // bool L1JetBXcleanSmart();
    // bool EtaPhi(uhh2::Event& evt);

    //jetmatching, find the jet in the event that corresponds to the jetid-th jet in the trigger object of the trigger with threshold trigger_th
    //returns -1 if the triggerobject does not contain less than jetid jets
    //returns -2 if no jet is matched within dR
    // int FindMatchingJet(unsigned int jetid, unsigned int trigger_th, bool use_fwd = false);
  };

}
