//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 13 16:05:52 2015 by ROOT version 5.34/32
// from TTree t/t
// found on file: /pnfs/desy.de/cms/tier2/store/user/mniedzie/Spring15_QCD_Pt-binned/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/JERmcSpring15/150810_142114/0000/output_mc_1.root
//////////////////////////////////////////////////////////

#ifndef MySelector_h
#define MySelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TMath.h>
#include <TDirectory.h>
#include <vector>


double Weight( TString filename );

class MySelector : public TSelector {
public:

  TString outdir;

  TTree *fChain;   //!pointer to the analyzed TTree or TChain
  // Declaration of leaf types
  Float_t         weight;
  Float_t         weight_pu;
  Float_t         weight_pu_down;
  Float_t         weight_pu_up;
  Float_t         pthat;
  Int_t           npuIT;
  Int_t njet;
  Float_t pt_ave;
  Float_t jet1_pt;
  Float_t jet2_pt;
  Float_t jet3_pt;
  Float_t jet1_eta;
  Float_t jet2_eta;
  Float_t jet3_eta;
  Float_t jet1_phi;
  Float_t jet2_phi;
  Float_t jet3_phi;
  Float_t barreljet_phi;
  Float_t barreljet_eta;
  Float_t barreljet_pt;
  Float_t probejet_phi;
  Float_t probejet_eta;
  Float_t probejet_pt;
  Float_t asymmetry;
  Float_t rho;
  Float_t alpha_;
  TBranch *b_njet;
  TBranch *b_pt_ave;
  TBranch *b_jet1_pt;
  TBranch *b_jet2_pt;
  TBranch *b_jet3_pt;
  TBranch *b_jet1_eta;
  TBranch *b_jet2_eta;
  TBranch *b_jet3_eta;
  TBranch *b_jet1_phi;
  TBranch *b_jet2_phi;
  TBranch *b_jet3_phi;
  TBranch *b_probejet_phi;
  TBranch *b_probejet_eta;
  TBranch *b_probejet_pt;
  TBranch *b_barreljet_phi;
  TBranch *b_barreljet_eta;
  TBranch *b_barreljet_pt;
  TBranch *b_asymmetry;
  TBranch *b_alpha;
  TBranch *b_rho;

  Int_t ngenjet;
  Float_t gen_pt_ave;
  Float_t genjet1_pt;
  Float_t genjet2_pt;
  Float_t genjet3_pt;
  Float_t genjet3_eta;
  Float_t genjet3_phi;
  Float_t barrelgenjet_phi;
  Float_t barrelgenjet_eta;
  Float_t barrelgenjet_pt;
  Float_t probegenjet_phi;
  Float_t probegenjet_eta;
  Float_t probegenjet_pt;
  Float_t gen_asymmetry;
  Float_t gen_alpha_;
  TBranch *b_ngenjet;
  TBranch *b_gen_pt_ave;
  TBranch *b_genjet1_pt;
  TBranch *b_genjet2_pt;
  TBranch *b_genjet3_pt;
  TBranch *b_genjet3_eta;
  TBranch *b_genjet3_phi;
  TBranch *b_probegenjet_phi;
  TBranch *b_probegenjet_eta;
  TBranch *b_probegenjet_pt;
  TBranch *b_barrelgenjet_phi;
  TBranch *b_barrelgenjet_eta;
  TBranch *b_barrelgenjet_pt;
  TBranch *b_gen_asymmetry;
  TBranch *b_gen_alpha;

  // List of branches
  TBranch        *b_weight;   //!
  TBranch        *b_weight_pu;   //!
  TBranch        *b_weight_pu_down;   //!
  TBranch        *b_weight_pu_up;   //!
  TBranch        *b_pthat;   //!
  TBranch        *b_npuIT;   //!

