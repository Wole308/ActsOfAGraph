# NOTE:
# ACTS_XbyYbyZbyM: X: num kernels, Y: num compute units, Z: num HBM channels per compute unit, M: num HBM channels paired in one HBM channel

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
# COMMON_REPO = /home/oj2zf/Documents/SDAccel_Examples/
# ABS_COMMON_REPO = /home/oj2zf/Documents/SDAccel_Examples/
COMMON_REPO = /home/oj2zf/Documents/SDAccel_Examples-master/SDAccel_Examples-master
ABS_COMMON_REPO = /home/oj2zf/Documents/SDAccel_Examples-master/SDAccel_Examples-master
# COMMON_REPO = /home/centos/src/project_data/oj2zf/SDAccel_Examples/
# ABS_COMMON_REPO = /home/centos/src/project_data/oj2zf/SDAccel_Examples/

# RELREF = ../

TARGETS := hw
TARGET := $(TARGETS)
DEVICE := $(DEVICES)
XCLBIN := ./xclbin
SYNFREQUENCY := 300

include ./utils.mk

# (***choice between CREBTREE or AWS***)
# DSA = xilinx_u280_xdma_201910_1
DSA = xilinx_u280_xdma_201920_3
# DSA = xilinx_aws_vu9p_f1
BUILD_DIR := ./_x.$(TARGET).$(DSA)

BUILD_DIR_topkernel = $(BUILD_DIR)/topkernel
BUILD_DIR_topkernelproc = $(BUILD_DIR)/topkernelproc
BUILD_DIR_topkernelsync = $(BUILD_DIR)/topkernelsync
BUILD_DIR_topkernelmerge = $(BUILD_DIR)/topkernelmerge
BUILD_DIR_topkernelapply = $(BUILD_DIR)/topkernelapply
BUILD_DIR_topkernelprocandsync = $(BUILD_DIR)/topkernelprocandsync

# CXX := $(XILINX_SDX)/bin/xcpp
# XOCC := $(XILINX_SDX)/bin/xocc
# /tools/Xilinx/Vitis/2020.1/bin/

CXX := $(XILINX_VITIS)/bin/xcpp
XOCC := $(XILINX_VITIS)/bin/v++

#Include Libraries
include $(ABS_COMMON_REPO)/libs/opencl/opencl.mk
include $(ABS_COMMON_REPO)/libs/xcl2/xcl2.mk
CXXFLAGS += $(xcl2_CXXFLAGS)
LDFLAGS += $(xcl2_LDFLAGS)
HOST_OCLSRCS += $(xcl2_SRCS)
CXXFLAGS += $(opencl_CXXFLAGS) -Wall -O0 -g -std=c++14
LDFLAGS += $(opencl_LDFLAGS)

KERNEL_TOP += $(RELREF)acts/acts/acts.cpp
# KERNEL_TOP += $(RELREF)acts/actsutility/actsutility.cpp

KERNEL_TOP_PROC += $(RELREF)acts/acts/actsproc.cpp
# KERNEL_TOP_PROC += $(RELREF)acts/actsutility/actsutility.cpp

KERNEL_TOP_SYNC += $(RELREF)acts/acts/actssync.cpp
# KERNEL_TOP_SYNC += $(RELREF)acts/actsutility/actsutility.cpp

KERNEL_TOP_MERGE += $(RELREF)acts/acts/actsmerge.cpp
# KERNEL_TOP_MERGE += $(RELREF)acts/actsutility/actsutility.cpp

KERNEL_TOP_APPLY += $(RELREF)acts/acts/actsapply.cpp
# KERNEL_TOP_APPLY += $(RELREF)acts/actsutility/actsutility.cpp

KERNEL_TOP_PROCANDSYNC += $(RELREF)acts/acts/actsproc.cpp
KERNEL_TOP_PROCANDSYNC += $(RELREF)acts/acts/actssync.cpp
# KERNEL_TOP_PROCANDSYNC += $(RELREF)acts/actsutility/actsutility.cpp

# CRITICAL NEWCHANGE.
# KERNEL_UTILITY += $(RELREF)acts/actsutility/actsutility.cpp

HOST_TOP += $(RELREF)examples/hostprocess.cpp

