#!/usr/bin/env zsh

run=$1
distr=$2
trigger=$3
DIR=$4
alpha_range=$5
dir=file/${DIR}/${run}_${distr}
echo $../lists/${DIR}/file_${run}.txt$


cp MySelector_${distr}_${trigger}.C MySelector.C
sed -i -e """s/jet_threshold=15/jet_threshold=${alpha_range}/g" MySelector.C
# grep -r "thresh" MySelector.C

sleep 5
wait
make clean
make
sleep 5

echo ${run}_${distr}
./Analysis.x ../lists/${DIR}/file_${run}.txt
rm -fr ${dir}
mkdir -p ${dir}
mv *root ${dir}
mv counts.txt ${dir}
