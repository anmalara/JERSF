from createConfigFiles import *

def cont_event(paths ="./submittedJobs/" , JECVersions_Data=["Fall17_17Nov2017_V6"], JetLabels=["AK4CHS"], systematics=["", "PU", "JEC", "JER"]):
    count = 0
    for newJECVersion in JECVersions_Data:
        for newJetLabel in JetLabels:
            for sys in systematics+[""]:
                for dir in ["", "up", "down"]:
                    if sys == "" and dir != "":
                        continue
                    path = paths+newJECVersion+"/"+newJetLabel+"/"+sys+"/"+dir+"/"
                    for sample in sorted(os.listdir(path)):
                        if not ".xml" in sample:
                            continue
                        count += 1
    return count

@timeit
def condor_control(original_dir ="./submittedJobs/" , JECVersions_Data=["Fall17_17Nov2017_V6"], JetLabels=["AK4CHS"], systematics=["", "PU", "JEC", "JER"], internal_option="-l"):
    count = 0
    for newJECVersion in JECVersions_Data:
        for newJetLabel in JetLabels:
            for sys in systematics+[""]:
                for dir in ["", "up", "down"]:
                    if sys == "" and dir != "":
                        continue
                    path = original_dir+newJECVersion+"/"+newJetLabel+"/"+sys+"/"+dir+"/"
                    for sample in sorted(os.listdir(path)):
                        if not ".xml" in sample:
                            continue
                        count += 1
                        all_events = cont_event(original_dir, JECVersions_Data, JetLabels, systematics)
                        print "Already completed "+str(count)+" out of "+str(all_events)+" jobs --> "+str(float(count)/float(all_events)*100)+"%."
                        os.chdir(original_dir)
                        os.chdir(path)
                        if internal_option:
                            command = ['sframe_batch.py', internal_option, sample]
                        else:
                            command = ['sframe_batch.py', sample]
                        process = subprocess.Popen(command)
                        process.wait()
                        if internal_option == "-a":
                            time.sleep(5)
                        os.chdir(original_dir)


from createConfigFiles import *
@timeit
def delete_workdir(original_dir ="./SubmittedJobs/" , JECVersions_Data=["Fall17_17Nov2017_V6", "Fall17_17Nov2017_V10"], JetLabels=["AK4CHS", "AK8PUPPI"], systematics=["", "PU", "JEC", "JER"]):
    add_name = original_dir[original_dir.find("SubmittedJobs")+len("SubmittedJobs"):-1]
    for sample in ["DATA", "QCD"]:
        for newJECVersion in JECVersions_Data:
            for newJetLabel in JetLabels:
                for sys in systematics+[""]:
                    for dir in ["", "up", "down"]:
                        if sys == "" and dir != "":
                            continue
                        path = "/nfs/dust/cms/user/amalara/sframe_all/"+outdir+add_name+"_"+sample+"/"+newJECVersion+"/"+newJetLabel+"/"+sys+"/"+dir+"/"
                        if os.path.isdir(path):
                            for workdir in sorted(os.listdir(path)):
                                if "workdir" in workdir:
                                    cmd = "rm -fr %s" % (path+workdir)
                                    a = os.system(cmd)
                        path = original_dir+newJECVersion+"/"+newJetLabel+"/"+sys+"/"+dir+"/"
                        if os.path.isdir(path):
                            for workdir in sorted(os.listdir(path)):
                                if "workdir" in workdir:
                                    cmd = "rm -fr %s" % (path+workdir)
                                    a = os.system(cmd)




##################################################
#                                                #
#                   MAIN Program                 #
#                                                #
##################################################

QCD_process= []
QCD_process.append("QCDPt15to30")
QCD_process.append("QCDPt30to50")
QCD_process.append("QCDPt50to80")
QCD_process.append("QCDPt80to120")
QCD_process.append("QCDPt120to170")
QCD_process.append("QCDPt170to300")
QCD_process.append("QCDPt300to470")
QCD_process.append("QCDPt470to600")
QCD_process.append("QCDPt600to800")
QCD_process.append("QCDPt800to1000")
QCD_process.append("QCDPt1000to1400")
QCD_process.append("QCDPt1400to1800")
QCD_process.append("QCDPt1800to2400")
QCD_process.append("QCDPt2400to3200")
QCD_process.append("QCDPt3200toInf")

QCD_process.append("QCDPt15to7000_MB")
QCD_process.append("QCDPt15to30_MB")
QCD_process.append("QCDPt30to50_MB")
QCD_process.append("QCDPt50to80_MB")
QCD_process.append("QCDPt80to120_MB")
QCD_process.append("QCDPt120to170_MB")
QCD_process.append("QCDPt170to300_MB")
QCD_process.append("QCDPt300to470_MB")
QCD_process.append("QCDPt470to600_MB")
QCD_process.append("QCDPt600to800_MB")
QCD_process.append("QCDPt800to1000_MB")
QCD_process.append("QCDPt1000to1400_MB")
QCD_process.append("QCDPt1400to1800_MB")
QCD_process.append("QCDPt1800to2400_MB")
QCD_process.append("QCDPt2400to3200_MB")
QCD_process.append("QCDPt3200toInf_MB")

