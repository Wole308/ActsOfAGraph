TARGETPLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
TARGETPLATFORM_AWS=/home/centos/src/project_data/aws-fpga/SDAccel/aws_platform/xilinx_aws-vu9p-f1-04261818_dynamic_5_0/xilinx_aws-vu9p-f1-04261818_dynamic_5_0.xpfm

# Memory assignment flags
# MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem0:bank0
# MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem1:bank0 
# MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem2:bank0 

MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem0:bank0 
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem1:bank1 
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem2:bank2 
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem3:bank3
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem4:bank0  
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem5:bank1  
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem6:bank2 
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem7:bank3
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem8:bank0 
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem9:bank1 
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem10:bank2 
MEMBANKCONFIG_AWS += --sp topkernel_1.m_axi_gmem11:bank3

# Kernel linker flags
# LDCLFLAGS += --slr topkernel_1:SLR0 --slr topkernel_2:SLR0 --slr topkernel_3:SLR1 --slr topkernel_4:SLR1

# Memory assignment flags (2 Instances Implementation)
MEMBANKCONFIG_1stInst += --sp topkernel_1.m_axi_gmem0:bank0 
MEMBANKCONFIG_1stInst += --sp topkernel_1.m_axi_gmem1:bank0 
MEMBANKCONFIG_1stInst += --sp topkernel_1.m_axi_gmem2:bank0 

MEMBANKCONFIG_2ndInst += --sp topkernel_2.m_axi_gmem0:bank1
MEMBANKCONFIG_2ndInst += --sp topkernel_2.m_axi_gmem1:bank1  
MEMBANKCONFIG_2ndInst += --sp topkernel_2.m_axi_gmem2:bank1 
 
MEMBANKCONFIG_3rdInst += --sp topkernel_3.m_axi_gmem0:bank2 
MEMBANKCONFIG_3rdInst += --sp topkernel_3.m_axi_gmem1:bank2
MEMBANKCONFIG_3rdInst += --sp topkernel_3.m_axi_gmem2:bank2 

MEMBANKCONFIG_4thInst += --sp topkernel_4.m_axi_gmem0:bank3 
MEMBANKCONFIG_4thInst += --sp topkernel_4.m_axi_gmem1:bank3 
MEMBANKCONFIG_4thInst += --sp topkernel_4.m_axi_gmem2:bank3

# Src files
# TOPKERNEL_ARG += kernels/enigma.cpp
TOPKERNEL_ARG += kernels/acts.cpp

HOST_TOP += src/hostprocess.cpp
HOST_TOP_NTHREADS += src/hostprocess.cpp
HOST_SRCS += 
HOST_SRCS += 
HOST_SRCS += 
HOST_SRCS += debugger/host_debugger.cpp

HOSTPROCESS_SRCS += src/actgraph_pr_sw.cpp
HOSTPROCESS_SRCS += src/actgraph_bfs_sw.cpp
HOSTPROCESS_SRCS += src/actgraph_bc_sw.cpp
HOSTPROCESS_SRCS += src/grafboost_pr_sw.cpp
HOSTPROCESS_SRCS += src/grafboost_bfs_sw.cpp
HOSTPROCESS_SRCS += src/grafboost_bc_sw.cpp
HOSTPROCESS_SRCS += src/host_enigma.cpp
HOSTPROCESS_SRCS += src/edge_process.cpp
HOSTPROCESS_SRCS += kernels/srkernelprocess.cpp
HOSTPROCESS_SRCS += graphs/create2Dgraph.cpp
HOSTPROCESS_SRCS += graphs/createsmartgraph.cpp
HOSTPROCESS_SRCS += graphs/graph.cpp
HOSTPROCESS_SRCS += graphs/generateactivevertices.cpp
HOSTPROCESS_SRCS += heuristics/heuristics.cpp
HOSTPROCESS_SRCS += algorithm/algorithm.cpp
HOSTPROCESS_SRCS += utility/utility.cpp
HOSTPROCESS_SRCS += kernels/kernelprocess.cpp

