import sys
import os
import subprocess
import time
import numpy as np

path = "file/StandardPtBins/Fall17_17Nov2017_V31/AK4CHS/standard/"

qcds = ["QCDPt","QCDHT"]
qcds = ["QCDPt"]
# qcd  =  "QCDPt"
runs = ["RunB","RunC","RunBC","RunD","RunE","RunDE","RunF","RunDEF","RunBCDEF"]

eta_bins = ["0.000", "0.522", "0.783", "1.131", "1.305", "1.740", "1.930", "2.043", "2.322", "2.500", "2.853", "2.964", "3.139", "5.191"];

for runs in [["RunB","RunC"],["RunD","RunE"],["RunD","RunE","RunF"], ["RunB","RunC","RunD","RunE"], ["RunB","RunC","RunD","RunE"], ["RunBC","RunDE"], ["RunBC","RunDE", "RunF"], ["RunDE", "RunF"], ["RunDE", "RunDEF"], ["RunB","RunC","RunD","RunE","RunF"], ["RunF","RunF_ECAL"]]:
  for qcd in qcds:
    SFs_SM  = []
    errs_SM = []
    SFs_FE  = []
    errs_FE = []

    SFs  = []
    errs = []

    for run in runs:
      with open(path+qcd+"/"+run+"/output/scalefactors_tex.txt", "U") as file:
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
        SFs_SM.append(SF_SM)
        SFs_FE.append(SF_FE)
        errs_SM.append(err_SM)
        errs_FE.append(err_FE)

      with open(path+qcd+"/"+run+"/SF_final_tex.txt", "U") as file:
        lines = file.readlines()
        SF  = []
        err = []
        for i, line in enumerate(lines):
            line = line.replace("{","").replace("}","").replace(",","").split()
            SF.append(float(line[1]))
            err.append(1-float(line[3])/float(line[1]))
        SFs.append(SF)
        errs.append(err)

    with open(path+qcd+"/table_SM.txt", "w") as outputfile:
      line = ""
      for el in runs:
        line += " & "+ el
      line += "\\\\\n"
      outputfile.write(line)
      for j in range(0,10):
        line = "$["+eta_bins[j]+"-"+eta_bins[j+1]+"]$ & "
        for i in range(len(SFs_SM)):
          line += "$"+str(round(SFs_SM[i][j],3))
          for z in range(0,5-len(str(round(SFs_SM[i][j],3)))):
            line += "0"
          line += " \pm "
          line += str(round(errs_SM[i][j],3))
          for z in range(0,5-len(str(round(errs_SM[i][j],3)))):
            line += "0"
          line += "$ & "
        line = line[:-2]
        line += "\\\\\n"
        outputfile.write(line)

    with open(path+qcd+"/table_FE.txt", "w") as outputfile:
      line = ""
      for el in runs:
        line += " & "+ el
      line += "\\\\\n"
      outputfile.write(line)
      for j in range(2,12):
        line = "$["+eta_bins[j+1]+"-"+eta_bins[j+2]+"]$ & "
        for i in range(len(SFs_FE)):
          line += "$"+str(round(SFs_FE[i][j],3))
          for z in range(0,5-len(str(round(SFs_FE[i][j],3)))):
            line += "0"
          line += " \pm "
          line += str(round(errs_FE[i][j],3))
          for z in range(0,5-len(str(round(errs_FE[i][j],3)))):
            line += "0"
          line += "$ & "
        line = line[:-2]
        line += "\\\\\n"
        outputfile.write(line)

    with open(path+qcd+"/table_final.txt", "w") as outputfile:
      line = ""
      for el in runs:
        line += " & "+ el
      line += "\\\\\n"
      outputfile.write(line)
      SFs_array = np.array(SFs)
      mean = np.mean(SFs_array, axis=0)
      std = np.std(SFs_array, axis=0)
      print "\teta\t  ", runs
      for j in range(0,13):
        line = "$["+eta_bins[j]+"-"+eta_bins[j+1]+"]$ & "
        for i in range(len(SFs)):
          line += str(round(SFs[i][j],2)) + " & "
        line += str(round(100*std[j]/mean[j],2)) + " \% \\\\"
        print line
        # print line, [round(SFs[i][j],2) for i in range(len(SFs))], round(mean[j],2), round(100*std[j]/mean[j],2), "%"
        for i in range(len(SFs)):
          line += "$"+str(round(SFs[i][j],3))
          for z in range(0,5-len(str(round(SFs[i][j],3)))):
            line += "0"
          line += " \pm "
          line += str(round(100*errs[i][j],1))
          for z in range(0,5-len(str(round(100*errs[i][j],1)))):
            line += "0"
          line += "\%$ & "
        line = line[:-2]
        line += "\\\\\n"
        outputfile.write(line)
