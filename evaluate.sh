#!/bin/bash
#!/bin/bash

ROOTDIR="/home/oj2zf/Documents/ActsOfAGraph"
OUTPUTDIRNAME="outputs"
DEVICEPATH=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm

# TESTKERNEL="RK"
TESTKERNEL="TESTKERNEL"

# XWARE="HW"
XWARE="SW" 

#evaluation_type=EV_CREATENDGRAPH
evaluation_type=EV_PERFORMANCEOFALGORITHM
#evaluation_type=EV_IMPACTOF__ACTS_VERYBARE
#evaluation_type=EV_IMPACTOF__ACTS_BARE
#evaluation_type=EV_IMPACTOF__ACTS_PLUS_MEMLAYOUT
#evaluation_type=EV_IMPACTOF__ACTS_PLUS_MEMLAYOUT_PLUS_PARTITIONFUNC
#evaluation_type=EV_IMPACTOF__ACTS_PLUS_MEMLAYOUT_PLUS_PARTITIONFUNC_PLUS_REPARTITIONFUNC
#evaluation_type=EV_IMPACTOF__ACTS_PLUS_MEMLAYOUT_PLUS_PARTITIONFUNC_PLUS_REPARTITIONFUNC_PLUS_HYBRIDFUNC

# make generatesrcs XWARE=$XWARE EVALUATION_TYPE=$evaluation_type NUM_PEs=3 TESTKERNEL=$TESTKERNEL	
make generatesrcs XWARE=$XWARE EVALUATION_TYPE=$evaluation_type NUM_PEs=16 TESTKERNEL=$TESTKERNEL	
# make generatesrcs XWARE=$XWARE EVALUATION_TYPE=$evaluation_type NUM_PEs=20 TESTKERNEL=$TESTKERNEL	
# make generatesrcs XWARE=$XWARE EVALUATION_TYPE=$evaluation_type NUM_PEs=24 TESTKERNEL=$TESTKERNEL										

# faketime -f "-1y" make hw PLATFORM=$DEVICEPATH	

echo 'finished: successfully finished all processing'





