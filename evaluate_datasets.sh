#!/bin/bash
#!/bin/bash

# ./evaluate_datasets.sh pr 0 4

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

# xilinx_u280_xdma_201920_3

# make run TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# make build TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# example run: ./evaluate_datasets.sh pr 0

# wget https://suitesparse-collection-website.herokuapp.com/MM/LAW/uk-2005.tar.gz
# wget https://suitesparse-collection-website.herokuapp.com/MM/LAW/sk-2005.tar.gz
# wget https://suitesparse-collection-website.herokuapp.com/MM/LAW/it-2004.tar.gz
# wget https://suitesparse-collection-website.herokuapp.com/MM/SNAP/com-Friendster.tar.gz
# wget https://suitesparse-collection-website.herokuapp.com/MM/LAW/webbase-2001.tar.gz

# tar xvf uk-2005.tar.gz
# tar xvf sk-2005.tar.gz
# tar xvf it-2004.tar.gz
# tar xvf com-Friendster.tar.gz
# tar xvf webbase-2001.tar.gz

# mv uk-2005.tar.gz /home/oj2zf/Documents/dataset/uk-2005.tar.gz
# mv sk-2005.tar.gz /home/oj2zf/Documents/dataset/sk-2005.tar.gz
# mv it-2004.tar.gz /home/oj2zf/Documents/dataset/it-2004.tar.gz
# mv com-Friendster.tar.gz /home/oj2zf/Documents/dataset/com-Friendster.tar.gz
# mv sk-2005.tar.gz /home/oj2zf/Documents/dataset/sk-2005.tar.gz
# mv sk-2005.tar.gz /home/oj2zf/Documents/dataset/sk-2005.tar.gz

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
RESULT_PATH=/home/oj2zf/Documents/acts-clusterscale/results

# rm -rf /home/oj2zf/Documents/acts-clusterscale/outputs/golden_2.xclbin
# make host
# make build TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# cp -rf build_dir.sw_emu.xilinx_u280_xdma_201920_3/vmult_vadd.link.xclbin /home/oj2zf/Documents/acts-clusterscale/outputs/golden_2.xclbin
# sleep 3

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

# DIRECTIONS=(1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0)
# DIRECTIONS=(1 1 1 1 1 1 1 1 1 1 1 1 1 1 1)
DIRECTIONS=(0)

DATSETS=(
		# com-Orkut.mtx #(undirected)(3M,234M)
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
		
		# kron_g500-logn20.mtx #(undirected)(1M,89M)
		# rmat_16m_256m.mtx #(directed)(16M,256M)
		twitter7/twitter7.mtx
		# rmat_16_1024/rmat_16_1024.mtx
		# rmat_32_2048/rmat_32_2048.mtx
		# sk-2005/sk-2005.mtx
		# uk-2005/uk-2005.mtx
		# it-2004/it-2004.mtx
		# com-Friendster/com-Friendster.mtx
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



