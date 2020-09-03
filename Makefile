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

# Points to Utility Directory
COMMON_REPO = /home/oj2zf/Documents/SDAccel_Examples/
ABS_COMMON_REPO = /home/oj2zf/Documents/SDAccel_Examples/

TARGETS := hw
TARGET := $(TARGETS)
DEVICE := $(DEVICES)
XCLBIN := ./xclbin

include ./utils.mk

DSA = xilinx_u280_xdma_201910_1
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

# KERNEL_TOP += acts/acts/acts.cpp
KERNEL_TOP += acts/acts_lw/actspartition.cpp
HOST_TOP += examples/hostprocess.cpp

# HOST_SRCS += src/host.cpp
# HOST_SRCS += examples/hostprocess.cpp
HOST_SRCS += src/algorithm/algorithm.cpp
HOST_SRCS += src/dataaccess/dataaccess.cpp
HOST_SRCS += src/edgeprocess/edge_process.cpp
HOST_SRCS += examples/helperfunctions/helperfunctions.cpp
HOST_SRCS += examples/pagerank/pagerank.cpp
HOST_SRCS += examples/bfs/bfs.cpp
HOST_SRCS += examples/test/test.cpp
HOST_SRCS += src/graphs/graph.cpp
HOST_SRCS += src/heuristics/heuristics.cpp
HOST_SRCS += src/stats/stats.cpp
HOST_SRCS += src/utility/utility.cpp
HOST_SRCS += src/dataset/dataset.cpp
HOST_SRCS += kernels/kernel.cpp
HOST_SRCS += kernels/swkernel.cpp
# HOST_SRCS += kernels/oclkernel.cpp
HOST_SRCS += kernels/goclkernel.cpp
HOST_SRCS += acts/actsutility/actsutility.cpp

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

# Kernel linker flags
LDCLFLAGS += --sp topkernel_1.m_axi_gmem0:HBM[0:3] --sp topkernel_1.m_axi_gmem1:HBM[0:3] --sp topkernel_1.m_axi_gmem2:HBM[0:3] --sp topkernel_1.m_axi_gmem3:HBM[0:3]		
LDCLFLAGS += --sp topkernel_2.m_axi_gmem0:HBM[4:7] --sp topkernel_2.m_axi_gmem1:HBM[4:7] --sp topkernel_2.m_axi_gmem2:HBM[4:7] --sp topkernel_2.m_axi_gmem3:HBM[4:7]
LDCLFLAGS += --sp topkernel_3.m_axi_gmem0:HBM[8:11] --sp topkernel_3.m_axi_gmem1:HBM[8:11] --sp topkernel_3.m_axi_gmem2:HBM[8:11] --sp topkernel_3.m_axi_gmem3:HBM[8:11]
LDCLFLAGS += --sp topkernel_4.m_axi_gmem0:HBM[12:15] --sp topkernel_4.m_axi_gmem1:HBM[12:15] --sp topkernel_4.m_axi_gmem2:HBM[12:15] --sp topkernel_4.m_axi_gmem3:HBM[12:15]
LDCLFLAGS += --sp topkernel_5.m_axi_gmem0:HBM[16:19] --sp topkernel_5.m_axi_gmem1:HBM[16:19] --sp topkernel_5.m_axi_gmem2:HBM[16:19] --sp topkernel_5.m_axi_gmem3:HBM[16:19]
LDCLFLAGS += --sp topkernel_6.m_axi_gmem0:HBM[20:23] --sp topkernel_6.m_axi_gmem1:HBM[20:23] --sp topkernel_6.m_axi_gmem2:HBM[20:23] --sp topkernel_6.m_axi_gmem3:HBM[20:23]
LDCLFLAGS += --sp topkernel_7.m_axi_gmem0:HBM[24:27] --sp topkernel_7.m_axi_gmem1:HBM[24:27] --sp topkernel_7.m_axi_gmem2:HBM[24:27] --sp topkernel_7.m_axi_gmem3:HBM[24:27]							
LDCLFLAGS += --sp topkernel_8.m_axi_gmem0:HBM[28:31] --sp topkernel_8.m_axi_gmem1:HBM[28:31] --sp topkernel_8.m_axi_gmem2:HBM[28:31] --sp topkernel_8.m_axi_gmem3:HBM[28:31]

