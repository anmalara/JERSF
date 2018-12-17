import os, sys, time
import numpy as np
import math
from array import array
from ROOT import *

eta_binning = [ 0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.139, 5.191 ]
eta_bin_center = []
eta_bin_center_ST = []
eta_bin_center_FE = []
for i in range(len(eta_binning)-4):
  eta_bin_center.append((eta_binning[i+1] + eta_binning[i])/2)
  print(eta_bin_center[i])
for i in range(len(eta_binning)-4):
  eta_bin_center_ST.append((eta_binning[i+1] + eta_binning[i])/2)
  eta_bin_center_FE.append((eta_binning[i+4] + eta_binning[i+3])/2)


stat = array('d', [0.0052,0.0113,0.0084,0.0197,0.0093,0.0334,0.0748,0.0381,0.1163,0.0757])
JECup = array('d', [-0.0017,-0.0005,0.0010,0.0080,0.0053,0.0050,0.0042,-0.0089,-0.0515,0.0060])
JECdown = array('d', [0.0047,0.0033,0.0075,0.0124,-0.0037,-0.0161,0.0113,-0.0059,-0.0021,0.0416])
PUup = array('d', [0.0015,0.0003,0.0081,0.0024,0.0071,0.0046,0.0085,0.0057,0.0131,-0.0113])
PUdown = array('d', [-0.0012,0.0011,-0.0085,-0.0037,-0.0067,-0.0054,-0.0075,-0.0040,-0.0134,0.0039])
PLIup = array('d', [-0.0147,-0.0173,-0.0100,-0.0097,-0.0063,-0.0070,0.0452,0.0029,0.1083,-0.0038])
PLIdown = array('d', [0.0099,0.0116,0.0070,0.0069,0.0045,0.0049,0.0393,0.0132,0.0905,0.0264])
ptCut10 = array('d', [-0.0205,-0.0186,-0.0200,-0.0163,-0.0120,-0.0285,0.0108,-0.0067,0.1541,-0.0018])
trunc = array('d', [-0.0148,-0.0135,-0.0059,-0.0072,-0.0107,-0.0074,0.0285,0.0184,0.1671,0.0390])
pTdep = array('d', [0.0561,0.0561,0.0561,0.0887,0.0887,0.0887,0.0887,0.0887,0.0887,0.0887])
ST_FEdiff = array('d', [0.0000,0.0000,0.0000,0.0246,0.0057,0.0197,0.0072,0.0435,0.0835,0.1249])
#combined = array('d', [0.0642,0.0642,0.0627,0.1012,0.0981,0.1047,0.1101,0.1098,0.2227,0.1997, 0,0])
combined = array('d', [0.0642,0.0642,0.0627,0.0925,0.0912,0.0956,0.1125,0.0927,0.2872,0.1096, 0,0])

st_vec = [ stat, JECup, JECdown, PUup, PUdown, PLIup, PLIdown, ptCut10 ,trunc, pTdep, ST_FEdiff, combined]

FE_stat = array('d', [0.0125,0.0106,0.0191,0.0276,0.0231,0.0464,0.0481,0.0648,0.0197,0.0386])
FE_JECup = array('d', [-0.0034,0.0098,0.0018,0.0138,0.0214,0.0101,0.0751,0.0729,0.0083,0.0467])
FE_JECdown = array('d', [0.0097,0.0027,0.0039,-0.0035,0.0056,0.0085,0.1020,0.1031,-0.0028,-0.0060])
FE_PUup = array('d', [0.0031,0.0042,0.0083,0.0033,0.0111,0.0103,0.0151,0.0226,0.0061,0.0058])
FE_PUdown = array('d', [-0.0010,-0.0011,-0.0177,-0.0140,-0.0083,-0.0041,-0.0105,-0.0008,0.0192,0.0056])
FE_PLIup = array('d', [-0.0110,-0.0103,-0.0115,-0.0142,-0.0190,-0.0418,-0.0687,-0.0475,0.0172,-0.0161])
FE_PLIdown = array('d', [0.0075,0.0071,0.0077,0.0095,0.0131,0.0275,0.0474,0.0367,0.0196,0.0141])
FE_ptCut10 = array('d', [-0.0136,-0.0124,-0.0280,-0.0191,-0.0172,-0.0478,-0.0352,-0.0163,-0.0183,-0.0666])
FE_trunc = array('d', [-0.0010,0.0145,0.0204,-0.0106,0.0072,0.0200,0.0772,0.0479,0.0105,-0.0041])
FE_pTdep = array('d', [0.1016,0.1016,0.1016,0.1016,0.1016,0.1016,0.1016,0.1161,0.1161,0.1161])
FE_STdiff = array('d', [0.0246,0.0057,0.0197,0.0072,0.0435,0.0835,0.1249,0.0000,0.0000,0.0000])
#FE_combined = array('d', [0.1012,0.0981,0.1047,0.1101,0.1098,0.2227,0.1997,0.1900,0.1228,0.1437])
FE_combined = array('d', [0.1039,0.1047,0.1101,0.1072,0.1090,0.1257,0.2022,0.1900,0.1228,0.1437])

fe_vec = [ FE_stat, FE_JECup, FE_JECdown, FE_PUup, FE_PUdown, FE_PLIup, FE_PLIdown, FE_ptCut10 ,FE_trunc, FE_pTdep, FE_STdiff, FE_combined]

