#!/usr/bin/env zsh

# run="RunBC"
# MC_samp="PtBinned"
# trigger="Single"
# distr="full"

run=$1
MC_samp=$2
trigger=$3
distr=$4

dir=file/${trigger}/${run}_${distr}



rm -fr $dir
mkdir -p $dir
cp iterFit*.C $dir
cp functions.C $dir
#cp plotting/tdrstyle_mod15.C $dir
cp tdrstyle_all.C $dir
cd $dir
pwd
mkdir -p pdfy
mkdir -p pdfy/MCTruth
mkdir -p pdfy/SFs
mkdir -p pdfy/NSC_SFs
mkdir -p pdfy/JERs
mkdir -p pdfy/widths
mkdir -p ClosureTest
mkdir -p output
mkdir -p output/asymmetries

export QUOTES='"'
export LABELMC="MC"
export LABELDATA="Data"
export MCFILE=/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/hist_preparation/MC/wide_eta_bin/file/$trigger/${MC_samp}_${distr}_${trigger}/histograms_mc_incl_full.root
export DATAFILE=/nfs/dust/cms/user/amalara/WorkingArea/UHH2_94/CMSSW_9_4_1/src/UHH2/JER2017/Analysis/hist_preparation/data/wide_eta_bin/file/$trigger/${run}_${distr}/histograms_data_incl_full.root


if [ "$run" = "RunB" ]; then
  export LABEL_LUMI_INV_FB="[MC 94X] Run2017B 4.77 fb^{-1}"
elif [ "$run" = "RunC" ]
then
  export LABEL_LUMI_INV_FB="[MC 94X] Run2017C 9.58 fb^{-1}"
elif [ "$run" = "RunD" ]
then
  export LABEL_LUMI_INV_FB="[MC 94X] Run2017D 4.22 fb^{-1}"
elif [ "$run" = "RunE" ]
then
  export LABEL_LUMI_INV_FB="[MC 94X] Run2017E 9.26 fb^{-1}"
elif [ "$run" = "RunF" ]
then
  export LABEL_LUMI_INV_FB="[MC 94X] Run2017F 13.46 fb^{-1}"
elif [ "$run" = "RunBC" ]
then
  export LABEL_LUMI_INV_FB="[MC 94X] Run2017BC 14.35 fb^{-1}"
elif [ "$run" = "RunDE" ]
then
  export LABEL_LUMI_INV_FB="[MC 94X] Run2017DE 13.48 fb^{-1}"
elif [ "$run" = "RunDEF" ]
then
  export LABEL_LUMI_INV_FB="[MC 94X] Run2017DEF 26.95 fb^{-1}"
elif [ "$run" = "RunBCDEF" ]
then
  export LABEL_LUMI_INV_FB="[MC 94X] 2017 41.3 fb^{-1}"
else
  export LABEL_LUMI_INV_FB="[MC 94X] (2017)"
fi

export MC_LABEL=$QUOTES$LABELMC$QUOTES
export DATA_LABEL=$QUOTES$LABELDATA$QUOTES
export MC=$QUOTES$MCFILE$QUOTES
export DATA=$QUOTES$DATAFILE$QUOTES
export LUMI_LABEL=$QUOTES$LABEL_LUMI_INV_FB$QUOTES
export ORIGIN="SF13invfb_smeared_pythia_vs_pythia"
export TRIGGER=$QUOTES$trigger$QUOTES

echo "Using data file "$DATA
echo "Using MC file "$MC
echo "Using data label "$DATA_LABEL
echo "Using MC label "$MC_LABEL

root -b -l << EOF
.L iterFit_full.C++
mainRun(false,${MC},${DATA},${LUMI_LABEL},${MC_LABEL},${DATA_LABEL}, ${TRIGGER})
.q
EOF
cd ../../..
