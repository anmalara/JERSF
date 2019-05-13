#include <iostream>
#include <memory>
#include <stdlib.h>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/EventHelper.h"
#include "UHH2/core/include/Jet.h"
#include "UHH2/core/include/Utils.h"

#include "UHH2/common/include/CommonModules.h"
#include <UHH2/common/include/MCWeight.h>
#include <UHH2/common/include/JetCorrections.h>
#include <UHH2/common/include/LumiSelection.h> //includes also LuminosityHists.h
#include <UHH2/common/include/TriggerSelection.h>
#include "UHH2/common/include/CleaningModules.h"
#include <UHH2/common/include/NSelections.h>
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/ElectronIds.h>
#include "UHH2/common/include/PrintingModules.h"


#include "UHH2/JERSF/include/JECAnalysisHists.h"
#include "UHH2/JERSF/include/TestHists.h"
#include "UHH2/JERSF/include/JECCrossCheckHists.h"
#include "UHH2/JERSF/include/JECRunnumberHists.h"

#include "UHH2/JERSF/include/JERSFSelections.h"
#include "UHH2/JERSF/include/JERSFHists.h"
#include "UHH2/JERSF/include/selection.h"
#include "UHH2/JERSF/include/constants.hpp"
#include "TClonesArray.h"
#include "TString.h"
#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"


#include <UHH2/JERSF/include/ModuleBase.h>

using namespace std;
using namespace uhh2;

class JERSFModuleTest: public ModuleBASE {

public:
  explicit JERSFModuleTest(uhh2::Context&);
  virtual bool process(uhh2::Event&) override;
  void book_histograms(uhh2::Context&);
  void fill_histograms(uhh2::Event&, string);
  void book_handles(uhh2::Context&, std::vector<TString> extras);
  void export_weights(uhh2::Event&);

  ~JERSFModuleTest();

protected:

  // correctors
  std::unique_ptr<JetCorrector> jet_corrector, jet_corrector_A, jet_corrector_B, jet_corrector_C, jet_corrector_D;
  // std::unique_ptr<GenericJetResolutionSmearer> jetER_smearer;

  // cleaners
  std::unique_ptr<JetLeptonCleaner> jetleptoncleaner, JLC_A, JLC_B, JLC_C, JLC_D;
  std::unique_ptr<JetCleaner> jetcleaner;
  //std::unique_ptr<JetResolutionSmearer> jet_resolution_smearer;
  std::unique_ptr<GenericJetResolutionSmearer> jet_resolution_smearer;
  std::unique_ptr<MuonCleaner>     muoSR_cleaner;
  std::unique_ptr<ElectronCleaner> eleSR_cleaner;

  // selections
  std::unique_ptr<uhh2::Selection> lumi_selection;
  std::unique_ptr<uhh2::AnalysisModule> PVCleaner;
  std::unique_ptr<uhh2::AndSelection> metfilters_sel;
  std::unique_ptr<uhh2::Selection> trigger40_sel;
  std::unique_ptr<uhh2::Selection> trigger60_sel;
  std::unique_ptr<uhh2::Selection> trigger80_sel;
  std::unique_ptr<uhh2::Selection> trigger140_sel;
  std::unique_ptr<uhh2::Selection> trigger200_sel;
  std::unique_ptr<uhh2::Selection> trigger260_sel;
  std::unique_ptr<uhh2::Selection> trigger320_sel;
  std::unique_ptr<uhh2::Selection> trigger400_sel;
  std::unique_ptr<uhh2::Selection> trigger450_sel;
  std::unique_ptr<uhh2::Selection> trigger500_sel;
  std::unique_ptr<uhh2::Selection> trigger60_HFJEC_sel;
  std::unique_ptr<uhh2::Selection> trigger80_HFJEC_sel;
  std::unique_ptr<uhh2::Selection> trigger100_HFJEC_sel;
  std::unique_ptr<uhh2::Selection> trigger160_HFJEC_sel;
  std::unique_ptr<uhh2::Selection> trigger220_HFJEC_sel;
  std::unique_ptr<uhh2::Selection> trigger300_HFJEC_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part0_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part1_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part2_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part3_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part4_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part5_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part6_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part7_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part8_sel;
  std::unique_ptr<uhh2::Selection> triggerMB_part9_sel;

  //// Data/MC scale factors
  std::unique_ptr<uhh2::AnalysisModule> pileupSF, LumiSF;
  unique_ptr<AnalysisModule>  Jet_printer, GenParticles_printer;


  Event::Handle<int> tt_hf_trigger; Event::Handle<int> tt_bl_trigger;
  Event::Handle<float> tt_gen_pthat; Event::Handle<float> tt_gen_weight;
  Event::Handle<float> tt_PU_pt_hat;//number of jets
  Event::Handle<float> tt_jet1_pt;     Event::Handle<float> tt_jet2_pt;     Event::Handle<float> tt_jet3_pt;
  Event::Handle<float> tt_jet1_eta;     Event::Handle<float> tt_jet2_eta;     Event::Handle<float> tt_jet3_eta;
  Event::Handle<float> tt_jet1_phi;     Event::Handle<float> tt_jet2_phi;     Event::Handle<float> tt_jet3_phi;
  Event::Handle<float> tt_jet1_ptRaw;  Event::Handle<float> tt_jet2_ptRaw;  Event::Handle<float> tt_jet3_ptRaw;
  Event::Handle<float> tt_jet1_ptGen;  Event::Handle<float> tt_jet2_ptGen;  Event::Handle<float> tt_jet3_ptGen;
  Event::Handle<float> tt_jet1_etaGen;  Event::Handle<float> tt_jet2_etaGen;  Event::Handle<float> tt_jet3_etaGen;
  Event::Handle<float> tt_jet1_phiGen;  Event::Handle<float> tt_jet2_phiGen;  Event::Handle<float> tt_jet3_phiGen;
  Event::Handle<int> tt_nvertices;
  Event::Handle<float> tt_probejet_eta;  Event::Handle<float> tt_probejet_phi; Event::Handle<float> tt_probejet_pt; Event::Handle<float> tt_probejet_ptRaw;
  Event::Handle<float> tt_barreljet_eta;  Event::Handle<float> tt_barreljet_phi; Event::Handle<float> tt_barreljet_pt; Event::Handle<float> tt_barreljet_ptRaw;
  Event::Handle<float> tt_pt_ave;
  Event::Handle<float> tt_alpha;
  Event::Handle<float> tt_alpha_p;
  Event::Handle<float> tt_rel_r; Event::Handle<float> tt_mpf_r;
  Event::Handle<float> tt_asymmetry;
  Event::Handle<float> tt_B;
  Event::Handle<float> tt_MET;
  Event::Handle<int> tt_nPU;
  Event::Handle<int> tt_matchJetId_0;
  Event::Handle<int> tt_matchJetId_1;
  Event::Handle<float> tt_ev_weight;
  Event::Handle<float> tt_jets_pt;//sum of jets pT
  Event::Handle<int> tt_jet_n;//number of jets
  Event::Handle<float> tt_rho;//event energy density
  // Event::Handle<int> tt_nGoodvertices;
  // Event::Handle<int> tt_partonFlavor; //only MC
  Event::Handle<int> tt_flavorBarreljet, tt_flavorProbejet, tt_flavorLeadingjet, tt_flavorSubleadingjet; //only MC
  Event::Handle<float> tt_response_leadingjet;
  Event::Handle<float> tt_had_n_Efrac, tt_had_ch_Efrac, tt_mu_Efrac, tt_ph_Efrac;
  Event::Handle<float> tt_inst_lumi, tt_integrated_lumi_in_bin, tt_integrated_lumi;
  Event::Handle<int> tt_lumibin;

  Event::Handle<int> tt_trigger40;
  Event::Handle<int> tt_trigger60;
  Event::Handle<int> tt_trigger80;
  Event::Handle<int> tt_trigger140;
  Event::Handle<int> tt_trigger200;
  Event::Handle<int> tt_trigger260;
  Event::Handle<int> tt_trigger320;
  Event::Handle<int> tt_trigger400;
  Event::Handle<int> tt_trigger450;
  Event::Handle<int> tt_trigger500;

  Event::Handle<int> tt_trigger60_HFJEC;
  Event::Handle<int> tt_trigger80_HFJEC;
  Event::Handle<int> tt_trigger100_HFJEC;
  Event::Handle<int> tt_trigger160_HFJEC;
  Event::Handle<int> tt_trigger220_HFJEC;
  Event::Handle<int> tt_trigger300_HFJEC;

  // MC
  Event::Handle<float> tt_genjet1_pt;     Event::Handle<float> tt_genjet2_pt;     Event::Handle<float> tt_genjet3_pt;
  Event::Handle<float> tt_genjet1_eta;     Event::Handle<float> tt_genjet2_eta;     Event::Handle<float> tt_genjet3_eta;
  Event::Handle<float> tt_genjet1_phi;     Event::Handle<float> tt_genjet2_phi;     Event::Handle<float> tt_genjet3_phi;
  //Event::Handle<float> tt_genjet1_ptRaw;  Event::Handle<float> tt_genjet2_ptRaw;  Event::Handle<float> tt_genjet3_ptRaw;
  Event::Handle<float> tt_probegenjet_eta;  Event::Handle<float> tt_probegenjet_phi; Event::Handle<float> tt_probegenjet_pt;// Event::Handle<float> tt_probegenjet_ptRaw;
  Event::Handle<float> tt_barrelgenjet_eta;  Event::Handle<float> tt_barrelgenjet_phi; Event::Handle<float> tt_barrelgenjet_pt;// Event::Handle<float> tt_barrelgenjet_ptRaw;
  Event::Handle<float> tt_gen_pt_ave;
  Event::Handle<float> tt_gen_alpha;
  Event::Handle<float> tt_gen_alpha_p;
  Event::Handle<float> tt_gen_asymmetry;
  Event::Handle<int> tt_genjet_n;//number of jets
  Event::Handle<int> tt_no_mc_spikes;//number of jets

  // Event::Handle<int> tt_jet1_l1bx;
  // Event::Handle<int> tt_jet2_l1bx;
  // Event::Handle<int> tt_jet3_l1bx;


  std::unique_ptr<TestHists> h_nocuts, h_postreweight, h_postleptonveto, h_postjetcleaning, h_postleptoncleaning, h_postjer, h_posttrigger, h_precleaning;

  std::unique_ptr<JECAnalysisHists> h_cleaned, h_sel, h_dijet, h_match, h_final;
  std::unique_ptr<JECAnalysisHists> h_trg40, h_trg60, h_trg80, h_trg140, h_trg200,h_trg260,h_trg320,h_trg400, h_trg450,h_trg500;
  std::unique_ptr<JECAnalysisHists> h_trgHF60, h_trgHF80,h_trgHF100, h_trgHF160,h_trgHF220, h_trgHF300;
  std::unique_ptr<LuminosityHists> h_lumi_nocuts, h_lumi_sel, h_lumi_dijet, h_lumi_match, h_lumi_final;
  std::unique_ptr<LuminosityHists> h_lumi_Trig40, h_lumi_Trig60, h_lumi_Trig80, h_lumi_Trig140, h_lumi_Trig200, h_lumi_Trig260, h_lumi_Trig320, h_lumi_Trig400, h_lumi_Trig450, h_lumi_Trig500;
  std::unique_ptr<LuminosityHists> h_lumi_TrigHF60, h_lumi_TrigHF80, h_lumi_TrigHF100, h_lumi_TrigHF160, h_lumi_TrigHF220, h_lumi_TrigHF300;
  std::unique_ptr<JECRunnumberHists> h_runnr_input;
  std::unique_ptr<uhh2bacon::Selection> sel;
  // uhh2bacon::Selection sel;

  bool debug, no_genp;
  bool isMC, closure, apply_EtaPhi_cut, apply_EtaPhi_HCAL, do_only_centraltriggers, do_only_forwardtriggers, do_fulltriggers, trigger_central, trigger_fwd, DO_Pu_ReWeighting, DO_Lumi_ReWeighting, apply_L1seed_from_bx1_filter;
  double lumiweight;
  string PtBinsTrigger, SysType_PU;
  TString dataset_version, JEC_Version, jetLabel;
  JetId Jet_PFID;
  int n_evt;
  bool test_trigger, isThreshold;
  std::unique_ptr<TFile> f_weights;

  std::map<run_lumi, double> rl2lumi;
  std::map<run_lumi, double> rl2intlumi;
  TBranch * brun ;
  TBranch * blumiblock;
  TBranch * bilumi;
  double integrated_lumi;
  vector<run_lumi> upper_binborders_runnrs;
  vector<double> lumi_in_bins;


};

