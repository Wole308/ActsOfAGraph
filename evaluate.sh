#!/bin/bash
#!/bin/bash

# 1. Vitis profile summary 
# https://xilinx.github.io/Vitis-Tutorials/2020-1/docs/Pathway3/ProfileAndTraceReports.html

# 2. Select the Profile Summary report, a... (vitis profile summary has both Compute and Memory information to calculate Arithmetic Intensity)
# https://xilinx.github.io/Vitis-Tutorials/2020-1/docs/Pathway3/ProfileAndTraceReports.html 
# https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Profile-Summary-Report
# https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/profile-Options *

ROOTDIR="/home/oj2zf/Documents/acts-clusterscale"
OUTPUTDIRNAME="outputs"
DEVICEPATH=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm 

TESTKERNEL="RK"
# TESTKERNEL="TESTKERNEL"

# XWARE="SWEMU" 
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

# make generatesrcs XWARE=$XWARE EVALUATION_TYPE=$evaluation_type NUM_PEs=1 TESTKERNEL=$TESTKERNEL
make generatesrcs XWARE=$XWARE EVALUATION_TYPE=$evaluation_type NUM_PEs=12 TESTKERNEL=$TESTKERNEL
# make generatesrcs XWARE=$XWARE EVALUATION_TYPE=$evaluation_type NUM_PEs=24 TESTKERNEL=$TESTKERNEL										

echo 'finished: successfully finished all processing'