  MySelector(TString name, TTree * /*tree*/ =0) : fChain(0), outdir(name) { }
  virtual ~MySelector() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) {
    int treeentry = entry;
    return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
  }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

  void BuildEvent();
  void MakeWeight();

  int TotalEvents, unmachedJets, unmatchegGenJets;

  int EtaBins_SM, EtaBins_SM_control, EtaBins_FE_reference, EtaBins_FE_control, EtaBins_FE;
  int etaShift_SM, etaShift_SM_control, etaShift_FE_reference, etaShift_FE_control, etaShift_FE;
  int PtBins_Central, PtBins_HF;
  int AlphaBins;

  TString whichRun;

  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_SM, 						asymmetries_pt_SM,						asymmetries_rho_SM,						asymmetries_pt3_SM;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_SM_control, 		asymmetries_pt_SM_control,		asymmetries_rho_SM_control,		asymmetries_pt3_SM_control;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_FE_reference, 	asymmetries_pt_FE_reference,	asymmetries_rho_FE_reference,	asymmetries_pt3_FE_reference;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_FE_control, 		asymmetries_pt_FE_control,		asymmetries_rho_FE_control,		asymmetries_pt3_FE_control;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_FE, 						asymmetries_pt_FE,						asymmetries_rho_FE,						asymmetries_pt3_FE;


  std::vector< std::vector< std::vector< TH1F* > > > gen_asymmetries_SM, 						gen_asymmetries_pt_SM,						gen_asymmetries_rho_SM,						gen_asymmetries_pt3_SM;
  std::vector< std::vector< std::vector< TH1F* > > > gen_asymmetries_SM_control, 		gen_asymmetries_pt_SM_control,		gen_asymmetries_rho_SM_control,		gen_asymmetries_pt3_SM_control;
  std::vector< std::vector< std::vector< TH1F* > > > gen_asymmetries_FE_reference, 	gen_asymmetries_pt_FE_reference,	gen_asymmetries_rho_FE_reference,	gen_asymmetries_pt3_FE_reference;
  std::vector< std::vector< std::vector< TH1F* > > > gen_asymmetries_FE_control, 		gen_asymmetries_pt_FE_control,		gen_asymmetries_rho_FE_control,		gen_asymmetries_pt3_FE_control;
  std::vector< std::vector< std::vector< TH1F* > > > gen_asymmetries_FE, 						gen_asymmetries_pt_FE,						gen_asymmetries_rho_FE,						gen_asymmetries_pt3_FE;

  std::vector< std::vector< TH1F* > > alpha_spectrum_SM, alpha_spectrum_SM_control, alpha_spectrum_FE_reference, alpha_spectrum_FE_control, alpha_spectrum_FE;
  std::vector< std::vector< TH2F* > > alpha2D_SM, alpha2D_SM_control, alpha2D_FE_reference, alpha2D_FE_control, alpha2D_FE;
  std::vector< std::vector< std::vector< TH1F* > > > MC_Truth_asymmetries_SM, MC_Truth_asymmetries_SM_control, MC_Truth_asymmetries_FE_reference, MC_Truth_asymmetries_FE_control, MC_Truth_asymmetries_FE;
  std::vector< std::vector< std::vector< TH2F* > > > MC_Truth_asymmetries_2D_SM, MC_Truth_asymmetries_2D_SM_control, MC_Truth_asymmetries_2D_FE_reference, MC_Truth_asymmetries_2D_FE_control, MC_Truth_asymmetries_2D_FE;

  std::vector< std::vector< std::vector< TH2F* > > > dR_SM, 					gen_dR_SM,						dR_probe_SM,						gen_dR_probe_SM,            dR_barrel_SM,						gen_dR_barrel_SM;
  std::vector< std::vector< std::vector< TH2F* > > > dR_SM_control, 	gen_dR_SM_control,		dR_probe_SM_control,		gen_dR_probe_SM_control,    dR_barrel_SM_control,		gen_dR_barrel_SM_control;
  std::vector< std::vector< std::vector< TH2F* > > > dR_FE_reference, gen_dR_FE_reference,  dR_probe_FE_reference,  gen_dR_probe_FE_reference,  dR_barrel_FE_reference, gen_dR_barrel_FE_reference;
  std::vector< std::vector< std::vector< TH2F* > > > dR_FE_control, 	gen_dR_FE_control,	  dR_probe_FE_control,		gen_dR_probe_FE_control,    dR_barrel_FE_control,		gen_dR_barrel_FE_control;
  std::vector< std::vector< std::vector< TH2F* > > > dR_FE, 					gen_dR_FE,						dR_probe_FE,						gen_dR_probe_FE,            dR_barrel_FE,						gen_dR_barrel_FE;

  std::vector< std::vector< std::vector< TH3F* > > > dR3_SM,            gen_dR3_SM;
  std::vector< std::vector< std::vector< TH3F* > > > dR3_SM_control,    gen_dR3_SM_control;
  std::vector< std::vector< std::vector< TH3F* > > > dR3_FE_reference,  gen_dR3_FE_reference;
  std::vector< std::vector< std::vector< TH3F* > > > dR3_FE_control,    gen_dR3_FE_control;
  std::vector< std::vector< std::vector< TH3F* > > > dR3_FE,            gen_dR3_FE;

  std::vector<double> dR_bins;

  std::vector< std::vector< std::vector< std::vector< TH2F* > > > > asy_dR_barrel_FE, asy_dR_probe_FE, gen_asy_dR_barrel_FE, gen_asy_dR_probe_FE;

  std::vector<TH1F*> histograms;

  TH1F *h_JetPt;
  TH1F *h_PU;

  TH1F *h_rho_SM;
  TH1F *h_rho_FE;
  TH1F *h_JetAvePt_SM;
  TH1F *h_Jet1Pt_SM;
  TH1F *h_Jet2Pt_SM;
  TH1F *h_Jet3Pt_SM;

  TH1F *h_JetAvePt_FE;
  TH1F *h_Jet1Pt_FE;
  TH1F *h_Jet2Pt_FE;
  TH1F *h_Jet3Pt_FE;
};

