#!/usr/bin/env zsh

for run in RunBC RunF RunBCDEF RunB RunC RunD RunD RunE RunDE RunDEF
do
	for distr in full
	do
		for trigger in Single
		do
			for DIR in "" _PUup _PUdown _JECup _JECdown _alpha_10
			do
				alpha_range=15
				if [[ "${DIR}" == *"10"* ]]; then
					alpha_range=10
				fi
				echo ${run} ${distr} ${trigger} ${trigger}${DIR} ${alpha_range}
				echo "source launch.sh ${run} ${distr} ${trigger} ${trigger}${DIR} ${alpha_range}"
				source launch.sh ${run} ${distr} ${trigger} ${trigger}${DIR} ${alpha_range}
			done
		done
	done
done
