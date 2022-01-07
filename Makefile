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

# KERNEL_ACTS += $(RELREF)acts/acts/acts.cpp
# KERNEL_TOP += $(RELREF)acts/actsutility/actsutility.cpp

KERNEL_TOP_PROC += $(RELREF)acts/acts/actsproc.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/top_usrcv_udstv.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/top_nusrcv_nudstv.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/processedges_unslicedgraph.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/processedges_slicedgraph.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/partitionupdates.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/reduceupdates.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/mem_access.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/acts_util.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/merge_vtxs.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/acts.cpp
KERNEL_TOP_PROC += $(RELREF)acts/acts/acts_merge.cpp
# KERNEL_TOP_PROC += $(RELREF)acts/actsutility/actsutility.cpp

#CRITICAL REMOVEME.
KERNEL_TOP_SYNC += $(RELREF)acts/acts/actssync.cpp
KERNEL_TOP_SYNC += $(RELREF)acts/acts/actssync_slicedgraph.cpp
# KERNEL_TOP_PROC += $(RELREF)acts/acts/acts_merge.cpp
# KERNEL_TOP_SYNC += $(RELREF)acts/acts/actsmerge.cpp
# KERNEL_TOP_SYNC += $(RELREF)acts/acts/actsapply.cpp
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
HOST_SRCS += $(RELREF)examples/helperfunctions/loadedges.cpp
HOST_SRCS += $(RELREF)examples/helperfunctions/loadedges_splitdstvxs.cpp
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

# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.PLACE_DESIGN.ARGS.MORE OPTIONS}={-fanout_opt}
# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.PLACE_DESIGN.ARGS.DIRECTIVE}={-Congestion_SpreadLogic_high}
# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={-Congestion_SSI_SpreadLogic_high}
# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={-Congestion_SpreadLogic_high}
# LDCLFLAGS_LOGICOPT += --xp param:vivado_prop:run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={-Congestion_SSI_SpreadLogic_high}
LDCLFLAGS_LOGICOPT += --xp param:vivado.prop:run.impl_1.STRATEGY=Congestion_SpreadLogic_high
LDCLFLAGS_LOGICOPT += --xp param:vivado.prop:run.impl_1.STRATEGY=Congestion_SSI_SpreadLogic_high

#### (1 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_1and1 += --sp topkernel_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_1and1 += --sp topkernel_1.m_axi_gmem1:HBM[1] 

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_1and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_1and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_1and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_1and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[2]

#### (3 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_1.m_axi_gmem1:HBM[1] 

LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_2.m_axi_gmem0:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_2.m_axi_gmem1:HBM[3]

LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_3.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_3.m_axi_gmem1:HBM[5]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[3] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[5]
LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[6]

#### (8 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem7:HBM[7]
LDCLFLAGS_HBM_PROC_1by1by1by0_8and1 += --sp topkernelP8_1.m_axi_gmem8:HBM[8]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[9]

#### (16 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_1.m_axi_gmem6:HBM[6]

LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_2.m_axi_gmem0:HBM[7] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_2.m_axi_gmem1:HBM[8] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_2.m_axi_gmem2:HBM[9] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_2.m_axi_gmem3:HBM[10]
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_2.m_axi_gmem4:HBM[11]
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_2.m_axi_gmem5:HBM[12]
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP6_2.m_axi_gmem6:HBM[13]

LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP4_1.m_axi_gmem0:HBM[14] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP4_1.m_axi_gmem1:HBM[15] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP4_1.m_axi_gmem2:HBM[16] 
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP4_1.m_axi_gmem3:HBM[17]
LDCLFLAGS_HBM_PROC_1by1by1by0_16and1 += --sp topkernelP4_1.m_axi_gmem4:HBM[18]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[6] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[13] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[18]
LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[19]

#### (20 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem7:HBM[7]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_1.m_axi_gmem8:HBM[8]

LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem0:HBM[9] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem1:HBM[10] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem2:HBM[11] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem3:HBM[12]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem4:HBM[13]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem5:HBM[14]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem6:HBM[15]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem7:HBM[16]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP8_2.m_axi_gmem8:HBM[17]

LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP4_1.m_axi_gmem0:HBM[18] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP4_1.m_axi_gmem1:HBM[19] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP4_1.m_axi_gmem2:HBM[20] 
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP4_1.m_axi_gmem3:HBM[21]
LDCLFLAGS_HBM_PROC_1by1by1by0_20and1 += --sp topkernelP4_1.m_axi_gmem4:HBM[22]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[17] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[22]
LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[23]

#### (22 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem7:HBM[7]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_1.m_axi_gmem8:HBM[8]

LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem0:HBM[9] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem1:HBM[10] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem2:HBM[11] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem3:HBM[12]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem4:HBM[13]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem5:HBM[14]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem6:HBM[15]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem7:HBM[16]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP8_2.m_axi_gmem8:HBM[17]

LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP6_1.m_axi_gmem0:HBM[18] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP6_1.m_axi_gmem1:HBM[19] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP6_1.m_axi_gmem2:HBM[20] 
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP6_1.m_axi_gmem3:HBM[21]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP6_1.m_axi_gmem4:HBM[22]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP6_1.m_axi_gmem5:HBM[23]
LDCLFLAGS_HBM_PROC_1by1by1by0_22and1 += --sp topkernelP6_1.m_axi_gmem6:HBM[24]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[17] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[24]
LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[25]

#### (24 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem7:HBM[7]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem8:HBM[8]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_1.m_axi_gmem9:HBM[9]

LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem0:HBM[10] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem1:HBM[11] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem2:HBM[12] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem3:HBM[13]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem4:HBM[14]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem5:HBM[15]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem6:HBM[16]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem7:HBM[17]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem8:HBM[18]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP9_2.m_axi_gmem9:HBM[19]

LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP6_1.m_axi_gmem0:HBM[20] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP6_1.m_axi_gmem1:HBM[21] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP6_1.m_axi_gmem2:HBM[22] 
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP6_1.m_axi_gmem3:HBM[23]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP6_1.m_axi_gmem4:HBM[24]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP6_1.m_axi_gmem5:HBM[25]
LDCLFLAGS_HBM_PROC_1by1by1by0_24and1 += --sp topkernelP6_1.m_axi_gmem6:HBM[26]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[9] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[19] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[26]
LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[27]

#### (25 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem7:HBM[7]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem8:HBM[8]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_1.m_axi_gmem9:HBM[9]

LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem0:HBM[10] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem1:HBM[11] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem2:HBM[12] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem3:HBM[13]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem4:HBM[14]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem5:HBM[15]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem6:HBM[16]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem7:HBM[17]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem8:HBM[18]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP9_2.m_axi_gmem9:HBM[19]

LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP7_1.m_axi_gmem0:HBM[20] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP7_1.m_axi_gmem1:HBM[21] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP7_1.m_axi_gmem2:HBM[22] 
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP7_1.m_axi_gmem3:HBM[23]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP7_1.m_axi_gmem4:HBM[24]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP7_1.m_axi_gmem5:HBM[25]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP7_1.m_axi_gmem6:HBM[26]
LDCLFLAGS_HBM_PROC_1by1by1by0_25and1 += --sp topkernelP7_1.m_axi_gmem7:HBM[27]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[9] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[19] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[27]
LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[28]

###############################################################################

#### (12 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem7:HBM[7] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem8:HBM[8] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem9:HBM[9] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_1.m_axi_gmem10:HBM[10]

LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem0:HBM[11] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem1:HBM[12] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem2:HBM[13] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem3:HBM[14]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem4:HBM[15]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem5:HBM[16]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem6:HBM[17]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem7:HBM[18] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem8:HBM[19] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem9:HBM[20] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP5_2.m_axi_gmem10:HBM[21]

LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP2_1.m_axi_gmem0:HBM[22] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP2_1.m_axi_gmem1:HBM[23] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP2_1.m_axi_gmem2:HBM[24] 
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP2_1.m_axi_gmem3:HBM[25]
LDCLFLAGS_HBM_PROC_1by1by1by1_12and1 += --sp topkernelP2_1.m_axi_gmem4:HBM[26]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[10] 
LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[21] 
LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[26]
LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[27]

#### (14 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem2:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem3:HBM[3]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem4:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem5:HBM[5]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem6:HBM[6]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem7:HBM[7] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem8:HBM[8] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem9:HBM[9] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem10:HBM[10]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem11:HBM[11]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_1.m_axi_gmem12:HBM[12]

LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem0:HBM[13] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem1:HBM[14] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem2:HBM[15] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem3:HBM[16]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem4:HBM[17]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem5:HBM[18]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem6:HBM[19]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem7:HBM[20] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem8:HBM[21] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem9:HBM[22] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem10:HBM[23]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem11:HBM[24]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP6_2.m_axi_gmem12:HBM[25]

LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP2_1.m_axi_gmem0:HBM[26] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP2_1.m_axi_gmem1:HBM[27] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP2_1.m_axi_gmem2:HBM[28] 
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP2_1.m_axi_gmem3:HBM[29]
LDCLFLAGS_HBM_PROC_1by1by1by1_14and1 += --sp topkernelP2_1.m_axi_gmem4:HBM[30]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1 += --sp topkernelsync_1.m_axi_gmem0:HBM[12] 
LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1 += --sp topkernelsync_1.m_axi_gmem1:HBM[25] 
LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1 += --sp topkernelsync_1.m_axi_gmem2:HBM[30]
LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1 += --sp topkernelsync_1.m_axi_gmem3:HBM[31]

################################### xclbins ###################################
EXECUTABLE = host
CMD_ARGS = $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin

EMCONFIG_DIR = $(XCLBIN)/$(DSA)

BINARY_CONTAINERS += $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelP1_OBJS += $(XCLBIN)/topkernelP1.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP2_OBJS += $(XCLBIN)/topkernelP2.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP4_OBJS += $(XCLBIN)/topkernelP4.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP5_OBJS += $(XCLBIN)/topkernelP5.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP6_OBJS += $(XCLBIN)/topkernelP6.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP7_OBJS += $(XCLBIN)/topkernelP7.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP8_OBJS += $(XCLBIN)/topkernelP8.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP9_OBJS += $(XCLBIN)/topkernelP9.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_PROC += $(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelproc_OBJS += $(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_SYNC += $(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelsync_OBJS += $(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_MERGE += $(XCLBIN)/topkernelmerge.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelmerge_OBJS += $(XCLBIN)/topkernelmerge.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_APPLY += $(XCLBIN)/topkernelapply.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_topkernelapply_OBJS += $(XCLBIN)/topkernelapply.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_1by1by1by0_1AND1 += $(XCLBIN)/topkernel_1by1by1by0_1and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_3AND1 += $(XCLBIN)/topkernel_1by1by1by0_3and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_8AND1 += $(XCLBIN)/topkernel_1by1by1by0_8and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_16AND1 += $(XCLBIN)/topkernel_1by1by1by0_16and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_20AND1 += $(XCLBIN)/topkernel_1by1by1by0_20and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_22AND1 += $(XCLBIN)/topkernel_1by1by1by0_22and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_24AND1 += $(XCLBIN)/topkernel_1by1by1by0_24and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_25AND1 += $(XCLBIN)/topkernel_1by1by1by0_25and1.$(TARGET).$(DSA).xclbin

BINARY_CONTAINERS_1by1by1by1_12AND1 += $(XCLBIN)/topkernel_1by1by1by1_12and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by1_14AND1 += $(XCLBIN)/topkernel_1by1by1by1_14and1.$(TARGET).$(DSA).xclbin

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

all_procandsync_1by1by1by0_1and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_1AND1) emconfig
# all_procandsync_1by1by1by0_3and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_3AND1) emconfig
all_procandsync_1by1by1by0_3and1: check-devices $(BINARY_CONTAINERS_1by1by1by0_3AND1) emconfig
all_procandsync_1by1by1by0_8and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_8AND1) emconfig
all_procandsync_1by1by1by0_16and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_16AND1) emconfig
all_procandsync_1by1by1by0_20and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_20AND1) emconfig
all_procandsync_1by1by1by0_22and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_22AND1) emconfig
all_procandsync_1by1by1by0_24and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_24AND1) emconfig
all_procandsync_1by1by1by0_25and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_25AND1) emconfig

