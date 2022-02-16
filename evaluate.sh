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

# ACTS_CONFIG="1by2by1by0"
# ACTS_CONFIG="1by2by1by1"
# ACTS_CONFIG="1by4by1by1"
# ACTS_CONFIG="1by2by1by4"
# ACTS_CONFIG="1by2by1by5"

# 1 PEs: 1by2by1by5_5and1 (NUMSUBCPUTHREADS=5, NUM_EDGE_BANKS=5)
# 2 PEs: 1by2by1by3_6and1 (NUMSUBCPUTHREADS=6, NUM_EDGE_BANKS=3)
# 3 PEs: 1by2by1by2_6and1 (NUMSUBCPUTHREADS=6, NUM_EDGE_BANKS=2)
# 4 PEs: 1by2by1by2_8and1 (NUMSUBCPUTHREADS=8, NUM_EDGE_BANKS=2)
# 5 PEs: 1by2by1by1_5and1 (NUMSUBCPUTHREADS=5, NUM_EDGE_BANKS=1)
# ACTS_CONFIG="1by2by1by5"
# ACTS_CONFIG="1by2by1by3"
# ACTS_CONFIG="1by2by1by2"
# ACTS_CONFIG="1by2by1by2"
# ACTS_CONFIG="1by2by1by1"

# NUM_PEs="1"
# NUM_PEs="1"
# NUM_PEs="3"
# NUM_PEs="4"
# NUM_PEs="5"

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

SW__ACTGRAPH_SETUP=1
HW__ACTGRAPH_SETUP=2
SWEMU__ACTGRAPH_SETUP=3
SW__GRAFBOOST_SETUP=4
SW__GUNROCK_SETUP=5

CTHWSYN__ACTGRAPH_SETUP=26

AWSHWSYN__ACTGRAPH_SETUP=30

VHLSVHLSHW__ACTGRAPH_SETUP=34

NODATASET=""
ORKUT_3M_212M="ORKUT_3M_212M"
HOLLYWOOD_1M_114M="HOLLYWOOD_1M_114M"
KRON20_1M_45M="KRON20_1M_45M"
KRON21_2M_91M="KRON21_2M_91M"
KRON22_4M_200M="KRON22_4M_200M"
RGG_N_2_24_S0_16M_265M="RGG_N_2_24_S0_16M_265M"
EUROPE_OSM_50M_108M="EUROPE_OSM_50M_108M"
KMER_V2A_55M_117M="KMER_V2A_55M_117M"
HUGEBUBBLES_00020_21M_63M="HUGEBUBBLES_00020_21M_63M"
KMER_U1A_67M_138M="KMER_U1A_67M_138M"

RMAT22_SPARSE0="RMAT22_SPARSE0"
RMAT22_SPARSE1="RMAT22_SPARSE1"
RMAT22_SPARSE2="RMAT22_SPARSE2"
RMAT22_SPARSE3="RMAT22_SPARSE3"
RMAT22_SPARSE4="RMAT22_SPARSE4"
RMAT22_SPARSE5="RMAT22_SPARSE5"
RMAT22_SPARSE6="RMAT22_SPARSE6"
RMAT22_SPARSE7="RMAT22_SPARSE7"

RMAT_RANGE0="RMAT_RANGE0"
RMAT_RANGE1="RMAT_RANGE1"
RMAT_RANGE2="RMAT_RANGE2"
RMAT_RANGE3="RMAT_RANGE3"
RMAT_RANGE4="RMAT_RANGE4"
RMAT_RANGE5="RMAT_RANGE5"
RMAT_RANGE6="RMAT_RANGE6"
RMAT_RANGE7="RMAT_RANGE7"

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
NUMTHREADS_EQ22=22
NUMTHREADS_EQ24=24
NUMTHREADS_EQ25=25
NUMTHREADS_EQ26=26
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

NUM_PEs_EQ0=0
NUM_PEs_EQ1=1
NUM_PEs_EQ2=2
NUM_PEs_EQ3=3
NUM_PEs_EQ4=4
NUM_PEs_EQ5=5
NUM_PEs_EQ6=6
NUM_PEs_EQ7=7
NUM_PEs_EQ8=8
NUM_PEs_EQ9=9
NUM_PEs_EQ10=10
NUM_PEs_EQ12=12
NUM_PEs_EQ14=14
NUM_PEs_EQ15=15
NUM_PEs_EQ16=16
NUM_PEs_EQ20=20
NUM_PEs_EQ22=22
NUM_PEs_EQ24=24
NUM_PEs_EQ25=25
NUM_PEs_EQ26=26
NUM_PEs_EQ32=32
NUM_PEs_EQX=64

