import sys
import os
import subprocess
import time

def main_program(path="", list_path="", out_path="", JECVersions=[], JetLabels=[], systematics=[], samples=[]):
    list_path_=list_path[:-1]+path[path.find("JER2017")+len("JER2017"):path.find("DATA")-1]+"/"
    out_path_=out_path[:-1]+path[path.find("JER2017")+len("JER2017"):path.find("DATA")-1]+"/"
    for newJECVersion in JECVersions:
        for newJetLabel in JetLabels:
            for sys in set(systematics+["", "alpha"]):
                if sys == "alpha":
                    alpha_cut = 10
                else:
                    alpha_cut = 15
                for dir in ["", "up", "down"]:
                    if (sys == "" and dir != "") or (sys == "alpha" and dir != "") or ((sys != "" and sys != "alpha") and dir == ""):
                        continue
                    pattern = newJECVersion+"/"+newJetLabel+"/"+sys+"/"+dir+"/"
                    if sys == "" or sys == "alpha":
                        pattern = newJECVersion+"/"+newJetLabel+"/"
                    source_path = path+pattern
                    if not os.path.isdir(source_path):
                        continue
                    if sys == "alpha":
                        pattern = newJECVersion+"/"+newJetLabel+"/"+sys+"/"
                    if not os.path.isdir(list_path_+pattern):
                        os.makedirs(list_path_+pattern)
                    for sample in samples:
                        run_list = list_path_+pattern+"file_DataRun"+sample+".txt"
                        with open(run_list, "w") as outputfile:
                            for writable in sorted(os.listdir(source_path)):
                                if not os.path.isfile(source_path+writable):
                                    continue
                                for el in sample:
                                    if "Run"+el in writable and ".root" in writable:
                                        outputfile.write(source_path+writable+"\n")
                        if not os.path.isfile(run_list):
                            continue
                        outdir = out_path_+pattern+"Run"+sample+"/"
                        if not os.path.isdir(outdir):
                            os.makedirs(outdir)
                        print "RUNNING ON ", run_list
                        temp_time=time.time()
                        cmd = "cp MySelector_full_Single.C MySelector.C"
                        a = os.system(cmd)
                        cmd = 'sed -i -e """s/jet_threshold=15/jet_threshold=%s/g" MySelector.C' % (alpha_cut)
                        a = os.system(cmd)
                        if "MB" in path:
                            cmd = 'sed -i -e """s/pt_bins_Si/pt_bins_MB/g" MySelector.C'
                            a = os.system(cmd)
                        cmd = "make clean"
                        a = os.system(cmd)
                        time.sleep(2)
                        cmd = "make"
                        a = os.system(cmd)
                        time.sleep(2)
                        logfilename = "log.txt"
                        f = open(logfilename,'w')
                        command = ['./Analysis.x', run_list]
                        process = subprocess.Popen(command, stdout=f)
                        process.wait()
                        f.close()
                        cmd = "mv *.root %s" % (outdir)
                        a = os.system(cmd)
                        cmd = "mv *.txt %s" % (outdir)
                        a = os.system(cmd)
                        print ("time needed: "+str((time.time()-temp_time))+" s")


samples = ["B","C","D","E","F","BC","DE","DEF","BCDEF"]
JECVersions = ["Fall17_17Nov2017_V6", "Fall17_17Nov2017_V10"]
JetLabels = ["AK4CHS", "AK8PUPPI"]
systematics = ["PU", "JEC"]
common_path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/hist_preparation/data/"
list_path = common_path+"lists/Single/"
out_path  = common_path+"wide_eta_bin/file/Single/"
os.chdir(common_path+"wide_eta_bin/")

sframe_ = "/nfs/dust/cms/user/amalara/sframe_all/"

# for el in ["JER2017_DATA", "JER2017_MB_DATA", "JER2017_MB_test_DATA", "JER2017_Threshold_DATA"]:
for el in ["JER2017_Threshold_DATA"]:
    path = sframe_+el+"/"
    main_program(path, list_path, out_path, JECVersions, JetLabels, systematics, samples)
