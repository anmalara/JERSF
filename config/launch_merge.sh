sframe_batch.py -a JER2017_PtBinnedQCD_Single_JECdown.xml
pgrep hadd | while read -r line ; do
    echo "Processing $line"
    wait $line
done
cd /nfs/dust/cms/user/amalara/sframe_all/JER2017_PtBinnedQCD
echo
cd /nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/config
echo 
