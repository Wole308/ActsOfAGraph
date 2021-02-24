.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform>"
	$(ECHO) "      Command to generate the design for specified Target and Device."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) "  make check TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform>"
	$(ECHO) "      Command to run application in emulation."
	$(ECHO) ""
	$(ECHO) "  make build TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform>"
	$(ECHO) "      Command to build xclbin application."
	$(ECHO) ""
	$(ECHO) "  make run_nimbix DEVICE=<FPGA platform>"
	$(ECHO) "      Command to run application on Nimbix Cloud."
	$(ECHO) ""
	$(ECHO) "  make aws_build DEVICE=<FPGA platform>"
	$(ECHO) "      Command to build AWS xclbin application on AWS Cloud."
	$(ECHO) ""

# Points to Utility Directory (***choice between CREBTREE or AWS***)
COMMON_REPO = /home/oj2zf/Documents/SDAccel_Examples/
ABS_COMMON_REPO = /home/oj2zf/Documents/SDAccel_Examples/
# COMMON_REPO = /home/centos/src/project_data/oj2zf/SDAccel_Examples/
# ABS_COMMON_REPO = /home/centos/src/project_data/oj2zf/SDAccel_Examples/

TARGETS := hw
TARGET := $(TARGETS)
DEVICE := $(DEVICES)
XCLBIN := ./xclbin

include ./utils.mk

# (***choice between CREBTREE or AWS***)
DSA = xilinx_u280_xdma_201910_1
# DSA = xilinx_aws_vu9p_f1
BUILD_DIR := ./_x.$(TARGET).$(DSA)

BUILD_DIR_topkernel = $(BUILD_DIR)/topkernel

CXX := $(XILINX_SDX)/bin/xcpp
XOCC := $(XILINX_SDX)/bin/xocc

#Include Libraries
include $(ABS_COMMON_REPO)/libs/opencl/opencl.mk
include $(ABS_COMMON_REPO)/libs/xcl2/xcl2.mk
CXXFLAGS += $(xcl2_CXXFLAGS)
LDFLAGS += $(xcl2_LDFLAGS)
HOST_OCLSRCS += $(xcl2_SRCS)
CXXFLAGS += $(opencl_CXXFLAGS) -Wall -O0 -g -std=c++14
LDFLAGS += $(opencl_LDFLAGS)

KERNEL_TOP += acts/acts/acts.cpp
KERNEL_TOP += acts/actsutility/actsutility.cpp

HOST_TOP += examples/hostprocess.cpp

HOST_SRCS += src/algorithm/algorithm.cpp
HOST_SRCS += examples/helperfunctions/loadgraph.cpp
HOST_SRCS += examples/helperfunctions/setupkernel.cpp
HOST_SRCS += examples/helperfunctions/evalparams.cpp
HOST_SRCS += examples/pagerank/pagerank.cpp
HOST_SRCS += examples/bfs/bfs.cpp
HOST_SRCS += examples/sssp/sssp.cpp
HOST_SRCS += examples/test/test.cpp
HOST_SRCS += src/graphs/graph.cpp
HOST_SRCS += src/stats/stats.cpp
HOST_SRCS += src/utility/utility.cpp
HOST_SRCS += src/dataset/dataset.cpp
HOST_SRCS += kernels/kernel.cpp
HOST_SRCS += kernels/swkernel.cpp
HOST_SRCS += kernels/goclkernel.cpp
HOST_SRCS += src/graphs/creategraphs.cpp
HOST_SRCS += src/graphs/mysort.cpp
HOST_SRCS += acts/sortreduce/sr.cpp