all_procandsync_1by1by1by1_12and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by1_12AND1) emconfig
all_procandsync_1by1by1by1_14and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by1_14AND1) emconfig

.PHONY: exe
exe: $(EXECUTABLE)

.PHONY: build
build: $(BINARY_CONTAINERS)

# Building kernel (ACTS 1 by 1: all proc & sync in 1 kernel)
$(XCLBIN)/topkernelP1.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY) $(KERNEL_ACTS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP1 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_ACTS) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP2.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP2 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP4.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP4 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP5.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP5 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)	
$(XCLBIN)/topkernelP6.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP6 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP7.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP7 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP8.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP8 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP9.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP9 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernel_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS) --nk topkernel:1 -o'$@' $(+)
	
# Building kernel (proc)
$(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelproc) -c -k topkernelproc -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelproc.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelproc_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelproc) -l $(LDCLFLAGS_PROC) $(LDCLFLAGS_LOGICOPT) --nk topkernelproc:4 -o'$@' $(+)
	
# Building kernel (sync)
$(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xo: $(KERNEL_TOP_SYNC) $(KERNEL_UTILITY)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelsync) -c -k topkernelsync -I'$(<D)' -o'$@' $(KERNEL_TOP_SYNC)
$(XCLBIN)/topkernelsync.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelsync) -l $(LDCLFLAGS_SYNC) --nk topkernelsync:1 -o'$@' $(+)	
	
#CRITICAL REMOVEME ## (1by1by1byN: 1 PE Workers) ##
LDCLFLAGS_SLR += --slr topkernel_1:SLR0 --slr topkernel_2:SLR1 --slr topkernel_3:SLR2
$(XCLBIN)/topkernel_1by1by1by0_1and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernel_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_1by1by1by0_1and1) $(LDCLFLAGS_SLR) $(LDCLFLAGS_LOGICOPT) --nk topkernel:3 -o'$@' $(+)

## (1by1by1byN: 3 PE Workers) ##
# LDCLFLAGS_SLR_1BY1BY1BYN_3PEs += --slr topkernelP1_1:SLR2
# LDCLFLAGS_SLR_1BY1BY1BYN_3PEs += --slr topkernelP1_2:SLR1
# LDCLFLAGS_SLR_1BY1BY1BYN_3PEs += --slr topkernelP1_3:SLR0
# LDCLFLAGS_SLR_1BY1BY1BYN_3PEs += --slr topkernelsync_1:SLR0
# $(XCLBIN)/topkernel_1by1by1by0_3and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP1_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	# mkdir -p $(XCLBIN)
	# $(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_3and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_3PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP1:3 --nk topkernelsync:1 -o'$@' $(+)
	
## (1by1by1byN: 3 PE Workers) ## REMOVEMEEEEEEEEEEEEEEEEEEEEEEE
$(XCLBIN)/topkernel_1by1by1by0_3and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP1_OBJS)
	mkdir -p $(XCLBIN)
	# $(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_3and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_3PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP1:3 --nk topkernelsync:1 -o'$@' $(+)
	
