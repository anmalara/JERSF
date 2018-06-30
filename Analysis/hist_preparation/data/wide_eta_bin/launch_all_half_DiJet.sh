cp MySelector_half_DiJet.C MySelector.C

make

#echo "RunB_half"
#./Analysis.x ../lists/file_DataRunB_DiJet.txt
#rm -fr file/DiJet/RunB_half
#mkdir -p file/DiJet/RunB_half
#mv *root file/DiJet/RunB_half
#mv counts.txt file/DiJet/RunB_half
#
#echo "RunC_half"
#./Analysis.x ../lists/file_DataRunC_DiJet.txt
#rm -fr file/DiJet/RunC_half
#mkdir -p file/DiJet/RunC_half
#mv *root file/DiJet/RunC_half
#mv counts.txt file/DiJet/RunC_half
#
#echo "RunD_half"
#./Analysis.x ../lists/file_DataRunD_DiJet.txt
#rm -fr file/DiJet/RunD_half
#mkdir -p file/DiJet/RunD_half
#mv *root file/DiJet/RunD_half
#mv counts.txt file/DiJet/RunD_half
#
#echo "RunE_half"
#./Analysis.x ../lists/file_DataRunE_DiJet.txt
#rm -fr file/DiJet/RunE_half
#mkdir -p file/DiJet/RunE_half
#mv *root file/DiJet/RunE_half
#mv counts.txt file/DiJet/RunE_half

echo "RunF_half"
./Analysis.x ../lists/file_DataRunF_DiJet.txt
rm -fr file/DiJet/RunF_half
mkdir -p file/DiJet/RunF_half
mv *root file/DiJet/RunF_half
mv counts.txt file/DiJet/RunF_half

echo "RunBC_half"
./Analysis.x ../lists/file_DataRunBC_DiJet.txt
rm -fr file/DiJet/RunBC_half
mkdir -p file/DiJet/RunBC_half
mv *root file/DiJet/RunBC_half
mv counts.txt file/DiJet/RunBC_half

#echo "RunDE_half"
#./Analysis.x ../lists/file_DataRunDE_DiJet.txt
#rm -fr file/DiJet/RunDE_half
#mkdir -p file/DiJet/RunDE_half
#mv *root file/DiJet/RunDE_half
#mv counts.txt file/DiJet/RunDE_half
#
#echo "RunDEF_half"
#./Analysis.x ../lists/file_DataRunDEF_DiJet.txt
#rm -fr file/DiJet/RunDEF_half
#mkdir -p file/DiJet/RunDEF_half
#mv *root file/DiJet/RunDEF_half
#mv counts.txt file/DiJet/RunDEF_half

echo "RunBCDEF_half"
./Analysis.x ../lists/file_DataRunBCDEF_DiJet.txt
rm -fr file/DiJet/RunBCDEF_half
mkdir -p file/DiJet/RunBCDEF_half
mv *root file/DiJet/RunBCDEF_half
mv counts.txt file/DiJet/RunBCDEF_half
