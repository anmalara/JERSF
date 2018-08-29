import sys
import os
import subprocess
import time

def getLabel(sample):
    if sample == "B":
        LABEL_LUMI_INV_FB = "[MC 94X] Run2017B 4.77 fb^{-1}"
    elif sample == "C":
        LABEL_LUMI_INV_FB = "[MC 94X] Run2017C 9.58 fb^{-1}"
    elif sample == "D":
        LABEL_LUMI_INV_FB="[MC 94X] Run2017D 4.22 fb^{-1}"
    elif sample == "E":
        LABEL_LUMI_INV_FB="[MC 94X] Run2017E 9.26 fb^{-1}"
    elif sample == "F":
        LABEL_LUMI_INV_FB="[MC 94X] Run2017F 13.46 fb^{-1}"
    elif sample == "BC":
        LABEL_LUMI_INV_FB="[MC 94X] Run2017BC 14.35 fb^{-1}"
    elif sample == "DE":
        LABEL_LUMI_INV_FB="[MC 94X] Run2017DE 13.48 fb^{-1}"
    elif sample == "DEF":
        LABEL_LUMI_INV_FB="[MC 94X] Run2017DEF 26.95 fb^{-1}"
    elif sample == "BCDEF":
        LABEL_LUMI_INV_FB="[MC 94X] 2017 41.3 fb^{-1}"
    else:
        LABEL_LUMI_INV_FB="[MC 94X] (2017)"
    return LABEL_LUMI_INV_FB


def main_function(gaustails=False, shiftForPLI="central"):
    outdir = out_path+pattern+run+"/"
    gaustail_num = 0.985
    shiftForPLI_num = 0.0
    if gaustails:
        outdir = out_path+newJECVersion+"/"+newJetLabel+"/gaustails/"+run+"/"
        gaustail_num = 0.95
    if shiftForPLI=="up":
        outdir = out_path+newJECVersion+"/"+newJetLabel+"/PLI/up/"+run+"/"
        shiftForPLI_num = 0.25
    if shiftForPLI=="down":
        outdir = out_path+newJECVersion+"/"+newJetLabel+"/PLI/down/"+run+"/"
        shiftForPLI_num = -0.25
    print "outdir ", outdir
    if os.path.isdir(outdir):
        for el in sorted(os.listdir(outdir)):
            cmd = "rm -fr %s" % (outdir+el)
            a = os.system(cmd)
    else:
        os.makedirs(outdir)
    cmd = "cp mainRun.cxx %s" % (outdir)
    a = os.system(cmd)
    cmd = "cp functions.C %s" % (outdir)
    a = os.system(cmd)
    cmd = "cp /nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/PersonalCode/tdrstyle_all.C %s" % (outdir)
    a = os.system(cmd)
    os.chdir(outdir)
    os.makedirs("pdfy")
    os.makedirs("pdfy/MCTruth")
    os.makedirs("pdfy/SFs")
    os.makedirs("pdfy/NSC_SFs")
    os.makedirs("pdfy/JERs")
    os.makedirs("pdfy/widths")
    os.makedirs("pdfy/maps")
    os.makedirs("ClosureTest")
    os.makedirs("output")
    os.makedirs("output/asymmetries")
    print pattern+run
    temp_time=time.time()
    # f = open("log.txt",'w')
    MC_type = '\\"MC\\"'
    data_type = '\\"Data\\"'
    trigger_type = '\\"'+study+'\\"'
    cmd = 'root -l -b -q "mainRun.cxx(false, %s, %s, %s, %s , %s, %s, %s, %s)" >> log.txt ' % (MC_file, Data_file, LABEL_LUMI_INV_FB, MC_type, data_type, trigger_type, gaustail_num, shiftForPLI_num)
    print cmd
    a = os.system(cmd)
    # command = ["root", "-l", "-b", "-q", test]
    # command = ["root", "-l", "-b", "-q", '"mainRun.cxx(false,'+MC_file+','+Data_file+','+LABEL_LUMI_INV_FB+','+MC_type+','+data_type+','+trigger_type+')"']
    # command = ["root", "-l", "-b", "-q"]
    # print command
    # process = subprocess.Popen(command, stdout=f)
    # process = subprocess.Popen(command)
    # process.wait()
    # f.close()
    print ("time needed: "+str((time.time()-temp_time))+" s")
    os.chdir(common_path)













studies = ["Single/","Single_MB/", "Single_MB_test/","Single_Threshold/"]
samples = ["B","C","D","E","F","BC","DE","DEF","BCDEF"]
JECVersions=["Fall17_17Nov2017_V6", "Fall17_17Nov2017_V10"]
JetLabels=["AK4CHS", "AK8PUPPI"]
systematics=["", "PU", "JEC", "alpha"]
dirs = ["", "up", "down"]


samples = ["BCDEF"]
JECVersions=["Fall17_17Nov2017_V10"]
JetLabels=["AK4CHS"]
# systematics=[""]
# dirs = [""]
common_path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/JER/wide_eta_binning/"
source_path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/hist_preparation/"

study = "Single/"
studies = ["Single/"]

for study in studies:
    out_path  = common_path+"file/"+study
    for newJECVersion in JECVersions:
        for newJetLabel in JetLabels:
            for sys in systematics:
                for dir in dirs:
                    if (sys == "" and dir != "") or (sys == "alpha" and dir != "") or ((sys != "" and sys != "alpha") and dir == ""):
                        continue
                    pattern = newJECVersion+"/"+newJetLabel+"/"+sys+"/"+dir+"/"
                    if sys == "":
                        pattern = newJECVersion+"/"+newJetLabel+"/standard/"
                    if sys == "alpha":
                        pattern = newJECVersion+"/"+newJetLabel+"/"+sys+"/"
                    print "pattern ", pattern
                    for sample in samples:
                        run = "Run"+sample
                        LABEL_LUMI_INV_FB=getLabel(sample)
                        LABEL_LUMI_INV_FB = '\\"'+LABEL_LUMI_INV_FB+'\\"'
                        # print "LABEL_LUMI_INV_FB ", LABEL_LUMI_INV_FB
                        MC_file   = '\\"'+source_path+"MC/wide_eta_bin/file/"+study+pattern.replace("/standard","")+"histograms_mc_incl_full.root"+'\\"'
                        Data_file = '\\"'+source_path+"data/wide_eta_bin/file/"+study+pattern.replace("/standard","")+run+"/histograms_data_incl_full.root"+'\\"'
                        # print "MC_file ", MC_file
                        # print "Data_file ", Data_file
                        if not os.path.isfile(str(MC_file.replace("\\","").strip("\""))) or not os.path.isfile(str(Data_file.replace("\\","").strip("\""))):
                            # print str(MC_file.replace("\\","").strip("\""))
                            # print str(Data_file.replace("\\","").strip("\""))
                            continue
                        main_function(gaustails=False)
                        if sys == "":
                            main_function(gaustails=True, shiftForPLI="central")
                            main_function(gaustails=False, shiftForPLI="up")
                            main_function(gaustails=False, shiftForPLI="down")
