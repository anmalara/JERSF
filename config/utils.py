# class ChangeLine():
#     def __init__(self, arg):
#         self.path = path
#         change_line


def is_commented(line, filename):
    #ignore empty line
    if len(line.split())<1: return False
    #get rid of whitespace
    beginning = line.split()[0]
    if ".c" in filename:
        if beginning[0:2] == "//": return True
        else: return False
    elif ".py" in filename:
        if beginning[0:1] == "#": return True
        else: return False
    elif ".sh" in filename:
        if beginning[0:1] == "#": return True
        else: return False
    elif ".xml" in filename:
        if beginning[0:4] == "<!--": return True
        else: return False
    else:
        print "File extension not known!"
        return False

def number_leading_whitespace(line):
    n = 0
    while line[n] == " ":
        n += 1
    return n

def comment_line(line, filename):
    if is_commented(line, filename):
        print "Warning: This line is already commented."
        return line
    elif len(line.split())<1:
        print "Warning: This line is empty."
        return line
    else:
        if ".c" in filename: return "// " + line
        elif ".py" in filename: return "# " + line
        elif ".sh" in filename: return "# " + line
        elif ".xml" in filename: return "<!-- " + line[0:len(line)-1] + " -->\n"

def uncomment_line(line, filename):
    if not is_commented(line, filename):
        print "Warning: This line is already uncommented."
        return line
    else:
        if ".c" in filename:
            n = number_leading_whitespace(line)
            newline = line[0:n]
            newline += line[n+3:]
            return newline
        elif ".py" in filename:
            n = number_leading_whitespace(line)
            newline = line[0:n]
            newline += line[n+2:]
            return newline
        elif ".xml" in filename:
            n = number_leading_whitespace(line)
            newline = line[0:n]
            newline += line[n+5:len(line)-4]
            return newline + "\n"


# def change_line(path, filename, input, output):
#     newText = []
#     with open(path+filename, "U") as file:
#         lines = file.readlines()
#         for line in lines:
#             if "Systematic" in line:
#                 newText.append(line.replace(line,uncomment_line(line,filename)))
#             else:
#                 newText.append(line)
#     with open(path+filename, "w") as outputfile:
#         for line in newText:
#             outputfile.write(line)



def change_line(path, filename, controls, inputs, outputs):
    newText = []
    with open(path+filename, "U") as file:
        lines = file.readlines()
        for line in lines:
            isToChange = True
            for control in controls:
                isToChange = isToChange and control in line
            if isToChange:
                newLine = line
                for index, el in enumerate(inputs):
                    newLine = newLine.replace(inputs[index],outputs[index])
                newText.append(newLine)
            else:
                newText.append(line)
    with open(path+filename, "w") as outputfile:
        for line in newText:
            outputfile.write(line)
    return False


path = "/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/config/"
filename = "JER2017_PtBinnedQCD_Single_copy.xml"



controls  = ["<!ENTITY", "QCDPt15to30", "SYSTEM", "MC_QCD_Ptbinned_15to30"]
inputs  = ["QCDPt15to30", "MC_QCD_Ptbinned_15to30"]
outputs = ["QCDPt30to50", "MC_QCD_Ptbinned_30to50"]

change_line(path, filename, controls, inputs, outputs)
