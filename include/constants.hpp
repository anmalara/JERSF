#pragma once

#ifndef  CONSTANTS_H
#define  CONSTANTS_H

// barrel region (|eta| < 1.3)
constexpr static float s_eta_barr = 1.131;
// two back-to-back leading jets (delta_phi(j1,j2) = min(|phi1 - phi2|, 2PI - |phi2 - phi1|) > 2.7)
constexpr static float s_delta_phi = 2.7;

// 2017 JER
//Pt bins
/* //SingleJet central triggers highest ------------- */
const int n_pt_bins_Si = 10;
const double pt_bins_Si[n_pt_bins_Si]       = { 40, 72,  95, 160, 226, 283, 344, 443, 577, 606};
const double pt_bins_SiAK8PUPPI[n_pt_bins_Si] = { 73,  90, 115, 181, 251, 312, 378, 457, 519, 566};
/* //SingleJet HF triggers highest ------------- */
const int n_pt_bins_Si_HF = 10;
const double pt_bins_Si_HF[n_pt_bins_Si_HF] = { 40, 72,  95, 160, 226, 283, 344, 443, 577, 606};

/* //Minimum bias triggers highest ------------- */
const int n_pt_bins_MB = 17;
const double pt_bins_MB[n_pt_bins_MB]       = { 5, 10, 15, 20, 25, 30, 35, 40, 72, 95, 160, 226, 283, 344, 443, 577, 606};
/* //Minimum bias HF triggers highest ------------- */
const int n_pt_bins_MB_HF = 17;
const double pt_bins_MB_HF[n_pt_bins_MB_HF] = { 5, 10, 15, 20, 25, 30, 35, 40, 72, 95, 160, 226, 283, 344, 443, 577, 606};

/* //DiJet central triggers highest ------------- */
const int n_pt_bins_Di = 9;
const double pt_bins_Di[n_pt_bins_Di]       = { 73, 85,  97, 179, 307, 370, 434, 520, 649};
/* //DiJet HF triggers highest ------------- */
const int n_pt_bins_Di_HF = 6;
const double pt_bins_Di_HF[n_pt_bins_Di_HF] = { 73, 93, 113, 176, 239, 318 };

/* //Eta bins ------------- */
const int n_eta_bins = 14;
const double eta_bins[n_eta_bins] = { 0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191 };

// taken from https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2017Analysis
constexpr static int s_runnr_A = 297019; //up to this one, including this one
constexpr static int s_runnr_B = 299329; //up to this one, including this one
constexpr static int s_runnr_C = 302029; //up to this one, including this one
constexpr static int s_runnr_D = 303434; //up to this one, including this one
constexpr static int s_runnr_E = 304826; //up to this one, including this one
constexpr static int s_runnr_F = 306462; //up to this one, including this one

constexpr static float s_asymm = 1.; //FIXME change back to 0.7?

constexpr static float s_Pt_AveMC_cut = 40;
const double eta_cut = 2.853;




//Old stuff not checked

// cut on the asymmetry for events with two jets  |(j2->pt - j1->pt /(j2->pt + j1->pt)| < 0.2
/* constexpr static float s_asymm = 0.2; */
// relative third jet fraction pt_rel = 2*j3_pt/(j1_pt + j2_pt) < 0.2
constexpr static float s_pt_rel = 0.4;
/** \brief good Primary Vertex reconstruction **/
// more than four tracks
constexpr static float s_n_PvTracks = 4;
// PV is located within 24cm in z vertex
constexpr static float s_n_Pv_z = 24.0;
// PV is located within 2cm in xy direction from the nominal interaction point
constexpr static float s_n_Pv_xy = 2.0;


//2017
// taken from https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2017Analysis

constexpr static float s_lumi_HF_60 = 19.605;
constexpr static float s_lumi_HF_80 = 28.773;
constexpr static float s_lumi_HF_100 = 98.341;
constexpr static float s_lumi_HF_160 = 561.485;
constexpr static float s_lumi_HF_220 = 28983.619;
constexpr static float s_lumi_HF_300 = 35672.846;

// 2017 RunB PromptReco, not completly filled, not used if apply lumi weights is false
//Lumi recorded by different triggers in pb-1
constexpr static float s_lumi_cent_40 = 0.09;
constexpr static float s_lumi_cent_60 = 0.024;
constexpr static float s_lumi_cent_80 = 0.;
constexpr static float s_lumi_cent_140 = 27.769;
constexpr static float s_lumi_cent_200 = 139.732;
constexpr static float s_lumi_cent_260 = 521.863;
constexpr static float s_lumi_cent_320 = 2965.774;
constexpr static float s_lumi_cent_400 = 9021.331;
constexpr static float s_lumi_cent_450 = 29280.311;
constexpr static float s_lumi_cent_500 = 29280.311;

/** \brief Jet Resolution Smearering **/
// doing the matching from GEN to RECO
constexpr static float s_delta_R   = 0.3;
//constant numberstaken from https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution

#endif

//};
//}
