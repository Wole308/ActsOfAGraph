#!/bin/bash
#!/bin/bash
#SBATCH --job-name=all_runs 
#SBATCH --cpus-per-task=1 
#SBATCH --error="all_runs.err" 
#SBATCH --output=all_runs.log 
#SBATCH --nodelist=slurm1
# ssh -X centos@52.54.149.43 -i /home/oj2zf/Documents/aws/Alif.pem

set -e # Courtesy : Jinja 2.0

ON=1
OFF=0

CRABTREE=$ON
AWS=$OFF
GUNROCK=$OFF

if [ $CRABTREE == $ON ]
then
	echo "crabtree env specified."
	ROOTDIR="/home/oj2zf/Documents/ActsOfAGraph"
	ENV="CRABTREE"
elif [ $AWS == $ON ]
then
	echo "aws env specified."
	ROOTDIR="/home/centos/src/project_data/oj2zf/ActsOfAGraph"
	source /opt/xilinx/xrt/setup.sh 
	source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
	ENV="AWS"
elif [ $AWS == $ON ]
then
	echo "gunrock env specified."
	ROOTDIR="/net/bigtemp/oj2zf/gunrock_wole"
	ENV="GUNROCK"
else
	echo "no env specified. specify crabtree, aws or gunrock"
fi

NCOMPUTEUNITS_IN_1KERNELS=$OFF
NCOMPUTEUNITS_IN_NKERNELS=$ON

KERNELTYPE="ACTSMODEL_LW"

XWARE="" 
SETUP="" 
ALGORITHM="" 
ALGORITHMABBRV="" 
DATASET=""
datasetpath=""

SETUP_NAME=""
RESULTDIR_RESULT=""
KERNEL_NAME=""
RESULTDIR_PROFILESUMMARY=""

SW__ACTGRAPH_SETUP__PR_ALGORITHM=1
HW__ACTGRAPH_SETUP__PR_ALGORITHM=2
SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM=3
SW__GRAFBOOST_SETUP__PR_ALGORITHM=4
SW__GUNROCK_SETUP__PR_ALGORITHM=5

SW__ACTGRAPH_SETUP__BFS_ALGORITHM=6
HW__ACTGRAPH_SETUP__BFS_ALGORITHM=7
SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM=8
SW__GRAFBOOST_SETUP__BFS_ALGORITHM=9
SW__GUNROCK_SETUP__BFS_ALGORITHM=10

SW__ACTGRAPH_SETUP__SSSP_ALGORITHM=11
HW__ACTGRAPH_SETUP__SSSP_ALGORITHM=12
SWEMU__ACTGRAPH_SETUP__SSSP_ALGORITHM=13
SW__GRAFBOOST_SETUP__SSSP_ALGORITHM=14
SW__GUNROCK_SETUP__SSSP_ALGORITHM=15

SW__ACTGRAPH_SETUP__BC_ALGORITHM=16
HW__ACTGRAPH_SETUP__BC_ALGORITHM=17
SWEMU__ACTGRAPH_SETUP__BC_ALGORITHM=18
SW__GRAFBOOST_SETUP__BC_ALGORITHM=19
SW__GUNROCK_SETUP__BC_ALGORITHM=20

SW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=21
HW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=22
SWEMU__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=23
SW__GRAFBOOST_SETUP__ADVANCE_ALGORITHM=24
SW__GUNROCK_SETUP__ADVANCE_ALGORITHM=25

CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM=26
CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM=27
CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM=28
CTHWSYN__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=29

AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM=30
AWSHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM=31
AWSHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM=32
AWSHWSYN__ACTGRAPH_SETUP__ADVANCE_ALGORITHM=33

HW__ACTGRAPH_SETUP__PR_VHLS=34
HW__ACTGRAPH_SETUP__BFS_VHLS=35
HW__ACTGRAPH_SETUP__SSSP_VHLS=36

NODATASET=50
ORKUT_3M_106M=51
HOLLYWOOD_1M_57M=52
INDOCHINA_7M_194M=53
KRON21_2M_91M=54
RGG_17M_132M=55
ROADNET_2M_3M=56
FLICKR_1M_10M=57

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

