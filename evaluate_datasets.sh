#!/bin/bash
#!/bin/bash

PR="pr"
CF="cf"
HITS="hits"
SPMV="spmv"
BFS="bfs"

XCLBIN_DIR="/home/oj2zf/Documents/actsofagraph/outputs/xclbins"
# XCLBIN_FILE="goldenTEST3_170mhz.xclbin"
# XCLBIN_FILE="goldenTEST3_250mhz.xclbin"
# XCLBIN_FILE="goldenRK16.xclbin"
XCLBIN_FILE="golden_x3.xclbin"
# XCLBIN_FILE="golden_x24.xclbin"
# XCLBIN_FILE="golden_realreduce.xclbin"
# XCLBIN_FILE="golden_dummyreduce.xclbin"
DATASET_DIR=/home/oj2zf/dataset
RESULT_PATH=/home/oj2zf/Documents/analysis_results/acts3/

./evaluate.sh

make cleanall
# make host
make actsobj

DATSETS=(
		# com-Orkut.mtx
		# soc-LiveJournal1.mtx
		# sx-stackoverflow.mtx
		# ljournal-2008.mtx
		# soc-Pokec.mtx
		# kron_g500-logn20.mtx
		# kron_g500-logn21.mtx
		# rmat_2m_128m.mtx
		# rmat_4m_128m.mtx 
		
		# uk-2002.mtx
		# delicious.mtx
		# rmat_16m_256m.mtx
		# rmat_32m_512m.mtx
		rmat_64m_512m.mtx
		
		# rmat_12m_128m.mtx 
		# rmat_16m_128m.mtx 
		# rmat_20m_128m.mtx 
		# rmat_24m_128m.mtx 
		# rmat_28m_128m.mtx 
		)
		
# DIRECTIONS=(
		# 1
		# 1
		# 1
		# 1
		# 1
		# 1
		# 1
		# 1
		# 1
		
		# 1
		# 0
		# 0
		# 0
		# 0
		
		# 0
		# 0
		# 0
		# 0
		# 0
		# )		
		
DIRECTIONS=(
		0
		)
	
# for algo in $PR $CF $HITS $SPMV $BFS	
# for algo in $PR $CF $HITS $BFS 
for algo in $PR 
# for algo in $CF
# for algo in $HITS 
# for algo in $BFS 
# for algo in $SPMV 
# for algo in $PR $BFS
do
	for ((i = 0; i < ${#DATSETS[@]}; i++)) do
		echo ${BUILD_DIR}/${algo} ${DATSETS[i]}
		./host "nap" "${algo}" 16 4 "$DATASET_DIR/${DATSETS[i]}" "${DIRECTIONS[i]}" "$XCLBIN_DIR/$XCLBIN_FILE" #> ${RESULT_PATH}/${algo}/${DATSETS[i]}.out
		# gdb ./host
		# exit 0
	done
done




