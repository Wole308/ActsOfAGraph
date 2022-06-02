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
DATASET_DIR="/home/oj2zf/dataset"

./evaluate.sh

make cleanall
# make host
make actsobj

# for algo in $PR $CF $HITS $SPMV $BFS
for algo in $PR 
do
	####### test
	# ./host "nap" "pr" 1 0 "$DATASET_DIR/com-Orkut.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	# ./host "nap" "pr" 1 0 "$DATASET_DIR/rmat_2m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	# ./host "nap" "pr" 1 0 "$DATASET_DIR/rmat_8m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "pr" 1 0 "$DATASET_DIR/rmat_33m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	exit 0

	####### 4M
	./host "nap" "$algo" 1 0 "$DATASET_DIR/hollywood-2009.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/com-LiveJournal.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/cit-Patents.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/com-Orkut.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/roadNet-CA.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"

	####### 8M
	./host "nap" "$algo" 1 0 "$DATASET_DIR/indochina-2004.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/great-britain_osm.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/wb-edu.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/hugetrace-00010.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/road_central.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/italy_osm.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/asia_osm.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"

	####### 16M
	./host "nap" "$algo" 1 0 "$DATASET_DIR/uk-2002.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/hugetrace-00020.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/hugebubbles-00000.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/hugebubbles-00020.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/GAP-road.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/road_usa.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"

	####### 32M
	./host "nap" "$algo" 1 0 "$DATASET_DIR/europe_osm.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/kmer_V2a.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/kmer_U1a.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/mawi_201512020000.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"

	####### synthetic
	./host "nap" "$algo" 1 0 "$DATASET_DIR/rmat_1m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/rmat_2m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/rmat_4m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/rmat_8m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/rmat_16m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/rmat_33m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
	./host "nap" "$algo" 1 0 "$DATASET_DIR/rmat_67m_134m.mtx" "$XCLBIN_DIR/$XCLBIN_FILE"
done