NUMTHREADS_EQ0=0
NUMTHREADS_EQ1=1
NUMTHREADS_EQ2=2
NUMTHREADS_EQ3=3
NUMTHREADS_EQ4=4
NUMTHREADS_EQ6=6
NUMTHREADS_EQ8=8
NUMTHREADS_EQ10=10
NUMTHREADS_EQ12=12
NUMTHREADS_EQ14=14
NUMTHREADS_EQ16=16
NUMTHREADS_EQ18=18
NUMTHREADS_EQ20=20
NUMTHREADS_EQ24=24
NUMTHREADS_EQ28=28
NUMTHREADS_EQ32=32

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
	# for setup in $HW__ACTGRAPH_SETUP__PR_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__PR_ALGORITHM
	# for setup in $SW__GUNROCK_SETUP__PR_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__PR_VHLS
	# for setup in $CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM
	# for setup in $AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__BFS_ALGORITHM
	for setup in $SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__BFS_ALGORITHM
	# for setup in $SW__GUNROCK_SETUP__BFS_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__BFS_VHLS
	# for setup in $CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $AWSHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__SSSP_ALGORITHM
	# for setup in $SW__GUNROCK_SETUP__SSSP_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__SSSP_VHLS
	# for setup in $CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $AWSHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__BC_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__BC_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__BC_ALGORITHM
	# for setup in $SW__GUNROCK_SETUP__BC_ALGORITHM
	
	# for setup in $SW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__ADVANCE_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__ADVANCE_ALGORITHM
	# for setup in $SW__GRAFBOOST_SETUP__ADVANCE_ALGORITHM
	# for setup in $SW__GUNROCK_SETUP__ADVANCE_ALGORITHM
	# for setup in $CTHWSYN__ACTGRAPH_SETUP__ADVANCE_ALGORITHM
	# for setup in $AWSHWSYN__ACTGRAPH_SETUP__ADVANCE_ALGORITHM

	# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM $SW__ACTGRAPH_SETUP__BFS_ALGORITHM $SW__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $SW__GUNROCK_SETUP__PR_ALGORITHM $SW__GUNROCK_SETUP__BFS_ALGORITHM $SW__GUNROCK_SETUP__SSSP_ALGORITHM
	
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
		elif [ $setup == $SW__GUNROCK_SETUP__PR_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="GUNROCK_SETUP" 
			ALGORITHM="PR_ALGORITHM" 
			ALGORITHMABBRV="pr"
			SETUP_NAME="gunrock_pr_sw"
			
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
		elif [ $setup == $SW__GUNROCK_SETUP__BFS_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="GUNROCK_SETUP" 
			ALGORITHM="BFS_ALGORITHM" 
			ALGORITHMABBRV="bfs"
			SETUP_NAME="gunrock_bfs_sw"
			
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
		elif [ $setup == $SW__GUNROCK_SETUP__SSSP_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="GUNROCK_SETUP" 
			ALGORITHM="SSSP_ALGORITHM" 
			ALGORITHMABBRV="sssp"
			SETUP_NAME="gunrock_sssp_sw"
			
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
		elif [ $setup == $SW__GUNROCK_SETUP__BC_ALGORITHM ]
		then
			XWARE="SW" 
			SETUP="GUNROCK_SETUP" 
			ALGORITHM="BC_ALGORITHM" 
			ALGORITHMABBRV="bc"
			SETUP_NAME="gunrock_bc_sw"
		
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
		elif [ $setup == $SW__GUNROCK_SETUP__ADVANCE_ALGORITHM ] # advance
		then 
			XWARE="SW" 
			SETUP="GUNROCK_SETUP" 
			ALGORITHM="ADVANCE_ALGORITHM"
			ALGORITHMABBRV="adv"
			SETUP_NAME="gunrock_adv_sw"
			
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
		elif [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM ]
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
		elif [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
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
			
		elif [ $setup == $HW__ACTGRAPH_SETUP__PR_VHLS ] #nothing
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
			
		elif [ $setup == $HW__ACTGRAPH_SETUP__BFS_VHLS ] #nothing
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
			
		elif [ $setup == $HW__ACTGRAPH_SETUP__SSSP_VHLS ] #nothing
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
			
		else 
			XWARE="" 
			SETUP="" 
			ALGORITHM="" 
			ALGORITHMABBRV=""
		fi

		for numsupercputhreads in $THREADCOUNT_EQ1
		do

		for numcputhreads in $THREADCOUNT_EQ1
		do
		
		# for numsubcputhreads in $NUMTHREADS_EQ0 $NUMTHREADS_EQ1 $NUMTHREADS_EQ2 $NUMTHREADS_EQ4 $NUMTHREADS_EQ8 $NUMTHREADS_EQ12 $NUMTHREADS_EQ16
		# for numsubcputhreads in $NUMTHREADS_EQ0
		# for numsubcputhreads in $NUMTHREADS_EQ1
		# for numsubcputhreads in $NUMTHREADS_EQ2
		# for numsubcputhreads in $NUMTHREADS_EQ3
		# for numsubcputhreads in $NUMTHREADS_EQ4
		for numsubcputhreads in $NUMTHREADS_EQ12
		# for numsubcputhreads in $NUMTHREADS_EQ14
		# for numsubcputhreads in $NUMTHREADS_EQ16 #
		# for numsubcputhreads in $NUMTHREADS_EQ18 #
		# for numsubcputhreads in $NUMTHREADS_EQ20
		# for numsubcputhreads in $NUMTHREADS_EQ24
		# for numsubcputhreads in $NUMTHREADS_EQ28
		# for numsubcputhreads in $NUMTHREADS_EQ32
		
		do
			### >>> LOOP3: locke (kernel-only evaluation)
			# for locke in $_NOLOCKE
			for locke in $_LOCKE
			# for locke in $_LOCKE $_NOLOCKE
			do
				### >>> LOOP3: datasets
				
				# for dataset in $NODATASET
				# for dataset in $HOLLYWOOD_1M_57M
				# for dataset in $KRON21_2M_91M #
				for dataset in $ORKUT_3M_106M #
				# for dataset in $INDOCHINA_7M_194M
				# for dataset in $RGG_17M_132M
				# for dataset in $ROADNET_2M_3M
				# for dataset in $FLICKR_1M_10M
				# for dataset in $HOLLYWOOD_1M_57M $KRON21_2M_91M $ORKUT_3M_106M
				
				# for dataset in $TWITTER_67M #
				# for dataset in $MOLIERE2016_33M
				# for dataset in $LARGEDATASET_67M
				# for dataset in $LARGEDATASET_268M
				# for dataset in $LARGEDATASET_1B
				# for dataset in $LARGEDATASET_4B
				# for dataset in $TWITTER_67M $MOLIERE2016_33M
				
				# for dataset in $NODATASET $ORKUT_3M_106M $HOLLYWOOD_1M_57M $KRON21_2M_91M
				do
					# for evaluation_param0 in 0 4
					for evaluation_param0 in 0
					do
						BACKUPDIR_KERNELXCLBIN="${ROOTDIR}/synkernels/goldenkernel${ALGORITHMABBRV}${numsubcputhreads}.xclbin"
						BACKUPDIR_KERNELXCLBIN1="${ROOTDIR}/synkernels/goldenkernelproc${ALGORITHMABBRV}${numsubcputhreads}.xclbin"
						BACKUPDIR_KERNELXCLBIN2="${ROOTDIR}/synkernels/goldenkernelsync${ALGORITHMABBRV}${numsubcputhreads}.xclbin"
						
						BACKUPDIR_AWSKERNELXCLBIN="${ROOTDIR}/synkernels/goldenkernel${ALGORITHMABBRV}${numsubcputhreads}.awsxclbin"
						BACKUPDIR_AWSKERNELXCLBIN1="${ROOTDIR}/synkernels/goldenkernelproc${ALGORITHMABBRV}${numsubcputhreads}.awsxclbin"
						BACKUPDIR_AWSKERNELXCLBIN2="${ROOTDIR}/synkernels/goldenkernelsync${ALGORITHMABBRV}${numsubcputhreads}.awsxclbin"
						
						RESULTSBACKUP_DIR="${ROOTDIR}/results"
						RESULT_NAME="result_${SETUP_NAME}"
						PROFILESUMMARY_NAME="profile_summary_${ALGORITHMABBRV}_${numsubcputhreads}threads_${evaluation_type}_evp${evaluation_param0}"
						
						if [ $dataset == $NODATASET ]  
						then	
							DATASET=""
							datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/small/bips98_606.mtx"
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_bips98_606.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_bips98_606.csv"
						elif [ $dataset == $ORKUT_3M_106M ]  
						then	
							DATASET="_ORKUT_3M_106M"
							datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/soc-orkut/soc-orkut.mtx"
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_orkut.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_orkut.csv"
						elif [ $dataset == $HOLLYWOOD_1M_57M ]  
						then	
							DATASET="_HOLLYWOOD_1M_57M"
							datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/hollywood-2009/hollywood-2009.mtx"
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_hollywood.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_hollywood.csv"
						elif [ $dataset == $INDOCHINA_7M_194M ]  
						then	
							DATASET="_INDOCHINA_7M_194M"
							datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/indochina-2004/indochina-2004.mtx"
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_indochina.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_indochina.csv"
						elif [ $dataset == $KRON21_2M_91M ]  
						then
							DATASET="_KRON21_2M_91M"
							datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/kron_g500-logn21/kron_g500-logn21.mtx"
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron21.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron21.csv"
						elif [ $dataset == $RGG_17M_132M ]  
						then	
							DATASET="_RGG_17M_132M"
							datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/rgg_n_2_24_s0/rgg_n_2_24_s0.mtx"
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_rgg.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_rgg.csv"
						elif [ $dataset == $ROADNET_2M_3M ]  
						then	
							DATASET="_ROADNET_2M_3M"
							datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/roadNet-CA/roadNet-CA.mtx"
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_roadnet.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_roadnet.csv"
						elif [ $dataset == $FLICKR_1M_10M ]  
						then	
							DATASET="_FLICKR_1M_10M"
							datasetpath=""
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_flickr.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_flickr.csv"
						
						elif [ $dataset == $TWITTER_67M ]  
						then	
							DATASET="_TWITTER_67M"
							datasetpath=""
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_twitter26.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_twitter26.csv"
						elif [ $dataset == $MOLIERE2016_33M ]  
						then	
							DATASET="_MOLIERE2016_33M"
							datasetpath=""
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_moliere33.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_moliere33.csv"
						elif [ $dataset == $LARGEDATASET_67M ]  
						then	
							DATASET="_LARGEDATASET_67M"
							datasetpath=""
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron26.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron26.csv"		
						elif [ $dataset == $LARGEDATASET_268M ]
						then
							DATASET="_LARGEDATASET_268M"
							datasetpath=""
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron28.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron28.csv"
						elif [ $dataset == $LARGEDATASET_1B ]
						then
							DATASET="_LARGEDATASET_1B"
							datasetpath=""
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron30.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron30.csv"
						elif [ $dataset == $LARGEDATASET_4B ]
						then
							DATASET="_LARGEDATASET_4B"
							datasetpath=""
							RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${RESULT_NAME}_kron32.out"
							RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron32.csv"
						else 
							DATASET=""
							datasetpath=""
							RESULTDIR_RESULT=""
						fi
						
						make generatesrcs XWARE=$XWARE SETUP=$SETUP ALGORITHM=$ALGORITHM DATASET=$DATASET NUMSUPERCPUTHREADS=$numsupercputhreads NUMCPUTHREADS=$numcputhreads NUMSUBCPUTHREADS=$numsubcputhreads LOCKE=$locke EVALUATION_TYPE=$evaluation_type EVALUATION_PARAM0=$evaluation_param0				

						# ================================================ PAGERANK ================================================
						if [ $setup == $SW__ACTGRAPH_SETUP__PR_ALGORITHM ] # pr 
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $SW__GRAFBOOST_SETUP__PR_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							# make demo_grafboost_nthreads
							make demo_grafboost_nthreads > $RESULTDIR_RESULT
						elif [ $setup == $SW__GUNROCK_SETUP__PR_ALGORITHM ]
						then
							echo 'SW__GUNROCK_SETUP__PR_ALGORITHM called.'
							/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market $datasetpath --normalized --compensate --undirected > $RESULTDIR_RESULT
							# make cleanall
							
						# ================================================ BFS ================================================
						elif [ $setup == $SW__ACTGRAPH_SETUP__BFS_ALGORITHM ] # bfs 
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $SW__GRAFBOOST_SETUP__BFS_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							# make demo_grafboost_nthreads_debug
							# make demo_grafboost_nthreads
							make demo_grafboost_nthreads > $RESULTDIR_RESULT
						elif [ $setup == $SW__GUNROCK_SETUP__BFS_ALGORITHM ]
						then
							echo 'SW__GUNROCK_SETUP__BFS_ALGORITHM called.'
							/net/bigtemp/oj2zf/gunrock_wole/build/bin/bfs market $datasetpath --normalized --compensate --undirected > $RESULTDIR_RESULT

						# ================================================ SSSP ================================================
						elif [ $setup == $SW__ACTGRAPH_SETUP__SSSP_ALGORITHM ] # sssp 
						then
							make cleanall
							# make build_acts_nthreads
							make demo_acts_nthreads
							# make demo_acts_nthreads > $RESULTDIR_RESULT
							# make demo_acts_nthreads_debug
						elif [ $setup == $SW__GRAFBOOST_SETUP__SSSP_ALGORITHM ]
						then
							make cleanall
							# make build_grafboost_nthreads
							# make demo_grafboost_nthreads_debug
							make demo_grafboost_nthreads
							# make demo_grafboost_nthreads > $RESULTDIR_RESULT
						elif [ $setup == $SW__GUNROCK_SETUP__SSSP_ALGORITHM ]
						then
							# make cleanall
							echo 'SW__GUNROCK_SETUP__SSSP_ALGORITHM called.'
							/net/bigtemp/oj2zf/gunrock_wole/build/bin/sssp market $datasetpath --normalized --compensate --undirected > $RESULTDIR_RESULT
							
						# ================================================ SYNTHESIS AND SIMULATIONS ================================================
						elif [ $setup == $HW__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $HW__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $HW__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
						then
							make cleanall
							if [ $CRABTREE == $ON ]
							then
								if [ $NCOMPUTEUNITS_IN_NKERNELS == $ON ]
								then
									echo "crabtree.NCOMPUTEUNITS_IN_NKERNELS setup specified."
									make host
									./host $BACKUPDIR_KERNELXCLBIN1 $BACKUPDIR_KERNELXCLBIN2
								elif [ $NCOMPUTEUNITS_IN_1KERNELS == $ON ]
								then
									echo "crabtree.NCOMPUTEUNITS_IN_1KERNELS setup specified."
									make host
									./host $BACKUPDIR_KERNELXCLBIN
								else
									echo "not specified (7). specify NCOMPUTEUNITS_IN_NKERNELS or NCOMPUTEUNITS_IN_1KERNELS"
								fi
							elif [ $AWS == $ON ]
							then
								echo "aws setup specified."
								sudo su
								make host
								source /opt/xilinx/xrt/setup.sh 
								source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
								./host $BACKUPDIR_AWSKERNELXCLBIN
								# ./host $BACKUPDIR_AWSKERNELXCLBIN > $RESULTDIR_RESULT
							else
								echo "no setup specified. specify crabtree or aws"
							fi
							wait 
							if test -f "profile_summary.csv"; then
								echo "profile_summary.csv exist"
								cp profile_summary.csv $RESULTDIR_PROFILESUMMARY
							fi	
							
						elif [ $setup == $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $SWEMU__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
						then
							if [ $CRABTREE == $ON ]
							then
								if [ $NCOMPUTEUNITS_IN_NKERNELS == $ON ]
								then
									echo "crabtree.NCOMPUTEUNITS_IN_NKERNELS setup specified."
									make cleanall
									make all_nk TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
									cp xclbin/topkernelproc.sw_emu.xilinx_u280_xdma_201910_1.xclbin $BACKUPDIR_KERNELXCLBIN1
									cp xclbin/topkernelsync.sw_emu.xilinx_u280_xdma_201910_1.xclbin $BACKUPDIR_KERNELXCLBIN2
									XCL_EMULATION_MODE=sw_emu ./host $BACKUPDIR_KERNELXCLBIN1 $BACKUPDIR_KERNELXCLBIN2
								elif [ $NCOMPUTEUNITS_IN_1KERNELS == $ON ]
								then
									echo "crabtree.NCOMPUTEUNITS_IN_1KERNELS setup specified."
									make cleanall
									make all TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
									cp xclbin/topkernel.sw_emu.xilinx_u280_xdma_201910_1.xclbin $BACKUPDIR_AWSKERNELXCLBIN
									XCL_EMULATION_MODE=sw_emu ./host $BACKUPDIR_AWSKERNELXCLBIN
								else
									echo "not specified (7). specify NCOMPUTEUNITS_IN_NKERNELS or NCOMPUTEUNITS_IN_1KERNELS"
								fi
							elif [ $AWS == $ON ]
							then
								echo "aws setup specified."
								make cleanall
								make all TARGET=sw_emu DEVICE=/home/centos/src/project_data/aws-fpga/SDAccel/aws_platform/xilinx_aws-vu9p-f1-04261818_dynamic_5_0/xilinx_aws-vu9p-f1-04261818_dynamic_5_0.xpfm		
								XCL_EMULATION_MODE=sw_emu ./host xclbin/topkernel.sw_emu.xilinx_u280_xdma_201910_1.xclbin
							else
								echo "no setup specified. specify crabtree or aws"
							fi
						
						elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
						then
							if [ $NCOMPUTEUNITS_IN_NKERNELS == $ON ]
							then
								make cleanall
								rm -rf xclbin
								make all_proc DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm > nohupsyn.out 
							
								echo "sleeping for 2 minuites before continuing ...."
								sleep 120
								
								if test -f "host"; then
									cp xclbin/topkernelproc.hw.xilinx_u280_xdma_201910_1.xclbin "synkernels/goldenkernelproc${ALGORITHMABBRV}${numsubcputhreads}.xclbin"
									# cp xclbin/topkernelsync.hw.xilinx_u280_xdma_201910_1.xclbin "synkernels/goldenkernelsync${ALGORITHMABBRV}${numsubcputhreads}.xclbin"
									# cp xclbin/topkernelproc.hw.xilinx_u280_xdma_201910_1.xclbin $BACKUPDIR_KERNELXCLBIN
									echo "host, kernel.xo, kernel.xclbin, nohupsyn.out saved"
								fi
								echo "sleeping for 2 minuites before continuing ...."
								sleep 120
							elif [ $NCOMPUTEUNITS_IN_1KERNELS == $ON ]
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
							else
								echo "not specified (7). specify NCOMPUTEUNITS_IN_NKERNELS or NCOMPUTEUNITS_IN_1KERNELS"
							fi
						
						elif [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
						then
							source /opt/xilinx/xrt/setup.sh 
							source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
							
							make cleanall
							rm -rf xclbin
							make all DEVICE=/home/centos/src/project_data/aws-fpga/SDAccel/aws_platform/xilinx_aws-vu9p-f1-04261818_dynamic_5_0/xilinx_aws-vu9p-f1-04261818_dynamic_5_0.xpfm > nohupsyn.out
							
							cp -rf xclbin/topkernel.hw.xilinx_aws_vu9p_f1.xclbin kernel.xclbin
							
							echo "sleeping for 2 minuites before continuing ...."
							sleep 120
							
							if test -f "host"; then
								# cp kernel.xclbin $BACKUPDIR_KERNELXCLBIN
								cp kernel.xclbin $BACKUPDIR_KERNELXCLBIN
								
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
							cp -rf /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/kernel.awsxclbin $BACKUPDIR_AWSKERNELXCLBIN
							cd /home/centos/src/project_data/oj2zf/ActsOfAGraph
							
						elif [ $setup == $HW__ACTGRAPH_SETUP__PR_VHLS ] || [ $setup == $HW__ACTGRAPH_SETUP__BFS_VHLS ] || [ $setup == $HW__ACTGRAPH_SETUP__SSSP_VHLS ]
						then
							make cleanall
							# open X2go
							# open terminal
							# ssh -X centos@52.54.149.43 -i /home/oj2zf/Documents/aws/Alif.pem
							
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





