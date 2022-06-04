#!/bin/bash
#!/bin/bash

PR="pr"
CF="cf"
HITS="hits"
SPMV="spmv"
BFS="bfs"

XCLBIN_DIR="/home/oj2zf/Documents/ActsOfAGraph/outputs/xclbins"
XCLBIN_FILE="goldenTEST3.xclbin"
# XCLBIN_FILE="goldenRK16.xclbin"
DATASET_DIR=/home/oj2zf/dataset
RESULT_PATH=/home/oj2zf/Documents/analysis_results/acts/

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
		
		# delicious.mtx
		# rmat_16m_134m.mtx
		# rmat_33m_134m.mtx
		# rmat_67m_134m.mtx
		
		# rmat_1m_134m.mtx 
		# rmat_2m_134m.mtx 
		# rmat_4m_134m.mtx 
		# rmat_8m_134m.mtx 
		# rmat_16m_134m.mtx 
		rmat_33m_134m.mtx 
		)
	
# for algo in $PR $CF $HITS $SPMV $BFS	
for algo in $PR 
# for algo in $CF 
# for algo in $HITS 
# for algo in $SPMV 
# for algo in $BFS 
do
	for ((i = 0; i < ${#DATSETS[@]}; i++)) do
		echo ${BUILD_DIR}/bench_pagerank ${DATSETS[i]}
		./host "nap" "${algo}" 1 0 "$DATASET_DIR/${DATSETS[i]}" "$XCLBIN_DIR/$XCLBIN_FILE" #> ${RESULT_PATH}/${algo}/${DATSETS[i]}.out
		exit 0
	done
done