JERSFModuleTest::JERSFModuleTest(uhh2::Context & ctx) {

  sel.reset(new uhh2bacon::Selection(ctx));
  // for(auto & kv : ctx.get_all()){
  //  cout << " " << kv.first << " = " << kv.second << endl;
  // }

  no_genp=false;
  cout << "start" << endl;
  dataset_version = ctx.get("dataset_version");
  jetLabel = ctx.get("JetLabel");
  isMC = (ctx.get("dataset_type") == "MC");
  test_trigger = string2bool(ctx.get("test_trigger", "false"));
  isThreshold = string2bool(ctx.get("isThreshold", "false"));
  cout << "Dataset is " << ((isMC) ? " mc " : " data") << endl;

  //#############################################  Filters  #########################################################
  // COMMON MODULES
  if(!isMC) lumi_selection.reset(new LumiSelection(ctx));
  PVCleaner.reset(new PrimaryVertexCleaner());
  /* MET filters */
  if(!isMC) {
    PrimaryVertexId pvid=StandardPrimaryVertexId();
    metfilters_sel.reset(new uhh2::AndSelection(ctx, "metfilters"));
    metfilters_sel->add<TriggerSelection>("goodVertices", "Flag_goodVertices");
    metfilters_sel->add<TriggerSelection>("globalSuperTightHalo2016Filter", "Flag_globalSuperTightHalo2016Filter");
    metfilters_sel->add<TriggerSelection>("HBHENoiseFilter", "Flag_HBHENoiseFilter");
    metfilters_sel->add<TriggerSelection>("HBHENoiseIsoFilter", "Flag_HBHENoiseIsoFilter");
    metfilters_sel->add<TriggerSelection>("EcalDeadCellTriggerPrimitiveFilter", "Flag_EcalDeadCellTriggerPrimitiveFilter");
    metfilters_sel->add<TriggerSelection>("BadPFMuonFilter", "Flag_BadPFMuonFilter");
    metfilters_sel->add<TriggerSelection>("BadChargedCandidateFilter", "Flag_BadChargedCandidateFilter");
    metfilters_sel->add<TriggerSelection>("eeBadScFilter", "Flag_eeBadScFilter"); // Not recommended for MC, but do check
    metfilters_sel->add<TriggerSelection>("ecalBadCalibFilter", "Flag_ecalBadCalibFilter"); // for 2017 and 2018 is always 1. need a EcalBadCalibSelection for the recalculated value.
    metfilters_sel->add<EcalBadCalibSelection>("EcalBadCalibSelection");
    metfilters_sel->add<NPVSelection>("1 good PV",1,-1,pvid);


  }
  //Jet cleaner
  Jet_PFID = JetPFID(JetPFID::WP_TIGHT_CHS);
  jetcleaner.reset(new JetCleaner(ctx, AndId<Jet>(Jet_PFID, PtEtaCut(10,5))));

  //Lepton cleaner
  const     MuonId muoSR(AndId<Muon>    (MuonID(Muon::CutBasedIdTight),PtEtaCut  (15, 2.4)));
  const ElectronId eleSR(AndId<Electron>(ElectronID_MVA_Fall17_loose_iso , PtEtaSCCut(15, 2.4)));

  muoSR_cleaner.reset(new     MuonCleaner(muoSR));
  eleSR_cleaner.reset(new ElectronCleaner(eleSR));

  //#############################################  Trigger  #########################################################

  trigger_central = (ctx.get("Trigger_Central") == "true");
  trigger_fwd     = (ctx.get("Trigger_FWD") == "true");
  PtBinsTrigger   =  ctx.get("PtBinsTrigger");

  #define GET_RESET_TRIGGER(trg_name)                                               \
  const std::string& trg_name = ctx.get( #trg_name , "NULL");                       \
  if ( trg_name != "NULL") trg_name##_sel.reset(new TriggerSelection( trg_name ));  \
  else trg_name##_sel.reset(new uhh2::AndSelection(ctx));                           \

  if(!isMC){

    if (PtBinsTrigger == "Single") {
      GET_RESET_TRIGGER(trigger40)
      GET_RESET_TRIGGER(trigger60)
      GET_RESET_TRIGGER(trigger80)
      GET_RESET_TRIGGER(trigger140)
      GET_RESET_TRIGGER(trigger200)
      GET_RESET_TRIGGER(trigger260)
      GET_RESET_TRIGGER(trigger320)
      GET_RESET_TRIGGER(trigger400)
      GET_RESET_TRIGGER(trigger450)
      GET_RESET_TRIGGER(trigger500)

      GET_RESET_TRIGGER(trigger60_HFJEC)
      GET_RESET_TRIGGER(trigger80_HFJEC)
      GET_RESET_TRIGGER(trigger100_HFJEC)
      GET_RESET_TRIGGER(trigger160_HFJEC)
      GET_RESET_TRIGGER(trigger220_HFJEC)
      GET_RESET_TRIGGER(trigger300_HFJEC)
    }
    else if (PtBinsTrigger == "DiJet") {
      GET_RESET_TRIGGER(trigger40)
      GET_RESET_TRIGGER(trigger60)
      GET_RESET_TRIGGER(trigger80)
      GET_RESET_TRIGGER(trigger140)
      GET_RESET_TRIGGER(trigger200)
      GET_RESET_TRIGGER(trigger260)
      GET_RESET_TRIGGER(trigger320)
      GET_RESET_TRIGGER(trigger400)
      GET_RESET_TRIGGER(trigger500)

      GET_RESET_TRIGGER(trigger60_HFJEC)
      GET_RESET_TRIGGER(trigger80_HFJEC)
      GET_RESET_TRIGGER(trigger100_HFJEC)
      GET_RESET_TRIGGER(trigger160_HFJEC)
      GET_RESET_TRIGGER(trigger220_HFJEC)
      GET_RESET_TRIGGER(trigger300_HFJEC)
    }
    else if (PtBinsTrigger == "ZeroBias") {
      if (test_trigger) {
        GET_RESET_TRIGGER(triggerMB_part0)
        GET_RESET_TRIGGER(triggerMB_part1)
        GET_RESET_TRIGGER(triggerMB_part2)
        GET_RESET_TRIGGER(triggerMB_part3)
        GET_RESET_TRIGGER(triggerMB_part4)
        GET_RESET_TRIGGER(triggerMB_part5)
        GET_RESET_TRIGGER(triggerMB_part6)
        GET_RESET_TRIGGER(triggerMB_part7)
        GET_RESET_TRIGGER(triggerMB_part8)
        GET_RESET_TRIGGER(triggerMB_part9)
      } else {
        GET_RESET_TRIGGER(triggerMB)
      }
    }
  }


  //#############################################  Jet Correction  ##################################################
  apply_EtaPhi_cut = (ctx.get("EtaPhi_cut") == "true");
  apply_EtaPhi_HCAL = (ctx.get("EtaPhi_HCAL") == "true");
  JEC_Version = ctx.get("JEC_Version");
  closure = (ctx.get("DO_JERSMEAR") == "true");
  // apply_L1seed_from_bx1_filter =  (ctx.get("apply_L1seed_from_bx1_filter") == "true" && !isMC);

  do_only_forwardtriggers = (ctx.get("Triggers") == "forward" || ctx.get("Triggers") == "Forward");
  do_only_centraltriggers = (ctx.get("Triggers") == "central" || ctx.get("Triggers") == "Central");
  do_fulltriggers = (ctx.get("Triggers") == "full" || ctx.get("Triggers") == "Full");
  if(do_only_forwardtriggers + do_only_centraltriggers + do_fulltriggers != 1) throw runtime_error("Invalid specification of triggers to be used. Not == 1 value is true.");

  std::vector<std::string> JEC_corr,       JEC_corr_A,       JEC_corr_B,       JEC_corr_C,       JEC_corr_D;
  std::vector<std::string> JEC_corr_L1RC,  JEC_corr_A_L1RC,  JEC_corr_B_L1RC,  JEC_corr_C_L1RC,  JEC_corr_D_L1RC;


  #define MAKE_JEC(jecv,jetLabel)         					                    \
  if(JEC_Version == #jecv){                                             \
    JEC_corr_A      = JERFiles::jecv##_B_L123_##jetLabel##_DATA;        \
    JEC_corr_B      = JERFiles::jecv##_B_L123_##jetLabel##_DATA;        \
    JEC_corr_C      = JERFiles::jecv##_C_L123_##jetLabel##_DATA;        \
    JEC_corr_D      = JERFiles::jecv##_D_L123_##jetLabel##_DATA;        \
    JEC_corr_A_L1RC = JERFiles::jecv##_B_L1RC_##jetLabel##_DATA;        \
    JEC_corr_B_L1RC = JERFiles::jecv##_B_L1RC_##jetLabel##_DATA;        \
    JEC_corr_C_L1RC = JERFiles::jecv##_C_L1RC_##jetLabel##_DATA;        \
    JEC_corr_D_L1RC = JERFiles::jecv##_D_L1RC_##jetLabel##_DATA;        \
  }                                                                     \

  #define MAKE_JEC2(jecv,jetLabel)         					                    \
  if(JEC_Version == #jecv){                                             \
    JEC_corr_A      = JERFiles::jecv##_L123_##jetLabel##_MC;        \
    JEC_corr_B      = JERFiles::jecv##_L123_##jetLabel##_MC;        \
    JEC_corr_C      = JERFiles::jecv##_L123_##jetLabel##_MC;        \
    JEC_corr_D      = JERFiles::jecv##_L123_##jetLabel##_MC;        \
    JEC_corr_A_L1RC = JERFiles::jecv##_L1RC_##jetLabel##_MC;        \
    JEC_corr_B_L1RC = JERFiles::jecv##_L1RC_##jetLabel##_MC;        \
    JEC_corr_C_L1RC = JERFiles::jecv##_L1RC_##jetLabel##_MC;        \
    JEC_corr_D_L1RC = JERFiles::jecv##_L1RC_##jetLabel##_MC;        \
  }                                                                     \

  #define MAKE_JEC2017(jecv,jetLabel)         					                \
  if(JEC_Version == #jecv){                                             \
    JEC_corr_A      = JERFiles::jecv##_B_L123_##jetLabel##_DATA;        \
    JEC_corr_B      = JERFiles::jecv##_B_L123_##jetLabel##_DATA;        \
    JEC_corr_C      = JERFiles::jecv##_C_L123_##jetLabel##_DATA;        \
    JEC_corr_D      = JERFiles::jecv##_D_L123_##jetLabel##_DATA;        \
    JEC_corr_A_L1RC = JERFiles::jecv##_B_L1RC_##jetLabel##_DATA;        \
    JEC_corr_B_L1RC = JERFiles::jecv##_B_L1RC_##jetLabel##_DATA;        \
    JEC_corr_C_L1RC = JERFiles::jecv##_C_L1RC_##jetLabel##_DATA;        \
    JEC_corr_D_L1RC = JERFiles::jecv##_D_L1RC_##jetLabel##_DATA;        \
  }                                                                     \


  #define MAKE_JEC_MC(jecv,jetLabel)					                          \
  if(JEC_Version == #jecv){                                             \
    JEC_corr      = JERFiles::jecv##_L123_##jetLabel##_MC;              \
    JEC_corr_L1RC = JERFiles::jecv##_L1RC_##jetLabel##_MC;              \
  }                                                                     \

  #define MAKE_JEC_MC_AK8(jecv,jetLabel)					                      \
  if(JEC_Version == #jecv){                                             \
    JEC_corr      = JERFiles::jecv##_L123_##jetLabel##_MC;              \
  }                                                                     \


  if(isMC){ //L123 for MC
    if (jetLabel == "AK4CHS") {
      MAKE_JEC_MC(Autumn18_V4, AK4PFchs)
      else MAKE_JEC_MC(Autumn18_V7, AK4PFchs)
      else MAKE_JEC_MC(Autumn18_V8, AK4PFchs)
      else MAKE_JEC_MC(Autumn18_V10, AK4PFchs)
      else throw runtime_error("In JERSFModuleTest.cxx: Invalid JEC_Version for deriving residuals on AK4CHS, MC specified ("+JEC_Version+") ");
    }
    else if (jetLabel == "AK8PUPPI") {
      MAKE_JEC_MC_AK8(Autumn18_V4, AK8PFPuppi)
      // else throw runtime_error("In JERSFModuleTest.cxx: Invalid JEC_Version for deriving residuals on AK8PUPPI, MC specified ("+JEC_Version+") ");
    }
    else throw runtime_error("In JERSFModuleTest.cxx: Invalid jetLabel: "+jetLabel+" ");
  }//MC
  else { //L123 + L2L3Res for Data
    if (jetLabel == "AK4CHS") {
      // MAKE_JEC(Autumn18_V4, AK4PFchs)
      MAKE_JEC2(Autumn18_V4, AK4PFchs)
      else MAKE_JEC(Autumn18_V7, AK4PFchs)
      else MAKE_JEC(Autumn18_V8, AK4PFchs)
      else MAKE_JEC(Autumn18_V10, AK4PFchs)
      else throw runtime_error("In JERSFModuleTest.cxx: Invalid JEC_Version for deriving residuals on AK4CHS "+JEC_Version+", DATA specified.");
    }
    else if (jetLabel == "AK8PUPPI") {
      // MAKE_JEC(Autumn18_V4, AK8PFPuppi)
      MAKE_JEC(Autumn18_V4, AK8PFPuppi)
      // else throw runtime_error("In JERSFModuleTest.cxx: Invalid JEC_Version for deriving residuals on AK8PUPPI "+JEC_Version+", DATA specified.");
    }
    else throw runtime_error("In JERSFModuleTest.cxx: Invalid jetLabel: "+jetLabel+" ");

  }//DATA


  if(jetLabel == "AK4CHS" || jetLabel == "AK8PUPPI"){
    //DATA
    if(!isMC){
      jet_corrector_A.reset(new JetCorrector(ctx, JEC_corr_A));
      jet_corrector_B.reset(new JetCorrector(ctx, JEC_corr_B));
      jet_corrector_C.reset(new JetCorrector(ctx, JEC_corr_C));
      jet_corrector_D.reset(new JetCorrector(ctx, JEC_corr_D));
      JLC_A.reset(new JetLeptonCleaner(ctx, JEC_corr_A));
      JLC_B.reset(new JetLeptonCleaner(ctx, JEC_corr_B));
      JLC_C.reset(new JetLeptonCleaner(ctx, JEC_corr_C));
      JLC_D.reset(new JetLeptonCleaner(ctx, JEC_corr_D));
    }
    else if(isMC){//MC : only one version of JECs exists
      jet_corrector.reset(new JetCorrector(ctx, JEC_corr));
      jetleptoncleaner.reset(new JetLeptonCleaner(ctx, JEC_corr));
    }
  }

  // JER Smearing for corresponding JEC-Version
  if(closure && isMC) {
    jet_resolution_smearer.reset(new GenericJetResolutionSmearer(ctx, "jets", "genjets", JERSmearing::SF_13TeV_Autumn18_RunABC_V1, "2018/Autumn18_V1_MC_PtResolution_AK4PFchs.txt"));
  }

  //output
  ctx.undeclare_all_event_output();

  // Do pileup reweighting (define it after undeclaring all other variables to keep the weights in the output)
  DO_Lumi_ReWeighting = string2bool(ctx.get("DO_Lumi_ReWeighting"));
  DO_Lumi_ReWeighting = DO_Lumi_ReWeighting && isMC;
  if (DO_Lumi_ReWeighting) LumiSF.reset(new MCLumiWeight(ctx));

  DO_Pu_ReWeighting = string2bool(ctx.get("DO_Pu_ReWeighting"));
  DO_Pu_ReWeighting = DO_Pu_ReWeighting && isMC;
  SysType_PU = ctx.get("SysType_PU");
  if (DO_Pu_ReWeighting) pileupSF.reset(new MCPileupReweight(ctx,SysType_PU));

  //    tt_dijet_event = ctx.declare_event_output<dijet_event>("dijet");
  //Store only vars needed for the dijet analysis
  //tt_gen_PUpthat = ctx.declare_event_output<float>("gen_PUpthat");
  tt_hf_trigger = ctx.declare_event_output<int>("hf_trigger");
  tt_bl_trigger = ctx.declare_event_output<int>("bl_trigger");
  tt_gen_pthat = ctx.declare_event_output<float>("gen_pthat");
  tt_PU_pt_hat= ctx.declare_event_output<float>("PU_pt_hat");
  tt_gen_weight = ctx.declare_event_output<float>("gen_weight");
  tt_jet1_pt = ctx.declare_event_output<float>("jet1_pt");
  tt_jet2_pt = ctx.declare_event_output<float>("jet2_pt");
  tt_jet3_pt = ctx.declare_event_output<float>("jet3_pt");
  tt_jet1_eta = ctx.declare_event_output<float>("jet1_eta");
  tt_jet2_eta = ctx.declare_event_output<float>("jet2_eta");
  tt_jet3_eta = ctx.declare_event_output<float>("jet3_eta");
  tt_jet1_phi = ctx.declare_event_output<float>("jet1_phi");
  tt_jet2_phi = ctx.declare_event_output<float>("jet2_phi");
  tt_jet3_phi = ctx.declare_event_output<float>("jet3_phi");
  tt_jet1_ptRaw = ctx.declare_event_output<float>("jet1_ptRaw");
  tt_jet2_ptRaw = ctx.declare_event_output<float>("jet2_ptRaw");
  tt_jet3_ptRaw = ctx.declare_event_output<float>("jet3_ptRaw");
  tt_jet1_ptGen = ctx.declare_event_output<float>("jet1_ptGen");
  tt_jet2_ptGen = ctx.declare_event_output<float>("jet2_ptGen");
  tt_jet3_ptGen = ctx.declare_event_output<float>("jet3_ptGen");
  tt_jet1_etaGen = ctx.declare_event_output<float>("jet1_etaGen");
  tt_jet2_etaGen = ctx.declare_event_output<float>("jet2_etaGen");
  tt_jet3_etaGen = ctx.declare_event_output<float>("jet3_etaGen");
  tt_jet1_phiGen = ctx.declare_event_output<float>("jet1_phiGen");
  tt_jet2_phiGen = ctx.declare_event_output<float>("jet2_phiGen");
  tt_jet3_phiGen = ctx.declare_event_output<float>("jet3_phiGen");
  tt_nvertices = ctx.declare_event_output<int>("nvertices");
  // tt_nGoodvertices = ctx.declare_event_output<int>("nGoodvertices");
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
  tt_alpha_p = ctx.declare_event_output<float>("alpha_p");
  tt_rel_r = ctx.declare_event_output<float>("rel_r");
  tt_mpf_r = ctx.declare_event_output<float>("mpf_r");
  tt_asymmetry = ctx.declare_event_output<float>("asymmetry");
  tt_B = ctx.declare_event_output<float>("B");
  tt_MET = ctx.declare_event_output<float>("MET");
  tt_nPU = ctx.declare_event_output<int>("nPU");
  tt_matchJetId_0 = ctx.declare_event_output<int>("matchJetId_0");
  tt_matchJetId_1 =ctx.declare_event_output<int>("matchJetId_1");
  tt_ev_weight = ctx.declare_event_output<float>("weight");
  tt_jets_pt= ctx.declare_event_output<float>("sum_jets_pt");
  tt_jet_n= ctx.declare_event_output<int>("Njet");
  tt_rho = ctx.declare_event_output<float>("rho");
  // tt_partonFlavor = ctx.declare_event_output<int>("partonFlavor");
  tt_flavorBarreljet = ctx.declare_event_output<int>("flavorBarreljet");
  tt_flavorProbejet = ctx.declare_event_output<int>("flavorProbejet");
  tt_flavorLeadingjet = ctx.declare_event_output<int>("flavorLeadingjet");
  tt_flavorSubleadingjet = ctx.declare_event_output<int>("flavorSubleadingjet");
  tt_response_leadingjet = ctx.declare_event_output<float>("leadingjet_response");
  tt_had_n_Efrac = ctx.declare_event_output<float>("neutralhad_Efraction");
  tt_had_ch_Efrac = ctx.declare_event_output<float>("chargedhad_Efraction");
  tt_mu_Efrac = ctx.declare_event_output<float>("mu_Efraction");
  tt_ph_Efrac = ctx.declare_event_output<float>("photon_Efraction");
  tt_inst_lumi = ctx.declare_event_output<float>("instantaneous_lumi");
  tt_integrated_lumi_in_bin = ctx.declare_event_output<float>("integrated_lumi_in_bin");
  tt_lumibin = ctx.declare_event_output<int>("lumibin");
  tt_integrated_lumi = ctx.declare_event_output<float>("integrated_lumi");

  tt_trigger40 = ctx.declare_event_output<int>("trigger40");
  tt_trigger60 = ctx.declare_event_output<int>("trigger60");
  tt_trigger80 = ctx.declare_event_output<int>("trigger80");
  tt_trigger140 = ctx.declare_event_output<int>("trigger140");
  tt_trigger200 = ctx.declare_event_output<int>("trigger200");
  tt_trigger260 = ctx.declare_event_output<int>("trigger260");
  tt_trigger320 = ctx.declare_event_output<int>("trigger320");
  tt_trigger400 = ctx.declare_event_output<int>("trigger400");
  tt_trigger450 = ctx.declare_event_output<int>("trigger450");
  tt_trigger500 = ctx.declare_event_output<int>("trigger500");

  tt_trigger60_HFJEC = ctx.declare_event_output<int>("trigger60_HFJEC");
  tt_trigger80_HFJEC = ctx.declare_event_output<int>("trigger80_HFJEC");
  tt_trigger100_HFJEC = ctx.declare_event_output<int>("trigger100_HFJEC");
  tt_trigger160_HFJEC = ctx.declare_event_output<int>("trigger160_HFJEC");
  tt_trigger220_HFJEC = ctx.declare_event_output<int>("trigger220_HFJEC");
  tt_trigger300_HFJEC = ctx.declare_event_output<int>("trigger300_HFJEC");

  // MC:
  tt_genjet1_pt = ctx.declare_event_output<float>("genjet1_pt");
  tt_genjet2_pt = ctx.declare_event_output<float>("genjet2_pt");
  tt_genjet3_pt = ctx.declare_event_output<float>("genjet3_pt");
  tt_genjet1_eta = ctx.declare_event_output<float>("genjet1_eta");
  tt_genjet2_eta = ctx.declare_event_output<float>("genjet2_eta");
  tt_genjet3_eta = ctx.declare_event_output<float>("genjet3_eta");
  tt_genjet1_phi = ctx.declare_event_output<float>("genjet1_phi");
  tt_genjet2_phi = ctx.declare_event_output<float>("genjet2_phi");
  tt_genjet3_phi = ctx.declare_event_output<float>("genjet3_phi");
  //tt_genjet1_ptRaw = ctx.declare_event_output<float>("genjet1_ptRaw");
  //tt_genjet2_ptRaw = ctx.declare_event_output<float>("genjet2_ptRaw");
  //tt_genjet3_ptRaw = ctx.declare_event_output<float>("genjet3_ptRaw");
  tt_probegenjet_eta = ctx.declare_event_output<float>("probegenjet_eta");
  tt_probegenjet_phi = ctx.declare_event_output<float>("probegenjet_phi");
  tt_probegenjet_pt = ctx.declare_event_output<float>("probegenjet_pt");
  //tt_probegenjet_ptRaw = ctx.declare_event_output<float>("probegenjet_ptRaw");
  tt_barrelgenjet_eta = ctx.declare_event_output<float>("barrelgenjet_eta");
  tt_barrelgenjet_phi = ctx.declare_event_output<float>("barrelgenjet_phi");
  tt_barrelgenjet_pt = ctx.declare_event_output<float>("barrelgenjet_pt");
  //tt_barrelgenjet_ptRaw = ctx.declare_event_output<float>("barrelgenjet_ptRaw");
  tt_gen_pt_ave = ctx.declare_event_output<float>("gen_pt_ave");
  tt_gen_alpha = ctx.declare_event_output<float>("gen_alpha");
  tt_gen_alpha_p = ctx.declare_event_output<float>("gen_alpha_p");
  tt_gen_asymmetry = ctx.declare_event_output<float>("gen_asymmetry");
  tt_genjet_n= ctx.declare_event_output<int>("Ngenjet");
  tt_no_mc_spikes= ctx.declare_event_output<int>("no_mc_spikes");

  // tt_jet1_l1bx = ctx.declare_event_output<int>("jet1_l1bx");
  // tt_jet2_l1bx = ctx.declare_event_output<int>("jet1_l2bx");
  // tt_jet3_l1bx = ctx.declare_event_output<int>("jet1_l3bx");

  h_runnr_input.reset(new JECRunnumberHists(ctx,"Runnr_input"));

  h_nocuts.reset(new TestHists(ctx,"NoCuts"));
  h_postreweight.reset(new TestHists(ctx,"postreweight"));
  h_postleptonveto.reset(new TestHists(ctx,"postleptonveto"));
  h_postjetcleaning.reset(new TestHists(ctx,"postjetcleaning"));
  h_postleptoncleaning.reset(new TestHists(ctx,"postleptoncleaning"));
  h_postjer.reset(new TestHists(ctx,"postjer"));
  h_posttrigger.reset(new TestHists(ctx,"posttrigger"));
  h_precleaning.reset(new TestHists(ctx,"precleaning"));
  h_cleaned.reset(new JECAnalysisHists(ctx,"Cleaned"));
  h_dijet.reset(new JECAnalysisHists(ctx,"diJet"));
  h_match.reset(new JECAnalysisHists(ctx,"JetMatching"));
  h_sel.reset(new JECAnalysisHists(ctx,"Selection"));
  h_final.reset(new JECAnalysisHists(ctx,"Final"));

  h_trg40.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve40"));
  h_trg60.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve60"));
  h_trg80.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve80"));
  h_trg140.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve140"));
  h_trg200.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve200"));
  h_trg260.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve260"));
  h_trg320.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve320"));
  h_trg400.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve400"));
  h_trg450.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve450"));
  h_trg500.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve500"));

  h_trgHF60.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve60_HFJEC"));
  h_trgHF80.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve80_HFJEC"));
  h_trgHF100.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve100_HFJEC"));
  h_trgHF160.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve160_HFJEC"));
  h_trgHF220.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve220_HFJEC"));
  h_trgHF300.reset(new JECAnalysisHists(ctx,"HLT_DiPFJetAve300_HFJEC"));

  h_lumi_nocuts.reset(new LuminosityHists(ctx,"Lumi_noCuts"));
  h_lumi_sel.reset(new LuminosityHists(ctx,"Lumi_Selection"));
  h_lumi_dijet.reset(new LuminosityHists(ctx,"Lumi_diJet"));
  h_lumi_match.reset(new LuminosityHists(ctx,"Lumi_JetMatching"));
  h_lumi_final.reset(new LuminosityHists(ctx,"Lumi_Final"));
  h_lumi_Trig40.reset(new LuminosityHists(ctx,"Lumi_Trig40"));
  h_lumi_Trig60.reset(new LuminosityHists(ctx,"Lumi_Trig60"));
  h_lumi_Trig80.reset(new LuminosityHists(ctx,"Lumi_Trig80"));
  h_lumi_Trig140.reset(new LuminosityHists(ctx,"Lumi_Trig140"));
  h_lumi_Trig200.reset(new LuminosityHists(ctx,"Lumi_Trig200"));
  h_lumi_Trig260.reset(new LuminosityHists(ctx,"Lumi_Trig260"));
  h_lumi_Trig320.reset(new LuminosityHists(ctx,"Lumi_Trig320"));
  h_lumi_Trig400.reset(new LuminosityHists(ctx,"Lumi_Trig400"));
  h_lumi_Trig450.reset(new LuminosityHists(ctx,"Lumi_Trig450"));
  h_lumi_Trig500.reset(new LuminosityHists(ctx,"Lumi_Trig500"));
  h_lumi_TrigHF60.reset(new LuminosityHists(ctx,"Lumi_TrigHF60"));
  h_lumi_TrigHF80.reset(new LuminosityHists(ctx,"Lumi_TrigHF80"));
  h_lumi_TrigHF100.reset(new LuminosityHists(ctx,"Lumi_TrigHF100"));
  h_lumi_TrigHF160.reset(new LuminosityHists(ctx,"Lumi_TrigHF160"));
  h_lumi_TrigHF220.reset(new LuminosityHists(ctx,"Lumi_TrigHF220"));
  h_lumi_TrigHF300.reset(new LuminosityHists(ctx,"Lumi_TrigHF300"));

  Jet_printer.reset(new JetPrinter("Jet-Printer", 0));
  GenParticles_printer.reset(new GenParticlesPrinter(ctx));

  debug = false;
  // debug = true;
  n_evt = 0;

  string lumifile = ctx.get("lumi_file");
  std::unique_ptr<TFile> file(TFile::Open(lumifile.c_str(), "read"));
  TTree * tree = dynamic_cast<TTree*>(file->Get("AnalysisTree"));
  if(!tree){
    throw runtime_error("LuminosityHists: Did not find TTree 'AnalysisTree' in file ;" + lumifile + "'");
  }
  // only fetch branches we really need:
  brun = tree->GetBranch("run");
  blumiblock = tree->GetBranch("luminosityBlock");
  bilumi = tree->GetBranch("intgRecLumi");


  run_lumi rl;
  double ilumi;
  double intlumi_pb = 0;
  brun->SetAddress(&rl.run);
  blumiblock->SetAddress(&rl.lumiblock);
  bilumi->SetAddress(&ilumi);

  //loop over all lumiblocks to save the map between run/lumiblock and stored lumi of the lumiblock (to be divided by 23s)
  auto ientries = tree->GetEntries();
  for(auto ientry = 0l; ientry < ientries; ientry++){
    for(auto b : {brun, blumiblock, bilumi}){
      b->GetEntry(ientry);
    }
    double ilumi_pb = ilumi * 1e-6; // convert units in file (microbarn) to pb.
    intlumi_pb += ilumi_pb;
    rl2lumi.insert(make_pair(rl, ilumi_pb));
    rl2intlumi.insert(make_pair(rl, intlumi_pb));
  }


  double ilumi_current_bin = 0.0;
  run_lumi last_entry;
  for(const auto & rl : rl2lumi){
    ilumi_current_bin += rl.second;
    if(ilumi_current_bin >= 2000){
      upper_binborders_runnrs.push_back(rl.first);
      lumi_in_bins.push_back(ilumi_current_bin - rl.second);
      ilumi_current_bin = ilumi_current_bin - 2000;
    }
    last_entry = rl.first;
  }
  upper_binborders_runnrs.push_back(last_entry); //this is not exactly an UPPER limit because it is equal to the highest possible entry, not greater than it...created exception for this case.
  lumi_in_bins.push_back(ilumi_current_bin);

  std::cout << "end Constructor" << '\n';

};





JERSFModuleTest::~JERSFModuleTest() {

}

bool JERSFModuleTest::process(Event & event) {

  #define ak4jets event.jets

  n_evt++;

  h_nocuts->fill(event);

  //Dump Input
  // h_input->fill(event);

  // Do pileup reweighting
  if (DO_Lumi_ReWeighting){
    bool pass_Lumi_reweighting = LumiSF->process(event);
    if (debug) std::cout << "DO_Lumi_ReWeighting " << pass_Lumi_reweighting << '\n';
    if(!pass_Lumi_reweighting) return false;
  }

  // Do pileup reweighting
  if (DO_Pu_ReWeighting){
    bool pass_reweighting = pileupSF->process(event);
    if (debug) std::cout << "DO_Pu_ReWeighting " << pass_reweighting << '\n';
    if(!pass_reweighting) return false;
  }

  h_postreweight->fill(event);

  //LEPTON selection
  muoSR_cleaner->process(event);
  sort_by_pt<Muon>(*event.muons);
  if(debug )std::cout<<"#muons = "<<event.muons->size()<<std::endl;

  eleSR_cleaner->process(event);
  sort_by_pt<Electron>(*event.electrons);
  if(debug) std::cout<<"#electrons = "<<event.electrons->size()<<std::endl;

  if (event.electrons->size()>0 || event.muons->size()>0) return false; //TEST lepton cleaning

  h_postleptonveto->fill(event);

  // PrimaryVertexV Cleaner
  if (!PVCleaner->process(event)) return false;

  // CMS-certified luminosity sections
  if (event.isRealData) {
    if (debug) std::cout << "checking for lumisel " << '\n';
    if (!lumi_selection->passes(event)) return false;
    if (debug) std::cout << "passed lumisel " << '\n';
  }

  // MET filters
  if (!isMC) {
    if(!metfilters_sel->passes(event)) return false;
    // std::cout << "after metfilters" << '\n';
  }

  int event_in_lumibin = -1;
  double fill_event_integrated_lumi = 0;
  double inst_lumi = -1;
  double int_lumi_event = -1;
  if(event.isRealData){
    run_lumi rl_event{event.run, event.luminosityBlock};
    double lumiblock_lumi = rl2lumi[rl_event];
    inst_lumi = lumiblock_lumi/23;
    int_lumi_event = rl2intlumi[rl_event];

    vector<run_lumi>::iterator it;
    if(!(rl_event < upper_binborders_runnrs.back())){
      if(upper_binborders_runnrs.back() < rl_event) throw runtime_error("JERSFModuleTest: run_lumi of event greater than largest bin-border.");
      else it = prev(upper_binborders_runnrs.end()); //force the entries with the highest run_lumi to enter the last bin instead of overflow.
    }
    else it = upper_bound(upper_binborders_runnrs.begin(), upper_binborders_runnrs.end(), rl_event); //find the first entry in the vector of binborders that is greater than rl_event
    event_in_lumibin = distance(upper_binborders_runnrs.begin(), it); //find how many elements of the vector of binborders are smaller than 'it', this is the bin to be filled
    fill_event_integrated_lumi = lumi_in_bins.at(event_in_lumibin);
  }
  int n_jets_beforeCleaner = ak4jets->size();
  //JetID
  jetcleaner->process(event);
  int n_jets_afterCleaner = ak4jets->size();
  //discard events if not all jets fulfill JetID instead of just discarding single jets
  if (debug) std::cout << "n_jets_beforeCleaner vs n_jets_afterCleaner " << n_jets_beforeCleaner << " " << n_jets_afterCleaner << '\n';
  if (n_jets_beforeCleaner != n_jets_afterCleaner) return false;
  sort_by_pt<Jet>(*ak4jets);

  const int jet_n = ak4jets->size();
  if (debug) std::cout << "jet_n " << jet_n << '\n';
  if(jet_n<2) return false;

  h_postjetcleaning->fill(event);

  bool apply_A = false;
  bool apply_B = false;
  bool apply_C = false;
  bool apply_D = false;
  bool apply_global = false;

  if(!isMC){
    if(event.run <= s_runnr_A)      apply_A = true;
    else if(event.run <= s_runnr_B) apply_B = true;
    else if(event.run <= s_runnr_C) apply_C = true;
    else if(event.run <= s_runnr_D) apply_D = true;
    else throw runtime_error("AnalysisModule: run number not covered by if-statements in process-routine.");
  }
  else if(isMC){
    apply_global = true;
  }

  if(apply_A+apply_B+apply_C+apply_D+apply_global != 1) throw runtime_error("In JERSFModuleTest.cxx: Sum of apply_* when applying JECs is not == 1. Fix this.");

  //apply proper JECs
  if(apply_A){
    JLC_A->process(event);
    jet_corrector_A->process(event);
  }
  if(apply_B){
    JLC_B->process(event);
    jet_corrector_B->process(event);
  }
  if(apply_C){
    JLC_C->process(event);
    jet_corrector_C->process(event);
  }
  if(apply_D){
    JLC_D->process(event);
    jet_corrector_D->process(event);
  }
  if(apply_global){
    jetleptoncleaner->process(event);
    jet_corrector->process(event);
  }

  h_postleptoncleaning->fill(event);

  if(closure && isMC) jet_resolution_smearer->process(event);

  h_postjer->fill(event);

  //correct MET only AFTER smearing the jets
  // if(apply_A){
  //   jet_corrector_A->correct_met(event,true);
  // }
  // if(apply_B){
  //   jet_corrector_B->correct_met(event,true);
  // }
  // if(apply_C){
  //   jet_corrector_C->correct_met(event,true);
  // }
  // if(apply_D){
  //   jet_corrector_D->correct_met(event,true);
  // }
  // if(apply_global){
  //   jet_corrector->correct_met(event,true);
  // }

  sort_by_pt<Jet>(*ak4jets);

  Jet* jet1 = &ak4jets->at(0);// leading jet
  Jet* jet2 = &ak4jets->at(1);// sub-leading jet
  float jet1_pt = jet1->pt(); float jet2_pt = jet2->pt();
  float jet1_eta = jet1->eta(); float jet2_eta = jet2->eta();
  float jet1_phi = jet1->phi(); float jet2_phi = jet2->phi();
  float pt_ave = (jet1_pt + jet2_pt)/2.;

  Jet* jet_barrel = jet1; Jet* jet_probe = jet2;
  //// HLT selection
  bool pass_trigger_bl=false; bool pass_trigger_hf=false;
  bool pass_trigger40=false; bool pass_trigger60=false; bool pass_trigger80=false;
  bool pass_trigger140=false; bool pass_trigger200=false; bool pass_trigger260=false;
  bool pass_trigger320=false; bool pass_trigger400=false; bool pass_trigger450=false; bool pass_trigger500=false;
  bool pass_trigger60_HFJEC=false; bool pass_trigger80_HFJEC=false;
  bool pass_trigger100_HFJEC=false; bool pass_trigger160_HFJEC=false;
  bool pass_trigger220_HFJEC=false; bool pass_trigger300_HFJEC=false;
  bool pass_triggerMB=false;
  bool pass_trigger_part0=false; bool pass_trigger_part1=false; bool pass_trigger_part2=false; bool pass_trigger_part3=false; bool pass_trigger_part4=false;
  bool pass_trigger_part5=false; bool pass_trigger_part6=false; bool pass_trigger_part7=false; bool pass_trigger_part8=false; bool pass_trigger_part9=false;

  int trigger40=0; int trigger60=0; int trigger80=0; int trigger140=0; int trigger200=0; int trigger260=0; int trigger320=0; int trigger400=0; int trigger450=0; int trigger500=0;
  int trigger60_HFJEC=0; int trigger80_HFJEC=0; int trigger100_HFJEC=0; int trigger160_HFJEC=0; int trigger220_HFJEC=0; int trigger300_HFJEC=0;

  std::vector<double> trg_thresh, trgHF_thresh;
  if (PtBinsTrigger == "Single") {
    for (int i = 0; i < n_pt_bins_Si; i++) trg_thresh.push_back(pt_bins_Si[i]);
    if (jetLabel == "AK8PUPPI") { trg_thresh.clear(); for (int i = 0; i < n_pt_bins_Si; i++) trg_thresh.push_back(pt_bins_SiAK8PUPPI[i]);}
    for (int i = 0; i < n_pt_bins_Si_HF; i++) trgHF_thresh.push_back(pt_bins_Si_HF[i]);
  } else if (PtBinsTrigger == "DiJet") {
    for (int i = 0; i < n_pt_bins_Di; i++) trg_thresh.push_back(pt_bins_Di[i]);
    for (int i = 0; i < n_pt_bins_Di_HF; i++) trgHF_thresh.push_back(pt_bins_Di_HF[i]);
  } else if (PtBinsTrigger == "ZeroBias") {
    for (int i = 0; i < n_pt_bins_MB; i++) trg_thresh.push_back(pt_bins_MB[i]);
    for (int i = 0; i < n_pt_bins_MB_HF; i++) trgHF_thresh.push_back(pt_bins_MB_HF[i]);
  }

  if(event.isRealData){
    if (debug) std::cout << "entrering trigger" << '\n';

    bool eta_cut_bool = std::max(abs(jet1->eta()),abs(jet2->eta())) <  eta_cut;
    if(!trigger_fwd && trigger_central) {
      eta_cut_bool = true;
      // pass_trigger_hf = false;
    }
    if(!trigger_central && trigger_fwd){
      eta_cut_bool = false;
      // pass_trigger_bl = false;
    }

    if (debug) std::cout << "eta_cut_bool " << eta_cut_bool << "\t" << "eta " << std::max(abs(jet1->eta()),abs(jet2->eta())) << "\t" << "trigger_central " << trigger_central<<  "\t" << "trigger_fwd " << trigger_fwd << "\t" << "PtBinsTrigger " << PtBinsTrigger << '\n';

    if (eta_cut_bool && trigger_central) {
      if (PtBinsTrigger == "Single") {
        pass_trigger40        = (trigger40_sel->passes(event)         && pt_ave>trg_thresh[0]   && pt_ave<trg_thresh[1]);
        pass_trigger60        = (trigger60_sel->passes(event)         && pt_ave>trg_thresh[1]   && pt_ave<trg_thresh[2]);
        pass_trigger80        = (trigger80_sel->passes(event)         && pt_ave>trg_thresh[2]   && pt_ave<trg_thresh[3]);
        pass_trigger140       = (trigger140_sel->passes(event)        && pt_ave>trg_thresh[3]   && pt_ave<trg_thresh[4]);
        pass_trigger200       = (trigger200_sel->passes(event)        && pt_ave>trg_thresh[4]   && pt_ave<trg_thresh[5]);
        pass_trigger260       = (trigger260_sel->passes(event)        && pt_ave>trg_thresh[5]   && pt_ave<trg_thresh[6]);
        pass_trigger320       = (trigger320_sel->passes(event)        && pt_ave>trg_thresh[6]   && pt_ave<trg_thresh[7]);
        pass_trigger400       = (trigger400_sel->passes(event)        && pt_ave>trg_thresh[7]   && pt_ave<trg_thresh[8]);
        pass_trigger450       = (trigger450_sel->passes(event)        && pt_ave>trg_thresh[8]   && pt_ave<trg_thresh[9]);
        pass_trigger500       = (trigger500_sel->passes(event)        && pt_ave>trg_thresh[9]);
        if (isThreshold) {
          pass_trigger40        = trigger40_sel->passes(event);
          pass_trigger60        = trigger60_sel->passes(event);
          pass_trigger80        = trigger80_sel->passes(event);
          pass_trigger140       = trigger140_sel->passes(event);
          pass_trigger200       = trigger200_sel->passes(event);
          pass_trigger260       = trigger260_sel->passes(event);
          pass_trigger320       = trigger320_sel->passes(event);
          pass_trigger400       = trigger400_sel->passes(event);
          pass_trigger450       = trigger450_sel->passes(event);
          pass_trigger500       = trigger500_sel->passes(event);
        }

        if (debug) std::cout << "pt_ave " << pt_ave << "\t" << "pass_trigger40 " << pass_trigger40 << "\t" << "pass_trigger60 " << pass_trigger60  << "\t" << "pass_trigger80 " << pass_trigger80 << "\t" << "pass_trigger140 " << pass_trigger140 << "\t" << "pass_trigger200 " << pass_trigger200 << "\t" << "pass_trigger260 " << pass_trigger260 << "\t" << "pass_trigger320 " << pass_trigger320 << "\t" << "pass_trigger400 " << pass_trigger400 << "\t" << "pass_trigger450 " << pass_trigger450 << "\t" << "pass_trigger500 " << pass_trigger500 << '\n';

        pass_trigger_bl = (pass_trigger40 || pass_trigger60 || pass_trigger80 || pass_trigger140 || pass_trigger200 || pass_trigger260 || pass_trigger320 || pass_trigger400 || pass_trigger450 || pass_trigger500);
      }
      else if (PtBinsTrigger == "DiJet") {
        pass_trigger40        = (trigger40_sel->passes(event)         && pt_ave>trg_thresh[0]   && pt_ave<trg_thresh[1]);
        pass_trigger60        = (trigger60_sel->passes(event)         && pt_ave>trg_thresh[1]   && pt_ave<trg_thresh[2]);
        pass_trigger80        = (trigger80_sel->passes(event)         && pt_ave>trg_thresh[2]   && pt_ave<trg_thresh[3]);
        pass_trigger140       = (trigger140_sel->passes(event)        && pt_ave>trg_thresh[3]   && pt_ave<trg_thresh[4]);
        pass_trigger200       = (trigger200_sel->passes(event)        && pt_ave>trg_thresh[4]   && pt_ave<trg_thresh[5]);
        pass_trigger260       = (trigger260_sel->passes(event)        && pt_ave>trg_thresh[5]   && pt_ave<trg_thresh[6]);
        pass_trigger320       = (trigger320_sel->passes(event)        && pt_ave>trg_thresh[6]   && pt_ave<trg_thresh[7]);
        pass_trigger400       = (trigger400_sel->passes(event)        && pt_ave>trg_thresh[7]   && pt_ave<trg_thresh[8]);
        pass_trigger500       = (trigger500_sel->passes(event)        && pt_ave>trg_thresh[8]);
        if (isThreshold) {
          pass_trigger40        = trigger40_sel->passes(event);
          pass_trigger60        = trigger60_sel->passes(event);
          pass_trigger80        = trigger80_sel->passes(event);
          pass_trigger140       = trigger140_sel->passes(event);
          pass_trigger200       = trigger200_sel->passes(event);
          pass_trigger260       = trigger260_sel->passes(event);
          pass_trigger320       = trigger320_sel->passes(event);
          pass_trigger400       = trigger400_sel->passes(event);
          pass_trigger500       = trigger500_sel->passes(event);
        }

        if (debug) std::cout << "pt_ave " << pt_ave << "\t" << "pass_trigger40 " << pass_trigger40 << "\t" << "pass_trigger60 " << pass_trigger60  << "\t" << "pass_trigger80 " << pass_trigger80 << "\t" << "pass_trigger140 " << pass_trigger140 << "\t" << "pass_trigger200 " << pass_trigger200 << "\t" << "pass_trigger260 " << pass_trigger260 << "\t" << "pass_trigger320 " << pass_trigger320 << "\t" << "pass_trigger400 " << pass_trigger400 << "\t" << "pass_trigger450 " << pass_trigger450 << "\t" << "pass_trigger500 " << pass_trigger500 << '\n';

        pass_trigger_bl = (pass_trigger40 || pass_trigger60 || pass_trigger80 || pass_trigger140 || pass_trigger200 || pass_trigger260 || pass_trigger320 || pass_trigger400 || pass_trigger500);
      }
      else if (PtBinsTrigger == "ZeroBias") {
        if (test_trigger) {
          try {pass_trigger_part0   = triggerMB_part0_sel->passes(event);}
          catch (...){pass_trigger_part0 = false;}
          try {pass_trigger_part1   = triggerMB_part1_sel->passes(event);}
          catch (...){pass_trigger_part1 = false;}
          try {pass_trigger_part2   = triggerMB_part2_sel->passes(event);}
          catch (...){pass_trigger_part2 = false;}
          try {pass_trigger_part3   = triggerMB_part3_sel->passes(event);}
          catch (...){pass_trigger_part3 = false;}
          try {pass_trigger_part4   = triggerMB_part4_sel->passes(event);}
          catch (...){pass_trigger_part4 = false;}
          try {pass_trigger_part5   = triggerMB_part5_sel->passes(event);}
          catch (...){pass_trigger_part5 = false;}
          try {pass_trigger_part6   = triggerMB_part6_sel->passes(event);}
          catch (...){pass_trigger_part6 = false;}
          try {pass_trigger_part7   = triggerMB_part7_sel->passes(event);}
          catch (...){pass_trigger_part7 = false;}
          try {pass_trigger_part8   = triggerMB_part8_sel->passes(event);}
          catch (...){pass_trigger_part8 = false;}
          try {pass_trigger_part9   = triggerMB_part9_sel->passes(event);}
          catch (...){pass_trigger_part9 = false;}
          pass_trigger_bl = (pass_trigger_part0 || pass_trigger_part1 || pass_trigger_part2 || pass_trigger_part3 || pass_trigger_part4 || pass_trigger_part5 || pass_trigger_part6 || pass_trigger_part7 || pass_trigger_part8 || pass_trigger_part9);
        }else {
          pass_triggerMB       = triggerMB_sel->passes(event);
          pass_trigger_bl = pass_triggerMB;
        }
      }
    }
    if (!eta_cut_bool && trigger_fwd ) {
      // std::cout << "ERROR" << '\n';
      if (PtBinsTrigger == "Single") {
        pass_trigger60_HFJEC  = (trigger60_HFJEC_sel->passes(event)   && pt_ave>trgHF_thresh[0] && pt_ave<trgHF_thresh[1]);
        pass_trigger80_HFJEC  = (trigger80_HFJEC_sel->passes(event)   && pt_ave>trgHF_thresh[1] && pt_ave<trgHF_thresh[2]);
        pass_trigger100_HFJEC = (trigger100_HFJEC_sel->passes(event)  && pt_ave>trgHF_thresh[2] && pt_ave<trgHF_thresh[3]);
        pass_trigger160_HFJEC = (trigger160_HFJEC_sel->passes(event)  && pt_ave>trgHF_thresh[3] && pt_ave<trgHF_thresh[4]);
        pass_trigger220_HFJEC = (trigger220_HFJEC_sel->passes(event)  && pt_ave>trgHF_thresh[4] && pt_ave<trgHF_thresh[5]);
        pass_trigger300_HFJEC = (trigger300_HFJEC_sel->passes(event)  && pt_ave>trgHF_thresh[5]);
        if (isThreshold) {
          pass_trigger60_HFJEC  = trigger60_HFJEC_sel->passes(event);
          pass_trigger80_HFJEC  = trigger80_HFJEC_sel->passes(event);
          pass_trigger100_HFJEC = trigger100_HFJEC_sel->passes(event);
          pass_trigger160_HFJEC = trigger160_HFJEC_sel->passes(event);
          pass_trigger220_HFJEC = trigger220_HFJEC_sel->passes(event);
          pass_trigger300_HFJEC = trigger300_HFJEC_sel->passes(event);
        }

        pass_trigger_hf = (pass_trigger60_HFJEC || pass_trigger80_HFJEC || pass_trigger100_HFJEC || pass_trigger160_HFJEC || pass_trigger220_HFJEC || pass_trigger300_HFJEC);
      }
      else if (PtBinsTrigger == "DiJet") {
        pass_trigger60_HFJEC  = (trigger60_HFJEC_sel->passes(event)   && pt_ave>trgHF_thresh[0] && pt_ave<trgHF_thresh[1]);
        pass_trigger80_HFJEC  = (trigger80_HFJEC_sel->passes(event)   && pt_ave>trgHF_thresh[1] && pt_ave<trgHF_thresh[2]);
        pass_trigger100_HFJEC = (trigger100_HFJEC_sel->passes(event)  && pt_ave>trgHF_thresh[2] && pt_ave<trgHF_thresh[3]);
        pass_trigger160_HFJEC = (trigger160_HFJEC_sel->passes(event)  && pt_ave>trgHF_thresh[3] && pt_ave<trgHF_thresh[4]);
        pass_trigger220_HFJEC = (trigger220_HFJEC_sel->passes(event)  && pt_ave>trgHF_thresh[4] && pt_ave<trgHF_thresh[5]);
        pass_trigger300_HFJEC = (trigger300_HFJEC_sel->passes(event)  && pt_ave>trgHF_thresh[5]);
        if (isThreshold) {
          pass_trigger60_HFJEC  = trigger60_HFJEC_sel->passes(event);
          pass_trigger80_HFJEC  = trigger80_HFJEC_sel->passes(event);
          pass_trigger100_HFJEC = trigger100_HFJEC_sel->passes(event);
          pass_trigger160_HFJEC = trigger160_HFJEC_sel->passes(event);
          pass_trigger220_HFJEC = trigger220_HFJEC_sel->passes(event);
          pass_trigger300_HFJEC = trigger300_HFJEC_sel->passes(event);
        }
        pass_trigger_hf = (pass_trigger60_HFJEC || pass_trigger80_HFJEC || pass_trigger100_HFJEC || pass_trigger160_HFJEC || pass_trigger220_HFJEC || pass_trigger300_HFJEC);
      }
      else if (PtBinsTrigger == "ZeroBias") {
        if (test_trigger) {
          try {pass_trigger_part0   = triggerMB_part0_sel->passes(event);}
          catch (...){pass_trigger_part0 = false;}
          try {pass_trigger_part1   = triggerMB_part1_sel->passes(event);}
          catch (...){pass_trigger_part1 = false;}
          try {pass_trigger_part2   = triggerMB_part2_sel->passes(event);}
          catch (...){pass_trigger_part2 = false;}
          try {pass_trigger_part3   = triggerMB_part3_sel->passes(event);}
          catch (...){pass_trigger_part3 = false;}
          try {pass_trigger_part4   = triggerMB_part4_sel->passes(event);}
          catch (...){pass_trigger_part4 = false;}
          try {pass_trigger_part5   = triggerMB_part5_sel->passes(event);}
          catch (...){pass_trigger_part5 = false;}
          try {pass_trigger_part6   = triggerMB_part6_sel->passes(event);}
          catch (...){pass_trigger_part6 = false;}
          try {pass_trigger_part7   = triggerMB_part7_sel->passes(event);}
          catch (...){pass_trigger_part7 = false;}
          try {pass_trigger_part8   = triggerMB_part8_sel->passes(event);}
          catch (...){pass_trigger_part8 = false;}
          try {pass_trigger_part9   = triggerMB_part9_sel->passes(event);}
          catch (...){pass_trigger_part9 = false;}
          pass_trigger_bl = (pass_trigger_part0 || pass_trigger_part1 || pass_trigger_part2 || pass_trigger_part3 || pass_trigger_part4 || pass_trigger_part5 || pass_trigger_part6 || pass_trigger_part7 || pass_trigger_part8 || pass_trigger_part9);
        } else {
          pass_triggerMB       = triggerMB_sel->passes(event);
          pass_trigger_bl = pass_triggerMB;
        }
      }
    }
    if (debug) std::cout << "evaluation of triggers " << pass_trigger_bl << "\t" << pass_trigger_hf << "\t" << (!(pass_trigger_bl || pass_trigger_hf)) << '\n';

    if(!(pass_trigger_bl || pass_trigger_hf)){
      return false;
    }
    if (debug) std::cout << "triggers passed " << '\n';
  }

  int n_trig = 0;
  if(pass_trigger40){ n_trig++; trigger40 = 1;}
  if(pass_trigger60){ n_trig++; trigger60 = 1;}
  if(pass_trigger80){ n_trig++; trigger80 = 1;}
  if(pass_trigger140){ n_trig++; trigger140 = 1;}
  if(pass_trigger200){ n_trig++; trigger200 = 1;}
  if(pass_trigger260){ n_trig++; trigger260 = 1;}
  if(pass_trigger320){ n_trig++; trigger320 = 1;}
  if(pass_trigger400){ n_trig++; trigger400 = 1;}
  if(pass_trigger450){ n_trig++; trigger450 = 1;}
  if(pass_trigger500){ n_trig++; trigger500 = 1;}

  if(pass_trigger60_HFJEC){ n_trig++; trigger60_HFJEC = 1;}
  if(pass_trigger80_HFJEC){ n_trig++; trigger80_HFJEC = 1;}
  if(pass_trigger100_HFJEC){ n_trig++; trigger100_HFJEC = 1;}
  if(pass_trigger160_HFJEC){ n_trig++; trigger160_HFJEC = 1;}
  if(pass_trigger220_HFJEC){ n_trig++; trigger220_HFJEC = 1;}
  if(pass_trigger300_HFJEC){ n_trig++; trigger300_HFJEC = 1;}


  h_posttrigger->fill(event);
  //read or calculated values for dijet events
  float gen_pthat = 0;
  float PU_pt_hat = 0;
  if (isMC) {
    gen_pthat = event.genInfo->qScale(); //pt hat (from QCD simulation) //todo!
    PU_pt_hat = event.genInfo->PU_pT_hat_max();
  }
  //    if(isMC) gen_pthat = event.genInfo->binningValues()[0];
  float gen_weight = 0;
  if(!event.isRealData) gen_weight = event.weight;
  float nvertices = event.pvs->size();
  float nPU = 0 ;//todo for data?
  if(!event.isRealData) nPU = event.genInfo->pileup_TrueNumInteractions();
  float genjet1_pt = 0;
  float genjet2_pt = 0;
  float genjet3_pt = 0;
  float genjet1_eta = 0;
  float genjet2_eta = 0;
  float genjet3_eta = 0;
  float genjet1_phi = 0;
  float genjet2_phi = 0;
  float genjet3_phi = 0;
  if(isMC){
    if(event.genjets->size()>0)genjet1_pt = event.genjets->at(0).pt();
    if(event.genjets->size()>1)genjet2_pt = event.genjets->at(1).pt();
    if(event.genjets->size()>2)genjet3_pt = event.genjets->at(2).pt();
    if(event.genjets->size()>0)genjet1_eta = event.genjets->at(0).eta();
    if(event.genjets->size()>1)genjet2_eta = event.genjets->at(1).eta();
    if(event.genjets->size()>2)genjet3_eta = event.genjets->at(2).eta();
    if(event.genjets->size()>0)genjet1_phi = event.genjets->at(0).phi();
    if(event.genjets->size()>1)genjet2_phi = event.genjets->at(1).phi();
    if(event.genjets->size()>2)genjet3_phi = event.genjets->at(2).phi();
  }
  auto factor_raw1 = jet1->JEC_factor_raw();     auto factor_raw2 = jet2->JEC_factor_raw();
  float jet1_ptRaw = jet1_pt*factor_raw1;  float jet2_ptRaw = jet2_pt*factor_raw2;
  float probejet_eta = jet_probe->eta();
  float  probejet_phi = jet_probe->phi();
  float  probejet_pt = jet_probe->pt();
  auto factor_raw_probe = jet_probe->JEC_factor_raw();
  float probejet_ptRaw = probejet_pt*factor_raw_probe;
  float barreljet_eta = jet_barrel->eta();
  float  barreljet_phi = jet_barrel->phi();
  float  barreljet_pt = jet_barrel->pt();
  auto factor_raw_barrel = jet_barrel->JEC_factor_raw();
  float barreljet_ptRaw = barreljet_pt*factor_raw_barrel;
  float jet3_pt = 0; float jet3_ptRaw = 0;
  float jet3_eta = 0; float jet3_phi = 0;
  Jet* jet3;
  TVector3 jet1_2Dv, jet2_2Dv, jet3_2Dv;

  jet1_2Dv.SetPtEtaPhi( jet1->pt(), 0.0, jet1->phi());
  jet2_2Dv.SetPtEtaPhi( jet2->pt(), 0.0, jet2->phi());

  float alpha_p = 0;

  if(jet_n>2){
    jet3 = &ak4jets->at(2);
    jet3_pt = jet3->pt();
    jet3_eta = jet3->eta();
    jet3_phi = jet3->phi();
    auto factor_raw3 = jet3->JEC_factor_raw();
    jet3_ptRaw = jet3_pt*factor_raw3;
    jet3_2Dv.SetPtEtaPhi( jet3->pt(), 0.0, jet3->phi());

    alpha_p = fabs(((jet1_2Dv - jet2_2Dv)*jet3_2Dv)/(pt_ave*(jet1_2Dv - jet2_2Dv).Pt()));
  }
  float alpha = jet3_pt/pt_ave;
  //std::cout << jet3_pt << " = " << jet3tlv.Pt() << std::endl;
  //std::cout << "alpha, alpha_p = " << alpha << ", " << alpha_p << std::endl;
  float asymmetry = (barreljet_pt - probejet_pt)/(barreljet_pt + probejet_pt);
  float rel_r = (1+asymmetry)/(1-asymmetry);
  TVector2 pt, met;
  met.Set(event.met->pt() * cos(event.met->phi()),event.met->pt() * sin(event.met->phi()));
  pt.Set(barreljet_pt * cos(barreljet_phi),barreljet_pt* sin(barreljet_phi));
  float mpf_r = 1 + (met.Px()*pt.Px() + met.Py()*pt.Py())/(pt.Px()*pt.Px() + pt.Py()*pt.Py());
  float B = (met.Px()*pt.Px() + met.Py()*pt.Py())/((probejet_pt + barreljet_pt) * sqrt(pt.Px()*pt.Px() + pt.Py()*pt.Py())); //vec_MET*vec_ptbarr/(2ptave*ptbarr)
  float jets_pt = 0;
  for(int i=2;i<jet_n;i++){
    jets_pt += ((Jet*)&ak4jets->at(i))->pt();
  }
  // int flavor = 0;
  //separate flat and fwd samples at |eta| = 2.853
  // if(dataset_version.Contains("_Fwd") && fabs(probejet_eta) < eta_cut && isMC) return false;
  // if((dataset_version.Contains("_Flat")) && fabs(probejet_eta) >= eta_cut && isMC) return false;

  double had_n_Efrac = ak4jets->at(0).neutralHadronEnergyFraction();
  double had_ch_Efrac = ak4jets->at(0).chargedHadronEnergyFraction();
  double mu_Efrac = ak4jets->at(0).muonEnergyFraction();
  double ph_Efrac = ak4jets->at(0).photonEnergyFraction();

  //fill the containers
  event.set(tt_hf_trigger,pass_trigger_hf);
  event.set(tt_bl_trigger,pass_trigger_bl);
  event.set(tt_gen_pthat,gen_pthat);
  event.set(tt_PU_pt_hat, PU_pt_hat);
  event.set(tt_gen_weight,gen_weight);
  event.set(tt_jet1_pt,jet1_pt);
  event.set(tt_jet2_pt,jet2_pt);
  event.set(tt_jet3_pt,jet3_pt);
  event.set(tt_jet1_eta,jet1_eta);
  event.set(tt_jet2_eta,jet2_eta);
  event.set(tt_jet3_eta,jet3_eta);
  event.set(tt_jet1_phi,jet1_phi);
  event.set(tt_jet2_phi,jet2_phi);
  event.set(tt_jet3_phi,jet3_phi);
  event.set(tt_jet1_ptRaw,jet1_ptRaw);
  event.set(tt_jet2_ptRaw,jet2_ptRaw);
  event.set(tt_jet3_ptRaw,jet3_ptRaw);
  event.set(tt_jet1_ptGen,genjet1_pt);
  event.set(tt_jet2_ptGen,genjet2_pt);
  event.set(tt_jet3_ptGen,genjet3_pt);
  event.set(tt_jet1_etaGen,genjet1_eta);
  event.set(tt_jet2_etaGen,genjet2_eta);
  event.set(tt_jet3_etaGen,genjet3_eta);
  event.set(tt_jet1_phiGen,genjet1_phi);
  event.set(tt_jet2_phiGen,genjet2_phi);
  event.set(tt_jet3_phiGen,genjet3_phi);
  event.set(tt_nvertices,nvertices);
  event.set(tt_probejet_eta,probejet_eta);
  event.set(tt_probejet_phi,probejet_phi);
  event.set(tt_probejet_pt,probejet_pt);
  event.set(tt_probejet_ptRaw,probejet_ptRaw);
  event.set(tt_barreljet_eta,barreljet_eta);
  event.set(tt_barreljet_phi,barreljet_phi);
  event.set(tt_barreljet_pt,barreljet_pt);
  event.set(tt_barreljet_ptRaw,barreljet_ptRaw);
  event.set(tt_pt_ave,pt_ave);
  event.set(tt_alpha,alpha);
  event.set(tt_alpha_p,alpha_p);
  event.set(tt_asymmetry,asymmetry);
  event.set(tt_B,B);
  event.set(tt_MET,event.met->pt());
  event.set(tt_rel_r,rel_r);
  event.set(tt_mpf_r,mpf_r);
  event.set(tt_nPU,nPU);
  event.set(tt_matchJetId_0,-10.);
  event.set(tt_matchJetId_1,-10.);
  event.set(tt_ev_weight,event.weight);
  event.set(tt_jets_pt,jets_pt);
  event.set(tt_jet_n,jet_n);
  event.set(tt_rho,event.rho);
  // event.set(tt_partonFlavor,flavor);
  event.set(tt_had_n_Efrac,had_n_Efrac);
  event.set(tt_had_ch_Efrac,had_ch_Efrac);
  event.set(tt_mu_Efrac,mu_Efrac);
  event.set(tt_ph_Efrac,ph_Efrac);
  event.set(tt_inst_lumi,inst_lumi);
  event.set(tt_integrated_lumi_in_bin,fill_event_integrated_lumi);
  event.set(tt_lumibin,event_in_lumibin);
  event.set(tt_integrated_lumi,int_lumi_event);

  event.set(tt_trigger40 , trigger40);
  event.set(tt_trigger60 , trigger60);
  event.set(tt_trigger80 , trigger80);
  event.set(tt_trigger140, trigger140);
  event.set(tt_trigger200, trigger200);
  event.set(tt_trigger260, trigger260);
  event.set(tt_trigger320, trigger320);
  event.set(tt_trigger400, trigger400);
  event.set(tt_trigger450, trigger450);
  event.set(tt_trigger500, trigger500);
  event.set(tt_trigger60_HFJEC, trigger60_HFJEC);
  event.set(tt_trigger80_HFJEC, trigger80_HFJEC);
  event.set(tt_trigger100_HFJEC, trigger100_HFJEC);
  event.set(tt_trigger160_HFJEC, trigger160_HFJEC);
  event.set(tt_trigger220_HFJEC, trigger220_HFJEC);
  event.set(tt_trigger300_HFJEC, trigger300_HFJEC);

  // Geninfo for MC
  // Start MC
  if(isMC){
    const int genjet_n = event.genjets->size();
    sort_by_pt<GenJet>(*event.genjets);
    if (debug) std::cout << "genjet_n " << genjet_n << '\n';
    if(genjet_n<2) return false; // Not sure about this
    // define barrel and probe genjets
    GenJet* genjet1 = &event.genjets->at(0);// leading genjet
    GenJet* genjet2 = &event.genjets->at(1);// sub-leading genjet
    float genjet1_pt = genjet1->pt(); float genjet2_pt = genjet2->pt();
    bool no_mc_spikes = (genjet1_pt < 1.5*gen_pthat || jet1_pt < 1.5*genjet1_pt); //(pTgen1 < 1.5*gen_pthat || pTreco1 < 1.5* pTgen1) // pthat cut from mikko
    float gen_pt_ave = (genjet1_pt + genjet2_pt)/2.;
    GenJet* genjet_barrel = genjet1; GenJet* genjet_probe = genjet2;
    // if ((fabs(genjet1->eta())<s_eta_barr)&&(fabs(genjet2->eta())<s_eta_barr)) {
    //   int ran = rand();
    //   int numb = ran % 2 + 1;
    //   if(numb==1){
    //     genjet_probe = genjet2;
    //     genjet_barrel = genjet1;
    //   }
    //   if(numb==2){
    //     genjet_probe = genjet1;
    //     genjet_barrel = genjet2;
    //   }
    // }
    // else if ((fabs(genjet1->eta())<s_eta_barr)||(fabs(genjet2->eta())<s_eta_barr)){
    //   if(fabs(genjet1->eta())<s_eta_barr){
    //     genjet_probe = genjet2;
    //     genjet_barrel = genjet1;
    //   }
    //   else{
    //     genjet_probe = genjet1;
    //     genjet_barrel = genjet2;
    //   }
    // }
    float probegenjet_eta = genjet_probe->eta();
    float  probegenjet_phi = genjet_probe->phi();
    float  probegenjet_pt = genjet_probe->pt();
    float barrelgenjet_eta = genjet_barrel->eta();
    float  barrelgenjet_phi = genjet_barrel->phi();
    float  barrelgenjet_pt = genjet_barrel->pt();

    TVector3 genjet1_2Dv, genjet2_2Dv, genjet3_2Dv;

    genjet1_2Dv.SetPtEtaPhi( genjet1->pt(), 0.0, genjet1->phi());
    genjet2_2Dv.SetPtEtaPhi( genjet2->pt(), 0.0, genjet2->phi());

    float gen_alpha_p = 0;

    float genjet3_pt = 0;// float genjet3_ptRaw = 0;
    if(genjet_n>2){
      Particle* genjet3 = &event.genjets->at(2);
      // For gengenjets or Gengenjets or so
      // genjet* genjet3 = &event.gengenjets->at(2);
      genjet3_pt = genjet3->pt();
      genjet3_2Dv.SetPtEtaPhi( genjet3->pt(), 0.0, genjet3->phi());

      gen_alpha_p = fabs(((genjet1_2Dv - genjet2_2Dv)*genjet3_2Dv)/(pt_ave*(genjet1_2Dv - genjet2_2Dv).Pt()));
    } else {
      genjet3_pt = 0;
    }
    //cout << "Gen pts: " << genjet1_pt << ", " << genjet2_pt << " and " << genjet3_pt << endl;
    //cout << "pts: " << jet1_pt << ", " << jet2_pt << " and " << jet3_pt << endl;
    //cout << "raw pts: " << jet1_ptRaw << ", " << jet2_ptRaw << ", " << " and " << jet3_ptRaw << endl;
    float gen_alpha = genjet3_pt/pt_ave;
    float gen_asymmetry = (barrelgenjet_pt - probegenjet_pt)/(barrelgenjet_pt + probegenjet_pt);
    //std::cout << "gen_alpha, gen_alpha_p = " << gen_alpha << ", " << gen_alpha_p << std::endl;
    //cout << " gen_alpha " << gen_alpha << " and gen_asym " << gen_asymmetry << endl;
    //cout << " alpha " << alpha << " and asym " << asymmetry << endl;

    event.set(tt_genjet1_pt,genjet1_pt);
    event.set(tt_genjet2_pt,genjet2_pt);
    event.set(tt_genjet3_pt,genjet3_pt);
    event.set(tt_genjet1_eta,genjet1_eta);
    event.set(tt_genjet2_eta,genjet2_eta);
    event.set(tt_genjet3_eta,genjet3_eta);
    event.set(tt_genjet1_phi,genjet1_phi);
    event.set(tt_genjet2_phi,genjet2_phi);
    event.set(tt_genjet3_phi,genjet3_phi);
    //event.set(tt_genjet1_ptRaw,genjet1_ptRaw);
    //event.set(tt_genjet2_ptRaw,genjet2_ptRaw);
    //event.set(tt_genjet3_ptRaw,genjet3_ptRaw);
    event.set(tt_nvertices,nvertices);
    event.set(tt_genjet_n,genjet_n);
    event.set(tt_no_mc_spikes, no_mc_spikes);
    event.set(tt_probegenjet_eta,probegenjet_eta);
    event.set(tt_probegenjet_phi,probegenjet_phi);
    event.set(tt_probegenjet_pt,probegenjet_pt);
    //event.set(tt_probegenjet_ptRaw,probegenjet_ptRaw);
    event.set(tt_barrelgenjet_eta,barrelgenjet_eta);
    event.set(tt_barrelgenjet_phi,barrelgenjet_phi);
    event.set(tt_barrelgenjet_pt,barrelgenjet_pt);
    //event.set(tt_barrelgenjet_ptRaw,barrelgenjet_ptRaw);
    event.set(tt_gen_pt_ave,gen_pt_ave);
    event.set(tt_gen_alpha,gen_alpha);
    event.set(tt_gen_alpha_p,gen_alpha_p);
    event.set(tt_gen_asymmetry,gen_asymmetry);
  } else {
    // End MC
    event.set(tt_genjet1_pt,0);
    event.set(tt_genjet2_pt,0);
    event.set(tt_genjet3_pt,0);
    event.set(tt_genjet1_eta,0);
    event.set(tt_genjet2_eta,0);
    event.set(tt_genjet3_eta,0);
    event.set(tt_genjet1_phi,0);
    event.set(tt_genjet2_phi,0);
    event.set(tt_genjet3_phi,0);

    //event.set(tt_genjet1_ptRaw,genjet1_ptRaw);
    //event.set(tt_genjet2_ptRaw,genjet2_ptRaw);
    //event.set(tt_genjet3_ptRaw,genjet3_ptRaw);
    event.set(tt_nvertices,0);
    event.set(tt_genjet_n,0);
    event.set(tt_no_mc_spikes,0);
    event.set(tt_probegenjet_eta,0);
    event.set(tt_probegenjet_phi,0);
    event.set(tt_probegenjet_pt,0);
    //event.set(tt_probegenjet_ptRaw,probegenjet_ptRaw);
    event.set(tt_barrelgenjet_eta,0);
    event.set(tt_barrelgenjet_phi,0);
    event.set(tt_barrelgenjet_pt,0);
    //event.set(tt_barrelgenjet_ptRaw,barrelgenjet_ptRaw);
    event.set(tt_gen_pt_ave,0);
    event.set(tt_gen_alpha,0);
    event.set(tt_gen_alpha_p,0);
    event.set(tt_gen_asymmetry,0);
  }

  sel->SetEvent(event);

  //good primary vertex
  // int nGoodVts = sel.goodPVertex();
  h_precleaning->fill(event);
  if(isMC) {
    //cout << "Checking PUpthat..." << endl;

    if(!event.isRealData){

      if(!sel->PUpthat(event)) return false;
    }

    // cout << "Passed PUpthat";
    if(debug){
      cout << "debug is: " << debug << endl;
      cout << "before good vertex selection : " << endl;
      cout << " Evt# "<<event.event<<" Run: "<<event.run<<" " << endl;
    }
  } else {
    //cout << "dataset does not contain PUpthat or QCD_Pt -- not applying pt hat cut" << endl;
    //cout << "dataset version:" << dataset_version << endl;
  }
  // if(nGoodVts<=0) return false;
  // event.set(tt_nGoodvertices, nGoodVts);
  if(debug){
    cout << "before 'dijet selection' : " << endl;
    cout << " Evt# "<<event.event<<" Run: "<<event.run<<" " << endl;
  }

  if(!sel->DiJet()) return false;

  h_cleaned->fill(event);
  h_lumi_nocuts->fill(event);
  if(debug){
    cout << "before 'dijet advanced selection' : " << endl;
    cout << " Evt# "<<event.event<<" Run: "<<event.run<<" " << endl;
  }
  // Selection from Jens and or Mikko to filter hot jet regions from ECAL

  //PhiEta Region cleaning

  if(apply_EtaPhi_cut && !sel->ApplyHotMap(event)) return false;

  if(apply_EtaPhi_HCAL && !sel->EtaPhiCleaning(event)) return false;

  if(false){
    cout << "after Event not in a hot region:" << endl;
    cout << "after jet1 eta: " << jet1->eta() << endl;
    cout << "after jet2 eta: " << jet2->eta() << endl;
    cout << "after jet1 phi: " << jet1->phi() << endl;
    cout << "after jet2 phi:" << jet2->phi() << endl;
  }

  if(!sel->DiJetAdvanced(event)) return false;

  h_dijet->fill(event);
  h_lumi_dijet->fill(event);
  h_match->fill(event);
  h_lumi_match->fill(event);
  if(event.isRealData){
    if(pass_trigger40) {h_trg40->fill(event); h_lumi_Trig40->fill(event);}
    if(pass_trigger60) {h_trg60->fill(event); h_lumi_Trig60->fill(event);}
    if(pass_trigger80) {h_trg80->fill(event); h_lumi_Trig80->fill(event);}
    if(pass_trigger140) {h_trg140->fill(event); h_lumi_Trig140->fill(event);}
    if(pass_trigger200) {h_trg200->fill(event); h_lumi_Trig200->fill(event);}
    if(pass_trigger260) {h_trg260->fill(event); h_lumi_Trig260->fill(event);}
    if(pass_trigger320) {h_trg320->fill(event); h_lumi_Trig320->fill(event);}
    if(pass_trigger400) {h_trg400->fill(event); h_lumi_Trig400->fill(event);}
    if(pass_trigger450) {h_trg450->fill(event); h_lumi_Trig450->fill(event);}
    if(pass_trigger500) {h_trg500->fill(event); h_lumi_Trig500->fill(event);}
    if(pass_trigger60_HFJEC) {h_trgHF60->fill(event); h_lumi_TrigHF60->fill(event);}
    if(pass_trigger80_HFJEC) {h_trgHF80->fill(event); h_lumi_TrigHF80->fill(event);}
    if(pass_trigger100_HFJEC) {h_trgHF100->fill(event); h_lumi_TrigHF100->fill(event);}
    if(pass_trigger160_HFJEC) {h_trgHF160->fill(event); h_lumi_TrigHF160->fill(event);}
    if(pass_trigger220_HFJEC) {h_trgHF220->fill(event); h_lumi_TrigHF220->fill(event);}
    if(pass_trigger300_HFJEC) {h_trgHF300->fill(event); h_lumi_TrigHF300->fill(event);}
  }
  else{
    if(debug){
      //cout << "before Pt selection (MC only) : ";
      //cout << " Evt# "<<event.event<<" Run: "<<event.run<<" " << endl;
    }
    //   //if(!sel.PtMC(event)) return false; // For MC only one Pt threshold
    //cout << "after Pt selection (MC only) : ";
    //cout << " Evt# "<<event.event<<" Run: "<<event.run<<" " << endl;
  }



  //######################################################################################################################################

  // // L1 jet seed cleaning
  // if(apply_L1seed_from_bx1_filter){
  //   if(debug) cout << "before the L1 seed filter" << endl;
  //   if(!sel.L1JetBXcleanSmart()){
  //     if(debug) cout << "L1 seed filtered" << endl;
  //     return false;
  //   }
  //   if(debug) cout << "after the first L1 seed filter" << endl;
  // }
  //
  // //get the corresponding L1 Jet and save the bx
  //
  // if(debug) cout << "declared L1Jet seeds" << endl;
  //
  // int jet1_l1bx, jet2_l1bx, jet3_l1bx;
  //
  // unsigned int n_l1jets =l1jets->size();
  // if(debug) cout << "l1jets size is "<<n_l1jets<<endl;
  // if(n_l1jets>0){
  //   double dRmin = 100.;
  //   int dRmin_seed_idx = -1;
  //   float dR;
  //   if(debug) cout << "before first L1Jet seeds dR loop" << endl;
  //   for(unsigned int i = 0; i<n_l1jets; i++){
  //     dR=uhh2::deltaR(l1jets->at(i),event.jets->at(0));
  //
  //     if(dR < dRmin){
  //       dRmin=dR;
  //       dRmin_seed_idx = i;
  //     }
  //   }
  //   if( ( l1jets->at(dRmin_seed_idx).pt() / event.jets->at(0).pt() ) < 0.2 ) jet1_l1bx = -10;
  //   else jet1_l1bx = l1jets->at(dRmin_seed_idx).bx();
  // }
  // else jet1_l1bx = 10;
  //
  // if(n_l1jets>1){
  //   double dRmin = 100.;
  //   int dRmin_seed_idx = -1;
  //   float dR;
  //   for(unsigned int i = 0; i<n_l1jets; i++){
  //     dR=uhh2::deltaR(l1jets->at(i),event.jets->at(1));
  //
  //     if(dR < dRmin){
  //       dRmin=dR;
  //       dRmin_seed_idx = i;
  //     }
  //   }
  //   if( ( l1jets->at(dRmin_seed_idx).pt() / event.jets->at(0).pt() ) < 0.2 ) jet2_l1bx = -10;
  //   else jet2_l1bx = l1jets->at(dRmin_seed_idx).bx();
  // }
  // else jet2_l1bx = 10;
  //
  // if(event.jets->size()>2){
  //   if(n_l1jets>2){
  //     double dRmin = 100.;
  //     int dRmin_seed_idx = -1;
  //     float dR;
  //     for(unsigned int i = 0; i<n_l1jets; i++){
  //       dR=uhh2::deltaR(l1jets->at(i),event.jets->at(2));
  //
  //       if(dR < dRmin){
  //         dRmin=dR;
  //         dRmin_seed_idx = i;
  //       }
  //     }
  //     if( ( l1jets->at(dRmin_seed_idx).pt() / event.jets->at(0).pt() ) < 0.2 ) jet3_l1bx = -10;
  //     else jet3_l1bx = l1jets->at(dRmin_seed_idx).bx();
  //   }
  //   else jet3_l1bx = 10;
  // }
  // else jet3_l1bx = 10;
  //
  // event.set(tt_jet1_l1bx,jet1_l1bx);
  // event.set(tt_jet2_l1bx,jet2_l1bx);
  // event.set(tt_jet3_l1bx,jet3_l1bx);

  //###############################################################################################

  if (event.get(tt_alpha) < 0.3) {
    h_sel->fill(event);
    h_lumi_sel->fill(event);
  }

  h_final->fill(event);
  h_lumi_final->fill(event);

  if(debug){
    cout<<"-- Event -- "<<endl;
    cout<<" Evt# "<<event.event<<" Run: "<<event.run<<" "<<endl;
    cout<<" Npv = "<<event.get(tt_nvertices)<<" jet_pt_ave = "<<event.get(tt_pt_ave)<<" MET = "<<met.Mod()<<endl;
    cout<<"Probe: "<<event.get(tt_probejet_eta)<<" "<<event.get(tt_probejet_phi)
    <<" "<<event.get(tt_probejet_pt)<<" "<<event.get(tt_probejet_ptRaw)<<endl;
    cout<<" Barrel: "<<event.get(tt_barreljet_eta)<<" "<<event.get(tt_barreljet_phi)
    <<" "<<event.get(tt_barreljet_pt)<<" "<<event.get(tt_barreljet_ptRaw)<<endl;
    cout<<" "<<event.get(tt_asymmetry)<<" "<<event.get(tt_rel_r)<<" "<<event.get(tt_mpf_r)<<""<<endl;
    cout<<" "<<endl;
  }

  if(debug && isMC){
    Jet_printer->process(event);
    GenParticles_printer->process(event);
    cout << "event has " << event.genjets->size() << " GenJets" << endl;
    for(size_t i=0; i< event.genjets->size(); ++i){
      const auto & jet = (*event.genjets)[i];
      cout << " GenJet[" << i << "]: pt=" << jet.pt() << "; eta=" << jet.eta() << "; phi=" << jet.phi() <<  endl;
    }
  }

  if(isMC){
    double flavor_barreljet = 0;
    double flavor_probejet = 0;
    double flavor_leadingjet = 0;
    double flavor_subleadingjet = 0;
    const unsigned int genjets_n = event.genjets->size();
    int idx_jet_matching_genjet[genjets_n];

    //match genp to gen-jets
    int idx_j=0;
    int idx_genp_min = -1;
    //this array contains one idx for each jet in the event. If -1: unmatched, else: idx of the closest genpart with dR<=0.2
    int idx_matched_jets[jet_n];
    for(int i=0; i<jet_n; i++){
      idx_matched_jets[i] = -1;
    }

    //matching gen- and reco-jets
    for(unsigned int i=0; i<event.genjets->size(); i++){
      double dR_min = 99999; int idx_matching_jet = -1;
      for(unsigned int j=0; j<ak4jets->size(); j++){
        double dR = deltaR(ak4jets->at(j), event.genjets->at(i));
        if(debug) cout << "dR between GenJet " << i << " and RecoJet " << j << ": " << dR << endl;
        if(dR<dR_min){
          dR_min = dR;
          if(dR_min<0.1) idx_matching_jet = j;
        }
      }
      idx_jet_matching_genjet[i] = idx_matching_jet;
      if(debug) cout << "the jet matching the genjet no. " << i << " is jet no. " << idx_matching_jet << endl;
    }
    /////////////////////

    for(Particle & genj : *event.genjets){
      double dr_min = 99999;
      double dr_cut = 0;
      if(jetLabel == "AK4CHS" || jetLabel == "AK4PUPPI") dr_cut = 0.2;
      else if (jetLabel == "AK8CHS" || jetLabel == "AK8PUPPI")dr_cut = 0.4;
      else throw runtime_error("JERSFModuleTest.cxx: Invalid jet-label specified.");
      int idx_g = 0;
      for(GenParticle & genp: *event.genparticles){
        double dr = deltaR(genj,genp);
        if(dr < dr_min){
          dr_min = dr;
          idx_genp_min = idx_g;
        }
        //cout << "dr between genjet " << idx_j << " and genp (flavor: " << genp.partonFlavour() << ") " << idx_g << "= " << dr << endl;
        idx_g++;
      }
      if(dr_min <= dr_cut) {
        if(debug) cout << "genjet " << idx_j << " is matched to genparticle " << idx_genp_min << " of flavor " << event.genparticles->at(idx_genp_min).partonFlavour() << " within dR = " << dr_min << ". " <<  endl;
        if(idx_jet_matching_genjet[idx_j] >= 0) idx_matched_jets[idx_jet_matching_genjet[idx_j]] = idx_genp_min;
      }
      idx_j++;
    }
    //only consider jets that could be matched to a genparticle, these shall take the partons flavor by definition
    //TEST
    if(debug){
      for (int i=0; i<jet_n; i++){
        if(idx_matched_jets[i] != -1) cout << "Jet no. " << i << " is matching genpart no. " << idx_matched_jets[i] << endl;
      }
    }

    // flavor-quantities

    if(debug && event.genjets->size() <2) cout << "WARNING: GENjets size < 2" << endl;

    //only consider the barreljet, is it leading or sub-leading jet?
    int idx_barreljet = -1;
    if(fabs(jet1->pt() - jet_barrel->pt()) < 0.001) idx_barreljet = 0;
    else if (fabs(jet2->pt() - jet_barrel->pt()) < 0.001) idx_barreljet = 1;
    else throw runtime_error("first two jets are not the barrel jets, how could this happen?");
    //obtain flavor of the barreljet
    //-1: unmatched, 0: alpha too large, >0: flavor of matching genparticle
    if(idx_matched_jets[idx_barreljet] != -1)	flavor_barreljet = fabs(event.genparticles->at(idx_matched_jets[idx_barreljet]).partonFlavour());
    else flavor_barreljet = -1;
    if(debug) cout << "barreljet is jet no. " << idx_barreljet << ", alpha = " << event.get(tt_alpha) << ", flavor of barreljet = " << flavor_barreljet << endl;


    //also for probe jets
    int idx_probejet = fabs(idx_barreljet - 1);
    //obtain flavor of the probejet
    //-1: unmatched,  >0: flavor of matching genparticle
    if(idx_matched_jets[idx_probejet] != -1) flavor_probejet = fabs(event.genparticles->at(idx_matched_jets[idx_probejet]).partonFlavour());
    else flavor_probejet = -1;
    if(debug) cout << "probejet is jet no. " << idx_probejet << ", alpha = " << event.get(tt_alpha) << ", flavor of probejet = " << flavor_probejet << endl;

    //same for leading jet
    //-1: unmatched, 0: alpha too large, >0: flavor of matching genparticle
    if(idx_matched_jets[0] != -1) flavor_leadingjet = fabs(event.genparticles->at(idx_matched_jets[0]).partonFlavour());
    else flavor_leadingjet = -1;
    if(debug) cout << "leadingjet is jet no. " << 0 << ", alpha = " << event.get(tt_alpha) << ", flavor of leadingjet = " << flavor_leadingjet << endl;


    //same for subleading jet
    //-1: unmatched, 0: alpha too large, >0: flavor of matching genparticle
    if(idx_matched_jets[1] != -1) flavor_subleadingjet = fabs(event.genparticles->at(idx_matched_jets[1]).partonFlavour());
    else flavor_subleadingjet = -1;
    if(debug) cout << "subleadingjet is jet no. " << 1 << ", alpha = " << event.get(tt_alpha) << ", flavor of subleadingjet = " << flavor_subleadingjet << endl;

    event.set(tt_flavorBarreljet,flavor_barreljet);
    event.set(tt_flavorProbejet,flavor_probejet);
    event.set(tt_flavorLeadingjet,flavor_leadingjet);
    event.set(tt_flavorSubleadingjet,flavor_subleadingjet);

    //response of leading jet
    //find corresponding genjet
    int idx_corresponding_genjet = -1;
    for(unsigned int i=0; i<event.genjets->size(); i++){
      if(debug) cout << idx_jet_matching_genjet[i] << endl;
      if(idx_jet_matching_genjet[i] == 0) idx_corresponding_genjet = i;
    }
    double response_jet1 = -1;
    if(idx_corresponding_genjet != -1) response_jet1 = ak4jets->at(0).pt() / event.genjets->at(idx_corresponding_genjet).pt();

    event.set(tt_response_leadingjet,response_jet1);

  } //isMC

  else{
    event.set(tt_flavorBarreljet,-1);
    event.set(tt_flavorProbejet,-1);
    event.set(tt_flavorLeadingjet,-1);
    event.set(tt_flavorSubleadingjet,-1);
    event.set(tt_response_leadingjet,-1.);
  }

  if (debug) std::cout << "END reached" << '\n';

  return true;
}



// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
// make sure the ExampleModule is found by class name. This is ensured by this macro:
UHH2_REGISTER_ANALYSIS_MODULE(JERSFModuleTest)
