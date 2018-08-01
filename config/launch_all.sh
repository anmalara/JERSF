# sframe_batch.py -s JER2017_Data_Single_JECdown.xml
# sframe_batch.py -s JER2017_Data_Single_JECup.xml
# sframe_batch.py -s JER2017_Data_Single_PUdown.xml
# sframe_batch.py -s JER2017_Data_Single_PUup.xml
# sframe_batch.py -s JER2017_Data_Single.xml
# sframe_batch.py -s JER2017_PtBinnedQCD_Single_JECdown.xml
# sframe_batch.py -s JER2017_PtBinnedQCD_Single_JECup.xml
# sframe_batch.py -s JER2017_PtBinnedQCD_Single_PUdown.xml
# sframe_batch.py -s JER2017_PtBinnedQCD_Single_PUup.xml
# sframe_batch.py -s JER2017_PtBinnedQCD_Single.xml
#



# for QCD in QCDPt15to30 QCDPt30to50 QCDPt50to80 QCDPt80to120 QCDPt120to170 QCDPt170to300 QCDPt300to470 QCDPt470to600 QCDPt600to800 QCDPt800to1000 QCDPt1000to1400 QCDPt1400to1800 QCDPt1800to2400 QCDPt2400to3200 QCDPt3200toInf; do
#   cp JER2017_PtBinnedQCD_Single_copy.xml JER2017_PtBinned${QCD}_Single.xml
#   sed -i -e """s/workdir_JER2017_PtBinnedQCD_Single/workdir_JER2017_PtBinned${QCD}_Single/g" JER2017_PtBinned${QCD}_Single.xml
#   sed -i -e """s/MyMCPileupHistogram/MyMCPileupHistogram${QCD}/g" JER2017_PtBinned${QCD}_Single.xml
# done



for QCD in QCDPt15to30 QCDPt30to50 QCDPt50to80 QCDPt80to120 QCDPt120to170 QCDPt170to300 QCDPt300to470 QCDPt470to600 QCDPt600to800 QCDPt800to1000 QCDPt1000to1400 QCDPt1400to1800 QCDPt1800to2400 QCDPt2400to3200 QCDPt3200toInf; do
  sed -i -e """s/MyMCPileupHistogram/MyMCPileupHistogram_/g" JER2017_PtBinned${QCD}_Single.xml
done

for QCD in QCDPt15to30 QCDPt30to50 QCDPt50to80 QCDPt80to120 QCDPt120to170 QCDPt170to300 QCDPt300to470 QCDPt470to600 QCDPt600to800 QCDPt800to1000 QCDPt1000to1400 QCDPt1400to1800 QCDPt1800to2400 QCDPt2400to3200 QCDPt3200toInf; do
  sframe_batch.py -a JER2017_PtBinned${QCD}_Single.xml
done



QCDPt15to30
QCDPt30to50
QCDPt50to80
QCDPt80to120
QCDPt120to170
QCDPt170to300
QCDPt300to470
QCDPt470to600
QCDPt600to800
QCDPt800to1000
QCDPt1000to1400
QCDPt1400to1800
QCDPt1800to2400
QCDPt2400to3200
QCDPt3200toInf
