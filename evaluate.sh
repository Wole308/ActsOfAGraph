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

# ROOTDIR="/home/centos/src/project_data/oj2zf"
ROOTDIR="/home/oj2zf/Documents"

# KERNELTYPE="ACTSMODEL"
KERNELTYPE="ACTSMODEL_LW"

# KERNELTYPE=0
# KERNELTYPE=1

XWARE="" 
SETUP="" 
ALGORITHM="" 
DATASET=""

SETUP_NAME=""
RESULTDIR_RESULT=""
KERNEL_NAME=""
RESULTDIR_PROFILESUMMARY=""

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

HWSYN__ACTGRAPH_SETUP__PR_ALGORITHM=24

LARGEDATASET_1M=1
LARGEDATASET_4M=2
TWITTERDATASET_67M=3
LARGEDATASET_67M=4
LARGEDATASET_268M=5
LARGEDATASET_1B=6
LARGEDATASET_4B=7

THREADCOUNT_EQ1=1
THREADCOUNT_EQ2=2
THREADCOUNT_EQ4=4
THREADCOUNT_EQ8=8
THREADCOUNT_EQ12=12
THREADCOUNT_EQ16=16

THREADPOW_EQ0=0
THREADPOW_EQ1=1
THREADPOW_EQ2=2
THREADPOW_EQ3=3
THREADPOW_EQ4=4
THREADPOW_EQ5=5

_LOCKE="LOCKE"
_NOLOCKE="NOLOCKE"

