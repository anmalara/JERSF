cp MySelector_half_DiJet.C MySelector.C

make

echo "PtBinnedQCD_half"
./Analysis.x ../lists/file_PtBinned_DiJet.txt
mkdir -p file/DiJet/PtBinned_half_DiJet
mv *root file/DiJet/PtBinned_half_DiJet
mv counts.txt file/DiJet/PtBinned_half_DiJet
