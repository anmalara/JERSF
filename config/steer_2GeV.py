from createConfigFiles import *

def cont_event(paths ="./submittedJobs/" , JECVersions=["Fall17_17Nov2017_V6"], JetLabels=["AK4CHS"], systematics=["", "PU", "JEC", "JER"]):
    count = 0
    for newJECVersion in JECVersions:
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
def condor_control(original_dir ="./submittedJobs/" , JECVersions=["Fall17_17Nov2017_V6"], JetLabels=["AK4CHS"], systematics=["", "PU", "JEC", "JER"], internal_option="-l"):
    count = 0
    for newJECVersion in JECVersions:
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
                        all_events = cont_event(original_dir, JECVersions, JetLabels, systematics)
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


from createConfigFiles import *
@timeit
def delete_workdir(original_dir ="./submittedJobs_2GeV/" , JECVersions=["Fall17_17Nov2017_V6", "Fall17_17Nov2017_V10"], JetLabels=["AK4CHS", "AK8PUPPI"], systematics=["", "PU", "JEC", "JER"]):
    for sample in ["DATA", "QCD"]:
        for newJECVersion in JECVersions:
            for newJetLabel in JetLabels:
                for sys in systematics+[""]:
                    for dir in ["", "up", "down"]:
                        if sys == "" and dir != "":
                            continue
                        path = "/nfs/dust/cms/user/amalara/sframe_all/JER2017_2GeV_"+sample+"/"+newJECVersion+"/"+newJetLabel+"/"+sys+"/"+dir+"/"
                        print path
                        if os.path.isdir(path):
                            for workdir in sorted(os.listdir(path)):
                                if "workdir" in workdir:
                                    cmd = "rm -fr %s" % (path+workdir)
                                    print cmd
                                    a = os.system(cmd)
                                    command = ['ls', path+workdir]
                                    # process = subprocess.Popen(command)
                                    # process.wait()
                    # process = subprocess.Popen(command)
                    # process.wait()
                    # path = original_dir+newJECVersion+"/"+newJetLabel+"/"+sys+"/"+dir+"/"



##################################################
#                                                #
#                   MAIN Program                 #
#                                                #
##################################################

QCD_process= []
QCD_process.append("QCDPt15to7000_ZB")
QCD_process.append("QCDPt15to30_ZB")
QCD_process.append("QCDPt30to50_ZB")
QCD_process.append("QCDPt50to80_ZB")
QCD_process.append("QCDPt80to120_ZB")
QCD_process.append("QCDPt120to170_ZB")
QCD_process.append("QCDPt170to300_ZB")
QCD_process.append("QCDPt300to470_ZB")
QCD_process.append("QCDPt470to600_ZB")
QCD_process.append("QCDPt600to800_ZB")
QCD_process.append("QCDPt800to1000_ZB")
QCD_process.append("QCDPt1000to1400_ZB")
QCD_process.append("QCDPt1400to1800_ZB")
QCD_process.append("QCDPt1800to2400_ZB")
QCD_process.append("QCDPt2400to3200_ZB")
QCD_process.append("QCDPt3200toInf_ZB")

Data_process= []
Data_process.append("DATA_RunB_ZB")
Data_process.append("DATA_RunC_ZB")
Data_process.append("DATA_RunD_ZB")
Data_process.append("DATA_RunE_ZB")
Data_process.append("DATA_RunF_ZB")

processes = QCD_process+Data_process

JECVersions = ["Fall17_17Nov2017_V6", "Fall17_17Nov2017_V10", "Fall17_17Nov2017_V11"]
JetLabels = ["AK4CHS", "AK8PUPPI"]
systematics = ["PU", "JEC", "JER"]

JECVersions = ["Fall17_17Nov2017_V10"]
JetLabels = ["AK4CHS"]
systematics = []

original_dir = os.getcwd()
original_dir += "/submittedJobs_2GeV_test/"
original_file = "JER2017_2GeV.xml"

isMB = True
test_trigger = False
if "test" in original_dir:
    test_trigger = True

try:
    option = sys.argv[1]
except:
    option = ""

if option == "resubmit":
    internal_option = "-r"
elif option == "submit":
    internal_option = "-s"
elif option == "add":
    internal_option = "-a"
elif option == "list":
    internal_option = "-l"
elif option == "new":
    internal_option = ""
else:
    internal_option = ""

if option == "new":
    createConfigFiles(processes,JECVersions,JetLabels, systematics, original_dir, original_file, isMB, test_trigger)
else:
    condor_control(original_dir, JECVersions, JetLabels, systematics, internal_option)
# delete_workdir(original_dir, JECVersions, JetLabels, systematics)
