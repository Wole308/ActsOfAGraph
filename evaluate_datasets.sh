#!/bin/bash
#!/bin/bash

# ssh oluwole308@cdsc0.cs.ucla.edu
# ssh oluwole308@10.10.128.45
# ssh -J oluwole308@cdsc0.cs.ucla.edu oluwole308@10.10.128.45
# ssh -J oluwole308@cdsc0.cs.ucla.edu oluwole308@10.1.128.45
# password: Elohim308* 
# oluwole308@ACTS:~$ ls -l /opt/tools/xilinx/
# oluwole308@ACTS:~$ xbutil examine (see FPGA installed devices)
# scp -J user@cdsc0.cs.ucla.edu PATH_LOCAL user@openneb-ip:PATH_VM
# scp -J oluwole308@cdsc0.cs.ucla.edu /home/oluwole308/Vitis_Accel_Examples-master.zip oluwole308@10.10.128.45:/home/oluwole308
# scp -J oj2zf@panther.cs.virginia.edu /home/oj2zf/Documents/Vitis_Accel_Examples-master/sys_opt/multiple_devices/build_dir.hw.xilinx_u280_xdma_201920_3/vector_addition.xclbin oluwole308@10.10.128.45:/home/oluwole308/Vitis_Accel_Examples-master/sys_opt/multiple_devices/build_dir.hw.xilinx_u280_xdma_201920_3/vector_addition.xclbin
# scp oj2zf@panther.cs.virginia.edu /home/oj2zf/Documents/Vitis_Accel_Examples-master/sys_opt/multiple_devices/build_dir.hw.xilinx_u280_xdma_201920_3/vector_addition.xclbin oluwole308@10.10.128.45:/home/oluwole308/Vitis_Accel_Examples-master/sys_opt/multiple_devices/

# scp -J oluwole308@cdsc0.cs.ucla.edu /home/oluwole308/multiple_devices oluwole308@10.10.128.45:/home/oluwole308/Vitis_Accel_Examples-master/sys_opt/multiple_devices/
# scp -J oluwole308@cdsc0.cs.ucla.edu /home/oluwole308/vector_addition.xclbin oluwole308@10.10.128.45:/home/oluwole308/Vitis_Accel_Examples-master/sys_opt/multiple_devices/
# ./multiple_devices vector_addition.xclbin

# source <Vitis_install_path>/Vitis/<Version>/settings64.sh
# source /opt/tools/xilinx/Vitis/2022.2/settings64.sh
# make build TARGET=hw PLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm 
# make build TARGET=hw PLATFORM=/opt/tools/xilinx/Vitis/2019.2/platforms/xilinx_u200_xdma_201830_2 (********** VAST *****)
# make build TARGET=hw PLATFORM=xilinx_u280_xdma_201920_3 (********** VAST *****)
# make host PLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# ./overlap build_dir.hw.xilinx_u280_xdma_201920_3/vector_addition.xclbin
# ./overlap vector_addition.xclbin

make cleanall
if [ $1 -eq 0 ]; then
	 echo -------------------------------------------------- EVALUATE_DATASET.SH: SOFTWARE ACTS APPLICATION RUNNING --------------------------------------------------
	 XWARE="SW"
	 RUNNING_SYNTHESIS=0
	 make generatesrcs XWARE=$XWARE RUNNING_SYNTHESIS=$RUNNING_SYNTHESIS NUM_PEs=12 NUM_FPGAs=$2		
	 make actsobj
elif [ $1 -eq 1 ]; then
	 echo -------------------------------------------------- EVALUATE_DATASET.SH: HARDWARE ACTS APPLICATION RUNNING --------------------------------------------------
	 XWARE="HW" 
	 RUNNING_SYNTHESIS=0
	 make generatesrcs XWARE=$XWARE RUNNING_SYNTHESIS=$RUNNING_SYNTHESIS NUM_PEs=12 NUM_FPGAs=$2		
	 make host
elif [ $1 -eq 2 ]; then
	 echo -------------------------------------------------- EVALUATE.SH: HARDWARE ACTS SYNTHESIS RUNNING --------------------------------------------------
	 XWARE="HW" 
	 RUNNING_SYNTHESIS=1
	 make generatesrcs XWARE=$XWARE RUNNING_SYNTHESIS=$RUNNING_SYNTHESIS NUM_PEs=12 NUM_FPGAs=$2	
else 
	 echo -------------------------------------------------- EVALUATE_DATASET.SH: CMD ARGS NOT DEFINED. EXITING.... --------------------------------------------------
	 quit	 
fi

# ./evaluate_datasets.sh 0 2
# ./evaluate_datasets.sh 0 4
# ./evaluate_datasets.sh 0 8
# ./evaluate_datasets.sh 2 2
		
# "USAGE: ./host [--algo] [--rootvid] [--direction] [--numiterations] [--XCLBIN] [--graph_path]"
# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/kron_g500-logn20.mtx
# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_256m.mtx
# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/twitter7/twitter7.mtx
# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_1024m/rmat_16m_1024m.mtx [FIXME]
# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_32m_2048m/rmat_32m_2048m.mtx
# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/sk-2005/sk-2005.mtx [FIXME]
# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/uk-2005/uk-2005.mtx
# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/com-Friendster/com-Friendster.mtx

# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/kron_g500-logn20.mtx
# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_256m.mtx
# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/twitter7/twitter7.mtx
# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_1024m/rmat_16m_1024m.mtx [FIXME]
# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_32m_2048m/rmat_32m_2048m.mtx
# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/sk-2005/sk-2005.mtx [FIXME]
# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/uk-2005/uk-2005.mtx
# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/com-Friendster/com-Friendster.mtx

# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/kron_g500-logn20.mtx
./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_256m.mtx
# ./host sssp 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_256m.mtx

# ./host

