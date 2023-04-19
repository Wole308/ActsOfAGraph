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
if [ $2 -eq 0 ]; then
	 echo -------------------------------------------------- EVALUATE_DATASET.SH: SOFTWARE ACTS APPLICATION RUNNING --------------------------------------------------
	 ./evaluate.sh 0 $3
	 make actsobj
elif [ $2 -eq 1 ]; then
	 echo -------------------------------------------------- EVALUATE_DATASET.SH: HARDWARE ACTS APPLICATION RUNNING --------------------------------------------------
	 ./evaluate.sh 1 $3
	 make host
else 
	 echo -------------------------------------------------- EVALUATE_DATASET.SH: CMD ARGS NOT DEFINED. EXITING.... --------------------------------------------------
	 quit
fi

# com-Orkut.mtx #(undirected)(3M,234M)
# soc-LiveJournal1.mtx #(directed)(5M,70M)
# sx-stackoverflow.mtx #(directed)(2.6M,36M)
# ljournal-2008.mtx #(directed)(5M,79M)
# soc-Pokec.mtx #(directed)(1.6M,30M)
# kron_g500-logn20.mtx #(undirected)(1M,89M)
# kron_g500-logn21.mtx #(undirected)(2M,182M)	
		
# ./evaluate_datasets.sh pr 0 2 /home/oj2zf/Documents/dataset/kron_g500-logn20.mtx
# ./evaluate_datasets.sh pr 0 8 /home/oj2zf/Documents/dataset/rmat_16m_256m.mtx
# ./evaluate_datasets.sh pr 0 8 /home/oj2zf/Documents/dataset/twitter7/twitter7.mtx
# ./evaluate_datasets.sh pr 0 8 /home/oj2zf/Documents/dataset/rmat_16_1024/rmat_16_1024.mtx
# ./evaluate_datasets.sh pr 0 8 /home/oj2zf/Documents/dataset/rmat_32_2048/rmat_32_2048.mtx
# ./evaluate_datasets.sh pr 0 8 /home/oj2zf/Documents/dataset/sk-2005/sk-2005.mtx
# ./evaluate_datasets.sh pr 0 8 /home/oj2zf/Documents/dataset/uk-2005/uk-2005.mtx
# ./evaluate_datasets.sh pr 0 8 /home/oj2zf/Documents/dataset/com-Friendster/com-Friendster.mtx

# ./host pr 1 0 12 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/kron_g500-logn20.mtx

./host "$1" 1 "0" 12 "/home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin" "$4" 


