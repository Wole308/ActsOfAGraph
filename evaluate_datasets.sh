#!/bin/bash
#!/bin/bash

# make run TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# make build TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm

# $PR 
# $SSSP 
# $BFS 
# $PR 
# $CF
# $HITS 
# $SPMV 
# $SSSP 
# $BFS

# com-Orkut.mtx (undirected)
# soc-LiveJournal1.mtx (directed)
# sx-stackoverflow.mtx (directed)
# ljournal-2008.mtx (directed)
# soc-Pokec.mtx (directed)
# kron_g500-logn20.mtx (undirected)
# kron_g500-logn21.mtx (undirected)
# uk-2002.mtx (directed)
# delicious.mtx (directed)
# rmat_16m_256m.mtx (directed)
# rmat_32m_256m.mtx (directed)

PR="pr"
CF="cf"
HITS="hits"
SPMV="spmv"
BFS="bfs"
SSSP="sssp"

# XCLBIN_DIR="/home/oj2zf/Documents/acts-clusterscale/ytest_portal/xclbin"
# XCLBIN_FILE="acts.hw.xilinx_u280_xdma_201920_3.xclbin"

# XCLBIN_DIR="/home/oj2zf/Documents/acts-clusterscale/outputs"
# XCLBIN_FILE="golden_2.xclbin"

# XCLBIN_DIR="/home/oj2zf/Documents/acts-clusterscale"
# XCLBIN_FILE="vector_addition.xclbin"

XCLBIN_DIR="/home/oj2zf/Documents/acts-clusterscale/outputs"
XCLBIN_FILE="vector_addition.xclbin"

DATASET_DIR=/home/oj2zf/Documents/dataset
RESULT_PATH=/home/oj2zf/Documents/analysis_results/acts

make cleanall

if [ $2 -eq 0 ]; then
     echo -------------------------------------------------- EVALUATE_DATASET.SH: SOFTWARE ACTS APPLICATION RUNNING --------------------------------------------------
	 ./evaluate.sh 0
	 make actsobj
elif [ $2 -eq 1 ]; then
     echo -------------------------------------------------- EVALUATE_DATASET.SH: HARDWARE ACTS APPLICATION RUNNING --------------------------------------------------
	 ./evaluate.sh 1
	 make host
else
     echo -------------------------------------------------- EVALUATE_DATASET.SH: CMD ARGS NOT DEFINED. EXITING.... --------------------------------------------------
	 quit
fi

# rm -rf /home/oj2zf/Documents/acts-clusterscale/outputs/golden_2.xclbin
# make host
# make build TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# cp -rf build_dir.sw_emu.xilinx_u280_xdma_201920_3/vmult_vadd.link.xclbin /home/oj2zf/Documents/acts-clusterscale/outputs/golden_2.xclbin
# sleep 3

# DIRECTIONS=(1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0)
DIRECTIONS=(0)

DATSETS=(
		com-Orkut.mtx #(undirected)(3M,234M)
		# soc-LiveJournal1.mtx #(directed)(5M,70M)
		# sx-stackoverflow.mtx #(directed)(2.6M,36M)
		# ljournal-2008.mtx #(directed)(5M,79M)
		# soc-Pokec.mtx #(directed)(1.6M,30M)
		# kron_g500-logn20.mtx #(undirected)(1M,89M)
		# kron_g500-logn21.mtx #(undirected)(2M,182M/)
		
		# uk-2002.mtx #(directed)(18.5M,298M)
		####################### delicious.mtx #(directed)(M,M)
		# rmat_16m_256m.mtx #(directed)(16M,256M)
		# rmat_32m_256m.mtx #(directed)(32M,256M)
		# rmat_64m_256m.mtx #(directed)(64M,256M)
		)

for algo in $1
do
	for ((i = 0; i < ${#DATSETS[@]}; i++)) do
		echo ${BUILD_DIR}/${algo} ${DATSETS[i]}	
		./host "nap" "${algo}" 1 "$DATASET_DIR/${DATSETS[i]}" "${DIRECTIONS[i]}" 12 "$XCLBIN_DIR/$XCLBIN_FILE" #> ${RESULT_PATH}/${algo}/${DATSETS[i]}.out
		# XCL_EMULATION_MODE=sw_emu ./host "nap" "${algo}" 1 "$DATASET_DIR/${DATSETS[i]}" "${DIRECTIONS[i]}" 12 "$XCLBIN_DIR/$XCLBIN_FILE"
		# gdb ./host
		# exit 0
	done
done

# parameter set giving correct results @ {  NUM_PEs=24, unsigned int threshold___activefrontiers = 128,  typedef HBM_axichannel_t HBM_channelX_t;, ___FORCE_SUCCESS___(disabled) ___ENABLE___COLLECTACTIVEDSTVIDS___(disabled)   }
# ./host "nap" "${algo}" 1 "$DATASET_DIR/${DATSETS[i]}" "${DIRECTIONS[i]}" 12 "$XCLBIN_DIR/$XCLBIN_FILE" #> ${RESULT_PATH}/${algo}/${DATSETS[i]}.out



