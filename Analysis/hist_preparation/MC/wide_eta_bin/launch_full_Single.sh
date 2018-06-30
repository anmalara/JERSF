cp MySelector_full_Single.C MySelector.C

make

echo "PtBinnedQCD_full"
./Analysis.x ../lists/file_PtBinned_Single.txt
mkdir -p file/Single/PtBinned_full_Single
mv *root file/Single/PtBinned_full_Single
mv counts.txt file/Single/PtBinned_full_Single