_LOCKE="LOCKE"
_NOLOCKE="NOLOCKE"

ALGORITHMABBRV="PR"
# ALGORITHMABBRV="BFS"
# ALGORITHMABBRV="SSSP"
# ALGORITHMABBRV="CC"
# ALGORITHMABBRV="CF"

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
	# for setup in $SW__ACTGRAPH_SETUP
	# for setup in $HW__ACTGRAPH_SETUP
	# for setup in $SWEMU__ACTGRAPH_SETUP
	# for setup in $SW__GRAFBOOST_SETUP
	# for setup in $SW__GUNROCK_SETUP
	# for setup in $VHLSVHLSHW__ACTGRAPH_SETUP
	for setup in $CTHWSYN__ACTGRAPH_SETUP
	# for setup in $AWSHWSYN__ACTGRAPH_SETUP
	
	do 
		if [ $setup == $SW__ACTGRAPH_SETUP ] # pr 
		then 
			XWARE="SW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="${ALGORITHMABBRV}_ALGORITHM" 
			SETUP_NAME="actgraph_${ALGORITHMABBRV}_sw"
		elif [ $setup == $HW__ACTGRAPH_SETUP ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="${ALGORITHMABBRV}_ALGORITHM" 
			SETUP_NAME="actgraph_${ALGORITHMABBRV}_hw"
		elif [ $setup == $SWEMU__ACTGRAPH_SETUP ]
		then
			XWARE="SWEMU" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="${ALGORITHMABBRV}_ALGORITHM" 
			SETUP_NAME="actgraph_${ALGORITHMABBRV}_swemu"
		elif [ $setup == $SW__GRAFBOOST_SETUP ]
		then
			XWARE="SW" 
			SETUP="GRAFBOOST_SETUP" 
			ALGORITHM="${ALGORITHMABBRV}_ALGORITHM" 
			SETUP_NAME="grafboost_${ALGORITHMABBRV}_sw"
		elif [ $setup == $SW__GUNROCK_SETUP ]
		then
			XWARE="SW" 
			SETUP="GUNROCK_SETUP" 
			ALGORITHM="${ALGORITHMABBRV}_ALGORITHM" 
			SETUP_NAME="gunrock_${ALGORITHMABBRV}_sw"

		elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP ] # syn
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="${ALGORITHMABBRV}_ALGORITHM" 
			SETUP_NAME="actgraph_${ALGORITHMABBRV}_hw"
			
		elif [ $setup == $AWSHWSYN__ACTGRAPH_SETUP ]
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="${ALGORITHMABBRV}_ALGORITHM" 
			SETUP_NAME="actgraph_${ALGORITHMABBRV}_hw"
			
		elif [ $setup == $VHLSVHLSHW__ACTGRAPH_SETUP ] #nothing
		then
			XWARE="HW" 
			SETUP="ACTGRAPH_SETUP" 
			ALGORITHM="${ALGORITHMABBRV}_ALGORITHM" 
			SETUP_NAME="actgraph_${ALGORITHMABBRV}_hw"
			
		else 
			XWARE="" 
			SETUP=""
		fi

		for numsupercputhreads in $THREADCOUNT_EQ1
		do
		for numcputhreads in $THREADCOUNT_EQ1
		do
		# for numsubcputhreads in $NUMTHREADS_EQ0
		for numsubcputhreads in $NUMTHREADS_EQ1
		# for numsubcputhreads in $NUMTHREADS_EQ2
		# for numsubcputhreads in $NUMTHREADS_EQ3
		# for numsubcputhreads in $NUMTHREADS_EQ4 #
		# for numsubcputhreads in $NUMTHREADS_EQ5 #
		# for numsubcputhreads in $NUMTHREADS_EQ6 
		# for numsubcputhreads in $NUMTHREADS_EQ7
		# for numsubcputhreads in $NUMTHREADS_EQ8 #
		# for numsubcputhreads in $NUMTHREADS_EQ9
		# for numsubcputhreads in $NUMTHREADS_EQ10 
		# for numsubcputhreads in $NUMTHREADS_EQ11
		# for numsubcputhreads in $NUMTHREADS_EQ12 #
		# for numsubcputhreads in $NUMTHREADS_EQ13
		# for numsubcputhreads in $NUMTHREADS_EQ14
		# for numsubcputhreads in $NUMTHREADS_EQ15
		# for numsubcputhreads in $NUMTHREADS_EQ16 #
		# for numsubcputhreads in $NUMTHREADS_EQ18
		# for numsubcputhreads in $NUMTHREADS_EQ20
		# for numsubcputhreads in $NUMTHREADS_EQ24 #
		# for numsubcputhreads in $NUMTHREADS_EQ28
		# for numsubcputhreads in $NUMTHREADS_EQ32 #
		# for numsubcputhreads in $NUMTHREADS_EQ4 $NUMTHREADS_EQ8 $NUMTHREADS_EQ12 $NUMTHREADS_EQ16 $NUMTHREADS_EQ24
		# for numsubcputhreads in $NUMTHREADS_EQ0 $NUMTHREADS_EQ1 $NUMTHREADS_EQ2 $NUMTHREADS_EQ4 $NUMTHREADS_EQ8 $NUMTHREADS_EQ12 $NUMTHREADS_EQ16
		do
		
		for numpartitions_pow in $NUMPARTITIONSPOW_EQ4
		do
			for locke in $_LOCKE
			do
				# for dataset in $NODATASET
				for dataset in $ORKUT_3M_212M
				# for dataset in $HOLLYWOOD_1M_114M
				# for dataset in $KRON20_1M_45M
				# for dataset in $KRON21_2M_91M
				# for dataset in $KRON22_4M_200M
				# for dataset in $RGG_N_2_24_S0_16M_265M
				# for dataset in $EUROPE_OSM_50M_108M
				# for dataset in $KMER_V2A_55M_117M
				# for dataset in $HUGEBUBBLES_00020_21M_63M
				# for dataset in $KMER_U1A_67M_138M
				
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
				
				# for dataset in $ORKUT_3M_212M $HOLLYWOOD_1M_114M $KRON20_1M_45M $KRON21_2M_91M $RGG_N_2_24_S0_16M_265M $EUROPE_OSM_50M_108M $KMER_V2A_55M_117M $HUGEBUBBLES_00020_21M_63M $KMER_U1A_67M_138M	
				# for dataset in $HOLLYWOOD_1M_114M $KRON20_1M_45M $KRON21_2M_91M	
				# for dataset in $EUROPE_OSM_50M_108M $KMER_V2A_55M_117M $HUGEBUBBLES_00020_21M_63M
				# for dataset in $RMAT22_SPARSE0 $RMAT22_SPARSE2 $RMAT22_SPARSE4	
				do
					# for evaluation_param0 in 0 4
					for evaluation_param0 in 0
					do
						for num_pes in $NUM_PEs_EQ3
						# for num_pes in $NUM_PEs_EQ16 $NUM_PEs_EQ22 $NUM_PEs_EQ24
						do
							ACTS_CONFIG="1by1by1by0"
							numsubcputhreads=$num_pes

							for synfreq in $SYNFREQUENCY_EQ300
							# for synfreq in $SYNFREQUENCY_EQ60 $SYNFREQUENCY_EQ120 $SYNFREQUENCY_EQ180 $SYNFREQUENCY_EQ240 $SYNFREQUENCY_EQ300
							do
								BACKUPDIR_KERNELXCLBIN="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernel${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.xclbin"
								BACKUPDIR_KERNELXCLBIN1="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelproc${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.xclbin"
								BACKUPDIR_KERNELXCLBIN2="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelsync${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.xclbin"
								
								BACKUPDIR_AWSKERNELXCLBIN="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernel${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.awsxclbin"
								BACKUPDIR_AWSKERNELXCLBIN1="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelproc${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.awsxclbin"
								BACKUPDIR_AWSKERNELXCLBIN2="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelsync${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.awsxclbin"
								
								BACKUPDIR_NOHUPOUT="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernel${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.out"
								BACKUPDIR_NOHUPOUT1="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelproc${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.out"
								BACKUPDIR_NOHUPOUT2="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelsync${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.out"
								
								RESULTSBACKUP_DIR="${ROOTDIR}/outputs/${RESULTSPATHNAME}"
								RESULT_NAME="${ALGORITHMABBRV}${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz"
								PROFILESUMMARY_NAME="profile_summary_${ALGORITHMABBRV}_${num_pes}PEs${numsubcputhreads}Ws_${evaluation_type}_evp${evaluation_param0}"
								
								SYNLOGDIR_KERNELXCLBIN="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernel${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.log"
								SYNLOGDIR_KERNELXCLBIN1="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelproc${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.log"
								SYNLOGDIR_KERNELXCLBIN2="${ROOTDIR}/outputs/${SYNKERNELSPATHNAME}/goldenkernelsync${num_pes}PEs${numsubcputhreads}Ws${XWARE}${synfreq}MHz.log"
								
								DATASET="DATASET_${dataset}"
								datasetpath="/net/bigtemp/oj2zf/gunrock_wole/dataset/large/${dataset}/${dataset}.mtx"
								RESULTDIR_RESULT="${RESULTSBACKUP_DIR}/${dataset}_${RESULT_NAME}.out"
								RESULTDIR_PROFILESUMMARY="${RESULTSBACKUP_DIR}/${PROFILESUMMARY_NAME}_${dataset}.csv"
								
								make generatesrcs XWARE=$XWARE SETUP=$SETUP ALGORITHM=$ALGORITHM DATASET=$DATASET NUMSUPERCPUTHREADS=$numsupercputhreads NUMCPUTHREADS=$numcputhreads NUMSUBCPUTHREADS=$numsubcputhreads NUMPARTITIONS=$numpartitions_pow LOCKE=$locke EVALUATION_TYPE=$evaluation_type EVALUATION_PARAM0=$evaluation_param0 NUM_PEs=$num_pes												
								
								# ================================================ SW RUNS ================================================
								if [ $setup == $SW__ACTGRAPH_SETUP ]
								then
									make cleanall
									# make build_acts_nthreads
									make demo_acts_nthreads #> $RESULTDIR_RESULT
									# make demo_acts_nthreads_debug #> $RESULTDIR_RESULT
								elif [ $setup == $SW__GRAFBOOST_SETUP ]
								then
									make cleanall
									# make build_grafboost_nthreads
									make demo_grafboost_nthreads > $RESULTDIR_RESULT
								elif [ $setup == $SW__GUNROCK_SETUP ]
								then
									echo 'SW__GUNROCK_SETUP called.'
									if [ $setup == $SW__GUNROCK_SETUP ]
									then 
										/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market $datasetpath --normalized --compensate --undirected #> $RESULTDIR_RESULT
									fi
								
								# ================================================ HW (FPGA) RUNS ================================================
								elif [ $setup == $HW__ACTGRAPH_SETUP ]
								then
									make cleanall
									if [ $CRABTREE == $ON ]
									then
										echo "crabtree.ACTS_1BY2BY1 setup specified.---"
										make host
										./host $BACKUPDIR_KERNELXCLBIN #> $RESULTDIR_RESULT
									elif [ $AWS == $ON ]
									then
										echo "aws setup specified."
										# sudo su
										make host
										# source /opt/xilinx/xrt/setup.sh 
										# source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
										./host $BACKUPDIR_AWSKERNELXCLBIN
									else
										echo "no setup specified. specify crabtree or aws"
									fi
									wait 
									if test -f "profile_summary.csv"; then
										echo "profile_summary.csv exist"
										cp profile_summary.csv $RESULTDIR_PROFILESUMMARY
									fi	
									
								# ================================================ SIMULATIONS ================================================
								elif [ $setup == $SWEMU__ACTGRAPH_SETUP ]
								then
									echo "crabtree.ACTS_1BY1BY1 setup specified. ----------------------------------- "
									make cleanall
									# make host 
									if [ $numsubcputhreads == $NUMTHREADS_EQ25 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_25and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ24 ]  
									then
										echo 'awwal.sw_emu.----------------------------------- 24and1'
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_24and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ22 ]  
									then
										echo 'awwal.sw_emu.----------------------------------- 22and1'
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_22and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ20 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_20and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ16 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_16and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ14 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_14and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ12 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_12and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ10 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_10and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ9 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_9and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ8 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_8and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ7 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_7and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ6 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_6and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ5 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_5and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ4 ]
									then
										echo 'awwal.sw_emu'
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_4and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ3 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_3and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ2 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_2and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ1 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_1and1 TARGET=sw_emu PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									else 
										echo "NOT IMPLEMENTED (654)"
									fi
									if test -f "host"; then
										if [ $numsubcputhreads == $NUMTHREADS_EQ25 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_25and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ24 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_24and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ22 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_22and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ20 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_20and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ16 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_16and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ14 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_14and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ12 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_12and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ10 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_10and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ9 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_9and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ8 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_8and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ7 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_7and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ6 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_6and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ5 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_5and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ4 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_4and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ3 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_3and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ2 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_2and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ1 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_1and1.sw_emu.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										else 
											echo "NOT IMPLEMENTED (554)"
										fi
										echo "kernel.xclbin saved"
									fi
									XCL_EMULATION_MODE=sw_emu ./host $BACKUPDIR_KERNELXCLBIN
								
								# ================================================ SYNTHESIS (CRABTREE) ================================================
								elif [ $setup == $CTHWSYN__ACTGRAPH_SETUP ]
								then
									make cleanall
									rm -rf xclbin
									if [ $numsubcputhreads == $NUMTHREADS_EQ25 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_25and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ24 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_24and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ22 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_22and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ20 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_20and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ16 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_16and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ14 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_14and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ12 ]  
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_12and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ10 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_10and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ9 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_9and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ8 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_8and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ7 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_7and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ6 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_6and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ5 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_5and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ4 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_4and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ3 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_3and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ2 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_2and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									elif [ $numsubcputhreads == $NUMTHREADS_EQ1 ]
									then
										faketime -f "-1y" make all_procandsync_${ACTS_CONFIG}_1and1 PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq}
									else 
										echo "NOT IMPLEMENTED (654)"
									fi
									if test -f "host"; then
										if [ $numsubcputhreads == $NUMTHREADS_EQ25 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_25and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_25and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ24 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_24and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_24and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ22 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_22and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_22and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ20 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_20and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_20and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ16 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_16and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_16and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ14 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_14and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_14and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ12 ]  
										then	
											cp xclbin/topkernel_${ACTS_CONFIG}_12and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_12and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ10 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_10and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_10and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ9 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_9and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_9and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ8 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_8and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_8and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ7 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_7and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_7and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ6 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_6and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_6and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ5 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_5and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_5and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ4 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_4and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_4and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ3 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_3and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_3and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ2 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_2and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_2and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										elif [ $numsubcputhreads == $NUMTHREADS_EQ1 ]
										then
											cp xclbin/topkernel_${ACTS_CONFIG}_1and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
											cp v++_topkernel_${ACTS_CONFIG}_1and1.hw.xilinx_u280_xdma_201920_3.log $SYNLOGDIR_KERNELXCLBIN
										else 
											echo "NOT IMPLEMENTED (554)"
										fi
										# cp xclbin/topkernel_1by2by1by2_4and1.hw.${DSA_NAME}.xclbin $BACKUPDIR_KERNELXCLBIN
										# cp v++_topkernel_1by2by1by2_4and1.hw.xilinx_u280_xdma_201920_3.log $BACKUPDIR_NOHUPOUT
										# topkernel_1by2by1by2_4and1.hw.xilinx_u280_xdma_201920_3.xclbin
										echo "kernel.xclbin saved"
									fi
									echo "sleeping for 5 seconds before continuing ...."
									sleep 5
								
								# ================================================ SYNTHESIS (AWS) ================================================
								elif [ $setup == $AWSHWSYN__ACTGRAPH_SETUP ]
								then
									source /opt/xilinx/xrt/setup.sh 
									source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
									
									make cleanall
									rm -rf xclbin
									
									make all PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq} > nohupsyn${ALGORITHMABBRV}${synfreq}MHz.out
									cp -rf xclbin/topkernelproc.hw.${DSA_NAME}.xclbin kernel.xclbin
										
									echo "sleeping for 5 seconds before continuing ...."
									sleep 5
									
									if test -f "host"; then
										make all PLATFORM=$DEVICEPATH SYNFREQUENCY=${synfreq} > nohupsyn${ALGORITHMABBRV}${synfreq}MHz.out
										cp kernel.xclbin $BACKUPDIR_KERNELXCLBIN
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
									
									cp -rf /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/kernel.awsxclbin $BACKUPDIR_AWSKERNELXCLBIN
									
								elif [ $setup == $VHLSVHLSHW__ACTGRAPH_SETUP ]
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
done
echo 'finished: successfully finished all processing'