#endif

#ifdef MySelector_cxx
void MySelector::Init(TTree *tree){
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);

  TFile* currentFile = ((TChain*)fChain)->GetFile();

  fChain->SetBranchAddress("Njet", &njet, &b_njet);
  fChain->SetBranchAddress("pt_ave", &pt_ave, &b_pt_ave);
  fChain->SetBranchAddress("jet1_pt", &jet1_pt, &b_jet1_pt);
  fChain->SetBranchAddress("jet2_pt", &jet2_pt, &b_jet2_pt);
  fChain->SetBranchAddress("jet3_pt", &jet3_pt, &b_jet3_pt);
  fChain->SetBranchAddress("jet1_eta", &jet1_eta, &b_jet1_eta);
  fChain->SetBranchAddress("jet2_eta", &jet2_eta, &b_jet2_eta);
  fChain->SetBranchAddress("jet3_eta", &jet3_eta, &b_jet3_eta);
  fChain->SetBranchAddress("jet1_phi", &jet1_phi, &b_jet1_phi);
  fChain->SetBranchAddress("jet2_phi", &jet2_phi, &b_jet2_phi);
  fChain->SetBranchAddress("jet3_phi", &jet3_phi, &b_jet3_phi);
  fChain->SetBranchAddress("barreljet_phi", &barreljet_phi, &b_barreljet_phi);
  fChain->SetBranchAddress("barreljet_eta", &barreljet_eta, &b_barreljet_eta);
  fChain->SetBranchAddress("barreljet_pt", &barreljet_pt, &b_barreljet_pt);
  fChain->SetBranchAddress("probejet_phi", &probejet_phi, &b_probejet_phi);
  fChain->SetBranchAddress("probejet_eta", &probejet_eta, &b_probejet_eta);
  fChain->SetBranchAddress("probejet_pt", &probejet_pt, &b_probejet_pt);
  fChain->SetBranchAddress("asymmetry", &asymmetry, &b_asymmetry);
  fChain->SetBranchAddress("alpha", &alpha_, &b_alpha);
  fChain->SetBranchAddress("Ngenjet", &ngenjet, &b_ngenjet);
  fChain->SetBranchAddress("gen_pt_ave", &gen_pt_ave, &b_pt_ave);
  fChain->SetBranchAddress("genjet1_pt", &genjet1_pt, &b_genjet1_pt);
  fChain->SetBranchAddress("genjet2_pt", &genjet2_pt, &b_genjet2_pt);
  fChain->SetBranchAddress("genjet3_pt", &genjet3_pt, &b_genjet3_pt);
  fChain->SetBranchAddress("genjet3_eta", &genjet3_eta, &b_genjet3_eta);
  fChain->SetBranchAddress("genjet3_phi", &genjet3_phi, &b_genjet3_phi);
  fChain->SetBranchAddress("barrelgenjet_phi", &barrelgenjet_phi, &b_barrelgenjet_phi);
  fChain->SetBranchAddress("barrelgenjet_eta", &barrelgenjet_eta, &b_barrelgenjet_eta);
  fChain->SetBranchAddress("barrelgenjet_pt", &barrelgenjet_pt, &b_barrelgenjet_pt);
  fChain->SetBranchAddress("probegenjet_phi", &probegenjet_phi, &b_probegenjet_phi);
  fChain->SetBranchAddress("probegenjet_eta", &probegenjet_eta, &b_probegenjet_eta);
  fChain->SetBranchAddress("probegenjet_pt", &probegenjet_pt, &b_probegenjet_pt);
  fChain->SetBranchAddress("gen_alpha", &gen_alpha_, &b_gen_alpha);
  fChain->SetBranchAddress("gen_asymmetry", &gen_asymmetry, &b_gen_asymmetry);
  fChain->SetBranchAddress("weight", &weight, &b_weight);
  fChain->SetBranchAddress("weight_pu", &weight_pu, &b_weight_pu);
  fChain->SetBranchAddress("weight_pu_down", &weight_pu_down, &b_weight_pu_down);
  fChain->SetBranchAddress("weight_pu_up", &weight_pu_up, &b_weight_pu_up);
  fChain->SetBranchAddress("rho", &rho, &b_rho);
  fChain->SetBranchAddress("nPU", &npuIT, &b_npuIT);
}

Bool_t MySelector::Notify(){
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  TFile *currentFile = fChain->GetCurrentFile();
  return kTRUE;
}

#endif // #ifdef MySelector_cxx