HOST_SRCS += $(RELREF)src/algorithm/algorithm.cpp
HOST_SRCS += $(RELREF)examples/helperfunctions/loadgraph.cpp
HOST_SRCS += $(RELREF)examples/helperfunctions/loadgraph_sw.cpp
HOST_SRCS += $(RELREF)examples/helperfunctions/setupkernel.cpp
HOST_SRCS += $(RELREF)examples/helperfunctions/evalparams.cpp
HOST_SRCS += $(RELREF)examples/app/app.cpp
HOST_SRCS += $(RELREF)examples/test/test.cpp
HOST_SRCS += $(RELREF)src/graphs/graph.cpp
HOST_SRCS += $(RELREF)src/stats/stats.cpp
HOST_SRCS += $(RELREF)src/utility/utility.cpp
HOST_SRCS += $(RELREF)src/dataset/dataset.cpp
HOST_SRCS += $(RELREF)kernels/swkernel.cpp
HOST_SRCS += $(RELREF)kernels/goclkernel.cpp
HOST_SRCS += $(RELREF)src/graphs/creategraphs.cpp
HOST_SRCS += $(RELREF)src/graphs/createundirectedgraph.cpp
HOST_SRCS += $(RELREF)src/graphs/mysort.cpp
HOST_SRCS += $(RELREF)acts/sortreduce/sr.cpp

