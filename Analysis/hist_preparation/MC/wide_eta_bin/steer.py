import sys
import os
import time

sys.path.append("/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/PersonalCode/")
from parallelise import *

def main_program(path="", list_path="", out_path="", JECVersions=[], JetLabels=[], systematics=[], samples=[], barrel_check = 0):
  list_path_=list_path[:-1]+path[path.find("JER2017")+len("JER2017"):path.find("QCD")-1]+"/"
  out_path_=out_path[:-1]+path[path.find("JER2017")+len("JER2017"):path.find("QCD")-1]+"/"
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
            run_list = list_path_+pattern+"file_QCD"+sample+".txt"
            with open(run_list, "w") as outputfile:
              for writable in sorted(os.listdir(source_path)):
                if not os.path.isfile(source_path+writable):
                  continue
                if (sample=="flat" and "15to7000" in writable) or (sample!="flat" and not "15to7000" in writable):
                  outputfile.write(source_path+writable+"\n")
            if not os.path.isfile(run_list):
              continue
            outdir = out_path_+pattern+sample+"/"
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
            if barrel_check>0:
              if barrel_check == 1:
                cmd = 'sed -i -e """s/s_eta_barr/0.522/g" MySelector.C'
                outdir = out_path_+pattern+sample+"_barrel_check_1/"
              if barrel_check == 2:
                cmd = 'sed -i -e """s/s_eta_barr/0.783/g" MySelector.C'
                outdir = out_path_+pattern+sample+"_barrel_check_2/"
              if barrel_check == 3:
                cmd = 'sed -i -e """s/s_eta_barr/1.131/g" MySelector.C'
                outdir = out_path_+pattern+sample+"_barrel_check_3/"
              a = os.system(cmd)
              if not os.path.isdir(outdir):
                os.makedirs(outdir)
            cmd = "cp Makefile %s" % (outdir)
            a = os.system(cmd)
            cmd = "cp *.C %s" % (outdir)
            a = os.system(cmd)
            cmd = "cp *.h %s" % (outdir)
            a = os.system(cmd)
            os.chdir(outdir)
            time.sleep(3)
            cmd = "make clean"
            a = os.system(cmd)
            cmd = "make"
            a = os.system(cmd)
            logfilename = "log.txt"
            f = open(logfilename,'w')
            cmd = './Analysis.x %s >> log.txt &' % (run_list)
            # print cmd
            # a = os.system(cmd)
            command = [outdir+"Analysis.x", run_list, outdir]
            list_processes.append(command)
            list_logfiles.append(outdir+"log.txt")
            f.close()
            os.chdir(common_path+"wide_eta_bin/")
            print ("time needed: "+str((time.time()-temp_time))+" s")


samples = ["","flat"]
JECVersions = ["Fall17_17Nov2017_V6", "Fall17_17Nov2017_V10"]
JetLabels = ["AK4CHS", "AK8PUPPI"]
systematics = ["PU", "JEC"]
common_path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/hist_preparation/MC/"
list_path = common_path+"lists/Single/"
out_path  = common_path+"wide_eta_bin/file/Single/"
os.chdir(common_path+"wide_eta_bin/")

sframe_ = "/nfs/dust/cms/user/amalara/sframe_all/"

list_processes = []
list_logfiles = []
# for el in ["JER2017_QCD", "JER2017_MB_QCD", "JER2017_MB_test_QCD", "JER2017_Threshold_QCD"]:
for el in ["JER2017_QCD", "JER2017_MB_QCD"]:
  path = sframe_+el+"/"
  samples = [""]
  JECVersions = ["Fall17_17Nov2017_V10"]
  JetLabels = ["AK4CHS"]
  systematics = ["PU", "JEC"]
  main_program(path, list_path, out_path, JECVersions, JetLabels, systematics, samples)
  if el == "JER2017_QCD":
    main_program(path, list_path, out_path, JECVersions, JetLabels, [], samples, barrel_check = 1)
    main_program(path, list_path, out_path, JECVersions, JetLabels, [], samples, barrel_check = 2)
    main_program(path, list_path, out_path, JECVersions, JetLabels, [], samples, barrel_check = 3)

parallelise(list_processes, 3, list_logfiles)
