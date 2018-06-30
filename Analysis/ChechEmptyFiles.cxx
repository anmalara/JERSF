#include <iostream>
#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TF1.h>


std::string itoa(int i){
  char res[10];
  sprintf(res, "%d", i);
  std::string ret(res);
  return ret;
}

void ChechEmptyFiles(std::string name_ = "/pnfs/desy.de/cms/tier2/store/user/akaravdi/Run2017_94X_v1_woHOTVR_woXCONE/JetHT_Run2017B_17Nov/DATA_JetHT_Run2016D_17Nov_"){



  std::string itoa(int i){
    char res[10];
    sprintf(res, "%d", i);
    std::string ret(res);
    return ret;
  }
  std::string name;
  // std::string name_ = "/pnfs/desy.de/cms/tier2//store/user/jmulthau/RunII_94X_v1/JetHT_chsMET_RunC//JetHT/crab_JetHT/180219_125404/000";
  std::string name_ = "/pnfs/desy.de/cms/tier2/store/user/cgarbers/Run2017_data_94X/Run2017_RunE_17Nov2017_v1_wCHSMET/JetHT_RunE_17Nov17_v1_";
  // TFile *file;
  // TTree* tree;

  int n_min, n_max;
  n_min=4940;
  n_max=4942;

  for (int i = n_min; i <+ n_max; i++) {
    // name = name+itoa(i)+"_Ntuple.root";
    // int j = 0;
    // if (i<1000) {
    //   j=0;
    // } else if (i<2000) {
    //   j=1;
    // } else if (i<3000) {
    //   j=2;
    // } else if (i<4000) {
    //   j=3;
    // } else if (i<5000) {
    //   j=4;
    // } else if (i<6000) {
    //   j=5;
    // } else if (i<7000) {
    //   j=6;
    // } else if (i<8000) {
    //   j=7;
    // } else if (i<1000) {
    //   j=8;
    // } else {
    //   j=9;
    // }
    // name = name_+itoa(j)+"/Ntuple_"+itoa(i)+".root";
    name = name_+itoa(i)+"_Ntuple.root";
    if (!gSystem->AccessPathName(name.c_str())) {
      TFile *file = new TFile(name.c_str());
      TTree *tree = (TTree*)file->Get("AnalysisTree");

      if (tree->GetEntries() == 0) {
        std::cout << "No entries: " << std::endl;
        std::cout << name << std::endl;
      }

      delete tree;
      delete file;

    }
    else{
      std::cout << "No file: " << std::endl;
      std::cout << name << std::endl;
    }
  }


  str::string name = ;

  for (int i = 0; i < 160; i++) {
    string name = "/nfs/dust/cms/user/amalara/sframe_all/JER2017_Data/workdir_JER2017_Data_RunE/uhh2.AnalysisModuleRunner.DATA.DATA_RunE_promptReco_2017_"+itoa(i)+".root";
    if (gSystem->AccessPathName(name.c_str())) {
      std::cout << i << std::endl;
    }
  }

}