# sort-reduce files
SORTREDUCE_INCLUDE=$(RELREF)sortreduce-master/include/
SORTREDUCE_LIB=$(RELREF)sortreduce-master/obj/
GRAPH_SRC=$(RELREF)sortreduce-master/examples/graph/src/
GRAPH_CPP=$(wildcard $(GRAPH_SRC)/*.cpp)
SRFLAGS=-DKVTYPES1='uint64_t,uint32_t'  -DKVTYPES2='uint32_t,uint32_t'
ACTSPROC_INCLUDE=$(RELREF)acts/acts/

# Host compiler global settings
CXXFLAGS += -fmessage-length=0
LDFLAGS += -lrt -lstdc++ 

# Kernel compiler global settings
CLFLAGS += -t $(TARGET) --platform $(DEVICE) --save-temps

# === HBM MEMORY ===
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
LDCLFLAGS += --sp topkernel_1.m_axi_gmem12:HBM[12]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem13:HBM[13]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem14:HBM[14]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem15:HBM[15]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem16:HBM[16]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem17:HBM[17]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem18:HBM[18]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem19:HBM[19]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem20:HBM[20]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem21:HBM[21]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem22:HBM[22]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem23:HBM[23]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem24:HBM[24]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem25:HBM[25]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem26:HBM[26]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem27:HBM[27]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem28:HBM[28]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem29:HBM[29]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem30:HBM[30]
LDCLFLAGS += --sp topkernel_1.m_axi_gmem31:HBM[31]

# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.PLACE_DESIGN.ARGS.MORE OPTIONS}={-fanout_opt}
# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.PLACE_DESIGN.ARGS.DIRECTIVE}={-Congestion_SpreadLogic_high}
# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={-Congestion_SSI_SpreadLogic_high}
# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={-Congestion_SpreadLogic_high}
# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={-Congestion_SSI_SpreadLogic_high}
LDCLFLAGS_LOGICOPT += --xp param:vivado.prop:run.impl_1.STRATEGY=Congestion_SpreadLogic_high
LDCLFLAGS_LOGICOPT += --xp param:vivado.prop:run.impl_1.STRATEGY=Congestion_SSI_SpreadLogic_high

LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_2.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_3.m_axi_gmem0:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_4.m_axi_gmem0:HBM[3]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_5.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_6.m_axi_gmem0:HBM[5]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_7.m_axi_gmem0:HBM[6]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_8.m_axi_gmem0:HBM[7]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_9.m_axi_gmem0:HBM[8]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_10.m_axi_gmem0:HBM[9]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_11.m_axi_gmem0:HBM[10]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_12.m_axi_gmem0:HBM[11]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_13.m_axi_gmem0:HBM[12]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_14.m_axi_gmem0:HBM[13]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_15.m_axi_gmem0:HBM[14]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_16.m_axi_gmem0:HBM[15]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_17.m_axi_gmem0:HBM[16]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_18.m_axi_gmem0:HBM[17]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_19.m_axi_gmem0:HBM[18]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_20.m_axi_gmem0:HBM[19]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_21.m_axi_gmem0:HBM[20]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_22.m_axi_gmem0:HBM[21]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_23.m_axi_gmem0:HBM[22]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_24.m_axi_gmem0:HBM[23]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_25.m_axi_gmem0:HBM[24]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_26.m_axi_gmem0:HBM[25]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_27.m_axi_gmem0:HBM[26]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_28.m_axi_gmem0:HBM[27]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_29.m_axi_gmem0:HBM[28]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_30.m_axi_gmem0:HBM[29]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_31.m_axi_gmem0:HBM[30]
LDCLFLAGS_HBM_PROC_1by1 += --sp topkernelproc_32.m_axi_gmem0:HBM[31]

################################### 1by2by1by0 ###################################
#proc 
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_3.m_axi_gmem0:HBM[2]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_4.m_axi_gmem0:HBM[3]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_5.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_6.m_axi_gmem0:HBM[5]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_7.m_axi_gmem0:HBM[6]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_8.m_axi_gmem0:HBM[7]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_9.m_axi_gmem0:HBM[8]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_10.m_axi_gmem0:HBM[9]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_11.m_axi_gmem0:HBM[10]
LDCLFLAGS_HBM_PROC_1by2by1by0_12and1 += --sp topkernelproc_12.m_axi_gmem0:HBM[11]

LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_3.m_axi_gmem0:HBM[2]
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_4.m_axi_gmem0:HBM[3]
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_5.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_6.m_axi_gmem0:HBM[5]
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_7.m_axi_gmem0:HBM[6]
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_8.m_axi_gmem0:HBM[7]
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_9.m_axi_gmem0:HBM[8]
LDCLFLAGS_HBM_PROC_1by2by1by0_10and1 += --sp topkernelproc_10.m_axi_gmem0:HBM[9]

LDCLFLAGS_HBM_PROC_1by2by1by0_8and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by0_8and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by0_8and1 += --sp topkernelproc_3.m_axi_gmem0:HBM[2]
LDCLFLAGS_HBM_PROC_1by2by1by0_8and1 += --sp topkernelproc_4.m_axi_gmem0:HBM[3]
LDCLFLAGS_HBM_PROC_1by2by1by0_8and1 += --sp topkernelproc_5.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by0_8and1 += --sp topkernelproc_6.m_axi_gmem0:HBM[5]
LDCLFLAGS_HBM_PROC_1by2by1by0_8and1 += --sp topkernelproc_7.m_axi_gmem0:HBM[6]
LDCLFLAGS_HBM_PROC_1by2by1by0_8and1 += --sp topkernelproc_8.m_axi_gmem0:HBM[7]

LDCLFLAGS_HBM_PROC_1by2by1by0_6and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by0_6and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by0_6and1 += --sp topkernelproc_3.m_axi_gmem0:HBM[2]
LDCLFLAGS_HBM_PROC_1by2by1by0_6and1 += --sp topkernelproc_4.m_axi_gmem0:HBM[3]
LDCLFLAGS_HBM_PROC_1by2by1by0_6and1 += --sp topkernelproc_5.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by0_6and1 += --sp topkernelproc_6.m_axi_gmem0:HBM[5]

LDCLFLAGS_HBM_PROC_1by2by1by0_4and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by0_4and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by0_4and1 += --sp topkernelproc_3.m_axi_gmem0:HBM[2]
LDCLFLAGS_HBM_PROC_1by2by1by0_4and1 += --sp topkernelproc_4.m_axi_gmem0:HBM[3]

LDCLFLAGS_HBM_PROC_1by2by1by0_2and1 += --sp topkernelproc_1.m_axi_gmem0:DDR[0] 
LDCLFLAGS_HBM_PROC_1by2by1by0_2and1 += --sp topkernelproc_2.m_axi_gmem0:DDR[1] 

LDCLFLAGS_HBM_PROC_1by2by1by0_1and1 += --sp topkernelproc_1.m_axi_gmem0:DDR[0]

# sync 
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem7:HBM[7]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem8:HBM[8]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem9:HBM[9]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem10:HBM[10]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem11:HBM[11]
LDCLFLAGS_HBM_SYNC_1by2by1by0_12and1 += --sp topkernelsync_1.m_axi_gmem12:HBM[12] 

LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem7:HBM[7]
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem8:HBM[8]
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem9:HBM[9]
LDCLFLAGS_HBM_SYNC_1by2by1by0_10and1 += --sp topkernelsync_1.m_axi_gmem10:HBM[10]

LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem7:HBM[7]
LDCLFLAGS_HBM_SYNC_1by2by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem8:HBM[8]

LDCLFLAGS_HBM_SYNC_1by2by1by0_6and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_6and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_6and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_6and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_SYNC_1by2by1by0_6and1 += --sp topkernelsync_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_SYNC_1by2by1by0_6and1 += --sp topkernelsync_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_SYNC_1by2by1by0_6and1 += --sp topkernelsync_1.m_axi_gmem6:HBM[6]

LDCLFLAGS_HBM_SYNC_1by2by1by0_4and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_4and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_4and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_4and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_SYNC_1by2by1by0_4and1 += --sp topkernelsync_1.m_axi_gmem4:HBM[4]

LDCLFLAGS_HBM_SYNC_1by2by1by0_2and1 += --sp topkernelsync_1.m_axi_gmem0:DDR[0] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_2and1 += --sp topkernelsync_1.m_axi_gmem1:DDR[1] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_2and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[0]

LDCLFLAGS_HBM_SYNC_1by2by1by0_1and1 += --sp topkernelsync_1.m_axi_gmem0:DDR[0] 
LDCLFLAGS_HBM_SYNC_1by2by1by0_1and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[0]

#### (1 PE Workers) ####
# proc 
LDCLFLAGS_HBM_PROC_1by2by1by5_5and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by5_5and1 += --sp topkernelproc_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by5_5and1 += --sp topkernelproc_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by2by1by5_5and1 += --sp topkernelproc_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by2by1by5_5and1 += --sp topkernelproc_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by5_5and1 += --sp topkernelproc_1.m_axi_gmem5:HBM[5]
# sync 
LDCLFLAGS_HBM_SYNC_1by2by1by5_5and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[5]
LDCLFLAGS_HBM_SYNC_1by2by1by5_5and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[6]

#### (2 PE Workers) ####
# proc 
LDCLFLAGS_HBM_PROC_1by2by1by3_6and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by3_6and1 += --sp topkernelproc_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by3_6and1 += --sp topkernelproc_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by2by1by3_6and1 += --sp topkernelproc_1.m_axi_gmem3:HBM[3] 
LDCLFLAGS_HBM_PROC_1by2by1by3_6and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by3_6and1 += --sp topkernelproc_2.m_axi_gmem1:HBM[5]
LDCLFLAGS_HBM_PROC_1by2by1by3_6and1 += --sp topkernelproc_2.m_axi_gmem2:HBM[6]
LDCLFLAGS_HBM_PROC_1by2by1by3_6and1 += --sp topkernelproc_2.m_axi_gmem3:HBM[7]
#sync 
LDCLFLAGS_HBM_SYNC_1by2by1by3_6and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[3]
LDCLFLAGS_HBM_SYNC_1by2by1by3_6and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[7]
LDCLFLAGS_HBM_SYNC_1by2by1by3_6and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[8]

#### (3 PE Workers) ####
# proc 
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_1.m_axi_gmem2:HBM[2]
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[3]
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_2.m_axi_gmem1:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_2.m_axi_gmem2:HBM[5]
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_3.m_axi_gmem0:HBM[6]
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_3.m_axi_gmem1:HBM[7]
LDCLFLAGS_HBM_PROC_1by2by1by2_6and1 += --sp topkernelproc_3.m_axi_gmem2:HBM[8]
#sync 
LDCLFLAGS_HBM_SYNC_1by2by1by2_6and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[2]
LDCLFLAGS_HBM_SYNC_1by2by1by2_6and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[5]
LDCLFLAGS_HBM_SYNC_1by2by1by2_6and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[8]
LDCLFLAGS_HBM_SYNC_1by2by1by2_6and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[9]

#### (4 PE Workers) ####
# proc 
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_1.m_axi_gmem2:HBM[2]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[3]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_2.m_axi_gmem1:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_2.m_axi_gmem2:HBM[5]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_3.m_axi_gmem0:HBM[6]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_3.m_axi_gmem1:HBM[7]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_3.m_axi_gmem2:HBM[8]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_4.m_axi_gmem0:HBM[9]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_4.m_axi_gmem1:HBM[10]
LDCLFLAGS_HBM_PROC_1by2by1by2_8and1 += --sp topkernelproc_4.m_axi_gmem2:HBM[11]
#sync 
LDCLFLAGS_HBM_SYNC_1by2by1by2_8and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[2]
LDCLFLAGS_HBM_SYNC_1by2by1by2_8and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[5]
LDCLFLAGS_HBM_SYNC_1by2by1by2_8and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[8]
LDCLFLAGS_HBM_SYNC_1by2by1by2_8and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[11]
LDCLFLAGS_HBM_SYNC_1by2by1by2_8and1 += --sp topkernelsync_1.m_axi_gmem4:HBM[12]

#### (5 PE Workers) ####
# proc 
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_2.m_axi_gmem0:HBM[2] 
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_2.m_axi_gmem1:HBM[3]
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_3.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_3.m_axi_gmem1:HBM[5]
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_4.m_axi_gmem0:HBM[6]
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_4.m_axi_gmem1:HBM[7]
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_5.m_axi_gmem0:HBM[8]
LDCLFLAGS_HBM_PROC_1by2by1by1_5and1 += --sp topkernelproc_5.m_axi_gmem1:HBM[9]
# sync 
LDCLFLAGS_HBM_SYNC_1by2by1by1_5and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by2by1by1_5and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[3] 
LDCLFLAGS_HBM_SYNC_1by2by1by1_5and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[5] 
LDCLFLAGS_HBM_SYNC_1by2by1by1_5and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[7]
LDCLFLAGS_HBM_SYNC_1by2by1by1_5and1 += --sp topkernelsync_1.m_axi_gmem4:HBM[9]
LDCLFLAGS_HBM_SYNC_1by2by1by1_5and1 += --sp topkernelsync_1.m_axi_gmem5:HBM[10]

################################### xclbins ###################################
EXECUTABLE = host
CMD_ARGS = $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin

EMCONFIG_DIR = $(XCLBIN)/$(DSA)

BINARY_CONTAINERS += $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernel_OBJS += $(XCLBIN)/topkernel.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_PROC += $(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelproc_OBJS += $(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_SYNC += $(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelsync_OBJS += $(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_MERGE += $(XCLBIN)/topkernelmerge.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelmerge_OBJS += $(XCLBIN)/topkernelmerge.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_APPLY += $(XCLBIN)/topkernelapply.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelapply_OBJS += $(XCLBIN)/topkernelapply.$(TARGET).$(DSA).xo

# BINARY_CONTAINERS_PROCANDSYNC += $(XCLBIN)/topkernelprocandsync.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_16AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by0_16and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_12AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by0_12and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_10AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by0_10and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_8AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by0_8and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_6AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by0_6and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_4AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by0_4and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_2AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by0_2and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_1AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by0_1and1.$(TARGET).$(DSA).xclbin

BINARY_CONTAINERS_PROCANDSYNC_1by2by1by5_5AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by5_5and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by3_6AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by3_6and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by2_6AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by2_6and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by2_8AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by2_8and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_PROCANDSYNC_1by2by1by1_5AND1 += $(XCLBIN)/topkernelprocandsync_1by2by1by1_5and1.$(TARGET).$(DSA).xclbin

BINARY_CONTAINER_topkernelprocandsync_OBJS += $(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelprocandsync_OBJS += $(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xo

CP = cp -rf

################################### make commands ###################################
.PHONY: all clean cleanall docs emconfig
all: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig

.PHONY: all_nk clean cleanall docs emconfig
# all_nk: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROC) emconfig
all_nk: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROC) $(BINARY_CONTAINERS_SYNC) emconfig
# all_nk: check-devices $(EXECUTABLE)

# builds # CRITICAL REMOVEME
.PHONY: all clean cleanall docs emconfig
all_proc: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROC) emconfig
# all_proc: check-devices $(BINARY_CONTAINERS_PROC) emconfig

.PHONY: all clean cleanall docs emconfig
all_sync: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_SYNC) emconfig
# all_sync: check-devices $(BINARY_CONTAINERS_SYNC) emconfig

.PHONY: all clean cleanall docs emconfig
all_procandsync: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC) emconfig

all_procandsync_1by2by1by0_16and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_16AND1) emconfig
all_procandsync_1by2by1by0_12and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_12AND1) emconfig
all_procandsync_1by2by1by0_10and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_10AND1) emconfig
all_procandsync_1by2by1by0_8and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_8AND1) emconfig
all_procandsync_1by2by1by0_6and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_6AND1) emconfig
all_procandsync_1by2by1by0_4and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_4AND1) emconfig
all_procandsync_1by2by1by0_2and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_2AND1) emconfig
all_procandsync_1by2by1by0_1and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by0_1AND1) emconfig

all_procandsync_1by2by1by5_5and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by5_5AND1) emconfig
all_procandsync_1by2by1by3_6and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by3_6AND1) emconfig
all_procandsync_1by2by1by2_6and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by2_6AND1) emconfig
all_procandsync_1by2by1by2_8and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by2_8AND1) emconfig
all_procandsync_1by2by1by1_5and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_PROCANDSYNC_1by2by1by1_5AND1) emconfig

.PHONY: exe
exe: $(EXECUTABLE)

.PHONY: build
build: $(BINARY_CONTAINERS)

# Building kernel (ACTS 1 by 1: all proc & sync in 1 kernel)
$(XCLBIN)/topkernel.$(TARGET).$(DSA).xo: $(KERNEL_TOP) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernel -I'$(<D)' -I'$(RELREF)acts/actsutility/' -o'$@' $(KERNEL_TOP) $(KERNEL_UTILITY)
$(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernel_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS) --nk topkernel:1 -o'$@' $(+)
	
# Building kernel (proc)
$(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelproc) -c -k topkernelproc -I'$(<D)' -I'$(RELREF)acts/actsutility/' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelproc_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelproc) -l $(LDCLFLAGS_PROC) $(LDCLFLAGS_LOGICOPT) --nk topkernelproc:4 -o'$@' $(+)
	
# Building kernel (sync)
$(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xo: $(KERNEL_TOP_SYNC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelsync) -c -k topkernelsync -I'$(<D)' -I'$(RELREF)acts/actsutility/' -o'$@' $(KERNEL_TOP_SYNC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelsync) -l $(LDCLFLAGS_SYNC) --nk topkernelsync:1 -o'$@' $(+)	
	
#### (1 PE Workers) ####
$(XCLBIN)/topkernelprocandsync_1by2by1by5_5and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelproc_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelprocandsync) -l $(LDCLFLAGS_HBM_PROC_1by2by1by5_5and1) $(LDCLFLAGS_HBM_SYNC_1by2by1by5_5and1) $(LDCLFLAGS_LOGICOPT) --nk topkernelproc:1 --nk topkernelsync:1 -o'$@' $(+)

#### (3 PE Workers) ####
$(XCLBIN)/topkernelprocandsync_1by2by1by2_6and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelproc_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelprocandsync) -l $(LDCLFLAGS_HBM_PROC_1by2by1by2_6and1) $(LDCLFLAGS_HBM_SYNC_1by2by1by2_6and1) $(LDCLFLAGS_LOGICOPT) --nk topkernelproc:3 --nk topkernelsync:1 -o'$@' $(+)

#### (4 PE Workers) ####
$(XCLBIN)/topkernelprocandsync_1by2by1by2_8and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelproc_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelprocandsync) -l $(LDCLFLAGS_HBM_PROC_1by2by1by2_8and1) $(LDCLFLAGS_HBM_SYNC_1by2by1by2_8and1) $(LDCLFLAGS_LOGICOPT) --nk topkernelproc:4 --nk topkernelsync:1 -o'$@' $(+)

#### (5 PE Workers) ####
$(XCLBIN)/topkernelprocandsync_1by2by1by1_5and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelproc_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelprocandsync) -l $(LDCLFLAGS_HBM_PROC_1by2by1by1_5and1) $(LDCLFLAGS_HBM_SYNC_1by2by1by1_5and1) $(LDCLFLAGS_LOGICOPT) --nk topkernelproc:5 --nk topkernelsync:1 -o'$@' $(+)

# Building Host (***choice between CREBTREE or AWS***)
# $(EXECUTABLE): check-xrt $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(HOST_HDRS) 
	# /tools/Xilinx/SDx/2019.1/bin/xcpp -Wall -O3 -g -std=c++11 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/SDx/2019.1/runtime/ -I/tools/Xilinx/Vivado/2019.1/include/ -std=c++0x $(CXXFLAGS) $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(KERNEL_TOP_SYNC) $(RELREF)acts/actsutility/actsutility.cpp $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -lsortreduce -pthread -laio -march=native -lrt ./xcl.c -o host -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt				
# $(EXECUTABLE): check-xrt $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(HOST_HDRS) 
	# /opt/Xilinx/SDx/2019.1.op2552052/bin/xcpp -Wall -O3 -g -std=c++11 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/SDx/2019.1/runtime/ -I/tools/Xilinx/Vivado/2019.1/include/ -std=c++0x $(CXXFLAGS) $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(KERNEL_TOP_SYNC) $(RELREF)acts/actsutility/actsutility.cpp $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -lsortreduce -pthread -laio -march=native -lrt ./xcl.c -o host -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt				
# $(EXECUTABLE): check-xrt $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS)
	# /tools/Xilinx/Vitis/2020.1/bin/xcpp -Wall -O3 -g -std=c++11 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/Vitis/2020.1/runtime/ -I/tools/Xilinx/Vivado/2020.1/include/ -std=c++0x $(CXXFLAGS) $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(KERNEL_TOP_SYNC) $(RELREF)acts/actsutility/actsutility.cpp -march=native ./xcl.c -o host -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64
# $(EXECUTABLE):
	# /tools/Xilinx/Vitis/2020.1/bin/xcpp ./xcl.c
$(EXECUTABLE): check-xrt $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(HOST_HDRS) 
	/tools/Xilinx/Vitis/2020.1/bin/xcpp -Wall -O3 -g -std=c++11 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/SDx/2019.1/runtime/ -I/tools/Xilinx/Vivado/2019.1/include/ -std=c++0x $(CXXFLAGS) $(HOST_TOP) $(HOST_OCLSRCS) $(HOST_SRCS) $(KERNEL_TOP_SYNC) $(RELREF)acts/actsutility/actsutility.cpp $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -lsortreduce -pthread -laio -march=native -lrt ./xcl.c -o host -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt	
	
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

# SORTREDUCE_INCLUDE=$(RELREF)sortreduce-master/include/
# build_acts_nthreads:
	# g++ $(HOST_TOP) $(HOST_SRCS) $(KERNEL_TOP) $(KERNEL_TOP_PROC) $(KERNEL_TOP_SYNC) $(KERNEL_UTILITY) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -I$(ACTSPROC_INCLUDE) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -o acts_nthreads				
# build_acts_nthreads:
	# g++ $(HOST_TOP) $(HOST_SRCS) $(KERNEL_TOP) $(KERNEL_TOP_PROC) $(KERNEL_TOP_SYNC) $(KERNEL_UTILITY) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -o acts_nthreads							
build_acts_nthreads:
	g++ $(HOST_TOP) $(HOST_SRCS) $(KERNEL_TOP) $(KERNEL_TOP_PROC) $(KERNEL_TOP_SYNC) $(RELREF)acts/actsutility/actsutility.cpp $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -o acts_nthreads
	
run_nthreads:
	./acts_nthreads

run_nthreads_debug:
	gdb ./acts_nthreads
	
### GrafBoost Multithreaded Implementation
demo_grafboost_nthreads: clean build_grafboost_nthreads run_grafboost_nthreads
demo_grafboost_nthreads_debug: clean build_grafboost_nthreads run_grafboost_nthreads_debug

build_grafboost_nthreads: $(SORTREDUCE_LIB)/libsortreduce.a
	mkdir -p obj
	g++ -o ./sr_nthreads $(HOST_TOP) $(HOST_SRCS) $(KERNEL_TOP) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -g

run_grafboost_nthreads:
	./sr_nthreads
	
run_grafboost_nthreads_debug:
	gdb ./sr_nthreads
	
### generate source files (python)
generatesrcs:
	python gen.py $(XWARE) $(SETUP) $(ALGORITHM) $(DATASET) $(NUMSUPERCPUTHREADS) $(NUMCPUTHREADS) $(NUMSUBCPUTHREADS) $(NUMPARTITIONS) $(LOCKE) $(EVALUATION_TYPE) $(EVALUATION_PARAM0) $(NUM_PEs)
	
# Cleaning stuff
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	-$(RMDIR) src/*.ll _xocc_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: clean
	rm -rf sr acts_nthreads sr_nthreads
	-$(RMDIR) $(XCLBIN)
	-$(RMDIR) _x.*