# LDCLFLAGS += --sp topkernel_1.m_axi_gmem0:HBM[0:3] --sp topkernel_1.m_axi_gmem1:HBM[0:3] --sp topkernel_1.m_axi_gmem2:HBM[0:3]
# LDCLFLAGS += --sp topkernel_2.m_axi_gmem0:HBM[4:7] --sp topkernel_2.m_axi_gmem1:HBM[4:7] --sp topkernel_2.m_axi_gmem2:HBM[4:7]
# LDCLFLAGS += --sp topkernel_3.m_axi_gmem0:HBM[8:11] --sp topkernel_3.m_axi_gmem1:HBM[8:11] --sp topkernel_3.m_axi_gmem2:HBM[8:11]
# LDCLFLAGS += --sp topkernel_4.m_axi_gmem0:HBM[12:15] --sp topkernel_4.m_axi_gmem1:HBM[12:15] --sp topkernel_4.m_axi_gmem2:HBM[12:15]
# LDCLFLAGS += --sp topkernel_5.m_axi_gmem0:HBM[16:19] --sp topkernel_5.m_axi_gmem1:HBM[16:19] --sp topkernel_5.m_axi_gmem2:HBM[16:19]
# LDCLFLAGS += --sp topkernel_6.m_axi_gmem0:HBM[20:23] --sp topkernel_6.m_axi_gmem1:HBM[20:23] --sp topkernel_6.m_axi_gmem2:HBM[20:23]
# LDCLFLAGS += --sp topkernel_7.m_axi_gmem0:HBM[24:27] --sp topkernel_7.m_axi_gmem1:HBM[24:27] --sp topkernel_7.m_axi_gmem2:HBM[24:27]						
# LDCLFLAGS += --sp topkernel_8.m_axi_gmem0:HBM[28:31] --sp topkernel_8.m_axi_gmem1:HBM[28:31] --sp topkernel_8.m_axi_gmem2:HBM[28:31]

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
	$(XOCC) $(CLFLAGS) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernel -I'$(<D)' -o'$@' '$<'
$(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernel_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS) --nk topkernel:8 -o'$@' $(+)
	# $(XOCC) $(CLFLAGS) -g --temp_dir $(BUILD_DIR_topkernel) -l --profile_kernel data:all:all:all $(LDCLFLAGS) --nk topkernel:8 -o'$@' $(+)

# Building Host
$(EXECUTABLE): check-xrt $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(HOST_HDRS)
	$(CXX) $(CXXFLAGS) $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS)	$(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -lsortreduce -pthread -laio -march=native -lrt $(HOST_HDRS) -o '$@' $(LDFLAGS)

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
$(error Nothing to be done for make)
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
	
### CPU Multithreaded Implementation
demo_acts_nthreads: clean build_acts_nthreads run_nthreads
demo_acts_nthreads_debug: clean build_acts_nthreads run_nthreads_debug

build_acts_nthreads:
	g++ -O3 $(HOST_TOP) $(HOST_SRCS) $(KERNEL_TOP) $(KERNEL_SRCS) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -o acts_nthreads				

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
	python gen.py $(XWARE) $(SETUP) $(ALGORITHM) $(DATASET) $(NUMSUPERCPUTHREADS) $(NUMCPUTHREADS) $(NUMSUBCPUTHREADS_POW) $(LOCKE) $(EVALUATION_TYPE) $(EVALUATION_PARAM0)

# Cleaning stuff
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	-$(RMDIR) src/*.ll _xocc_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: clean
	rm -rf sr acts_nthreads sr_nthreads
	# -$(RMDIR) $(XCLBIN)
	-$(RMDIR) _x.*