c1 = TCanvas( 'c1', 'Example', 200, 10, 700, 500 )

#c1.SetFillColor( 42 )
c1.SetGrid()

x, y , ex, ey= array( 'd' ), array( 'd' ), array( 'd' ), array( 'd' )

for i in range( len(eta_bin_center_ST) ):
   x.append( eta_bin_center_ST[i] )
   y.append( 0. )
   ex.append( 0.2 )
   ey.append( stat[i] )
   print(' i %i %f %f %f %f ' % (i,x[i],y[i],ex[i],ey[i]))

x.append(2.6766)
y.append( 0. )
ex.append( 0.2 )

x.append(4.)
y.append( 0. )
ex.append( 0.2 )

comb_gr = TGraphErrors( 12, x, y, ex, combined )
comb_gr.SetLineColor( 2 )
comb_gr.GetYaxis().SetRangeUser( -0.2,0.3 )
comb_gr.GetXaxis().SetRangeUser( 0.0,4.00 )
comb_gr.SetLineWidth( 4 )
comb_gr.SetMarkerColor( 4 )
comb_gr.SetMarkerStyle( 21 )
comb_gr.SetTitle( 'Std meth  uncertainties' )
comb_gr.GetXaxis().SetTitle( '#eta' )
comb_gr.GetYaxis().SetTitle( 'JER uncertainty' )
comb_gr.SetFillColor(6);
comb_gr.SetFillStyle(3002);
gStyle.SetOptTitle(0)
comb_gr.Draw("a3");

names = ["stat","JECup","JECdown","PUup","PUdown","PLIup","PLIdown","ptCut10","trunc","pTdep", "stand-fwd diff", "combined"]
gr_vec = []

for i in range(len(st_vec)):
  gr_vec.append( TGraph( 10, x, st_vec[i] ))
  gr_vec[i].SetFillColor( kWhite )
  gr_vec[i].SetLineWidth( 2 )
  if i != 9:
    gr_vec[i].SetMarkerColor( i+1 )
    gr_vec[i].SetLineColor( i+1 )
  else:
    gr_vec[i].SetMarkerColor( 13 )
    gr_vec[i].SetLineColor( 13 )
  gr_vec[i].SetTitle(names[i])
  gr_vec[i].SetMarkerStyle( 20+i )
  gr_vec[i].Draw("LP");

gPad.BuildLegend(0.65,0.2,0.9,0.8)
# TCanvas.Update() draws the frame, after which one can change it
c1.Update()
#c1.GetFrame().SetFillColor( 21 )
#c1.GetFrame().SetBorderSize( 12 )
#c1.Modified()
#c1.Update()
c1.Print("ST_unc.pdf","pdf")




x2, y2 , ex2, ey2= array( 'd' ), array( 'd' ), array( 'd' ), array( 'd' )

for i in range( len(eta_bin_center_FE) ):
   x2.append( eta_bin_center_FE[i] )
   y2.append( 0. )
   ex2.append( 0.2 )
   ey2.append( FE_stat[i] )
   print(' i %i %f %f %f %f ' % (i,x2[i],y2[i],ex2[i],ey2[i]))

x2.append(4.166)
y2.append( 0. )
ex2.append( 0.2 )

x2.append(6.)
y2.append( 0. )
ex2.append( 0.2 )

comb_gr2 = TGraphErrors( 12, x2, y2, ex2, FE_combined )
comb_gr2.SetLineColor( 2 )
comb_gr2.GetYaxis().SetRangeUser( -0.2,0.23 )
comb_gr2.GetXaxis().SetRangeUser( 1.0,6.0 )
comb_gr2.SetLineWidth( 4 )
comb_gr2.SetMarkerColor( 4 )
comb_gr2.SetMarkerStyle( 21 )
comb_gr2.GetXaxis().SetTitle( '#eta' )
comb_gr2.GetYaxis().SetTitle( 'JER uncertainty' )
comb_gr2.SetFillColor(6);
comb_gr2.SetFillStyle(3002);
comb_gr2.Draw("a3");
comb_gr2.SetTitle( 'Fw ext uncertainties' )

names = ["stat","JECup","JECdown","PUup","PUdown","PLIup","PLIdown","ptCut10","trunc","pTdep", "stand-fwd diff", "combined"]
gr_vec2 = []

for i in range(len(st_vec)):
  gr_vec2.append( TGraph( 10, x2, fe_vec[i] ))
  gr_vec2[i].SetFillColor( kWhite )
  gr_vec2[i].SetLineWidth( 2 )
  if i != 9:
    gr_vec2[i].SetMarkerColor( i+1 )
    gr_vec2[i].SetLineColor( i+1 )
  else:
    gr_vec2[i].SetMarkerColor( 13 )
    gr_vec2[i].SetLineColor( 13 )
  gr_vec2[i].SetTitle(names[i])
  gr_vec2[i].SetMarkerStyle( 20+i )
  gr_vec2[i].Draw("LP");

gPad.BuildLegend(0.65,0.2,0.9,0.8)
# TCanvas.Update() draws the frame, after which one can change it
c1.Update()
#c1.GetFrame().SetFillColor( 21 )
#c1.GetFrame().SetBorderSize( 12 )
#c1.Modified()
#c1.Update()
c1.Print("FE_unc.pdf","pdf")
