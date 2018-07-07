cp MySelector_half_Single.C MySelector.C

make

echo "RunB_half"
./Analysis.x ../lists/file_DataRunB_Single.txt
rm -fr file/Single/RunB_half
mkdir -p file/Single/RunB_half
mv *root file/Single/RunB_half
mv counts.txt file/Single/RunB_half

echo "RunC_half"
./Analysis.x ../lists/file_DataRunC_Single.txt
rm -fr file/Single/RunC_half
mkdir -p file/Single/RunC_half
mv *root file/Single/RunC_half
mv counts.txt file/Single/RunC_half

echo "RunD_half"
./Analysis.x ../lists/file_DataRunD_Single.txt
rm -fr file/Single/RunD_half
mkdir -p file/Single/RunD_half
mv *root file/Single/RunD_half
mv counts.txt file/Single/RunD_half

echo "RunE_half"
./Analysis.x ../lists/file_DataRunE_Single.txt
rm -fr file/Single/RunE_half
mkdir -p file/Single/RunE_half
mv *root file/Single/RunE_half
mv counts.txt file/Single/RunE_half

echo "RunF_half"
./Analysis.x ../lists/file_DataRunF_Single.txt
rm -fr file/Single/RunF_half
mkdir -p file/Single/RunF_half
mv *root file/Single/RunF_half
mv counts.txt file/Single/RunF_half

echo "RunBC_half"
./Analysis.x ../lists/file_DataRunBC_Single.txt
rm -fr file/Single/RunBC_half
mkdir -p file/Single/RunBC_half
mv *root file/Single/RunBC_half
mv counts.txt file/Single/RunBC_half

echo "RunDE_half"
./Analysis.x ../lists/file_DataRunDE_Single.txt
rm -fr file/Single/RunDE_half
mkdir -p file/Single/RunDE_half
mv *root file/Single/RunDE_half
mv counts.txt file/Single/RunDE_half

echo "RunDEF_half"
./Analysis.x ../lists/file_DataRunDEF_Single.txt
rm -fr file/Single/RunDEF_half
mkdir -p file/Single/RunDEF_half
mv *root file/Single/RunDEF_half
mv counts.txt file/Single/RunDEF_half

echo "RunBCDEF_half"
./Analysis.x ../lists/file_DataRunBCDEF_Single.txt
rm -fr file/Single/RunBCDEF_half
mkdir -p file/Single/RunBCDEF_half
mv *root file/Single/RunBCDEF_half
mv counts.txt file/Single/RunBCDEF_half
