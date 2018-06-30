cp MySelector_full_Single.C MySelector.C

make

#echo "RunB_full"
#./Analysis.x ../lists/file_DataRunB_Single.txt
#rm -fr file/Single/RunB_full
#mkdir -p file/Single/RunB_full
#mv *root file/Single/RunB_full
#mv counts.txt file/Single/RunB_full
#
#echo "RunC_full"
#./Analysis.x ../lists/file_DataRunC_Single.txt
#rm -fr file/Single/RunC_full
#mkdir -p file/Single/RunC_full
#mv *root file/Single/RunC_full
#mv counts.txt file/Single/RunC_full
#
#echo "RunD_full"
#./Analysis.x ../lists/file_DataRunD_Single.txt
#rm -fr file/Single/RunD_full
#mkdir -p file/Single/RunD_full
#mv *root file/Single/RunD_full
#mv counts.txt file/Single/RunD_full
#
#echo "RunE_full"
#./Analysis.x ../lists/file_DataRunE_Single.txt
#rm -fr file/Single/RunE_full
#mkdir -p file/Single/RunE_full
#mv *root file/Single/RunE_full
#mv counts.txt file/Single/RunE_full

echo "RunF_full"
./Analysis.x ../lists/file_DataRunF_Single.txt
rm -fr file/Single/RunF_full
mkdir -p file/Single/RunF_full
mv *root file/Single/RunF_full
mv counts.txt file/Single/RunF_full

echo "RunBC_full"
./Analysis.x ../lists/file_DataRunBC_Single.txt
rm -fr file/Single/RunBC_full
mkdir -p file/Single/RunBC_full
mv *root file/Single/RunBC_full
mv counts.txt file/Single/RunBC_full

#echo "RunDE_full"
#./Analysis.x ../lists/file_DataRunDE_Single.txt
#rm -fr file/Single/RunDE_full
#mkdir -p file/Single/RunDE_full
#mv *root file/Single/RunDE_full
#mv counts.txt file/Single/RunDE_full
#
#echo "RunDEF_full"
#./Analysis.x ../lists/file_DataRunDEF_Single.txt
#rm -fr file/Single/RunDEF_full
#mkdir -p file/Single/RunDEF_full
#mv *root file/Single/RunDEF_full
#mv counts.txt file/Single/RunDEF_full

echo "RunBCDEF_full"
./Analysis.x ../lists/file_DataRunBCDEF_Single.txt
rm -fr file/Single/RunBCDEF_full
mkdir -p file/Single/RunBCDEF_full
mv *root file/Single/RunBCDEF_full
mv counts.txt file/Single/RunBCDEF_full
