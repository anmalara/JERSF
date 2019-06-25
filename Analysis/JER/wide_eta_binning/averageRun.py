import sys
import os
import numpy as np

import ROOT
#
# xml_path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_102X_v1/CMSSW_10_2_10/src/UHH2/common/datasets/RunII_102X_v1/2018/"
# # xml_file = "DATA_JetHT2018A_TMP.xml"
# # xml_file = "DATA_JetHT2018B.xml"
# # xml_file = "DATA_JetHT2018C.xml"
# xml_file = "DATA_JetHT2018D_TMP.xml"

#
#
# with open(xml_path+xml_file, "U") as file:
#   lines = file.readlines()
#   n_events = 0
#   for line in lines:
#       fname = line.replace('<In FileName="',"").replace('" Lumi="0.0"/>',"").replace("\n","")
#       print fname
#       f = ROOT.TFile(fname)
#       n_events += f.Get('AnalysisTree').GetEntriesFast()
#       f.Close()


n_events = {"RunA":137792959.0, "RunB":78225208, "RunC":70007804, "RunD":139850752.0, "RunABC":286025971.0}
lumi = {"RunD":31.93, "RunABC":28.04}

path = "file/StandardPtBins/Autumn18_V10/AK4CHS/standard/QCDHT/"

# run = "RunD"
# file_ = "SF_final_tex.txt"
file_ = "SF_final_twiki.txt"

SFs = {}
for run in lumi:
    print run
    with open(path+run+"/"+file_, "U") as file:
        lines = file.readlines()
        SF = []
        SF.append(np.expand_dims(np.array(lines[0].split()[-1].replace("|"," ").split()),axis=0))
        for line in lines[1:]:
            print line
            # SF.append(np.expand_dims(np.array([float(i) for i in line.replace("{","").replace("}","").replace(",","").split()]),axis=0))
            # for i in line.split()[2].replace("|"," ").split():
            SF.append(np.expand_dims(np.array([float(i) for i in line.split()[-1].replace("|"," ").split()]),axis=0))
        SFs[run] = np.concatenate(SF)

SF = []
for i in range(1,5):
    temp = []
    for eta in range(SFs[run].shape[1]):
        temp.append((float(SFs["RunABC"][i][eta])*lumi["RunABC"]+float(SFs["RunD"][i][eta])*lumi["RunD"])/(lumi["RunABC"]+lumi["RunD"]))
    SF.append(np.expand_dims(np.array(temp),axis=0))

SFs["RunABCD"] = np.concatenate(SF)



lines = []
last = 0.
line = "|  *abs(eta) region* |"
for eta in range(SFs["RunABCD"].shape[1]):
    line += "|"+SFs["RunABC"][0][eta]+"|"

lines.append(line)

line = "|  *Data/MC SF*      |"
for eta in range(SFs["RunABCD"].shape[1]):
    line += "|"+str(round(SFs["RunABCD"][0][eta],3))+"|"

lines.append(line)

line = "|  *Stat.Unc*        |"
for eta in range(SFs["RunABCD"].shape[1]):
    line += "|"+str(round(SFs["RunABCD"][1][eta],3))+"|"

lines.append(line)


line = "|  *Syst.Unc*        |"
for eta in range(SFs["RunABCD"].shape[1]):
    line += "|"+str(round(SFs["RunABCD"][2][eta],3))+"|"

lines.append(line)


line = "|  *Total.Unc*       |"
for eta in range(SFs["RunABCD"].shape[1]):
    line += "|"+str(round(SFs["RunABCD"][3][eta],3))+"|"

lines.append(line)

with open(path+run+"/SF_final_twiki_average.txt", "w") as file:
    for line in lines:
        file.write(line+"\n")



for eta in range(SFs["RunABCD"].shape[1]):
    print (np.sqrt(SFs["RunABCD"][1][eta]**2+SFs["RunABCD"][2][eta]**2)-SFs["RunABCD"][3][eta])/SFs["RunABCD"][3][eta]*100
