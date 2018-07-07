#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

void tex_Maker() {

  std::string filename = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/JER/wide_eta_binning/file/DiJet/RunBC_full/output/scalefactors_ST.txt";

  std::ifstream file(filename.c_str(), ios::in);
  std::cout<<"Opened file "<<filename<<std::endl;
  std::string line;

  std::vector<std::vector<double> > SFs;
  std::vector<double> eta_bin_center;
  std::vector<double> eta_bin_err;
  std::vector<double> st;
  std::vector<double> st_err;
  std::vector<double> st_cor;
  std::vector<double> st_err_cor;

  SFs.push_back(eta_bin_center);
  SFs.push_back(eta_bin_err);
  SFs.push_back(st);
  SFs.push_back(st_err);
  SFs.push_back(st_cor);
  SFs.push_back(st_err_cor);

  while (!file.eof() && !gSystem->AccessPathName(filename.c_str())) {
    getline(file, line);
    // std::cout << line << '\n';
    std::istringstream iss(line);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    for (unsigned int i = 0; i < results.size(); i++) { SFs.at(0).push_back(results[i]);}
  }



}
