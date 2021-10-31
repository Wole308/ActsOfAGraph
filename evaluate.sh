#!/bin/bash
#!/bin/bash
#SBATCH --job-name=all_runs 
#SBATCH --cpus-per-task=1 
#SBATCH --error="all_runs.err" 
#SBATCH --output=all_runs.log 
#SBATCH --nodelist=slurm1
# ssh -X centos@52.54.149.43 -i /home/oj2zf/Documents/aws/Alif.pem /opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm

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
	DSA_NAME=xilinx_u280_xdma_201920_3
	DEVICEPATH=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
elif [ $AWS == $ON ]
then
	echo "aws env specified."
	ROOTDIR="/home/centos/src/project_data/oj2zf/ActsOfAGraph"
	source /opt/xilinx/xrt/setup.sh 
	source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
	ENV="AWS"
	DSA_NAME=xilinx_aws_vu9p_f1
	DEVICEPATH=/home/centos/src/project_data/aws-fpga/SDAccel/aws_platform/xilinx_aws-vu9p-f1-04261818_dynamic_5_0/xilinx_aws-vu9p-f1-04261818_dynamic_5_0.xpfm
elif [ $GUNROCK == $ON ]
then
	echo "gunrock env specified."
	ROOTDIR="/net/bigtemp/oj2zf/gunrock_wole"
	ENV="GUNROCK"
else
	echo "no env specified. specify crabtree, aws or gunrock"
fi

SYNKERNELSPATHNAME="synkernels_acts"
RESULTSPATHNAME="results_acts"
# SYNKERNELSPATHNAME="synkernels_prior"
# RESULTSPATHNAME="results_prior"
# SYNKERNELSPATHNAME="synkernels_trad"
# RESULTSPATHNAME="results_trad"

# ACTS_1BY2BY1=$OFF
# ACTS_2BY1BY1=$ON
ACTS_1BY2BY1=$ON
ACTS_2BY1BY1=$OFF

ACTS_CONFIG="1by2by1by1"
# ACTS_CONFIG="1by2by1by2"

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

CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM=26
CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM=27
CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM=28

AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM=30
AWSHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM=31
AWSHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM=32

HW__ACTGRAPH_SETUP__PR_VHLS=34
HW__ACTGRAPH_SETUP__BFS_VHLS=35
HW__ACTGRAPH_SETUP__SSSP_VHLS=36

NODATASET=0
ORKUT_3M_106M=1
HOLLYWOOD_1M_57M=2
KRON20_1M_45M=3
KRON21_2M_91M=4
KRON22_4M_200M=5

RMAT22_SPARSE0=30
RMAT22_SPARSE1=31
RMAT22_SPARSE2=32
RMAT22_SPARSE3=33
RMAT22_SPARSE4=34
RMAT22_SPARSE5=35
RMAT22_SPARSE6=36
RMAT22_SPARSE7=37

RMAT_RANGE0=40
RMAT_RANGE1=41
RMAT_RANGE2=42
RMAT_RANGE3=43
RMAT_RANGE4=44
RMAT_RANGE5=45
RMAT_RANGE6=46
RMAT_RANGE7=47

INDOCHINA_7M_194M=10
RGG_4M_32M=11
RGG_17M_132M=12
ROADNET_2M_3M=13
FLICKR_1M_10M=14

TWITTER_67M=20
MOLIERE2016_33M=21
LARGEDATASET_67M=22
LARGEDATASET_268M=23
LARGEDATASET_1B=24
LARGEDATASET_4B=25

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
NUMTHREADS_EQ5=5
NUMTHREADS_EQ6=6
NUMTHREADS_EQ7=7
NUMTHREADS_EQ8=8
NUMTHREADS_EQ9=9
NUMTHREADS_EQ10=10
NUMTHREADS_EQ11=11
NUMTHREADS_EQ12=12
NUMTHREADS_EQ13=13
NUMTHREADS_EQ14=14
NUMTHREADS_EQ15=15
NUMTHREADS_EQ16=16
NUMTHREADS_EQ17=17
NUMTHREADS_EQ18=18
NUMTHREADS_EQ19=19
NUMTHREADS_EQ20=20
NUMTHREADS_EQ24=24
NUMTHREADS_EQ28=28
NUMTHREADS_EQ32=32

