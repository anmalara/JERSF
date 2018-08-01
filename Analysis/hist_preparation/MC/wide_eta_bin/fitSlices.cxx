
#include <iostream>
#include <algorithm>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TLorentzVector.h>
#include <TRandom3.h>
#include "MySelector.h"
#include "MyJet.h"
#include "../../../../include/constants.h"

TCanvas* fitSlices_inFile(TString filename = "file/Single/PtBinned_full/histograms_mc_incl_full_2D_dR.root",
TString histo_name = "asy_dR_barrel_forward_probe10_pt2_alpha6_dR_probe3") {
  TFile *file = new TFile( filename, "READ");
  TH2F *h2 = (TH2F*)file->Get(histo_name);
  h2->RebinX(4);
  h2->RebinY(4);
  // TObjArray *a = histo_name.Tokenize("_")
  // for (size_t i = 0; i < a->GetEntries(); i++) {
  //   /* code */
  // }
  // h2->SetTitle("test2")
  // Create a canvas and divide it
  TCanvas *c1 = new TCanvas("c1","c1",50, 50, 800, 600);
  // c1->SetFillColor(42);
  c1->Divide(2,1);
  TPad *leftPad = (TPad*)c1->cd(1);
  leftPad->Divide(1,2);
  // Draw 2-d original histogram
  leftPad->cd(1);
  gPad->SetTopMargin(0.12);
  // gPad->SetFillColor(33);
  h2->Draw("colz");
  h2->GetXaxis()->SetLabelSize(0.06);
  h2->GetYaxis()->SetLabelSize(0.06);
  // h2->SetMarkerColor(kYellow);
  // Fit slices projected along Y fron bins in X [7,32] with more than 20 bins  in Y filled
  // h2->FitSlicesX(0, 0, -1, 2);
  h2->FitSlicesX();
  // Show fitted "mean" for each slice
  leftPad->cd(2);
  // gPad->SetFillColor(33);
  TH2F *h2_0 = (TH2F*)file->Get(histo_name+"_0");
  h2_0->Draw();
  TPad *rightPad = (TPad*)c1->cd(2);
  rightPad->Divide(1,2);
  rightPad->cd(1);
  gPad->SetTopMargin(0.12);
  gPad->SetLeftMargin(0.15);
  // gPad->SetFillColor(33);
  TH2F *h2_1 = (TH2F*)file->Get(histo_name+"_1");
  h2_1->GetYaxis()->SetRangeUser(-0.3, 0.3);
  h2_1->Draw();
  // Show fitted "sigma" for each slice
  rightPad->cd(2);
  gPad->SetTopMargin(0.12);
  gPad->SetLeftMargin(0.15);
  // gPad->SetFillColor(33);
  TH2F *h2_2 = (TH2F*)file->Get(histo_name+"_2");
  h2_2->GetYaxis()->SetRangeUser(-0.1, 0.4);
  h2_2->Draw();
  //attributes
  h2_0->SetLineColor(2);
  h2_1->SetLineColor(2);
  h2_2->SetLineColor(2);
  h2_0->SetMarkerColor(2);
  h2_1->SetMarkerColor(2);
  h2_2->SetMarkerColor(2);
  h2_0->SetMarkerStyle(21);
  h2_1->SetMarkerStyle(21);
  h2_2->SetMarkerStyle(21);
  h2_0->SetMarkerSize(0.6);
  h2_1->SetMarkerSize(0.6);
  h2_2->SetMarkerSize(0.6);
  return c1;
}