Data_process= []
Data_process.append("DATA_RunB")
Data_process.append("DATA_RunC")
Data_process.append("DATA_RunD")
Data_process.append("DATA_RunE")
Data_process.append("DATA_RunF")

Data_process.append("DATA_RunB_MB")
Data_process.append("DATA_RunC_MB")
Data_process.append("DATA_RunD_MB")
Data_process.append("DATA_RunE_MB")
Data_process.append("DATA_RunF_MB")

processes = QCD_process+Data_process

JECVersions_Data = ["Fall17_17Nov2017_V6", "Fall17_17Nov2017_V10", "Fall17_17Nov2017_V11"]
JetLabels = ["AK4CHS", "AK8PUPPI"]
systematics = ["PU", "JEC", "JER"]

original_file = "JER2017.xml"
outdir = "JER2017_v2"
original_dir_ = os.getcwd()

try:
    option = sys.argv[1]
except:
    option = ""

if option == "resubmit":
    internal_option = "-r"
elif option == "submit":
    internal_option = "-s"
elif option == "add" or option == "merge":
    internal_option = "-f"
elif option == "list":
    internal_option = "-l"
elif option == "new":
    internal_option = ""
elif option == "remove" or option == "delete":
    internal_option = ""
else:
    internal_option = ""



def main_program(option="", internal_option="", processes=[], JECVersions_Data=[], JECVersions_MC=[], JetLabels=[], systematics=[], original_dir="./SubmittedJobs/", original_file="JER2017.xml", isMB=False, test_trigger=False, isThreshold=False, isLowPt=False):
    if option == "new":
        createConfigFiles(processes, JECVersions_Data, JECVersions_MC, JetLabels, systematics, original_dir, original_file, outdir, isMB, test_trigger, isThreshold,isLowPt)
    elif option == "remove" or option == "delete":
        delete_workdir(original_dir, JECVersions_Data, JetLabels, systematics)
    else:
        condor_control(original_dir, JECVersions_Data, JetLabels, systematics, internal_option)


original_dir = original_dir_
original_dir += "/SubmittedJobs/"
isLowPt = False
isMB = False
test_trigger = False
isThreshold = False
JECVersions_Data = ["Fall17_17Nov2017_V27"]
JECVersions_MC = ["Fall17_17Nov2017_V23"]
# JECVersions_Data = ["Fall17_17Nov2017_V10"]
# JECVersions_MC = ["Fall17_17Nov2017_V10"]
JetLabels = ["AK4CHS"]
systematics = ["PU", "JEC"]
# main_program(option, internal_option, processes, JECVersions_Data, JECVersions_MC, JetLabels, systematics, original_dir, original_file, isMB, test_trigger, isThreshold, isLowPt)
original_dir = original_dir_
original_dir += "/SubmittedJobs_LowPt/"
isLowPt = True
main_program(option, internal_option, processes, JECVersions_Data, JECVersions_MC, JetLabels, systematics, original_dir, original_file, isMB, test_trigger, isThreshold, isLowPt)


# original_dir = original_dir_
# original_dir += "/SubmittedJobs_MB/"
# isMB = True
# test_trigger = False
# isThreshold = False
# JECVersions_Data = ["Fall17_17Nov2017_V24"]
# JECVersions_MC = ["Fall17_17Nov2017_V23"]
# JetLabels = ["AK4CHS"]
# systematics = []
# main_program(option, internal_option, processes, JECVersions_Data, JECVersions_MC, JetLabels, systematics, original_dir, original_file, isMB, test_trigger, isThreshold)


original_dir = original_dir_
original_dir += "/SubmittedJobs_MB_test/"
isMB = True
test_trigger = True
isThreshold = False
JECVersions_Data = ["Fall17_17Nov2017_V24"]
JECVersions_MC = ["Fall17_17Nov2017_V23"]
JetLabels = ["AK4CHS"]
systematics = []
# main_program(option, internal_option, processes, JECVersions_Data, JECVersions_MC, JetLabels, systematics, original_dir, original_file, isMB, test_trigger, isThreshold)


original_dir = original_dir_
original_dir += "/SubmittedJobs_Threshold/"
isMB = False
test_trigger = False
isThreshold = True
JECVersions_Data = ["Fall17_17Nov2017_V24"]
JECVersions_MC = ["Fall17_17Nov2017_V23"]
JetLabels = ["AK4CHS"]
systematics = []
# main_program(option, internal_option, processes, JECVersions_Data, JECVersions_MC, JetLabels, systematics, original_dir, original_file, isMB, test_trigger, isThreshold)
