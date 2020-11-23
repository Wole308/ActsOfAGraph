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
ALGORITHMABBRV="" 
DATASET=""

SETUP_NAME=""
RESULTDIR_RESULT=""
KERNEL_NAME=""
RESULTDIR_PROFILESUMMARY=""

SW__ACTGRAPH_SETUP__PR_ALGORITHM=1
HW__ACTGRAPH_SETUP__PR_ALGORITHM=2
SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM=3
SW__GRAFBOOST_SETUP__PR_ALGORITHM=4

SW__ACTGRAPH_SETUP__BFS_ALGORITHM=5
HW__ACTGRAPH_SETUP__BFS_ALGORITHM=6
SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM=7
SW__GRAFBOOST_SETUP__BFS_ALGORITHM=8

SW__ACTGRAPH_SETUP__SSSP_ALGORITHM=9
HW__ACTGRAPH_SETUP__SSSP_ALGORITHM=10
SWEMU__ACTGRAPH_SETUP__SSSP_ALGORITHM=11
SW__GRAFBOOST_SETUP__SSSP_ALGORITHM=12

SW__ACTGRAPH_SETUP__BC_ALGORITHM=13
HW__ACTGRAPH_SETUP__BC_ALGORITHM=14
SWEMU__ACTGRAPH_SETUP__BC_ALGORITHM=15
SW__GRAFBOOST_SETUP__BC_ALGORITHM=16

SW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=17
HW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=18
SWEMU__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=19
SW__GRAFBOOST_SETUP__ADVANCE_ALGORITHM=20

CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM=21
CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM=22
CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM=23
CTHWSYN__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=24
AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM=25

ORKUT_3M_106M=30
HOLLYWOOD_1M_57M=31
INDOCHINA_7M_194M=32
KRON21_2M_91M=33
RGG_17M_132M=34
ROADNET_2M_3M=35
FLICKR_1M_10M=36

