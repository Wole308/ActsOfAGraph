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
XCLBIN_FILE="golden.xclbin"
DATASET_DIR=/home/oj2zf/dataset
RESULT_PATH=/home/oj2zf/Documents/analysis_results/acts/

./evaluate.sh

make cleanall
# make host
make actsobj

DATSETS=(
		com-Orkut.mtx
		# soc-LiveJournal1.mtx
		# sx-stackoverflow.mtx
		# ljournal-2008.mtx
		# soc-Pokec.mtx
		
		# rmat_12m_128m.mtx 
		# rmat_16m_128m.mtx 
		# rmat_20m_128m.mtx 
		# rmat_24m_128m.mtx 
		# rmat_28m_128m.mtx 
		# uk-2002.mtx
		# dbpedia-link.mtx
		# rgg_n_2_24_s0.mtx
		
		# delicious.mtx
		# rmat_16m_256m.mtx
		# rmat_33m_512m.mtx
		)
		
DIRECTIONS=(
		1
		1
		1
		1
		1
		
		0
		0
		0
		0
		0
		0
		0
		0
		
		0
		0
		0
		)		
	
# for algo in $PR $CF $HITS $SPMV $BFS	
# for algo in $PR $CF $HITS $BFS 
for algo in $PR 
# for algo in $CF
# for algo in $HITS 
# for algo in $BFS 
# for algo in $SPMV 
do
	for ((i = 0; i < ${#DATSETS[@]}; i++)) do
		echo ${BUILD_DIR}/${algo} ${DATSETS[i]}
		./host "nap" "${algo}" 16 4 "$DATASET_DIR/${DATSETS[i]}" "${DIRECTIONS[i]}" "$XCLBIN_DIR/$XCLBIN_FILE" #> ${RESULT_PATH}/${algo}/${DATSETS[i]}.out
		# exit 0
	done
done