# sort-reduce files
SORTREDUCE_INCLUDE=sortreduce-master/include/
SORTREDUCE_LIB=sortreduce-master/obj/
GRAPH_SRC=sortreduce-master/examples/graph/src/
GRAPH_CPP=$(wildcard $(GRAPH_SRC)/*.cpp)
SRFLAGS=-DKVTYPES1='uint64_t,uint32_t'  -DKVTYPES2='uint32_t,uint32_t'

# Host compiler global settings
CXXFLAGS += -fmessage-length=0
LDFLAGS += -lrt -lstdc++ 

# Kernel compiler global settings
CLFLAGS += -t $(TARGET) --platform $(DEVICE) --save-temps

# Kernel linker flags (***choice between CREBTREE or AWS***)
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem0:HBM[0]

# LDCLFLAGS += --sp topkernel_1.m_axi_gmem0:HBM[0] 
# LDCLFLAGS += --sp topkernel_2.m_axi_gmem0:HBM[1] 
# LDCLFLAGS += --sp topkernel_3.m_axi_gmem0:HBM[2] 
# LDCLFLAGS += --sp topkernel_4.m_axi_gmem0:HBM[3]
# LDCLFLAGS += --sp topkernel_5.m_axi_gmem0:HBM[4]
# LDCLFLAGS += --sp topkernel_6.m_axi_gmem0:HBM[5]
# LDCLFLAGS += --sp topkernel_7.m_axi_gmem0:HBM[6]
# LDCLFLAGS += --sp topkernel_8.m_axi_gmem0:HBM[7]
# LDCLFLAGS += --sp topkernel_9.m_axi_gmem0:HBM[8]
# LDCLFLAGS += --sp topkernel_10.m_axi_gmem0:HBM[9]
# LDCLFLAGS += --sp topkernel_11.m_axi_gmem0:HBM[10]
# LDCLFLAGS += --sp topkernel_12.m_axi_gmem0:HBM[11]
# LDCLFLAGS += --sp topkernel_13.m_axi_gmem0:HBM[12]
# LDCLFLAGS += --sp topkernel_14.m_axi_gmem0:HBM[13]
# LDCLFLAGS += --sp topkernel_15.m_axi_gmem0:HBM[14]
# LDCLFLAGS += --sp topkernel_16.m_axi_gmem0:HBM[15]

LDCLFLAGS += --sp topkernel_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS += --sp topkernel_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS += --sp topkernel_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS += --sp topkernel_1.m_axi_gmem3:HBM[3]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem4:HBM[4]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem5:HBM[5]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem6:HBM[6]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem7:HBM[7]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem8:HBM[8]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem9:HBM[9]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem10:HBM[10]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem11:HBM[11]
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem12:HBM[12]
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem13:HBM[13]
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem14:HBM[14]
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem15:HBM[15]
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem16:HBM[16]

# LDCLFLAGS += --sp topkernel_1.m_axi_gmem0:DDR[0] 
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem1:DDR[1] 
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem2:DDR[2] 
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem3:DDR[3]

# LDCLFLAGS += --sp topkernel_1.m_axi_gmem0:bank0
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem1:bank1 
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem2:bank2 
# LDCLFLAGS += --sp topkernel_1.m_axi_gmem3:bank3 

EXECUTABLE = host
CMD_ARGS = $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin

EMCONFIG_DIR = $(XCLBIN)/$(DSA)

BINARY_CONTAINERS += $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernel_OBJS += $(XCLBIN)/topkernel.$(TARGET).$(DSA).xo

CP = cp -rf

.PHONY: all clean cleanall docs emconfig
all: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig

.PHONY: exe
exe: $(EXECUTABLE)

.PHONY: build
build: $(BINARY_CONTAINERS)

# Building kernel
$(XCLBIN)/topkernel.$(TARGET).$(DSA).xo: $(KERNEL_TOP)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernel -I'$(<D)' -I'acts/actsutility/' -o'$@' $(KERNEL_TOP)
$(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernel_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS) --nk topkernel:1 -o'$@' $(+)

# Building Host (***choice between CREBTREE or AWS***)
$(EXECUTABLE): check-xrt $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(HOST_HDRS) 
	/tools/Xilinx/SDx/2019.1/bin/xcpp -Wall -O3 -g -std=c++11 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/SDx/2019.1/runtime/ -I/tools/Xilinx/Vivado/2019.1/include/ -std=c++0x $(CXXFLAGS) $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) acts/actsutility/actsutility.cpp $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -lsortreduce -pthread -laio -march=native -lrt ./xcl.c -o host -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt				
# $(EXECUTABLE): check-xrt $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(HOST_HDRS) 
	# /opt/Xilinx/SDx/2019.1.op2552052/bin/xcpp -Wall -O3 -g -std=c++11 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/SDx/2019.1/runtime/ -I/tools/Xilinx/Vivado/2019.1/include/ -std=c++0x $(CXXFLAGS) $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) acts/actsutility/actsutility.cpp $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -lsortreduce -pthread -laio -march=native -lrt ./xcl.c -o host -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt				

emconfig:$(EMCONFIG_DIR)/emconfig.json
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(DEVICE) --od $(EMCONFIG_DIR)
	
check: all
ifeq ($(findstring xilinx_u200_xdma, $(DEVICE)), xilinx_u200_xdma)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx_u200_qdma, $(DEVICE)), xilinx_u200_qdma)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx_u250_xdma, $(DEVICE)), xilinx_u250_xdma)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx_u250_qdma, $(DEVICE)), xilinx_u250_qdma)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx_vcu1525_dynamic, $(DEVICE)), xilinx_vcu1525_dynamic)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx_aws-vu9p-f1-04261818, $(DEVICE)), xilinx_aws-vu9p-f1-04261818)
# $(error Nothing to be done for make $(ENV) --- )
endif
ifeq ($(findstring xilinx:u200:xdma, $(DEVICE)), xilinx:u200:xdma)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx:u200:qdma, $(DEVICE)), xilinx:u200:qdma)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx:u250:xdma, $(DEVICE)), xilinx:u250:xdma)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx:u250:qdma, $(DEVICE)), xilinx:u250:qdma)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx:vcu1525:dynamic, $(DEVICE)), xilinx:vcu1525:dynamic)
$(error Nothing to be done for make)
endif
ifeq ($(findstring xilinx:aws-vu9p-f1-04261818:dynamic, $(DEVICE)), xilinx:aws-vu9p-f1-04261818:dynamic)
$(error Nothing to be done for make)
endif

ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	$(CP) $(EMCONFIG_DIR)/emconfig.json .
	XCL_EMULATION_MODE=$(TARGET) ./$(EXECUTABLE) $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin
else
	 ./$(EXECUTABLE) $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin
endif
	sdx_analyze profile -i profile_summary.csv -f html

run_nimbix: all
	$(COMMON_REPO)/utility/nimbix/run_nimbix.py $(EXECUTABLE) $(CMD_ARGS) $(DSA)

aws_build: check-aws_repo $(BINARY_CONTAINERS)
	$(COMMON_REPO)/utility/aws/run_aws.py $(BINARY_CONTAINERS)
	
### CPU Multithreaded Implementation (-fstack-protector -fno-stack-protector)  -O3
demo_acts_nthreads: clean build_acts_nthreads run_nthreads
demo_acts_nthreads_debug: clean build_acts_nthreads run_nthreads_debug
			
build_acts_nthreads:
	g++ $(HOST_TOP) $(HOST_SRCS) $(KERNEL_TOP) $(KERNEL_SRCS) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -o acts_nthreads							

run_nthreads:
	./acts_nthreads
	
run_nthreads_debug:
	gdb ./acts_nthreads
	
### GrafBoost Multithreaded Implementation
demo_grafboost_nthreads: clean build_grafboost_nthreads run_grafboost_nthreads
demo_grafboost_nthreads_debug: clean build_grafboost_nthreads run_grafboost_nthreads_debug

build_grafboost_nthreads: $(SORTREDUCE_LIB)/libsortreduce.a
	mkdir -p obj
	g++ -o ./sr_nthreads $(HOST_TOP) $(HOST_SRCS) $(KERNEL_TOP) $(KERNEL_SRCS) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -g

run_grafboost_nthreads:
	./sr_nthreads
	
run_grafboost_nthreads_debug:
	gdb ./sr_nthreads
	
### generate source files (python)
generatesrcs:
	python gen.py $(XWARE) $(SETUP) $(ALGORITHM) $(DATASET) $(NUMSUPERCPUTHREADS) $(NUMCPUTHREADS) $(NUMSUBCPUTHREADS) $(LOCKE) $(EVALUATION_TYPE) $(EVALUATION_PARAM0)
	
# Cleaning stuff
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	-$(RMDIR) src/*.ll _xocc_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: clean
	rm -rf sr acts_nthreads sr_nthreads
	-$(RMDIR) $(XCLBIN)
	-$(RMDIR) _x.*