## (1by1by1byN: 8 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_8PEs += --slr topkernelP8_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_8PEs += --slr topkernelsync_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_8and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP8_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_8and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_8PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP8:1 --nk topkernelsync:1 -o'$@' $(+)

## (1by1by1byN: 16 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_16PEs += --slr topkernelP6_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_16PEs += --slr topkernelP6_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_16PEs += --slr topkernelP4_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_16PEs += --slr topkernelsync_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_16and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP4_OBJS) $(BINARY_CONTAINER_topkernelP6_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_16and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_16PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP6:2 --nk topkernelP4:1 --nk topkernelsync:1 -o'$@' $(+)
		
## (1by1by1byN: 20 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_20PEs += --slr topkernelP8_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_20PEs += --slr topkernelP8_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_20PEs += --slr topkernelP4_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_20PEs += --slr topkernelsync_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_20and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP4_OBJS) $(BINARY_CONTAINER_topkernelP8_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_20and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_20PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP8:2 --nk topkernelP4:1 --nk topkernelsync:1 -o'$@' $(+)
	
## (1by1by1byN: 22 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_22PEs += --slr topkernelP8_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_22PEs += --slr topkernelP8_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_22PEs += --slr topkernelP6_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_22PEs += --slr topkernelsync_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_22and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP6_OBJS) $(BINARY_CONTAINER_topkernelP8_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_22and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_22PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP8:2 --nk topkernelP6:1 --nk topkernelsync:1 -o'$@' $(+)
	
## (1by1by1byN: 24 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_24PEs += --slr topkernelP9_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_24PEs += --slr topkernelP9_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_24PEs += --slr topkernelP6_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_24PEs += --slr topkernelsync_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_24and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP9_OBJS) $(BINARY_CONTAINER_topkernelP6_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_24and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_24PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP9:2 --nk topkernelP6:1 --nk topkernelsync:1 -o'$@' $(+)

## (1by1by1byN: 25 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_25PEs += --slr topkernelP9_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_25PEs += --slr topkernelP9_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_25PEs += --slr topkernelP7_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_25PEs += --slr topkernelsync_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_25and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP9_OBJS) $(BINARY_CONTAINER_topkernelP7_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_25and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_25PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP9:2 --nk topkernelP7:1 --nk topkernelsync:1 -o'$@' $(+)
		
###############################################################################

## (1by1by1byN: 12 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_12PEs += --slr topkernelP5_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_12PEs += --slr topkernelP5_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_12PEs += --slr topkernelP2_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_12PEs += --slr topkernelsync_1:SLR0
$(XCLBIN)/topkernel_1by1by1by1_12and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP5_OBJS) $(BINARY_CONTAINER_topkernelP2_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by1_12and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_12PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP5:2 --nk topkernelP2:1 --nk topkernelsync:1 -o'$@' $(+)
		
## (1by1by1byN: 14 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_14PEs += --slr topkernelP6_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_14PEs += --slr topkernelP6_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_14PEs += --slr topkernelP2_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_14PEs += --slr topkernelsync_1:SLR0
$(XCLBIN)/topkernel_1by1by1by1_14and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP6_OBJS) $(BINARY_CONTAINER_topkernelP2_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by1_14and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_14PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP6:2 --nk topkernelP2:1 --nk topkernelsync:1 -o'$@' $(+)
		
# Building Host (***choice between CREBTREE or AWS***)
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
	g++ $(HOST_TOP) $(HOST_SRCS) $(KERNEL_TOP_PROC) $(KERNEL_TOP_SYNC) $(RELREF)acts/actsutility/actsutility.cpp $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -o acts_nthreads
	
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
	# -$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt
	-$(RMDIR) src/*.ll _xocc_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: clean
	rm -rf sr acts_nthreads sr_nthreads
	-$(RMDIR) $(XCLBIN)
	-$(RMDIR) _x.*

