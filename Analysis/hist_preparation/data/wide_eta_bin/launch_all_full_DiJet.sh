cp MySelector_full_DiJet.C MySelector.C

make

#echo "RunB_full"
#./Analysis.x ../lists/file_DataRunB_DiJet.txt
#rm -fr file/DiJet/RunB_full
#mkdir -p file/DiJet/RunB_full
#mv *root file/DiJet/RunB_full
#mv counts.txt file/DiJet/RunB_full
#
#echo "RunC_full"
#./Analysis.x ../lists/file_DataRunC_DiJet.txt
#rm -fr file/DiJet/RunC_full
#mkdir -p file/DiJet/RunC_full
#mv *root file/DiJet/RunC_full
#mv counts.txt file/DiJet/RunC_full
#
#echo "RunD_full"
#./Analysis.x ../lists/file_DataRunD_DiJet.txt
#rm -fr file/DiJet/RunD_full
#mkdir -p file/DiJet/RunD_full
#mv *root file/DiJet/RunD_full
#mv counts.txt file/DiJet/RunD_full
#
#echo "RunE_full"
#./Analysis.x ../lists/file_DataRunE_DiJet.txt
#rm -fr file/DiJet/RunE_full
#mkdir -p file/DiJet/RunE_full
#mv *root file/DiJet/RunE_full
#mv counts.txt file/DiJet/RunE_full

echo "RunF_full"
./Analysis.x ../lists/file_DataRunF_DiJet.txt
rm -fr file/DiJet/RunF_full
mkdir -p file/DiJet/RunF_full
mv *root file/DiJet/RunF_full
mv counts.txt file/DiJet/RunF_full

echo "RunBC_full"
./Analysis.x ../lists/file_DataRunBC_DiJet.txt
rm -fr file/DiJet/RunBC_full
mkdir -p file/DiJet/RunBC_full
mv *root file/DiJet/RunBC_full
mv counts.txt file/DiJet/RunBC_full

#echo "RunDE_full"
#./Analysis.x ../lists/file_DataRunDE_DiJet.txt
#rm -fr file/DiJet/RunDE_full
#mkdir -p file/DiJet/RunDE_full
#mv *root file/DiJet/RunDE_full
#mv counts.txt file/DiJet/RunDE_full
#
#echo "RunDEF_full"
#./Analysis.x ../lists/file_DataRunDEF_DiJet.txt
#rm -fr file/DiJet/RunDEF_full
#mkdir -p file/DiJet/RunDEF_full
#mv *root file/DiJet/RunDEF_full
#mv counts.txt file/DiJet/RunDEF_full

echo "RunBCDEF_full"
./Analysis.x ../lists/file_DataRunBCDEF_DiJet.txt
rm -fr file/DiJet/RunBCDEF_full
mkdir -p file/DiJet/RunBCDEF_full
mv *root file/DiJet/RunBCDEF_full
mv counts.txt file/DiJet/RunBCDEF_full
