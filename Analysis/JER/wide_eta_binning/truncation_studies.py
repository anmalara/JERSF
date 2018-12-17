import sys
import os
import subprocess
import time
import numpy as np


import matplotlib
# matplotlib.use("Agg")
import matplotlib.pyplot as plt

path = "file/StandardPtBins/Fall17_17Nov2017_V31/AK4CHS/"

qcds = ["QCDPt","QCDHT"]
# qcds = ["QCDPt"]
# qcd  =  "QCDPt"
runs = ["RunB","RunC","RunBC","RunD","RunE","RunDE","RunF","RunDEF","RunBCDEF"]
# modes = ["standard", "gaustails", "gaustails_0.85", "gaustails_0.93", "gaustails_0.95","gaustails_0.97", "gaustails_0.985"]
labels = [0.985, 0.95, 0.85, 0.93, 0.95,0.97, 0.985]
labels = np.arange(0.8,1.0,0.005)

modes = []
for i in labels:
    modes.append("gaustails_"+str(i))

filled_markers = ['o', 'v', '^', '<', '>', '8', 's', 'p', '*', 'h', 'H', 'D', 'd', 'P', 'X']
colors = ["black", "blue", "red", "gold", "orange", "green", "pink", "lightblue", "violet", "lightgreen", "black", "blue"]

eta_bins = ["0.000", "0.522", "0.783", "1.131", "1.305", "1.740", "1.930", "2.043", "2.322", "2.500", "2.853", "2.964", "3.139", "5.191"];

for run in ["RunBCDEF"]:
    for qcd in qcds:
        SFs  = []
        errs = []
        SFs_SM  = []
        errs_SM = []
        SFs_FE  = []
        errs_FE = []

        for mode in modes:
            with open(path+mode+"/"+qcd+"/"+run+"/output/scalefactors_tex.txt", "U") as file:
                lines = file.readlines()
                SF_SM  = []
                err_SM = []
                SF_FE  = []
                err_FE = []
                for i, line in enumerate(lines):
                    if  not i in (5,7):
                        continue
                    line = line.replace("\\pm"," ")
                    line = line.replace("&"," ")
                    line = line.replace("\\\\"," ")
                    for j, el in enumerate(line.split()):
                        if j%2 ==0:
                            if i==5:
                              SF_SM.append(float(el))
                            else:
                              SF_FE.append(float(el))
                        else:
                          if i==5:
                            err_SM.append(float(el))
                          else:
                            err_FE.append(float(el))
                # print SF_SM
                # print SF_FE
                SFs_SM.append(SF_SM)
                SFs_FE.append(SF_FE)
                errs_SM.append(err_SM)
                errs_FE.append(err_FE)

        line = ""
        line += "\\\\\n"
        plt.clf()
        for j in range(0,10):
            array = []
            x = []
            for i in range(2,len(SFs_SM)):
                # print SFs_SM[i][j]
                # array.append(SFs_SM[i][j]/SFs_SM[-3][j])
                array.append(SFs_SM[i][j])
                x.append(labels[i])
            plt.plot(x, array, label= "["+eta_bins[j]+"-"+eta_bins[j+1]+"]", c=colors[j])
        plt.legend(loc='best', shadow=True)
        plt.xlim([0.80, 1.00])
        plt.grid(True, which='both')
        plt.xlabel("% truncation")
        plt.ylabel("SF/SF.985")
        plt.savefig("truncation_studies/SF_SM_"+qcd+"_trunc_few.png")
        plt.savefig("truncation_studies/SF_SM_"+qcd+"_trunc_few.pdf")
        plt.show()



        arrays = []
        plt.clf()
        for j in range(2,12):
            array = []
            x = []
            for i in range(2,len(SFs_FE)):
                # print SFs_FE[i][j]
                # array.append(SFs_FE[i][j]/SFs_FE[-3][j])
                array.append(SFs_FE[i][j])
                x.append(labels[i])
            plt.plot(x, array, label= "["+eta_bins[j+1]+"-"+eta_bins[j+2]+"]", c=colors[j])
            # plt.scatter(labels, array, label= "["+eta_bins[j+1]+"-"+eta_bins[j+2]+"]", linewidths=1, filled_markers=filled_markers[j])
        plt.legend(loc='best', shadow=True)
        plt.xlim([0.80, 1.00])
        plt.grid(True, which='both')
        plt.xlabel("% truncation")
        plt.ylabel("SF/SF.985")
        plt.savefig("truncation_studies/SF_FE_"+qcd+"_trunc_few.png")
        plt.savefig("truncation_studies/SF_FE_"+qcd+"_trunc_few.pdf")
        plt.show()