LARGEDATASET_1M=1
LARGEDATASET_4M=2
TWITTER_67M=3
MOLIERE2016_33M=4
LARGEDATASET_67M=5
LARGEDATASET_268M=6
LARGEDATASET_1B=7
LARGEDATASET_4B=8

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
# for evaluation_type in EV_CREATENDGRAPH
for evaluation_type in EV_PERFORMANCEOFALGORITHM
# for evaluation_type in EV_SIMPLETEST
# for evaluation_type in EV_IMPACTOFRANGE
# for evaluation_type in EV_IMPACTOFPARTITIONFANOUT
# for evaluation_type in EV_IMPACTOFNUMSUBWORKERS
# for evaluation_type in EV_IMPACTOFBANDWIDTH
# for evaluation_type in EV_IMPACTOFPLATFORM
# for evaluation_type in EV_IMPACTOFRANGE EV_IMPACTOFPARTITIONFANOUT EV_IMPACTOFNUMSUBWORKERS EV_IMPACTOFBANDWIDTH EV_IMPACTOFPLATFORM
do 
	### >>> LOOP1: hardware types
	# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM
	for setup in $HW__ACTGRAPH_SETUP__PR_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__PR_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__BFS_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__SSSP_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__BC_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__BC_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__BC_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__ADVANCE_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__ADVANCE_ALGORITHM

	# for setup in $CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM
	# for setup in $CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $CTHWSYN__ACTGRAPH_SETUP__ADVANCE_ALGORITHM
	# for setup in $AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM
	do 
		if [ $setup == $SW__ACTGRAPH_SETUP__PR_ALGORITHM ] # pr 
		then 
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM"
			ALGORITHMABBRV="pr"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_pr_sw"
			else 
				SETUP_NAME="actgraph_pr_sw"
			fi
		elif [ $setup == $HW__ACTGRAPH_SETUP__PR_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM" 
			ALGORITHMABBRV="pr"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_pr_hw"
			else 
				SETUP_NAME="actgraph_pr_hw"
			fi
		elif [ $setup == $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM ]
		then
			XWARE="SWEMU" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM"
			ALGORITHMABBRV="pr"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_pr_swemu"
			else 
				SETUP_NAME="actgraph_pr_swemu"
			fi
		elif [ $setup == $SW__GRAFBOOST_SETUP__PR_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="GRAFBOOST_SETUP" 
			ALGORITHM="PR_ALGORITHM" 
			ALGORITHMABBRV="pr"
			SETUP_NAME="grafboost_pr_sw"
			
		elif [ $setup == $SW__ACTGRAPH_SETUP__BFS_ALGORITHM ] # bfs
		then
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BFS_ALGORITHM"
			ALGORITHMABBRV="bfs"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bfs_sw"
			else 
				SETUP_NAME="actgraph_bfs_sw"
			fi
		elif [ $setup == $HW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BFS_ALGORITHM" 
			ALGORITHMABBRV="bfs"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bfs_hw"
			else 
				SETUP_NAME="actgraph_bfs_hw"
			fi
		elif [ $setup == $SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM ]
		then
			XWARE="SWEMU" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BFS_ALGORITHM"
			ALGORITHMABBRV="bfs"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bfs_swemu"
			else 
				SETUP_NAME="actgraph_bfs_swemu"
			fi
		elif [ $setup == $SW__GRAFBOOST_SETUP__BFS_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="GRAFBOOST_SETUP" 
			ALGORITHM="BFS_ALGORITHM" 
			ALGORITHMABBRV="bfs"
			SETUP_NAME="grafboost_bfs_sw"
			
		elif [ $setup == $SW__ACTGRAPH_SETUP__SSSP_ALGORITHM ] # sssp
		then
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="SSSP_ALGORITHM"
			ALGORITHMABBRV="sssp"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_sssp_sw"
			else 
				SETUP_NAME="actgraph_sssp_sw"
			fi
		elif [ $setup == $HW__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="SSSP_ALGORITHM" 
			ALGORITHMABBRV="sssp"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_sssp_hw"
			else 
				SETUP_NAME="actgraph_sssp_hw"
			fi
		elif [ $setup == $SWEMU__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
		then
			XWARE="SWEMU" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="SSSP_ALGORITHM"
			ALGORITHMABBRV="sssp"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_sssp_swemu"
			else 
				SETUP_NAME="actgraph_sssp_swemu"
			fi
		elif [ $setup == $SW__GRAFBOOST_SETUP__SSSP_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="GRAFBOOST_SETUP" 
			ALGORITHM="SSSP_ALGORITHM" 
			ALGORITHMABBRV="sssp"
			SETUP_NAME="grafboost_sssp_sw"
			
		elif [ $setup == $SW__ACTGRAPH_SETUP__BC_ALGORITHM ] # bc
		then
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BC_ALGORITHM"
			ALGORITHMABBRV="bc"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bc_sw"
			else 
				SETUP_NAME="actgraph_bc_sw"
			fi	
		elif [ $setup == $HW__ACTGRAPH_SETUP__BC_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BC_ALGORITHM"
			ALGORITHMABBRV="bc"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bc_hw"
			else 
				SETUP_NAME="actgraph_bc_hw"
			fi
		elif [ $setup == $SW__GRAFBOOST_SETUP__BC_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="GRAFBOOST_SETUP" 
			ALGORITHM="BC_ALGORITHM" 
			ALGORITHMABBRV="bc"
			SETUP_NAME="grafboost_bc_sw"
		
		elif [ $setup == $SW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM ] # advance
		then 
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="ADVANCE_ALGORITHM"
			ALGORITHMABBRV="adv"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_adv_sw"
			else 
				SETUP_NAME="actgraph_adv_sw"
			fi
		elif [ $setup == $HW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="ADVANCE_ALGORITHM" 
			ALGORITHMABBRV="adv"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_adv_hw"
			else 
				SETUP_NAME="actgraph_adv_hw"
			fi
		elif [ $setup == $SWEMU__ACTGRAPH_SETUP__ADVANCE_ALGORITHM ]
		then
			XWARE="SWEMU" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="ADVANCE_ALGORITHM"
			ALGORITHMABBRV="adv"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_adv_swemu"
			else 
				SETUP_NAME="actgraph_adv_swemu"
			fi
		elif [ $setup == $SW__GRAFBOOST_SETUP__ADVANCE_ALGORITHM ] # advance
		then 
			XWARE="SW" 
			SETUP="GRAFBOOST_SETUP" 
			ALGORITHM="ADVANCE_ALGORITHM"
			ALGORITHMABBRV="adv"
			SETUP_NAME="sortreduce_adv_sw"
			
		elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ] # syn
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM"
			ALGORITHMABBRV="pr"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_pr_hw"
			else 
				SETUP_NAME="actgraph_pr_hw"
			fi
		elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM ] # syn
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="BFS_ALGORITHM"
			ALGORITHMABBRV="bfs"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_bfs_hw"
			else 
				SETUP_NAME="actgraph_bfs_hw"
			fi
		elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM ] # syn
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="SSSP_ALGORITHM"
			ALGORITHMABBRV="sssp"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_sssp_hw"
			else 
				SETUP_NAME="actgraph_sssp_hw"
			fi
		elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__ADVANCE_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="ADVANCE_ALGORITHM"
			ALGORITHMABBRV="adv"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_adv_hw"
			else 
				SETUP_NAME="actgraph_adv_hw"
			fi
		elif [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="PR_ALGORITHM"
			ALGORITHMABBRV="pr"
			if [ $KERNELTYPE == "ACTSMODEL_LW" ]
			then 
				SETUP_NAME="actgraphlw_pr_hw"
			else 
				SETUP_NAME="actgraph_pr_hw"
			fi
			
		else 
			XWARE="" 
			SETUP="" 
			ALGORITHM="" 
			ALGORITHMABBRV=""
		fi

		for numsupercputhreads in $THREADCOUNT_EQ1
		do

		# for numcputhreads in $THREADCOUNT_EQ1 $THREADCOUNT_EQ2 $THREADCOUNT_EQ4 $THREADCOUNT_EQ8 $THREADCOUNT_EQ12 $THREADCOUNT_EQ16
		for numcputhreads in $THREADCOUNT_EQ1
		# for numcputhreads in $THREADCOUNT_EQ16
		do
		
		# for numsubcputhreads_pow in $THREADPOW_EQ0 $THREADPOW_EQ1 $THREADPOW_EQ2 $THREADPOW_EQ3 $THREADPOW_EQ4
		for numsubcputhreads_pow in $THREADPOW_EQ4
		# for numsubcputhreads_pow in $THREADPOW_EQ0
		# for numsubcputhreads_pow in $THREADPOW_EQ2
		
		do
			### >>> LOOP3: locke (kernel-only evaluation)
			# for locke in $_NOLOCKE
			for locke in $_LOCKE
			# for locke in $_LOCKE $_NOLOCKE
			do
				### >>> LOOP3: datasets
				
				for dataset in $ORKUT_3M_106M
				# for dataset in $HOLLYWOOD_1M_57M
				# for dataset in $INDOCHINA_7M_194M
				# for dataset in $KRON21_2M_91M
				# for dataset in $RGG_17M_132M
				# for dataset in $ROADNET_2M_3M
				# for dataset in $FLICKR_1M_10M
				# for dataset in $ORKUT_3M_106M $HOLLYWOOD_1M_57M $INDOCHINA_7M_194M $KRON21_2M_91M $RGG_17M_132M $ROADNET_2M_3M $FLICKR_1M_10M
				
				# for dataset in $TWITTER_67M
				# for dataset in $MOLIERE2016_33M
				# for dataset in $LARGEDATASET_67M
				# for dataset in $LARGEDATASET_268M
				# for dataset in $LARGEDATASET_1B
				# for dataset in $LARGEDATASET_4B
				# for dataset in $TWITTER_67M $MOLIERE2016_33M
				
				# for dataset in $ORKUT_3M_106M $HOLLYWOOD_1M_57M $INDOCHINA_7M_194M $KRON21_2M_91M $RGG_17M_132M $ROADNET_2M_3M $FLICKR_1M_10M $TWITTER_67M $MOLIERE2016_33M
				do
					# for evaluation_param0 in 0 4
					for evaluation_param0 in 4
					do
						KERNELBACKUP_DIR="${ROOTDIR}/ActsOfAGraph_Kernels"
						KERNELBACKUP_NAME="goldenkernel${ALGORITHMABBRV}${numsubcputhreads_pow}"
						# KERNELBACKUP_NAME="kernel_${SETUP_NAME}_${evaluation_type}_param${evaluation_param0}"
						
						RESULTSBACKUP_DIR="${ROOTDIR}/ActsOfAGraph_Results"
						RESULT_NAME="result_${SETUP_NAME}_${numcputhreads}threads_${numsubcputhreads_pow}subthreadspow_${locke}_${evaluation_type}_evaluation_param${evaluation_param0}"
						PROFILESUMMARY_NAME="profile_summary_${SETUP_NAME}_${numcputhreads}threads_${numsubcputhreads_pow}subthreadspow_${locke}_${evaluation_type}_evaluation_param${evaluation_param0}"
						
						if [ $dataset == $ORKUT_3M_106M ]  
						then	
							DATASET="_ORKUT_3M_106M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_orkut.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_orkut.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_orkut.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_orkut.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_orkut"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_orkut.xo"
							# BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_orkut.xclbin"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_orkut.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_orkut.out"
						elif [ $dataset == $HOLLYWOOD_1M_57M ]  
						then	
							DATASET="_HOLLYWOOD_1M_57M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_hollywood.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_hollywood.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_hollywood.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_hollywood.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_hollywood"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_hollywood.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_hollywood.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_hollywood.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_hollywood.out"
						elif [ $dataset == $INDOCHINA_7M_194M ]  
						then	
							DATASET="_INDOCHINA_7M_194M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_indochina.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_indochina.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_indochina.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_indochina.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_indochina"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_indochina.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_indochina.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_indochina.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_indochina.out"
						elif [ $dataset == $KRON21_2M_91M ]  
						then
							DATASET="_KRON21_2M_91M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_kron21.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_kron21.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron21.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron21.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron21"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron21.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron21.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron21.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_kron21.out"
						elif [ $dataset == $RGG_17M_132M ]  
						then	
							DATASET="_RGG_17M_132M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_rgg.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_rgg.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_rgg.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_rgg.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_rgg"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_rgg.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_rgg.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_rgg.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_rgg.out"
						elif [ $dataset == $ROADNET_2M_3M ]  
						then	
							DATASET="_ROADNET_2M_3M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_roadnet.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_roadnet.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_roadnet.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_roadnet.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_roadnet"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_roadnet.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_roadnet.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_roadnet.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_roadnet.out"
						elif [ $dataset == $FLICKR_1M_10M ]  
						then	
							DATASET="_FLICKR_1M_10M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_flickr.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_flickr.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_flickr.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_flickr.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_flickr"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_flickr.xo"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_flickr.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_flickr.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_flickr.out"
						
						elif [ $dataset == $TWITTER_67M ]  
						then	
							DATASET="_TWITTER_67M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_twitter26.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_twitter26.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_twitter26.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_twitter26.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26.xo"
							# BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26.xclbin"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_twitter26.out"
						elif [ $dataset == $MOLIERE2016_33M ]  
						then	
							DATASET="_MOLIERE2016_33M"
							KERNEL_NAME="${KERNELBACKUP_NAME}_moliere33.xclbin"
							KERNEL_NAME_AWS="${KERNELBACKUP_NAME}_moliere33.awsxclbin"
							
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_moliere33.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_moliere33.csv"
							BACKUPDIR_HOST="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_moliere33"
							BACKUPDIR_KERNELXO="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_moliere33.xo"
							# BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_moliere33.xclbin"
							BACKUPDIR_KERNELXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}.xclbin"
							BACKUPDIR_KERNELAWSXCLBIN="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_moliere33.awsxclbin"
							BACKUPDIR_NOHUP="${KERNELBACKUP_DIR}/${KERNELBACKUP_NAME}_moliere33.out"
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

						if [ $setup == $SW__ACTGRAPH_SETUP__PR_ALGORITHM ] # pr 
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $HW__ACTGRAPH_SETUP__PR_ALGORITHM ]
						then
							make cleanall
							# rm -rf host
							
							# make host
							# ./host $BACKUPDIR_KERNELXCLBIN
							# ./host $BACKUPDIR_KERNELXCLBIN > $RESULTDIR_RESULT
							
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
						elif [ $setup == $SW__GRAFBOOST_SETUP__PR_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							make demo_grafboost_nthreads
							# make demo_grafboost_nthreads > $RESULTDIR_RESULT
							
						elif [ $setup == $SW__ACTGRAPH_SETUP__BFS_ALGORITHM ] # bfs 
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $HW__ACTGRAPH_SETUP__BFS_ALGORITHM ]
						then
							make cleanall
							# rm -rf host
							
							make host
							./host $BACKUPDIR_KERNELXCLBIN
							# ./host $BACKUPDIR_KERNELXCLBIN > $RESULTDIR_RESULT
							
							# make build_host_aws
							# ./host $BACKUPDIR_KERNELAWSXCLBIN
							# ./host $BACKUPDIR_KERNELAWSXCLBIN > $RESULTDIR_RESULT
							wait 
							if test -f "profile_summary.csv"; then
								echo "profile_summary.csv exist"
								cp profile_summary.csv $RESULTDIR_PROFILESUMMARY
							fi	
						elif [ $setup == $SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM ]
						then
							make cleanall
							make all TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
							XCL_EMULATION_MODE=sw_emu ./host xclbin/topkernel.sw_emu.xilinx_u280_xdma_201910_1.xclbin
						elif [ $setup == $SW__GRAFBOOST_SETUP__BFS_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							# make demo_grafboost_nthreads_debug
							make demo_grafboost_nthreads
							# make demo_grafboost_nthreads > $RESULTDIR_RESULT
							
						elif [ $setup == $SW__ACTGRAPH_SETUP__SSSP_ALGORITHM ] # sssp 
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $HW__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
						then
							make cleanall
							# rm -rf host
							
							make host
							./host $BACKUPDIR_KERNELXCLBIN
							# ./host $BACKUPDIR_KERNELXCLBIN > $RESULTDIR_RESULT
							
							# make build_host_aws
							# ./host $BACKUPDIR_KERNELAWSXCLBIN
							# ./host $BACKUPDIR_KERNELAWSXCLBIN > $RESULTDIR_RESULT
							wait 
							if test -f "profile_summary.csv"; then
								echo "profile_summary.csv exist"
								cp profile_summary.csv $RESULTDIR_PROFILESUMMARY
							fi	
						elif [ $setup == $SWEMU__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
						then
							make cleanall
							make all TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
							XCL_EMULATION_MODE=sw_emu ./host xclbin/topkernel.sw_emu.xilinx_u280_xdma_201910_1.xclbin
						elif [ $setup == $SW__GRAFBOOST_SETUP__SSSP_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							# make demo_grafboost_nthreads_debug
							make demo_grafboost_nthreads
							# make demo_grafboost_nthreads > $RESULTDIR_RESULT
							
						elif [ $setup == $SW__ACTGRAPH_SETUP__BC_ALGORITHM ] # bc 
						then
							make cleanall
							# make build_acts_nthreads
							# make demo_acts_nthreads
							make demo_acts_nthreads > $RESULTDIR_RESULT
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
						elif [ $setup == $SW__GRAFBOOST_SETUP__BC_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							# make demo_grafboost_nthreads
							make demo_grafboost_nthreads > $RESULTDIR_RESULT
							
						elif [ $setup == $SW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM ] # advance 
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $HW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM ]
						then
							make cleanall
							# rm -rf host
							
							make host
							./host $BACKUPDIR_KERNELXCLBIN
							# ./host $BACKUPDIR_KERNELXCLBIN > $RESULTDIR_RESULT
							
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
						elif [ $setup == $SWEMU__ACTGRAPH_SETUP__ADVANCE_ALGORITHM ]
						then
							make cleanall
							make all TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
							XCL_EMULATION_MODE=sw_emu ./host xclbin/topkernel.sw_emu.xilinx_u280_xdma_201910_1.xclbin
						elif [ $setup == $SW__GRAFBOOST_SETUP__ADVANCE_ALGORITHM ] 
						then
							make cleanall
							# make build_grafboost_nthreads
							make demo_grafboost_nthreads
							# make demo_grafboost_nthreads > $RESULTDIR_RESULT
							# make demo_grafboost_nthreads_debug
						
						elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ] # syn
						then
							make cleanall
							rm -rf xclbin
							make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm > nohupsyn.out 
							
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
							
							if test -f "host"; then
								# cp host $BACKUPDIR_HOST
								# cp xclbin/topkernel.hw.xilinx_u280_xdma_201910_1.xo $BACKUPDIR_KERNELXO
								cp xclbin/topkernel.hw.xilinx_u280_xdma_201910_1.xclbin $BACKUPDIR_KERNELXCLBIN
								# cp nohupsyn.out $BACKUPDIR_NOHUP
								echo "host, kernel.xo, kernel.xclbin, nohupsyn.out saved"
							fi
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
						elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM ] # syn
						then
							make cleanall
							rm -rf xclbin
							make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm > nohupsyn.out 
							
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
							
							if test -f "host"; then
								cp xclbin/topkernel.hw.xilinx_u280_xdma_201910_1.xclbin $BACKUPDIR_KERNELXCLBIN
								echo "host, kernel.xo, kernel.xclbin, nohupsyn.out saved"
							fi
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
						elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM ] # syn
						then
							make cleanall
							rm -rf xclbin
							make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm > nohupsyn.out 
							
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
							
							if test -f "host"; then
								cp xclbin/topkernel.hw.xilinx_u280_xdma_201910_1.xclbin $BACKUPDIR_KERNELXCLBIN
								echo "host, kernel.xo, kernel.xclbin, nohupsyn.out saved"
							fi
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
						elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__ADVANCE_ALGORITHM ]
						then
							make cleanall
							rm -rf xclbin
							make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm > nohupsyn.out 
							# make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm
							
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
							
							if test -f "host"; then
								# cp host $BACKUPDIR_HOST
								# cp xclbin/topkernel.hw.xilinx_u280_xdma_201910_1.xo $BACKUPDIR_KERNELXO
								cp xclbin/topkernel.hw.xilinx_u280_xdma_201910_1.xclbin $BACKUPDIR_KERNELXCLBIN
								# cp nohupsyn.out $BACKUPDIR_NOHUP
								echo "host, kernel.xo, kernel.xclbin, nohupsyn.out saved"
							fi
							# echo "sleeping for 2 minuites before continuing ...."
							# sleep 120
						elif [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ]
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