# sort-reduce files
SORTREDUCE_INCLUDE=sortreduce-master/include/
SORTREDUCE_LIB=sortreduce-master/obj/
GRAPH_SRC=sortreduce-master/examples/graph/src/
GRAPH_CPP=$(wildcard $(GRAPH_SRC)/*.cpp)
SRFLAGS=-DKVTYPES1='uint64_t,uint32_t'  -DKVTYPES2='uint32_t,uint32_t'

$(info *** ACTS synthesis started! ***)

### FPGA Implementation
swemu: cleanall build_xo_swemu build_xclbin_swemu build_host run_swemu
swemu_2inst: cleanall build_xo_swemu build_xclbin_swemu_2inst build_host run_swemu

swemu_aws: cleanall build_xo_swemu_aws build_xclbin_swemu_aws build_host_aws run_swemu
swemu_2inst_aws: cleanall build_xo_swemu_aws build_xclbin_swemu_2inst_aws build_host_aws run_swemu

hw: cleanall build_xo_hw build_xclbin_hw build_host
hw_4inst: cleanall build_xo_hw build_xclbin_hw_4inst build_host

hw_aws: cleanall build_xo_hw_aws build_xclbin_hw_aws build_host_aws
hw_4inst_aws: cleanall build_xo_hw_aws build_xclbin_hw_4inst_aws build_host_aws
	
build_xo_swemu:
	/tools/Xilinx/SDx/2019.1/bin/xocc -c --target sw_emu --platform $(TARGETPLATFORM) $(TOPKERNEL_ARG) $(KERNEL_SRCS) --kernel topkernel -o kernel.xo
build_xclbin_swemu:
	/tools/Xilinx/SDx/2019.1/bin/xocc -l --target sw_emu --platform $(TARGETPLATFORM) $(MEMBANKCONFIG) kernel.xo -o kernel.xclbin
build_xclbin_swemu_2inst:
	/tools/Xilinx/SDx/2019.1/bin/xocc -l --target sw_emu --platform $(TARGETPLATFORM) $(MEMBANKCONFIG_1stInst) $(MEMBANKCONFIG_2ndInst) $(MEMBANKCONFIG_3rdInst) $(MEMBANKCONFIG_4thInst) $(LDCLFLAGS) kernel.xo -o kernel.xclbin
	
build_xo_swemu_aws:
	/opt/Xilinx/SDx/2019.1.op2552052/bin/xocc -c --target sw_emu --platform $(TARGETPLATFORM_AWS) $(TOPKERNEL_ARG) $(KERNEL_SRCS) --kernel topkernel -o kernel.xo
build_xclbin_swemu_aws:
	/opt/Xilinx/SDx/2019.1.op2552052/bin/xocc -l --target sw_emu --platform $(TARGETPLATFORM_AWS) $(MEMBANKCONFIG_AWS) kernel.xo -o kernel.xclbin
build_xclbin_swemu_2inst_aws:
	/opt/Xilinx/SDx/2019.1.op2552052/bin/xocc -l --target sw_emu --platform $(TARGETPLATFORM_AWS) $(MEMBANKCONFIG_1stInst) $(MEMBANKCONFIG_2ndInst) $(MEMBANKCONFIG_3rdInst) $(MEMBANKCONFIG_4thInst) $(LDCLFLAGS) kernel.xo -o kernel.xclbin
	
build_xo_hw:
	/tools/Xilinx/SDx/2019.1/bin/xocc -g -c --kernel_frequency=300 --target hw --platform $(TARGETPLATFORM) $(TOPKERNEL_ARG) $(KERNEL_SRCS) --kernel topkernel -o kernel.xo
build_xclbin_hw:
	/tools/Xilinx/SDx/2019.1/bin/xocc -g -l --kernel_frequency=300 --profile_kernel data:all:all:all --target hw --platform $(TARGETPLATFORM) $(MEMBANKCONFIG) kernel.xo -o kernel.xclbin
build_xclbin_hw_4inst:
	/tools/Xilinx/SDx/2019.1/bin/xocc -g -l --kernel_frequency=300 --profile_kernel data:all:all:all --target hw --platform $(TARGETPLATFORM) $(MEMBANKCONFIG_1stInst) $(MEMBANKCONFIG_2ndInst) $(MEMBANKCONFIG_3rdInst) $(MEMBANKCONFIG_4thInst) $(LDCLFLAGS) --nk topkernel:4 kernel.xo -o kernel.xclbin
	
build_xo_hw_aws:
	/opt/Xilinx/SDx/2019.1.op2552052/bin/xocc -g -c --kernel_frequency=300 --target hw --platform $(TARGETPLATFORM_AWS) $(TOPKERNEL_ARG) $(KERNEL_SRCS) --kernel topkernel -o kernel.xo
build_xclbin_hw_aws:
	/opt/Xilinx/SDx/2019.1.op2552052/bin/xocc -g -l --kernel_frequency=300 --profile_kernel data:all:all:all --target hw --platform $(TARGETPLATFORM_AWS) $(MEMBANKCONFIG_AWS) kernel.xo -o kernel.xclbin
build_xclbin_hw_4inst_aws:
	/opt/Xilinx/SDx/2019.1.op2552052/bin/xocc --xp param:compiler.userPostSysLinkTcl=/home/centos/src/project_data/oj2zf/ActsOfAGraph/userPostSysLink.tcl -g -l --kernel_frequency=300 --profile_kernel data:all:all:all --target hw --platform $(TARGETPLATFORM_AWS) $(MEMBANKCONFIG_1stInst) $(MEMBANKCONFIG_2ndInst) $(MEMBANKCONFIG_3rdInst) $(MEMBANKCONFIG_4thInst) $(LDCLFLAGS) --nk topkernel:4 kernel.xo -o kernel.xclbin
	
build_host:
	/tools/Xilinx/SDx/2019.1/bin/xcpp -Wall -O0 -g -std=c++14 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/SDx/2019.1/runtime/ -I/tools/Xilinx/Vivado/2019.1/include/ -std=c++0x $(HOST_TOP) $(HOST_SRCS) $(HOSTPROCESS_SRCS) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -lsortreduce -pthread -laio -march=native -lrt ./xcl.c -o host -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt
build_host_aws:
	/opt/Xilinx/SDx/2019.1.op2552052/bin/xcpp -Wall -O0 -g -std=c++14 -I/opt/xilinx/xrt/include/ -I/opt/Xilinx/SDx/2019.1.op2552052/runtime/ -I/opt/Xilinx/Vivado/2019.1.op2552052/include/ -std=c++0x $(HOST_TOP) $(HOST_SRCS) $(HOSTPROCESS_SRCS) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt ./xcl.c -o host -L/opt/xilinx/xrt/lib/ -lOpenCL -pthread -lrt				

run_swemu:
	XCL_EMULATION_MODE=sw_emu ./host kernel.xclbin
	
gen_emulation_config_file:
	emconfigutil --nd 1 --od . --platform xilinx_u280_xdma_201910_1

source_sdaccellibs:
	source /opt/Xilinx/SDx/2018.2/settings64.sh 
	source /opt/XRT/build/Release/opt/xilinx/xrt/setup.sh 
	export XILINX_SDX=/opt/Xilinx/SDx/2018.2
	
### CPU Multithreaded Implementation
demo_acts_nthreads: clean build_acts_nthreads run_nthreads
demo_acts_nthreads_debug: clean build_acts_nthreads run_nthreads_debug

build_acts_nthreads:
	g++ -O3 $(HOST_TOP_NTHREADS) $(HOST_SRCS) $(HOSTPROCESS_SRCS) $(TOPKERNEL_ARG) $(KERNEL_SRCS) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -o acts_nthreads

run_nthreads:
	./acts_nthreads
	
run_nthreads_debug:
	gdb ./acts_nthreads
	
### GrafBoost Multithreaded Implementation
demo_grafboost_nthreads: clean build_grafboost_nthreads run_grafboost_nthreads

build_grafboost_nthreads: $(SORTREDUCE_LIB)/libsortreduce.a
	mkdir -p obj
	g++ -o ./sr_nthreads $(HOST_TOP_NTHREADS) $(HOST_SRCS) $(HOSTPROCESS_SRCS) $(TOPKERNEL_ARG) $(KERNEL_SRCS) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -g

run_grafboost_nthreads:
	./sr_nthreads
	
### generate source files (python)
generatesrcs:
	python gen.py $(XWARE) $(SETUP) $(ALGORITHM) $(DATASET) $(NUMCPUTHREADS) $(LOCKE)

### Cleaning stuff
clean:
	rm -rf acts sr acts_nthreads sr_nthreads
	rm -rf *.xo xocc_kernel* _x* pfm_top_* *.xo *.exe host xocc* *.info *.ltx
	rm -rf profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	rm -rf src/*.ll _xocc_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb
	$(info clean successful)
	
cleanall:
	rm -rf acts sr acts_nthreads sr_nthreads
	rm -rf *.xo xocc_kernel* _x* pfm_top_* *.xo *.exe host xocc* *.info *.ltx
	rm -rf profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	rm -rf src/*.ll _xocc_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb
	$(info cleanall successful)
	
	
	
	
	