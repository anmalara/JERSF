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
  Int_t pass_trigger_hf;
  Int_t pass_trigger_bl;
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
  TBranch *b_pass_trigger_hf;
  TBranch *b_pass_trigger_bl;
  TBranch *b_asymmetry;
  TBranch *b_alpha;
  TBranch *b_rho;


  // List of branches
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

  int TotalEvents;

  int EtaBins_SM, EtaBins_SM_control, EtaBins_FE_reference, EtaBins_FE_control, EtaBins_FE;
  int etaShift_SM, etaShift_SM_control, etaShift_FE_reference, etaShift_FE_control, etaShift_FE;
  int PtBins_Central, PtBins_HF;
  int AlphaBins;

  TString whichRun;

  std::vector< std::vector< std::vector< double > > > nevents_central,nevents_HF,nevents_COMB;

  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_SM, 						asymmetries_pt_SM,						asymmetries_rho_SM,						asymmetries_pt3_SM;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_SM_control, 		asymmetries_pt_SM_control,		asymmetries_rho_SM_control,		asymmetries_pt3_SM_control;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_FE_reference, 	asymmetries_pt_FE_reference,	asymmetries_rho_FE_reference,	asymmetries_pt3_FE_reference;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_FE_control, 		asymmetries_pt_FE_control,		asymmetries_rho_FE_control,		asymmetries_pt3_FE_control;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_FE, 						asymmetries_pt_FE,						asymmetries_rho_FE,						asymmetries_pt3_FE;

  std::vector< std::vector< TH1F* > > alpha_spectrum_SM, alpha_spectrum_SM_control, alpha_spectrum_FE_reference, alpha_spectrum_FE_control, alpha_spectrum_FE;

  std::vector<TH1F*> histograms;

  TH1F *h_PU;
  TH1F *h_alpha_raw;
  TH1F *h_alpha_select;

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
  fChain->SetBranchAddress("hf_trigger", &pass_trigger_hf, &b_pass_trigger_hf);
  fChain->SetBranchAddress("bl_trigger", &pass_trigger_bl, &b_pass_trigger_bl);
  fChain->SetBranchAddress("rho", &rho, &b_rho);
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