### >>> LOOP0: evaluation types
# for evaluation_type in EV_PERFORMANCEOFALGORITHM
for evaluation_type in EV_SIMPLETEST
# for evaluation_type in EV_IMPACTOFRANGE
# for evaluation_type in EV_IMPACTOFPARTITIONFANOUT
# for evaluation_type in EV_IMPACTOFNUMSUBWORKERS
# for evaluation_type in EV_IMPACTOFBANDWIDTH
# for evaluation_type in EV_IMPACTOFPLATFORM
# for evaluation_type in EV_IMPACTOFRANGE EV_IMPACTOFPARTITIONFANOUT EV_IMPACTOFNUMSUBWORKERS EV_IMPACTOFBANDWIDTH EV_IMPACTOFPLATFORM
do 
	### >>> LOOP1: hardware types
	for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__PR_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM

	# for setup in $SW__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__BFS_ALGORITHM

	# for setup in $SW__ACTGRAPH_SETUP__BC_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__BC_ALGORITHM

	# for setup in $HWSYN__ACTGRAPH_SETUP__PR_ALGORITHM
	
	# for setup in $SW__GRAFBOOST_SETUP__PR_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__BFS_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__BC_ALGORITHM

	# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM $SW__ACTGRAPH_SETUP__PR_ALGORITHM $SW__GRAFBOOST_SETUP__PR_ALGORITHM
	# for setup in $SW__ACTGRAPH_SETUP__BFS_ALGORITHM $SW__ACTGRAPH_SETUP__BFS_ALGORITHM $SW__GRAFBOOST_SETUP__BFS_ALGORITHM
	# for setup in $SW__ACTGRAPH_SETUP__BC_ALGORITHM $SW__ACTGRAPH_SETUP__BC_ALGORITHM $SW__GRAFBOOST_SETUP__BC_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM $SW__ACTGRAPH_SETUP__BFS_ALGORITHM $SW__ACTGRAPH_SETUP__BC_ALGORITHM
	# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM $HW__ACTGRAPH_SETUP__PR_ALGORITHM $SW__ACTGRAPH_SETUP__BFS_ALGORITHM $HW__ACTGRAPH_SETUP__BFS_ALGORITHM $SW__ACTGRAPH_SETUP__BC_ALGORITHM $HW__ACTGRAPH_SETUP__BC_ALGORITHM
	do 
		if [ $setup == $SW__ACTGRAPH_SETUP__PR_ALGORITHM ]  
		then 
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_pr_sw"
			else 
				SETUP_NAME="actgraph_pr_sw"
			fi
			
		elif [ $setup == $SW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BFS_ALGORITHM"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bfs_sw"
			else 
				SETUP_NAME="actgraph_bfs_sw"
			fi
			
		elif [ $setup == $SW__ACTGRAPH_SETUP__BC_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BC_ALGORITHM"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bc_sw"
			else 
				SETUP_NAME="actgraph_bc_sw"
			fi
			
		elif [ $setup == $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM ]
		then
			XWARE="SWEMU" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_pr_swemu"
			else 
				SETUP_NAME="actgraph_pr_swemu"
			fi
			
		elif [ $setup == $HW__ACTGRAPH_SETUP__PR_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM" 
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_pr_hw"
			else 
				SETUP_NAME="actgraph_pr_hw"
			fi
		elif [ $setup == $HW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BFS_ALGORITHM" 
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bfs_hw"
			else 
				SETUP_NAME="actgraph_bfs_hw"
			fi
			
		elif [ $setup == $HW__ACTGRAPH_SETUP__BC_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BC_ALGORITHM"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bc_hw"
			else 
				SETUP_NAME="actgraph_bc_hw"
			fi
			
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
		elif [ $setup == $HWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM" 
			SETUP_NAME="actgraph_pr_hw"
		else 
			XWARE="" 
			SETUP="" 
			ALGORITHM="" 
		fi

		for numsupercputhreads in $THREADCOUNT_EQ1
		do

		# for numcputhreads in $THREADCOUNT_EQ1 $THREADCOUNT_EQ2 $THREADCOUNT_EQ4 $THREADCOUNT_EQ8 $THREADCOUNT_EQ12 $THREADCOUNT_EQ16
		# for numcputhreads in $THREADCOUNT_EQ1
		for numcputhreads in $THREADCOUNT_EQ2
		# for numcputhreads in $THREADCOUNT_EQ4
		# for numcputhreads in $THREADCOUNT_EQ16
		do
		
		# for numsubcputhreads_pow in $THREADPOW_EQ0 $THREADPOW_EQ1 $THREADPOW_EQ2 $THREADPOW_EQ3 $THREADPOW_EQ4
		# for numsubcputhreads_pow in $THREADPOW_EQ4
		for numsubcputhreads_pow in $THREADPOW_EQ2
		# for numsubcputhreads_pow in $THREADPOW_EQ0
		do
			### >>> LOOP3: locke (kernel-only evaluation)
			# for locke in $_NOLOCKE
			for locke in $_LOCKE
			# for locke in $_LOCKE $_NOLOCKE
			do
				### >>> LOOP3: datasets
				# for dataset in $LARGEDATASET_1M
				for dataset in $LARGEDATASET_4M
				# for dataset in $TWITTERDATASET_67M
				# for dataset in $LARGEDATASET_67M
				# for dataset in $LARGEDATASET_268M
				# for dataset in $LARGEDATASET_1B
				# for dataset in $LARGEDATASET_67M $LARGEDATASET_268M $LARGEDATASET_1B
				do
					# for evaluation_param0 in 0 1 2 3 4 5
					# for evaluation_param0 in 4
					for evaluation_param0 in 0
					do
						KERNELBACKUP_DIR="${ROOTDIR}/ActsOfAGraph_Kernels"
						KERNELBACKUP_NAME="kernel_${SETUP_NAME}_${numcputhreads}threads_${locke}_${evaluation_type}_evaluation_param${evaluation_param0}"
						
						RESULTSBACKUP_DIR="${ROOTDIR}/ActsOfAGraph_Results"
						RESULT_NAME="result_${SETUP_NAME}_${numcputhreads}threads_${numsubcputhreads_pow}subthreadspow_${locke}_${evaluation_type}_evaluation_param${evaluation_param0}"
						PROFILESUMMARY_NAME="profile_summary_${SETUP_NAME}_${numcputhreads}threads_${numsubcputhreads_pow}subthreadspow_${locke}_${evaluation_type}_evaluation_param${evaluation_param0}"
						
						if [ $dataset == $LARGEDATASET_1M ]  
						then	
							DATASET="_LARGEDATASET_1M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_kron20.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_kron20.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron20.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron20.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron20"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron20.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron20.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron20.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron20.out"
						elif [ $dataset == $LARGEDATASET_4M ]  
						then	
							DATASET="_LARGEDATASET_4M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_kron22.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_kron22.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron22.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron22.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron22"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron22.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron22.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron22.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron22.out"
						elif [ $dataset == $TWITTERDATASET_67M ]  
						then	
							DATASET="_TWITTERDATASET_67M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_kron26.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_kron26.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_twitter26.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_twitter26.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26.out"
						elif [ $dataset == $LARGEDATASET_67M ]  
						then	
							DATASET="_LARGEDATASET_67M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_kron26.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_kron26.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron26.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron26.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron26"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron26.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron26.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron26.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron26.out"			
						elif [ $dataset == $LARGEDATASET_268M ]
						then
							DATASET="_LARGEDATASET_268M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_kron28.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_kron28.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron28.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron28.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron28"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron28.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron28.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron28.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron28.out"
						elif [ $dataset == $LARGEDATASET_1B ]
						then
							DATASET="_LARGEDATASET_1B"
							KERNEL_NAME="${KERNELBACKUP_NAME}_kron30.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_kron30.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron30.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron30.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron30"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron30.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron30.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron30.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron30.out"
						elif [ $dataset == $LARGEDATASET_4B ]
						then
							DATASET="_LARGEDATASET_4B"
							KERNEL_NAME="${KERNELBACKUP_NAME}_kron32.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_kron32.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron32.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron32.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron32"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron32.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron32.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron32.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron32.out"
						else 
							DATASET=""
							RESULTDIR_RESULT=""
							KERNEL_NAME=""
						fi
						
						make generatesrcs XWARE=$XWARE SETUP=$SETUP ALGORITHM=$ALGORITHM DATASET=$DATASET NUMSUPERCPUTHREADS=$numsupercputhreads NUMCPUTHREADS=$numcputhreads NUMSUBCPUTHREADS_POW=$numsubcputhreads_pow LOCKE=$locke EVALUATION_TYPE=$evaluation_type EVALUATION_PARAM0=$evaluation_param0				

						if [ $setup == $SW__ACTGRAPH_SETUP__PR_ALGORITHM ]  
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $SW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $SW__ACTGRAPH_SETUP__BC_ALGORITHM ]
						then
							make cleanall
							# make build_acts_nthreads
							# make demo_acts_nthreads
							make demo_acts_nthreads > $RESULTDIR_RESULT
						elif [ $setup == $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM ]
						then
							make cleanall
							# rm -rf host
							# make build_host
							# make build_host_aws
							# XCL_EMULATION_MODE=sw_emu ./host kernel.xclbin
							# make swemu
							# make swemu_ncomputeunits 
							# make swemu_aws
							# make swemu_aws > $RESULTDIR_RESULT
							make all TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
							XCL_EMULATION_MODE=sw_emu ./host xclbin/topkernel.sw_emu.xilinx_u280_xdma_201910_1.xclbin
						elif [ $setup == $HW__ACTGRAPH_SETUP__PR_ALGORITHM ]
						then
							make cleanall
							# rm -rf host
							
							make host
							# ./host $BACKUPDIR_KERNELXCLBIN
							./host $BACKUPDIR_KERNELXCLBIN > $RESULTDIR_RESULT
							
							# make build_host_aws
							# ./host $BACKUPDIR_KERNELAWSXCLBIN
							# ./host $BACKUPDIR_KERNELAWSXCLBIN > $RESULTDIR_RESULT
							
							# make host
							# nohup make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
							# ./host xclbin/topkernel.hw.xilinx_u280_xdma_201910_1.xclbin
					
							wait 
							if test -f "profile_summary.csv"; then
								echo "profile_summary.csv exist"
								cp profile_summary.csv $RESULTDIR_PROFILESUMMARY
							fi
						elif [ $setup == $HW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
						then
							make cleanall
							# rm -rf host
							# make build_host
							make build_host_aws
							# ./host kernel.awsxclbin
							# ./host $BACKUPDIR_KERNELAWSXCLBIN
							./host $BACKUPDIR_KERNELAWSXCLBIN > $RESULTDIR_RESULT
							wait 
							if test -f "profile_summary.csv"; then
								echo "profile_summary.csv exist"
								cp profile_summary.csv $RESULTDIR_PROFILESUMMARY
							fi
						elif [ $setup == $HW__ACTGRAPH_SETUP__BC_ALGORITHM ]
						then
							make cleanall
							# rm -rf host
							# make build_host
							make build_host_aws
							# ./host $BACKUPDIR_KERNELAWSXCLBIN
							./host $BACKUPDIR_KERNELAWSXCLBIN > $RESULTDIR_RESULT
							wait 
							if test -f "profile_summary.csv"; then
								echo "profile_summary.csv exist"
								cp profile_summary.csv $RESULTDIR_PROFILESUMMARY
							fi
						elif [ $setup == $SW__GRAFBOOST_SETUP__PR_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							make demo_grafboost_nthreads
							# make demo_grafboost_nthreads > $RESULTDIR_RESULT
						elif [ $setup == $SW__GRAFBOOST_SETUP__BFS_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							# make demo_grafboost_nthreads_debug
							make demo_grafboost_nthreads
							# make demo_grafboost_nthreads > $RESULTDIR_RESULT
						elif [ $setup == $SW__GRAFBOOST_SETUP__BC_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							# make demo_grafboost_nthreads
							make demo_grafboost_nthreads > $RESULTDIR_RESULT
							
						elif [ $setup == $HWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ]
						then
							source /opt/xilinx/xrt/setup.sh 
							source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
							make cleanall
							# make hw > nohupsyn.out
							make hw_aws > nohupsyn.out
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
							
							if test -f "host"; then
								# cp host $BACKUPDIR_HOST
								# cp kernel.xo $BACKUPDIR_KERNELXO
								cp kernel.xclbin $BACKUPDIR_KERNELXCLBIN
								cp nohupsyn.out $BACKUPDIR_NOHUP
								echo "host, kernel.xo, kernel.xclbin, nohupsyn.out saved"
							fi
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
							
							source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
							source /opt/xilinx/xrt/setup.sh 
							cd /home/centos/src/project_data/aws-fpga
							./createawsxclbin.sh
							echo "sleeping for 60 minuites before continuing ...."
							sleep 3600
							cp -rf /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/kernel.awsxclbin $BACKUPDIR_KERNELAWSXCLBIN
							cd /home/centos/src/project_data/oj2zf/ActsOfAGraph
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
						echo 'RESULTDIR_RESULT:' $RESULTDIR_RESULT
						echo 'BACKUPDIR_KERNELXCLBIN:' $BACKUPDIR_KERNELXCLBIN
					done
				done
			done

		done

		done

		done
	done
done
echo 'finished: successfully finished all processing'





