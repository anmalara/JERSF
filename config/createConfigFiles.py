from utils import *

@timeit
def createConfigFiles(processes=["QCDPt15to30","DATA_RunF"], JECVersions=["Fall17_17Nov2017_V6"], JetLabels=["AK4CHS"], systematics=["PU", "JEC", "JER"]):
    original_file = "JER2017.xml"
    for newJECVersion in JECVersions:
        for newJetLabel in JetLabels:
            add_path = newJECVersion+"/"+newJetLabel+"/"
            path = "./submittedJobs/"+add_path
            if not os.path.exists(path):
                os.makedirs(path)
            for process in processes:
                filename = original_file[:len(original_file)-4]+"_"+process+".xml"
                cmd = "cp %s %s" % (original_file, path+filename)
                a = os.system(cmd)
                cmd = "cp %s %s" % ("JobConfig.dtd", path)
                a = os.system(cmd)
                controls = []
                for el in list(set(processes) -set([process])):
                    if "QCD" in el:
                        controls.append(["<InputData", "Type", "MC", el])
                    elif "DATA" in el:
                        controls.append(["<InputData", "Type", "DATA", el])
                comment_lines(path, filename, controls, remove=True)
                if "QCD" in process:
                    sample = "QCD"
                elif "DATA" in process:
                    sample = "DATA"
                controls = []
                controls.append(["<ConfigSGE", "Workdir", "workdir_JER2017", "workdir_JER2017_"+process])
                controls.append(["<!ENTITY", "JEC_VERSION", '"Fall17_17Nov2017_V6"', '"'+newJECVersion+'"'])
                controls.append(["<!ENTITY", "JETLABEL", '"AK4CHS"', '"'+newJetLabel+'"'])
                controls.append(["<!ENTITY", "OUTDIR", "JER2017" , "JER2017_"+sample+"/"+add_path])
                if "QCD" in process:
                    controls.append(["<!ENTITY", "PILEUP_DIRECTORY ", "MyMCPileupHistogram" , "MyMCPileupHistogram_"+process])
                change_lines(path, filename, [el[0:2] for el in controls ], [el[2:3] for el in controls ], [el[3:4] for el in controls ])

                for sys in systematics:
                    for dir in ["up", "down"]:
                        add_path_sys = sys+"/"+dir+"/"
                        if not os.path.exists(path+add_path_sys):
                            os.makedirs(path+add_path_sys)
                        newfilename = original_file[:len(original_file)-4]+"_"+process+"_"+sys+dir+".xml"
                        cmd = "cp %s %s" % (path+filename, path+add_path_sys+newfilename)
                        a = os.system(cmd)
                        cmd = "cp %s %s" % ("JobConfig.dtd", path+add_path_sys)
                        a = os.system(cmd)
                        controls = []
                        controls.append(["<ConfigSGE", "Workdir", "workdir_JER2017_"+process, "workdir_JER2017_"+process+"_"+sys+dir])
                        controls.append(["<!ENTITY", "OUTDIR", "JER2017_"+sample+"/"+add_path , "JER2017_"+sample+"/"+add_path+add_path_sys])
                        if "JEC" in sys:
                            controls.append(["<!ENTITY", "JECSMEAR_DIRECTION", '"nominal"', '"'+dir+'"'])
                        elif "JER" in sys:
                            controls.append(["<!ENTITY", "JERSMEAR_DIRECTION", '"nominal"', '"'+dir+'"'])
                        elif "PU" in sys:
                            controls.append(["<!ENTITY", "SYSTYPE_PU", '"central"', '"'+dir+'"'])
                        change_lines(path+add_path_sys, newfilename, [el[0:2] for el in controls ], [el[2:3] for el in controls ], [el[3:4] for el in controls ])
