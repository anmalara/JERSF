#include <iostream>
#include <TChain.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TFile.h>
#include <TLine.h>
#include <TStyle.h>
#include <string>
#include <TMath.h>
#include <TLegend.h>
#include <TPaveStats.h>

#include <TGraphAsymmErrors.h>

int divide()
{
	TFile f1("PU.root", "READ");
	TFile f2("MyDataPileupHistogram_25ns_69000_28_10.root", "READ");

	TH1F *mc = ( TH1F* )f1.Get("PileUp");
	TH1F *data = ( TH1F* )f2.Get("pileup");

	Double_t scaleMC = 1/(mc->Integral());
	Double_t scaleDt = 1/(data->Integral());

	mc->Scale(scaleMC);
	data->Scale(scaleDt);

	data -> Divide( mc );
	data -> DrawCopy();

	data ->GetXaxis()->SetTitle("PU");
	data ->GetYaxis()->SetTitle("weight");

	TFile h( "PUscale.root" ,"new" );

	data -> Write();

	h.Close();

	return 0;
}











