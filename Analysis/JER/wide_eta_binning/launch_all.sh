#!/usr/bin/env zsh

# source launch_full.sh RunBC     PtBinned Single full
# source launch_full.sh RunF      PtBinned Single full
# source launch_full.sh RunBCDEF  PtBinned Single full

# source launch_full.sh RunBC     PtBinned DiJet  full
# source launch_full.sh RunF      PtBinned DiJet  full
# source launch_full.sh RunBCDEF  PtBinned DiJet  full


source launch_half.sh RunBC     PtBinned Single half
source launch_half.sh RunF      PtBinned Single half
source launch_half.sh RunBCDEF  PtBinned Single half

# source launch_half.sh RunBC     PtBinned DiJet  half
# source launch_half.sh RunF      PtBinned DiJet  half
# source launch_half.sh RunBCDEF  PtBinned DiJet  half
