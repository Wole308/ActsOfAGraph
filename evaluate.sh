#!/bin/bash
#!/bin/bash
#SBATCH --job-name=all_runs 
#SBATCH --cpus-per-task=1 
#SBATCH --error="all_runs.err" 
#SBATCH --output=all_runs.log 
#SBATCH --nodelist=slurm1

set -e # Courtesy : Jinja 2.0

ON=1
OFF=0

XWARE="" 
SETUP="" 
ALGORITHM="" 
DATASET=""

SETUP_NAME=""
OUTFILE_NAME=""
KERNEL_NAME=""
PROFILESUMMARYNAME=""

SW__ACTGRAPH_SETUP__PR_ALGORITHM=1
SW__ACTGRAPH_SETUP__BFS_ALGORITHM=2
SW__ACTGRAPH_SETUP__BC_ALGORITHM=3

SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM=10
HW__ACTGRAPH_SETUP__PR_ALGORITHM=11
HW__ACTGRAPH_SETUP__BFS_ALGORITHM=12
HW__ACTGRAPH_SETUP__BC_ALGORITHM=13

SW__GRAFBOOST_SETUP__PR_ALGORITHM=21
SW__GRAFBOOST_SETUP__BFS_ALGORITHM=22
SW__GRAFBOOST_SETUP__BC_ALGORITHM=23

LARGEDATASET_1M=1
LARGEDATASET_67M=2
LARGEDATASET_268M=3
LARGEDATASET_1B=4
LARGEDATASET_4B=5

_1THREADS=1
_2THREADS=2
_4THREADS=4
_8THREADS=8

_LOCKE="LOCKE"
_NOLOCKE="NOLOCKE"

### >>> LOOP1: basic experiement setup
# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM
# for setup in $HW__ACTGRAPH_SETUP__PR_ALGORITHM

for setup in $SW__ACTGRAPH_SETUP__BFS_ALGORITHM
# for setup in $HW__ACTGRAPH_SETUP__BFS_ALGORITHM

# for setup in $SW__ACTGRAPH_SETUP__BC_ALGORITHM
# for setup in $HW__ACTGRAPH_SETUP__BC_ALGORITHM

# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM $HW__ACTGRAPH_SETUP__PR_ALGORITHM
# for setup in $SW__ACTGRAPH_SETUP__BFS_ALGORITHM $HW__ACTGRAPH_SETUP__BFS_ALGORITHM
# for setup in $SW__ACTGRAPH_SETUP__BC_ALGORITHM $HW__ACTGRAPH_SETUP__BC_ALGORITHM
# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM $HW__ACTGRAPH_SETUP__PR_ALGORITHM $SW__ACTGRAPH_SETUP__BFS_ALGORITHM $HW__ACTGRAPH_SETUP__BFS_ALGORITHM $SW__ACTGRAPH_SETUP__BC_ALGORITHM $HW__ACTGRAPH_SETUP__BC_ALGORITHM
do 
	if [ $setup == $SW__ACTGRAPH_SETUP__PR_ALGORITHM ]  
	then 
		XWARE="SW" 
		SETUP="ACTGRAPH_SETUP" 
		ALGORITHM="PR_ALGORITHM" 
		SETUP_NAME="actgraph_pr_sw"
	elif [ $setup == $SW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
	then
		XWARE="SW" 
		SETUP="ACTGRAPH_SETUP" 
		ALGORITHM="BFS_ALGORITHM" 
		SETUP_NAME="actgraph_bfs_sw"
	elif [ $setup == $SW__ACTGRAPH_SETUP__BC_ALGORITHM ]
	then
		XWARE="SW" 
		SETUP="ACTGRAPH_SETUP" 
		ALGORITHM="BC_ALGORITHM" 
		SETUP_NAME="actgraph_bc_sw"
	elif [ $setup == $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM ]
	then
		XWARE="SWEMU" 
		SETUP="ACTGRAPH_SETUP" 
		ALGORITHM="PR_ALGORITHM" 
		SETUP_NAME="actgraph_pr_swemu"
	elif [ $setup == $HW__ACTGRAPH_SETUP__PR_ALGORITHM ]
	then
		XWARE="HW" 
		SETUP="ACTGRAPH_SETUP" 
		ALGORITHM="PR_ALGORITHM" 
		SETUP_NAME="actgraph_pr_hw"
	elif [ $setup == $HW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
	then
		XWARE="HW" 
		SETUP="ACTGRAPH_SETUP" 
		ALGORITHM="BFS_ALGORITHM" 
		SETUP_NAME="actgraph_bfs_hw"
		
	elif [ $setup == $HW__ACTGRAPH_SETUP__BC_ALGORITHM ]
	then
		XWARE="HW" 
		SETUP="ACTGRAPH_SETUP" 
		ALGORITHM="BC_ALGORITHM" 
		SETUP_NAME="actgraph_bc_hw"
	elif [ $setup == $SW__GRAFBOOST_SETUP__PR_ALGORITHM ]
	then
		XWARE="SW" 
		SETUP="GRAFBOOST_SETUP" 
		ALGORITHM="PR_ALGORITHM" 
		SETUP_NAME="grafboost_pr_sw"
	elif [ $setup == $SW__GRAFBOOST_SETUP__BFS_ALGORITHM ]
	then
		XWARE="SW" 
		SETUP="GRAFBOOST_SETUP" 
		ALGORITHM="BFS_ALGORITHM" 
		SETUP_NAME="grafboost_bfs_sw"
		
	elif [ $setup == $SW__GRAFBOOST_SETUP__BC_ALGORITHM ]
	then
		XWARE="SW" 
		SETUP="GRAFBOOST_SETUP" 
		ALGORITHM="BC_ALGORITHM" 
		SETUP_NAME="grafboost_bc_sw"
	else 
		XWARE="" 
		SETUP="" 
		ALGORITHM="" 
	fi
	
	### >>> LOOP2: number of threads
	# for numthreads in $_1THREADS $_2THREADS $_4THREADS $_8THREADS
	for numthreads in $_4THREADS
	do
		### >>> LOOP3: locke (kernel-only evaluation)
		# for locke in $_LOCKE $_NOLOCKE
		for locke in $_NOLOCKE
		# for locke in $_LOCKE
		do
			### >>> LOOP3: datasets
			# for dataset in $LARGEDATASET_1M
			for dataset in $LARGEDATASET_67M
			# for dataset in $LARGEDATASET_268M
			# for dataset in $LARGEDATASET_1B
			# for dataset in $LARGEDATASET_67M $LARGEDATASET_268M $LARGEDATASET_1B
			do 
				if [ $dataset == $LARGEDATASET_1M ]  
				then	
					DATASET="_LARGEDATASET_1M"
					OUTFILE_NAME="results/${SETUP_NAME}_${numthreads}threads_${locke}_kron26.out"
					KERNEL_NAME="kernel26.awsxclbin"
					PROFILESUMMARYNAME="results/profile_summary_${SETUP_NAME}_${numthreads}threads_${locke}_kron26.csv"
				elif [ $dataset == $LARGEDATASET_67M ]  
				then	
					DATASET="_LARGEDATASET_67M"
					OUTFILE_NAME="results/${SETUP_NAME}_${numthreads}threads_${locke}_kron26.out"
					KERNEL_NAME="kernel26.awsxclbin"
					PROFILESUMMARYNAME="results/profile_summary_${SETUP_NAME}_${numthreads}threads_${locke}_kron26.csv"
				elif [ $dataset == $LARGEDATASET_268M ]
				then
					DATASET="_LARGEDATASET_268M"
					OUTFILE_NAME="results/${SETUP_NAME}_${numthreads}threads_${locke}_kron28.out"
					KERNEL_NAME="kernel28.awsxclbin"
					PROFILESUMMARYNAME="results/profile_summary_${SETUP_NAME}_${numthreads}threads_${locke}_kron28.csv"
				elif [ $dataset == $LARGEDATASET_1B ]
				then
					DATASET="_LARGEDATASET_1B"
					OUTFILE_NAME="results/${SETUP_NAME}_${numthreads}threads_${locke}_kron30.out"
					KERNEL_NAME="kernel30.awsxclbin"
					PROFILESUMMARYNAME="results/profile_summary_${SETUP_NAME}_${numthreads}threads_${locke}_kron30.csv"
				elif [ $dataset == $LARGEDATASET_4B ]
				then
					DATASET="_LARGEDATASET_4B"
					OUTFILE_NAME="results/${SETUP_NAME}_${numthreads}threads_${locke}_kron32.out"
					KERNEL_NAME="kernel32.awsxclbin"
					PROFILESUMMARYNAME="results/profile_summary_${SETUP_NAME}_${numthreads}threads_${locke}_kron32.csv"
				else 
					DATASET=""
					OUTFILE_NAME=""
					KERNEL_NAME=""
				fi
				
				make generatesrcs XWARE=$XWARE SETUP=$SETUP ALGORITHM=$ALGORITHM DATASET=$DATASET NUMCPUTHREADS=$numthreads LOCKE=$locke

				if [ $setup == $SW__ACTGRAPH_SETUP__PR_ALGORITHM ]  
				then
					make cleanall
					# make build_acts_nthreads
					# make demo_acts_nthreads_debug
					make demo_acts_nthreads
					# make demo_acts_nthreads > $OUTFILE_NAME
				elif [ $setup == $SW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
				then
					make cleanall
					# make build_acts_nthreads
					make demo_acts_nthreads
					# make demo_acts_nthreads_debug
					# make demo_acts_nthreads > $OUTFILE_NAME
				elif [ $setup == $SW__ACTGRAPH_SETUP__BC_ALGORITHM ]
				then
					make cleanall
					# make build_acts_nthreads
					# make demo_acts_nthreads
					make demo_acts_nthreads > $OUTFILE_NAME
				elif [ $setup == $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM ]
				then
					make cleanall
					# rm -rf host
					# make build_host
					# make build_host_aws
					# XCL_EMULATION_MODE=sw_emu ./host kernel.xclbin
					# make swemu 
					make swemu_aws
					# make swemu_aws > $OUTFILE_NAME
				elif [ $setup == $HW__ACTGRAPH_SETUP__PR_ALGORITHM ]
				then
					make cleanall
					# rm -rf host
					# make build_host
					# make build_host_aws
					# ./host kernel.awsxclbin
					# ./host ../ACTGraph_kernels/$KERNEL_NAME
					# ./host ../ACTGraph_kernels/$KERNEL_NAME > $OUTFILE_NAME
					wait 
					if test -f "profile_summary.csv"; then
						echo "profile_summary.csv exist"
						cp profile_summary.csv $PROFILESUMMARYNAME
					fi
				elif [ $setup == $HW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
				then
					make cleanall
					# rm -rf host
					# make build_host
					make build_host_aws
					# ./host kernel.awsxclbin
					# ./host ../ACTGraph_kernels/$KERNEL_NAME
					./host ../ACTGraph_kernels/$KERNEL_NAME > $OUTFILE_NAME
					wait 
					if test -f "profile_summary.csv"; then
						echo "profile_summary.csv exist"
						cp profile_summary.csv $PROFILESUMMARYNAME
					fi
				elif [ $setup == $HW__ACTGRAPH_SETUP__BC_ALGORITHM ]
				then
					make cleanall
					# rm -rf host
					# make build_host
					make build_host_aws
					# ./host ../ACTGraph_kernels/$KERNEL_NAME
					./host ../ACTGraph_kernels/$KERNEL_NAME > $OUTFILE_NAME
					wait 
					if test -f "profile_summary.csv"; then
						echo "profile_summary.csv exist"
						cp profile_summary.csv $PROFILESUMMARYNAME
					fi
				elif [ $setup == $SW__GRAFBOOST_SETUP__PR_ALGORITHM ]
				then
					make cleanall
					# make build_grafboost_nthreads
					# make demo_grafboost_nthreads
					make demo_grafboost_nthreads > $OUTFILE_NAME
				elif [ $setup == $SW__GRAFBOOST_SETUP__BFS_ALGORITHM ]
				then
					make cleanall
					# make build_grafboost_nthreads
					# make demo_grafboost_nthreads
					make demo_grafboost_nthreads > $OUTFILE_NAME
				elif [ $setup == $SW__GRAFBOOST_SETUP__BC_ALGORITHM ]
				then
					make cleanall
					# make build_grafboost_nthreads
					# make demo_grafboost_nthreads
					make demo_grafboost_nthreads > $OUTFILE_NAME
				else 
					XWARE="" 
					SETUP="" 
					ALGORITHM="" 
				fi

				echo 'finished: variables:'
				echo 'XWARE:' $XWARE 
				echo 'SETUP:' $SETUP
				echo 'ALGORITHM:' $ALGORITHM
				echo 'DATASET:' $DATASET
				echo 'NUMCPUTHREADS:' $NUMCPUTHREADS
				echo 'OUTFILE_NAME:' $OUTFILE_NAME	
			done
		done
	done 
done
echo 'finished: successfully finished all processing'