NUMPARTITIONSPOW_EQ3=3
NUMPARTITIONSPOW_EQ4=4
NUMPARTITIONSPOW_EQ5=5
NUMPARTITIONSPOW_EQ6=6
NUMPARTITIONSPOW_EQ7=7
NUMPARTITIONSPOW_EQ8=8

SYNFREQUENCY_EQ60=60
SYNFREQUENCY_EQ120=120
SYNFREQUENCY_EQ180=180 
SYNFREQUENCY_EQ240=240
SYNFREQUENCY_EQ300=300				

_LOCKE="LOCKE"
_NOLOCKE="NOLOCKE"

### >>> LOOP0: evaluation types
# for evaluation_type in EV_CREATENDGRAPH #
for evaluation_type in EV_PERFORMANCEOFALGORITHM
# for evaluation_type in EV_SIMPLETEST
# for evaluation_type in EV_IMPACTOFRANGE #
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
	
	for setup in $SW__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__BFS_ALGORITHM
	# for setup in $SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM
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

	# for setup in $SW__ACTGRAPH_SETUP__PR_ALGORITHM $SW__ACTGRAPH_SETUP__BFS_ALGORITHM $SW__ACTGRAPH_SETUP__SSSP_ALGORITHM
	# for setup in $HW__ACTGRAPH_SETUP__PR_ALGORITHM $HW__ACTGRAPH_SETUP__BFS_ALGORITHM $HW__ACTGRAPH_SETUP__SSSP_ALGORITHM
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
		
		# for numsubcputhreads in $NUMTHREADS_EQ0
		# for numsubcputhreads in $NUMTHREADS_EQ1 
		# for numsubcputhreads in $NUMTHREADS_EQ2 #
		# for numsubcputhreads in $NUMTHREADS_EQ3
		for numsubcputhreads in $NUMTHREADS_EQ4 #
		# for numsubcputhreads in $NUMTHREADS_EQ5
		# for numsubcputhreads in $NUMTHREADS_EQ6 
		# for numsubcputhreads in $NUMTHREADS_EQ7
		# for numsubcputhreads in $NUMTHREADS_EQ8 
		# for numsubcputhreads in $NUMTHREADS_EQ9
		# for numsubcputhreads in $NUMTHREADS_EQ10 
		# for numsubcputhreads in $NUMTHREADS_EQ11
		# for numsubcputhreads in $NUMTHREADS_EQ12 
		# for numsubcputhreads in $NUMTHREADS_EQ14
		# for numsubcputhreads in $NUMTHREADS_EQ16 
		# for numsubcputhreads in $NUMTHREADS_EQ18
		# for numsubcputhreads in $NUMTHREADS_EQ20
		# for numsubcputhreads in $NUMTHREADS_EQ24 
		# for numsubcputhreads in $NUMTHREADS_EQ28
		# for numsubcputhreads in $NUMTHREADS_EQ32 
		# for numsubcputhreads in $NUMTHREADS_EQ1 $NUMTHREADS_EQ2
		# for numsubcputhreads in $NUMTHREADS_EQ4 $NUMTHREADS_EQ6
		# for numsubcputhreads in $NUMTHREADS_EQ8 $NUMTHREADS_EQ10
		# for numsubcputhreads in $NUMTHREADS_EQ12
		# for numsubcputhreads in $NUMTHREADS_EQ4 $NUMTHREADS_EQ6 $NUMTHREADS_EQ8 $NUMTHREADS_EQ10
		# for numsubcputhreads in $NUMTHREADS_EQ4 $NUMTHREADS_EQ5 $NUMTHREADS_EQ6 $NUMTHREADS_EQ7 $NUMTHREADS_EQ8
		# for numsubcputhreads in $NUMTHREADS_EQ0 $NUMTHREADS_EQ1 $NUMTHREADS_EQ2 $NUMTHREADS_EQ4 $NUMTHREADS_EQ8 $NUMTHREADS_EQ12 $NUMTHREADS_EQ16
		do
		
		for numpartitions_pow in $NUMPARTITIONSPOW_EQ4
		do
		
			### >>> LOOP3: locke (kernel-only evaluation)
			for locke in $_LOCKE
			do
				### >>> LOOP3: datasets
				
				# for dataset in $NODATASET
				for dataset in $ORKUT_3M_106M
				# for dataset in $HOLLYWOOD_1M_57M
				# for dataset in $KRON20_1M_45M
				# for dataset in $KRON21_2M_91M
				# for dataset in $KRON22_4M_200M
				
				# for dataset in $RMAT22_SPARSE0
				# for dataset in $RMAT22_SPARSE1
				# for dataset in $RMAT22_SPARSE2
				# for dataset in $RMAT22_SPARSE3
				# for dataset in $RMAT22_SPARSE4
				# for dataset in $RMAT22_SPARSE5
				# for dataset in $RMAT22_SPARSE6
				# for dataset in $RMAT22_SPARSE7
				
				# for dataset in $RMAT_RANGE0
				# for dataset in $RMAT_RANGE1
				# for dataset in $RMAT_RANGE2
				# for dataset in $RMAT_RANGE3
				# for dataset in $RMAT_RANGE4
				# for dataset in $RMAT_RANGE5
				# for dataset in $RMAT_RANGE6
				# for dataset in $RMAT_RANGE7
				
				# for dataset in $INDOCHINA_7M_194M
				# for dataset in $RGG_4M_32M
				# for dataset in $RGG_17M_132M
				# for dataset in $ROADNET_2M_3M
				# for dataset in $FLICKR_1M_10M
				
				# for dataset in $TWITTER_67M #
				# for dataset in $MOLIERE2016_33M
				# for dataset in $LARGEDATASET_67M
				# for dataset in $LARGEDATASET_268M
				# for dataset in $LARGEDATASET_1B
				# for dataset in $LARGEDATASET_4B
				# for dataset in $TWITTER_67M
				
				# for dataset in $RMAT_RANGE0 $RMAT_RANGE1 $RMAT_RANGE2
				# for dataset in $RMAT_RANGE4 $RMAT_RANGE5
				
				# for dataset in $ORKUT_3M_106M $HOLLYWOOD_1M_57M $RMAT22_SPARSE0 $RMAT22_SPARSE2 $RMAT22_SPARSE4
				# for dataset in $RMAT_RANGE0 $RMAT_RANGE1 $RMAT_RANGE2 $RMAT_RANGE3 $RMAT_RANGE4 $RMAT_RANGE5		
				do
					# for evaluation_param0 in 0 4
					for evaluation_param0 in 0
					do
						for synfreq in $SYNFREQUENCY_EQ300
						# for synfreq in $SYNFREQUENCY_EQ60 $SYNFREQUENCY_EQ120 $SYNFREQUENCY_EQ180 $SYNFREQUENCY_EQ240 $SYNFREQUENCY_EQ300
						do
							BACKUPDIR_KERNELXCLBIN="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernel${numsubcputhreads}${XWARE}${synfreq}MHz.xclbin"
							BACKUPDIR_KERNELXCLBIN1="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelproc${numsubcputhreads}${XWARE}${synfreq}MHz.xclbin"
							BACKUPDIR_KERNELXCLBIN2="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelsync${numsubcputhreads}${XWARE}${synfreq}MHz.xclbin"
							
							BACKUPDIR_AWSKERNELXCLBIN="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernel${numsubcputhreads}${XWARE}${synfreq}MHz.awsxclbin"
							BACKUPDIR_AWSKERNELXCLBIN1="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelproc${numsubcputhreads}${XWARE}${synfreq}MHz.awsxclbin"
							BACKUPDIR_AWSKERNELXCLBIN2="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelsync${numsubcputhreads}${XWARE}${synfreq}MHz.awsxclbin"
							
							BACKUPDIR_NOHUPOUT="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernel${numsubcputhreads}${XWARE}${synfreq}MHz.out"
							BACKUPDIR_NOHUPOUT1="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelproc${numsubcputhreads}${XWARE}${synfreq}MHz.out"
							BACKUPDIR_NOHUPOUT2="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelsync${numsubcputhreads}${XWARE}${synfreq}MHz.out"
							
							RESULTSBACKUP_DIR="${ROOTDIR}/outputs/${RESULTSPATHNAME}"
							RESULT_NAME="${ALGORITHMABBRV}${numsubcputhreads}${XWARE}${synfreq}MHz"
							PROFILESUMMARY_NAME="profile_summary_${ALGORITHMABBRV}_${numsubcputhreads}threads_${evaluation_type}_evp${evaluation_param0}"
							
							if [ $dataset == $NODATASET ]  
							then	
								DATASET=""
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/small/bips98_606.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/bips98_606_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_bips98_606.csv"
							elif [ $dataset == $ORKUT_3M_106M ]  
							then	
								DATASET="_ORKUT_3M_106M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/soc-orkut/soc-orkut.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/orkut_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_orkut.csv"
							elif [ $dataset == $HOLLYWOOD_1M_57M ]  
							then	
								DATASET="_HOLLYWOOD_1M_57M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/hollywood-2009/hollywood-2009.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/hollywood_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_hollywood.csv"
							elif [ $dataset == $KRON20_1M_45M ]  
							then
								DATASET="_KRON20_1M_45M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/kron_g500-logn20/kron_g500-logn20.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/kron20_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron20.csv"
							elif [ $dataset == $KRON21_2M_91M ]  
							then
								DATASET="_KRON21_2M_91M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/kron_g500-logn21/kron_g500-logn21.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/kron21_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron21.csv"
							elif [ $dataset == $KRON22_4M_200M ]  
							then
								DATASET="_KRON22_4M_200M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/kron_g500-logn21/kron_g500-logn21.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/kron22_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron21.csv"	
								
							elif [ $dataset == $RMAT22_SPARSE0 ]  
							then
								DATASET="_RMAT22_SPARSE0"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE0/RMAT22_SPARSE0.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMAT22SPARSE0_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMAT22SPARSE0.csv"	
							elif [ $dataset == $RMAT22_SPARSE1 ]  
							then
								DATASET="_RMAT22_SPARSE1"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE1/RMAT22_SPARSE1.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMAT22SPARSE1_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMAT22SPARSE1.csv"	
							elif [ $dataset == $RMAT22_SPARSE2 ]  
							then
								DATASET="_RMAT22_SPARSE2"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE2/RMAT22_SPARSE2.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMAT22SPARSE2_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMAT22SPARSE2.csv"	
							elif [ $dataset == $RMAT22_SPARSE3 ]  
							then
								DATASET="_RMAT22_SPARSE3"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE3/RMAT22_SPARSE3.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMAT22SPARSE3_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMAT22SPARSE0.csv"	
							elif [ $dataset == $RMAT22_SPARSE4 ]  
							then
								DATASET="_RMAT22_SPARSE4"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE4/RMAT22_SPARSE4.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMAT22SPARSE4_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMAT22SPARSE4.csv"	
							elif [ $dataset == $RMAT22_SPARSE5 ]  
							then
								DATASET="_RMAT22_SPARSE5"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE5/RMAT22_SPARSE5.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMAT22SPARSE5_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMAT22SPARSE5.csv"
							elif [ $dataset == $RMAT22_SPARSE6 ]  
							then
								DATASET="_RMAT22_SPARSE6"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE6/RMAT22_SPARSE6.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMAT22SPARSE6_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMAT22SPARSE6.csv"
							elif [ $dataset == $RMAT22_SPARSE7 ]  
							then
								DATASET="_RMAT22_SPARSE7"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE7/RMAT22_SPARSE7.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMAT22SPARSE7_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMAT22SPARSE7.csv"

							elif [ $dataset == $RMAT_RANGE0 ]  
							then
								DATASET="_RMAT_RANGE0"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT_RANGE0/RMAT_RANGE0.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMATRANGE0_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMATRANGE0.csv"
							elif [ $dataset == $RMAT_RANGE1 ]  
							then
								DATASET="_RMAT_RANGE1"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT_RANGE1/RMAT_RANGE1.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMATRANGE1_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMATRANGE1.csv"
							elif [ $dataset == $RMAT_RANGE2 ]  
							then
								DATASET="_RMAT_RANGE2"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT_RANGE2/RMAT_RANGE2.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMATRANGE2_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMATRANGE2.csv"
							elif [ $dataset == $RMAT_RANGE3 ]  
							then
								DATASET="_RMAT_RANGE3"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT_RANGE3/RMAT_RANGE3.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMATRANGE3_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMATRANGE3.csv"
							elif [ $dataset == $RMAT_RANGE4 ]  
							then
								DATASET="_RMAT_RANGE4"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT_RANGE4/RMAT_RANGE4.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMATRANGE4_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMATRANGE4.csv"
							elif [ $dataset == $RMAT_RANGE5 ]  
							then
								DATASET="_RMAT_RANGE5"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT_RANGE5/RMAT_RANGE5.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMATRANGE5_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMATRANGE5.csv"
							elif [ $dataset == $RMAT_RANGE6 ]  
							then
								DATASET="_RMAT_RANGE6"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT_RANGE6/RMAT_RANGE6.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMATRANGE6_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMATRANGE6.csv"
							elif [ $dataset == $RMAT_RANGE7 ]  
							then
								DATASET="_RMAT_RANGE7"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT_RANGE7/RMAT_RANGE7.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/RMATRANGE7_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_RMATRANGE7.csv"
						
							elif [ $dataset == $INDOCHINA_7M_194M ]  
							then	
								DATASET="_INDOCHINA_7M_194M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/indochina-2004/indochina-2004.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/indochina_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_indochina.csv"
							elif [ $dataset == $RGG_4M_32M ]  
							then	
								DATASET="_RGG_4M_32M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/rgg_n_2_22_s0/rgg_n_2_22_s0.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/rgg_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_rgg.csv"
							elif [ $dataset == $RGG_17M_132M ]  
							then	
								DATASET="_RGG_17M_132M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/rgg_n_2_24_s0/rgg_n_2_24_s0.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/rgg_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_rgg.csv"
							elif [ $dataset == $ROADNET_2M_3M ]  
							then	
								DATASET="_ROADNET_2M_3M"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/roadNet-CA/roadNet-CA.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/roadnet_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_roadnet.csv"
							elif [ $dataset == $FLICKR_1M_10M ]  
							then	
								DATASET="_FLICKR_1M_10M"
								datasetpath=""
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/flickr_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_flickr.csv"
								
							elif [ $dataset == $TWITTER_67M ]  
							then	
								DATASET="_TWITTER_67M"
								datasetpath=""
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/twitter26_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_twitter26.csv"
							elif [ $dataset == $MOLIERE2016_33M ]  
							then	
								DATASET="_MOLIERE2016_33M"
								datasetpath=""
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/moliere33_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_moliere33.csv"
							elif [ $dataset == $LARGEDATASET_67M ]  
							then	
								DATASET="_LARGEDATASET_67M"
								datasetpath=""
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/kron26_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron26.csv"		
							elif [ $dataset == $LARGEDATASET_268M ]
							then
								DATASET="_LARGEDATASET_268M"
								datasetpath=""
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/kron28_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron28.csv"
							elif [ $dataset == $LARGEDATASET_1B ]
							then
								DATASET="_LARGEDATASET_1B"
								datasetpath=""
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/kron30_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron30.csv"
							elif [ $dataset == $LARGEDATASET_4B ]
							then
								DATASET="_LARGEDATASET_4B"
								datasetpath=""
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/kron32_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_kron32.csv"
							else 
								DATASET=""
								datasetpath=""
								RESULTDIR_RESULT=""
							fi
							
							make generatesrcs XWARE=$XWARE SETUP=$SETUP ALGORITHM=$ALGORITHM DATASET=$DATASET NUMSUPERCPUTHREADS=$numsupercputhreads NUMCPUTHREADS=$numcputhreads NUMSUBCPUTHREADS=$numsubcputhreads NUMPARTITIONS=$numpartitions_pow LOCKE=$locke EVALUATION_TYPE=$evaluation_type EVALUATION_PARAM0=$evaluation_param0 												

							# ================================================ SW RUNS ================================================
							if [ $setup == $SW__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $SW__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $SW__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
							then
								make cleanall
								# make build_acts_nthreads
								make demo_acts_nthreads #> $RESULTDIR_RESULT
								# make demo_acts_nthreads_debug #> $RESULTDIR_RESULT
							elif [ $setup == $SW__GRAFBOOST_SETUP__PR_ALGORITHM ] || [ $setup == $SW__GRAFBOOST_SETUP__BFS_ALGORITHM ] || [ $setup == $SW__GRAFBOOST_SETUP__SSSP_ALGORITHM ]
							then
								make cleanall
								# make build_grafboost_nthreads
								make demo_grafboost_nthreads > $RESULTDIR_RESULT
							elif [ $setup == $SW__GUNROCK_SETUP__PR_ALGORITHM ] || [ $setup == $SW__GUNROCK_SETUP__BFS_ALGORITHM ] || [ $setup == $SW__GUNROCK_SETUP__SSSP_ALGORITHM ]
							then
								echo 'SW__GUNROCK_SETUP__PR_ALGORITHM called.'
								if [ $setup == $SW__GUNROCK_SETUP__PR_ALGORITHM ]
								then 
									/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market $datasetpath --normalized --compensate --undirected #> $RESULTDIR_RESULT
								elif [ $setup == $SW__GUNROCK_SETUP__BFS_ALGORITHM ]
								then
									/net/bigtemp/oj2zf/gunrock_wole/build/bin/bfs market $datasetpath --normalized --compensate --undirected #> $RESULTDIR_RESULT
								elif [ $setup == $SW__GUNROCK_SETUP__SSSP_ALGORITHM ]
								then
									/net/bigtemp/oj2zf/gunrock_wole/build/bin/sssp market $datasetpath --normalized --compensate --undirected #> $RESULTDIR_RESULT
								else
									echo "..."
								fi
							
							# ================================================ HW (FPGA) RUNS ================================================
							elif [ $setup == $HW__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $HW__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $HW__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
							then
								make cleanall
								if [ $CRABTREE == $ON ]
								then
									if [ $ACTS_2BY1BY1 == $ON ]
									then
										echo "crabtree.ACTS_2BY1BY1 setup specified."
										make host
										./host $BACKUPDIR_KERNELXCLBIN1 $BACKUPDIR_KERNELXCLBIN2 #> $RESULTDIR_RESULT
									elif [ $ACTS_1BY2BY1 == $ON ]
									then
										echo "crabtree.ACTS_1BY2BY1 setup specified."
										make host
										./host $BACKUPDIR_KERNELXCLBIN #> $RESULTDIR_RESULT
									else
										echo "not specified (7). specify ACTS_2BY1BY1 or ACTS_1BY2BY1"
									fi
								elif [ $AWS == $ON ]
								then
									echo "aws setup specified."
									# sudo su
									make host
									# source /opt/xilinx/xrt/setup.sh 
									# source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
									if [ $ACTS_2BY1BY1 == $ON ]
									then
										./host $BACKUPDIR_AWSKERNELXCLBIN1 $BACKUPDIR_AWSKERNELXCLBIN2
									elif [ $ACTS_1BY2BY1 == $ON ]
									then
										./host $BACKUPDIR_AWSKERNELXCLBIN
									else
										echo "not specified (7). specify ACTS_2BY1BY1 or ACTS_1BY2BY1"
									fi
								else
									echo "no setup specified. specify crabtree or aws"
								fi
								wait 
								if test -f "profile_summary.csv"; then
									echo "profile_summary.csv exist"
									# cp profile_summary.csv $RESULTDIR_PROFILESUMMARY
								fi	
								
							# ================================================ SIMULATIONS ================================================
							elif [ $setup == $SWEMU__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $SWEMU__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $SWEMU__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
							then
								if [ $ACTS_2BY1BY1 == $ON ]
								then
									echo "crabtree.ACTS_2BY1BY1 setup specified."
									
									make cleanall
									make host 
									make all_nk TARGET=sw_emu DEVICE=$DEVICEPATH 
									cp xclbin/topkernelproc.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN1
									cp xclbin/topkernelsync.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN2
									XCL_EMULATION_MODE=sw_emu ./host $BACKUPDIR_KERNELXCLBIN1 $BACKUPDIR_KERNELXCLBIN2
									
									# make cleanall
									# make host 
									# make all_sync TARGET=sw_emu DEVICE=$DEVICEPATH 
									# cp xclbin/topkernelproc.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN1
									# cp xclbin/topkernelsync.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN2
									# XCL_EMULATION_MODE=sw_emu ./host $BACKUPDIR_KERNELXCLBIN1 $BACKUPDIR_KERNELXCLBIN2
									
								elif [ $ACTS_1BY2BY1 == $ON ]
								then
									echo "crabtree.ACTS_1BY2BY1 setup specified."
									make cleanall
									make all TARGET=sw_emu DEVICE=$DEVICEPATH 
									cp xclbin/topkernel.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_AWSKERNELXCLBIN
									XCL_EMULATION_MODE=sw_emu ./host $BACKUPDIR_AWSKERNELXCLBIN
								else
									echo "not specified (7). specify ACTS_2BY1BY1 or ACTS_1BY2BY1"
								fi
							
							# ================================================ SYNTHESIS (CRABTREE) ================================================
							elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $CTHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $CTHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
							then
								if [ $ACTS_2BY1BY1 == $ON ]
								then
									# make cleanall
									# rm -rf xclbin
									# make all_proc DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} #> nohupsyn${ALGORITHMABBRV}${synfreq}MHz_proc.out 
									# if test -f "host"; then
										# cp xclbin/topkernelproc.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN1
										# echo "kernel.xclbin saved"
									# fi
									# echo "sleeping for 5 seconds before continuing ...."
									# sleep 5
									
									# make cleanall
									# rm -rf xclbin
									# make all_sync DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} > nohupsyn${ALGORITHMABBRV}${synfreq}MHz_sync.out 
									# if test -f "host"; then
										# cp xclbin/topkernelsync.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN2
										# echo "kernel.xclbin saved"
									# fi
									# echo "sleeping for 5 seconds before continuing ...."
									# sleep 5
									
									make cleanall
									rm -rf xclbin
									make all_procandsync DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} #> nohupsyn${ALGORITHMABBRV}${synfreq}MHz_proc.out 
									if test -f "host"; then
										cp xclbin/topkernelprocandsync.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN1
										echo "kernel.xclbin saved"
									fi
									echo "sleeping for 5 seconds before continuing ...."
									sleep 5
									
									# make cleanall
									# rm -rf xclbin
									# make all DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} #> nohupsyn${ALGORITHMABBRV}${synfreq}MHz_proc.out 
									# if test -f "host"; then
										# cp xclbin/topkernel.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										# echo "kernel.xclbin saved"
									# fi
									# echo "sleeping for 5 seconds before continuing ...."
									# sleep 5
									
								elif [ $ACTS_1BY2BY1 == $ON ]
								then
									# make cleanall
									# rm -rf xclbin
									# make all DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} > nohupsyn${ALGORITHMABBRV}${synfreq}MHz.out 
								
									# echo "sleeping for 5 seconds before continuing ...."
									# sleep 5
									
									# if test -f "host"; then
										# cp xclbin/topkernel.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										# echo "host, kernel.xo, kernel.xclbin, nohupsyn${ALGORITHMABBRV}${synfreq}MHz.out saved"
									# fi
									# echo "sleeping for 5 seconds before continuing ...."
									# sleep 5
									
									make cleanall
									rm -rf xclbin
									# make all_procandsync DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} #> nohupsyn${ALGORITHMABBRV}${synfreq}MHz_proc.out 
									if [ $numsubcputhreads == $NUMTHREADS_EQ12 ]  
									then
										make all_procandsync_${ACTS_CONFIG}_12and1 DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ10 ]
									then
										make all_procandsync_${ACTS_CONFIG}_10and1 DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ8 ]
									then
										make all_procandsync_${ACTS_CONFIG}_8and1 DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ6 ]
									then
										make all_procandsync_${ACTS_CONFIG}_6and1 DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ4 ]
									then
										make all_procandsync_${ACTS_CONFIG}_4and1 DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ2 ]
									then
										echo "------------------------------------------------ FOUND --------------------------------------------------"
										make all_procandsync_${ACTS_CONFIG}_2and1 DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ1 ]
									then
										make all_procandsync_${ACTS_CONFIG}_1and1 DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq}
									else 
										echo "NOT IMPLEMENTED (654)"
									fi
									if test -f "host"; then
										if [ $numsubcputhreads == $NUMTHREADS_EQ12 ]  
										then	
											cp xclbin/topkernelprocandsync_${ACTS_CONFIG}_12and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ10 ]
										then
											cp xclbin/topkernelprocandsync_${ACTS_CONFIG}_10and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ8 ]
										then
											cp xclbin/topkernelprocandsync_${ACTS_CONFIG}_8and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ6 ]
										then
											cp xclbin/topkernelprocandsync_${ACTS_CONFIG}_6and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ4 ]
										then
											cp xclbin/topkernelprocandsync_${ACTS_CONFIG}_4and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ2 ]
										then
											echo "------------------------------------------------ FOUND (2) --------------------------------------------------"
											cp xclbin/topkernelprocandsync_${ACTS_CONFIG}_2and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ1 ]
										then
											cp xclbin/topkernelprocandsync_${ACTS_CONFIG}_1and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										else 
											echo "NOT IMPLEMENTED (554)"
										fi
										# cp xclbin/topkernelprocandsync_1by2by1by2_4and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										# cp v++_topkernelprocandsync_1by2by1by2_4and1.hw.xilinx_u280_xdma_201920_3.log $BACKUPDIR_NOHUPOUT
										# topkernelprocandsync_1by2by1by2_4and1.hw.xilinx_u280_xdma_201920_3.xclbin
										echo "kernel.xclbin saved"
									fi
									echo "sleeping for 5 seconds before continuing ...."
									sleep 5
									
									
								else
									echo "not specified (7). specify ACTS_2BY1BY1 or ACTS_1BY2BY1"
								fi
							
							# ================================================ SYNTHESIS (AWS) ================================================
							elif [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__PR_ALGORITHM ] || [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__BFS_ALGORITHM ] || [ $setup == $AWSHWSYN__ACTGRAPH_SETUP__SSSP_ALGORITHM ]
							then
								source /opt/xilinx/xrt/setup.sh 
								source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
								
								make cleanall
								rm -rf xclbin
								
								if [ $ACTS_2BY1BY1 == $ON ]
								then
									# make all_nk DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} > nohupsyn${ALGORITHMABBRV}${synfreq}MHz.out 
									make all_proc DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} > nohupsyn${ALGORITHMABBRV}${synfreq}MHz_proc.out 
									# make all_sync DEVICE=$DEVICEPATH > nohupsyn_sync.out 
									cp -rf xclbin/topkernelproc.hw.${DSA_NAME}.xclbin kernel.xclbin
									# cp -rf xclbin/topkernelsync.hw.${DSA_NAME}.xclbin kernel.xclbin
								elif [ $ACTS_1BY2BY1 == $ON ]
								then
									make all DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} > nohupsyn${ALGORITHMABBRV}${synfreq}MHz.out
									cp -rf xclbin/topkernelproc.hw.${DSA_NAME}.xclbin kernel.xclbin
								else
									echo "not specified (7)."
								fi
									
								echo "sleeping for 5 seconds before continuing ...."
								sleep 5
								
								if test -f "host"; then
									if [ $ACTS_2BY1BY1 == $ON ]
									then
										cp kernel.xclbin $BACKUPDIR_KERNELXCLBIN1
										# cp kernel.xclbin $BACKUPDIR_KERNELXCLBIN2
									elif [ $ACTS_1BY2BY1 == $ON ]
									then
										make all DEVICE=$DEVICEPATH SYNFREQUENCY=${synfreq} > nohupsyn${ALGORITHMABBRV}${synfreq}MHz.out
										cp kernel.xclbin $BACKUPDIR_KERNELXCLBIN
									else
										echo "not specified (7)."
									fi
									echo "host, kernel.xo, kernel.xclbin, nohupsyn${ALGORITHMABBRV}${synfreq}MHz.out saved"
								fi
								echo "sleeping for 5 seconds before continuing ...."
								sleep 5
								
								source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
								source /opt/xilinx/xrt/setup.sh 
								cd /home/centos/src/project_data/aws-fpga
								./createawsxclbin.sh
								echo "sleeping for 60 minuites before continuing ...."
								sleep 3600
								
								if [ $ACTS_2BY1BY1 == $ON ]
								then
									cp -rf /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/kernel.awsxclbin $BACKUPDIR_AWSKERNELXCLBIN1
									# cp -rf /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/kernel.awsxclbin $BACKUPDIR_AWSKERNELXCLBIN2
								elif [ $ACTS_1BY2BY1 == $ON ]
								then
									cp -rf /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/kernel.awsxclbin $BACKUPDIR_AWSKERNELXCLBIN
								else
									echo "not specified (7)."
								fi
								
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
	done
done
echo 'finished: successfully finished all processing'