void fitSlices(){
  double dRbins [] = { 0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.4, 4.8, 5.2, 5.6, 6.0};
  std::vector<double> dR_bins(dRbins, dRbins + sizeof(dRbins)/sizeof(double));
  int min = 0;
  int max = dR_bins.size()-1;
  bool debug = false;

  // debug = true;
  // min = 1;
  // max = 2;
  TString filename = "file/Single/PtBinned_full/histograms_mc_incl_full_2D_dR.root";
  TString histo_name = "asy_dR_barrel_forward_probe10_pt2_alpha6_dR_probe";
  for (unsigned int i = min; i < max; i++) {
    histo_name = "asy_dR_barrel_forward_probe10_pt2_alpha6_dR_probe"; histo_name += i;
    TCanvas *c = fitSlices_inFile(filename, histo_name);
    TString outname = "test/asy_dR_barrel"; outname += i;
    // c->Print(outname+".pdf", "pdf");
    c->Print(outname+".png", "png");
    delete c;
  }

  for (unsigned int i = min; i < max; i++) {
    histo_name = "asy_dR_probe_forward_probe10_pt2_alpha6_dR_barrel"; histo_name += i;
    TCanvas *c = fitSlices_inFile(filename, histo_name);
    TString outname = "test/asy_dR_probe"; outname += i;
    // c->Print(outname+".pdf", "pdf");
    c->Print(outname+".png", "png");
    delete c;
  }

  if (debug){
    TString filename = "file/Single/PtBinned_full/histograms_mc_incl_full_2D_dR.root";
    TString histo_name = "asy_dR_barrel_forward_probe10_pt2_alpha6_dR_probe";
    int bin_min, bin_max;
    TH1D *h2_test = 0;
    filename = "file/Single/PtBinned_full/histograms_mc_incl_full_2D_dR.root";
    histo_name = "asy_dR_barrel_forward_probe10_pt2_alpha6_dR_probe"; histo_name += 2;
    TFile *file = new TFile( filename, "READ");
    TH2F *h2 = (TH2F*)file->Get(histo_name);
    h2->RebinX(4);
    h2->RebinY(4);
    new TCanvas;
    h2->Draw("colz");
    bin_min = h2->GetYaxis()->FindBin(2.4);
    bin_max = h2->GetYaxis()->FindBin(2.8);
    h2->ProjectionX("h2_test", bin_min+1, bin_max+1, "e");
    h2_test = (TH1D*)gDirectory->Get("h2_test");
    new TCanvas;
    h2_test->Draw();

    for (int i = 0; i < 16; i++) {
      TString name = i;
      h2->ProjectionX(name, i+1, i+2, "e");
      h2_test = (TH1D*)gDirectory->Get(name);
      new TCanvas;
      h2_test->Draw();
    }
  }
}
//
//
// void SwitchAxis(TH3* h_old, TString order = "XYZ") {
//   /* code */
// }
//
// //
// {
//   //   auto c1 = new TCanvas("c1","Profile histogram example",200,10,700,500);
//   //   auto hprof  = new TProfile("hprof","Profile of pz versus px",100,-5,5,-5,5);
//   //   TH2D *h2 = new TH2D("h2","h2",100,-5,5, 100, -5,5);
//   //   TH1D *h1 = new TH1D("h1","h1",100, -5,5);
//   //   TH1D *h11 = new TH1D("h11","h11",100, -5,5);
//   TH3D *h3 = new TH3D("h3","h3",100,-5,5, 100, -5,5, 100, 0,20);
//   Float_t px, py, pz;
//   for ( Int_t i=0; i<50000; i++) {
//     gRandom->Rannor(px,py);
//     pz = px*px + py*py;
//     h3->Fill(px,py, pz);
//     // hprof->Fill(px,py);
//     // h2->Fill(px,py);
//     // h1->Fill(px);
//     // h11->Fill(py);
//   }
//   //   new TCanvas;
//   //   hprof->Draw();
//   //   new TCanvas;
//   //   h2->Draw("colz");
//   //   new TCanvas;
//   //   h1->Draw();
//   //   new TCanvas;
//   //   h11->Draw();
//   //   h2->ProjectionX("h2_px0_20", 10, 80, "o");
//   //   h2->ProjectionX();
//   //   h2->ProjectionY();
//   //   TH1D *h_px = (TH1D*)gDirectory->Get("h2_px");
//   //   TH1D *h_py = (TH1D*)gDirectory->Get("h2_py");
//   //   TH1D *h_px0_20 = (TH1D*)gDirectory->Get("h2_px0_20");
//   //   new TCanvas;
//   //   h_px->Draw();
//   //   new TCanvas;
//   //   h_py->Draw();
//   //   new TCanvas;
//   //   h_px0_20->Draw();
//   //
//   //
//   //
// }
