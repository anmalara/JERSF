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
#include <TMath.h>
#include <TDirectory.h>

#include "MyJet.h"


// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxp4 = 164;
const Int_t kMaxgen_p4 = 164;

double Weight( std::string filename );

class MySelector : public TSelector {
  public :

  TTree *fChain;   //!pointer to the analyzed TTree or TChain

  // Declaration of leaf types

  Int_t           npuIT;
  Int_t njet;
  Float_t pt_ave;
  Float_t jet1_pt;
  Float_t jet2_pt;
  Float_t jet3_pt;
  Float_t barreljet_phi;
  Float_t barreljet_eta;
  Float_t probejet_phi;
  Float_t probejet_eta;
  Float_t asymmetry;
  Float_t rho;
  Float_t alpha;
  Int_t pass_trigger_hf;
  Int_t pass_trigger_bl;
  TBranch *b_alpha;
  TBranch *b_njet;
  TBranch *b_pt_ave;
  TBranch *b_jet1_pt;
  TBranch *b_jet2_pt;
  TBranch *b_jet3_pt;
  TBranch *b_probejet_phi;
  TBranch *b_probejet_eta;
  TBranch *b_barreljet_phi;
  TBranch *b_barreljet_eta;
  TBranch *b_pass_trigger_hf;
  TBranch *b_pass_trigger_bl;
  TBranch *b_asymmetry;
  TBranch *b_rho;


  // List of branches

  TBranch        *b_npuIT;   //!

  MySelector(TTree * /*tree*/ =0) : fChain(0) { }
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
  int EtaForwardBinsNo;
  int EtaBinsNo;
  int EtaFtBinsNo;
  int EtaFtControlBinsNo;
  int PtBinsNo;
  int PtFTBinsNo;
  int AlphaBinsNo;
  TString whichRun;
  std::vector<MyJet> Jets;

  std::vector< std::vector< std::vector< TH1F* > > > forward_hist;
  std::vector< std::vector< std::vector< TH1F* > > > forward_hist_dijet;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_all;

  std::vector< std::vector< std::vector< TH1F* > > > forward_pt_hist;
  std::vector< std::vector< std::vector< TH1F* > > > forward_pt_hist_dijet;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_pt_all;

  std::vector< std::vector< std::vector< TH1F* > > > forward_pt3_hist;
  std::vector< std::vector< std::vector< TH1F* > > > forward_pt3_hist_dijet;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_pt3_all;

  std::vector< std::vector< std::vector< TH1F* > > > forward_rho_hist;
  std::vector< std::vector< std::vector< TH1F* > > > forward_rho_hist_dijet;
  std::vector< std::vector< std::vector< TH1F* > > > asymmetries_rho_all;

  std::vector< std::vector< TH1F* > > alpha_spectrum, forward_alpha_spectrum, forward_alpha_spectrum_dijet;

  std::vector<TH1F*> histograms;

  TH1F *h_PU;
  TH1F *h_alpha_raw;
  TH1F *h_alpha_select;

  TH1F *h_JetAvePt;
  TH1F *h_Jet1Pt;
  TH1F *h_Jet2Pt;
  TH1F *h_Jet3Pt;

  TH1F *h_FEJetAvePt;
  TH1F *h_FEJet1Pt;
  TH1F *h_FEJet2Pt;
  TH1F *h_FEJet3Pt;

  // ClassDef(MySelector,0);
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

  //TFile *currentFile = tree->GetCurrentFile();
  TFile* currentFile = ((TChain*)fChain)->GetFile();

  fChain->SetBranchAddress("Njet", &njet, &b_njet);
  fChain->SetBranchAddress("pt_ave", &pt_ave, &b_pt_ave);
  fChain->SetBranchAddress("jet1_pt", &jet1_pt, &b_jet1_pt);
  fChain->SetBranchAddress("jet2_pt", &jet2_pt, &b_jet2_pt);
  fChain->SetBranchAddress("jet3_pt", &jet3_pt, &b_jet3_pt);
  fChain->SetBranchAddress("barreljet_phi", &barreljet_phi, &b_barreljet_phi);
  fChain->SetBranchAddress("barreljet_eta", &barreljet_eta, &b_barreljet_eta);
  fChain->SetBranchAddress("probejet_phi", &probejet_phi, &b_probejet_phi);
  fChain->SetBranchAddress("probejet_eta", &probejet_eta, &b_probejet_eta);
  fChain->SetBranchAddress("asymmetry", &asymmetry, &b_asymmetry);
  fChain->SetBranchAddress("alpha", &alpha, &b_alpha);
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
  fChain->SetBranchAddress("nPU", &npuIT, &b_npuIT);
  TString tempName = currentFile->GetName();
  if (tempName.Contains("RunB")) {  whichRun = "RunB"; }
  else if (tempName.Contains("RunC")) {  whichRun = "RunC"; }
  else if (tempName.Contains("RunD")) {  whichRun = "RunD"; }
  else if (tempName.Contains("RunE")) {  whichRun = "RunE"; }
  else if (tempName.Contains("RunF")) {  whichRun = "RunF"; }
  else { whichRun = "MC"; }

  return kTRUE;
}

#endif // #ifdef MySelector_cxx
