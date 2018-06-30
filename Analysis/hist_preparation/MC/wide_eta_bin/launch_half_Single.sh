cp MySelector_half_Single.C MySelector.C

make

echo "PtBinnedQCD_half"
./Analysis.x ../lists/file_PtBinned_Single.txt
mkdir -p file/Single/PtBinned_half_Single
mv *root file/Single/PtBinned_half_Single
mv counts.txt file/Single/PtBinned_half_Single
