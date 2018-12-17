#include "tdrstyle_mod15.C"
#include "TGraphErrors.h"
#include "TF1.h"

void JERplot() {

  setTDRStyle();

  // TWiki for 7, 8 and 13 TeV:
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution#JER_Scaling_factors_and_Uncertai

  // binning and scale factor values for 7 and 8 TeV

  double eta7[] = {0, 0.5, 1.1, 1.7, 2.3, 5.0};
  const int neta7 = sizeof(eta7)/sizeof(eta7[0])-1;
  double jer7[5][2] =
    {{1.052 ,(1.115-0.990)/2}, {1.057 ,(1.114-1.001)/2},
     {1.096 ,(1.161-1.032)/2}, {1.134 ,(1.228-1.042)/2},
     {1.2588 ,(1.488-1.089)/2}};
  TH1D *h7 = new TH1D("h7",";#eta;Scale factor",neta7,eta7);
  for (int i = 0; i != neta7; ++i) {
    h7->SetBinContent(i+1, jer7[i][0]);
    h7->SetBinError(i+1, jer7[i][1]);
  }

  double eta8[] = {0, 0.5, 1.1, 1.7, 2.3, 2.8, 3.2, 5.0};
  const int neta8 = sizeof(eta8)/sizeof(eta8[0])-1;
  double jer8[7][2] =
    {{1.079 ,(1.105-1.053)/2}, {1.099 ,(1.127-1.071)/2},
     {1.121 ,(1.150-1.092)/2}, {1.208 ,(1.254-1.162)/2},
     {1.254 ,(1.316-1.192)/2}, {1.395 ,(1.458-1.332)/2},
     {1.056 ,(1.247-0.865)/2}};
  TH1D *h8 = new TH1D("h8",";#eta;Scale factor",neta8,eta8);
  for (int i = 0; i != neta8; ++i) {
    h8->SetBinContent(i+1, jer8[i][0]);
    h8->SetBinError(i+1, jer8[i][1]);
  }

  // binning for 2016 data and choice of how to combine ST and FW results

  double eta13[] = {0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191}; // from Feb28
  const int neta13 = sizeof(eta13)/sizeof(eta13[0])-1;

  // 36 inv fb scale factors (Legacy_mc_vs_Legacy_data)

  // Standard
    double jer13_run2[13][2] =
      {{1.15953,0.0322243894},{1.19477,0.0337499818},
       {1.1464,0.0298593582},{1.18455,0.0329298591},
       {1.12286,0.0233230706},{1.08037,0.0489382838},
       {1.13319,0.1019378518},{1.10806,0.0467432949},
       {1.15493,0.2974803617},{1.25344,0.0994116976},
       {0, 0},{0,0},{0,0}};
    //Forward
    double jer13_run2b[13][2] =
      {{0,0},{0,0},{0,0},
       {1.13538,0.10466519834214237},{1.13423,0.1052319382765518},
       {1.11979,0.11170186420472127},{1.14751,0.11073658510180814},
       {1.19513,0.11142421419440206},{1.322,0.13399183519319377},
       {1.50318,0.20784889159155986},{1.77881,0.20075481999184983},
       {1.18695,0.1243408341977807},{1.19218,0.14883297424280686}};

    // (self-)combined FW and ST results.
    double jer13_comb[13][2] =
      {{1.15953,0.0322243894},{1.19477,0.0337499818},
       {1.1464, 0.0298593582},{1.1608608730818335, 0.0314114337},
       {1.1277674503328965, 0.0227705413},{1.1000419290738948, 0.0448277854},
       {1.1426190202932343, 0.0749862918},{1.1511576138738635, 0.0431038630},
       {1.2962786307493799, 0.1221696907},{1.3418116992379743, 0.0896812121},
       {1.77881,0.2007462079},{1.18695,0.1243701331},
       {1.19218,0.1487939851}};

    // statistical uncertainty of the combined result.

    double jer13_run2s[13] =
        { 0.005, 0.011, 0.008, 0.012, 0.009, 0.019, 0.028, 0.023, 0.046, 0.048, 0.065, 0.019, 0.039 };



  //**************** Run2 **************************//
  TH1D *h13_run2 = new TH1D("h13_run2",";#eta;Scale factor",neta13,eta13);   // STD
  TH1D *h13_run2b = new TH1D("h13_run2b",";#eta;Scale factor",neta13,eta13); // FE
  TH1D *h13_run2ds = new TH1D("h13_run2ds",";#eta;Scale factor",neta13,eta13); // w/ stat only
  TH1D *h13_comb = new TH1D("h13_run2b",";#eta;Scale factor",neta13,eta13); // FE
  //******************************************//

  for (int i = 0; i != neta13; ++i) {
    /// RUN 2

    if (h13_run2->GetBinCenter(i+1)<3.0) {
      h13_run2->SetBinContent(i+1, jer13_run2[i][0]);
      h13_run2->SetBinError(i+1, jer13_run2[i][1]);
    }
    //if (h13_run2b->GetBinCenter(i+1)>0.8) { // up to Jan29
    if (h13_run2b->GetBinCenter(i+1)>1.1) { // Feb28
      h13_run2b->SetBinContent(i+1, jer13_run2b[i][0]);
      h13_run2b->SetBinError(i+1, jer13_run2b[i][1]);
    }
    h13_comb->SetBinContent(i+1, jer13_comb[i][0]);
    h13_comb->SetBinError(i+1, jer13_comb[i][1]);


    h13_run2ds->SetBinContent(i+1, jer13_comb[i][0]);
    h13_run2ds->SetBinError(i+1, jer13_run2s[i]);
    //******************************************//

  }

  // The canvas on which SF plot will be made

  TH1D *h = new TH1D("h",";|#eta|;Scale factor",52,0,5.2);
  h->SetMinimum(0.80);
  h->SetMaximum(2.00);
  TCanvas *c1 = tdrCanvas("c1",h,4,11,kSquare);

  TLegend *leg = tdrLeg(0.20,0.60,0.50,0.80);

    //******************************************//
    // Run 2
  lumi_13TeV = "Run2016";
  gStyle->SetEndErrorSize(5);
  h13_run2ds->SetLineWidth(3);
  h13_run2ds->SetMarkerColor(kOrange);
  h13_run2ds->SetLineColor(kOrange);
  h13_run2ds->SetMarkerStyle(kFullStar);
  h13_run2ds->SetMarkerSize(1.5);

// To prepare the comparison of ST, FW and combined results, use the code below

  tdrDraw(h13_comb, "PE2", kFullSquare, kMagenta+2, kSolid, kMagenta+2, 3001, kMagenta-9);
  tdrDraw(h13_run2ds, "PE2", kFullStar, kMagenta+2, kSolid, kMagenta+2, 3001, kMagenta-6);
  tdrDraw(h13_run2, "PE1", kFullStar, kBlue);
  h13_run2->SetLineWidth(3);
  h13_run2->SetMarkerSize(1.5);
  tdrDraw(h13_run2b, "PE1", kFullStar, kGreen+1);
  h13_run2b->SetLineWidth(3);
  h13_run2b->SetMarkerSize(1.5);

  leg->AddEntry(h13_run2ds, "2016 comb", "PL");
  leg->AddEntry(h13_run2,   "2016 ST", "PL");
  leg->AddEntry(h13_run2b,  "2016 FW", "PL");


  gPad->RedrawAxis();

  gPad->SaveAs("eps/JERplot.eps");
  gPad->SaveAs("pdf/JERplot.pdf");
  gPad->SaveAs("pdfC/JERplot.C");


// To prepare 7, 8 and 13 TeV comparison, use the code below

  // The canvas on which SF plot will be made

  TH1D *h2 = new TH1D("h2",";|#eta|;Scale factor",52,0,5.2);
  h2->SetMinimum(0.80);
  h2->SetMaximum(2.00);
  TCanvas *c2 = tdrCanvas("c2",h2,4,11,kSquare);

  TLegend *leg2 = tdrLeg(0.20,0.60,0.50,0.80);
  tdrDraw(h7, "PE2", kFullSquare, kBlue, kSolid, kBlue, 1001, kBlue-9);
  tdrDraw(h8, "PE2", kFullSquare, kGreen+2, kSolid, kGreen+2, 3001, kGreen-9);
  tdrDraw(h13_comb, "PE1", kFullSquare, kMagenta+2, kSolid, kMagenta+2, 3001, kMagenta-9,3);
  tdrDraw(h13_run2ds, "PE2", kFullStar, kMagenta+2, kSolid, kMagenta+2, 3001, kMagenta-6);

  leg2->AddEntry(h7, " 7 TeV","FP");
  leg2->AddEntry(h8, " 8 TeV","FP");
  leg2->AddEntry(h13_run2ds, "13 TeV", "PL");

  gPad->RedrawAxis();

  gPad->SaveAs("eps/JERplot2.eps");
  gPad->SaveAs("pdf/JERplot2.pdf");
  gPad->SaveAs("pdfC/JERplot2.C");

}
