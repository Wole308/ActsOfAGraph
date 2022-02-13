#
# Copyright 2019-2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# makefile-generator v1.0.3
#

# make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# make all PLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm

############################## Help Section ##############################
.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> HOST_ARCH=<aarch32/aarch64/x86> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to generate the design for specified Target and Shell."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) "  make test PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to run the application. This is same as 'run' target but does not have any makefile dependency."
	$(ECHO) ""
	$(ECHO) "  make sd_card TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> HOST_ARCH=<aarch32/aarch64/x86> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to prepare sd_card files."
	$(ECHO) ""
	$(ECHO) "  make run TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> HOST_ARCH=<aarch32/aarch64/x86> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to run application in emulation."
	$(ECHO) ""
	$(ECHO) "  make build TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> HOST_ARCH=<aarch32/aarch64/x86> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to build xclbin application."
	$(ECHO) ""
	$(ECHO) "  make host HOST_ARCH=<aarch32/aarch64/x86> EDGE_COMMON_SW=<rootfs and kernel image path>"
	$(ECHO) "      Command to build host application."
	$(ECHO) "  By default, HOST_ARCH=x86. HOST_ARCH and EDGE_COMMON_SW is required for SoC shells"
	$(ECHO) ""
	
##############################################################################################################################################################
########################## C++ Sources #######################
KERNEL_TOP_PROC += $(RELREF)acts/acts/acts_all.cpp
KERNEL_TOP_DEBUG += $(RELREF)acts/acts/mydebug.cpp
KERNEL_TOP_SYNC = $(KERNEL_TOP_PROC)

HOST_SRCS += $(RELREF)examples/hostprocess.cpp
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

########################## HBM Memory Assignments #######################
#### (1 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_1and1 += --sp topkernel_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_1and1 += --sp topkernel_1.m_axi_gmem1:HBM[1] 

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_1and1 += --sp topkernelS_1.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_1and1 += --sp topkernelS_1.m_axi_gmem1:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_1and1 += --sp topkernelS_1.m_axi_gmem2:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_1and1 += --sp topkernelS_1.m_axi_gmem3:HBM[2]

#### (3 PE Workers: 1by1by1byN) ####
# proc 
LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_1.m_axi_gmem0:HBM[0] 
LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_1.m_axi_gmem1:HBM[1] 

LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_2.m_axi_gmem0:HBM[2] 
LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_2.m_axi_gmem1:HBM[3]

LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_3.m_axi_gmem0:HBM[4]
LDCLFLAGS_HBM_PROC_1by1by1by0_3and1 += --sp topkernelP1_3.m_axi_gmem1:HBM[5]

# sync
LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1 += --sp topkernelS_1.m_axi_gmem0:HBM[1] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1 += --sp topkernelS_1.m_axi_gmem1:HBM[3] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1 += --sp topkernelS_1.m_axi_gmem2:HBM[5]
LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1 += --sp topkernelS_1.m_axi_gmem3:HBM[6]

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
LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1 += --sp topkernelS_1.m_axi_gmem0:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1 += --sp topkernelS_1.m_axi_gmem1:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1 += --sp topkernelS_1.m_axi_gmem2:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1 += --sp topkernelS_1.m_axi_gmem3:HBM[9]

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
LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1 += --sp topkernelS_1.m_axi_gmem0:HBM[6] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1 += --sp topkernelS_1.m_axi_gmem1:HBM[13] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1 += --sp topkernelS_1.m_axi_gmem2:HBM[18]
LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1 += --sp topkernelS_1.m_axi_gmem3:HBM[19]

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
LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1 += --sp topkernelS_1.m_axi_gmem0:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1 += --sp topkernelS_1.m_axi_gmem1:HBM[17] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1 += --sp topkernelS_1.m_axi_gmem2:HBM[22]
LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1 += --sp topkernelS_1.m_axi_gmem3:HBM[23]

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
LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1 += --sp topkernelS_1.m_axi_gmem0:HBM[8] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1 += --sp topkernelS_1.m_axi_gmem1:HBM[17] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1 += --sp topkernelS_1.m_axi_gmem2:HBM[24]
LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1 += --sp topkernelS_1.m_axi_gmem3:HBM[25]

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
LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1 += --sp topkernelS_1.m_axi_gmem0:HBM[9] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1 += --sp topkernelS_1.m_axi_gmem1:HBM[19] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1 += --sp topkernelS_1.m_axi_gmem2:HBM[26]
LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1 += --sp topkernelS_1.m_axi_gmem3:HBM[27]

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
LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1 += --sp topkernelS_1.m_axi_gmem0:HBM[9] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1 += --sp topkernelS_1.m_axi_gmem1:HBM[19] 
LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1 += --sp topkernelS_1.m_axi_gmem2:HBM[27]
LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1 += --sp topkernelS_1.m_axi_gmem3:HBM[28]

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
LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1 += --sp topkernelS_1.m_axi_gmem0:HBM[10] 
LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1 += --sp topkernelS_1.m_axi_gmem1:HBM[21] 
LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1 += --sp topkernelS_1.m_axi_gmem2:HBM[26]
LDCLFLAGS_HBM_SYNC_1by1by1by1_12and1 += --sp topkernelS_1.m_axi_gmem3:HBM[27]

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
LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1 += --sp topkernelS_1.m_axi_gmem0:HBM[12] 
LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1 += --sp topkernelS_1.m_axi_gmem1:HBM[25] 
LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1 += --sp topkernelS_1.m_axi_gmem2:HBM[30]
LDCLFLAGS_HBM_SYNC_1by1by1by1_14and1 += --sp topkernelS_1.m_axi_gmem3:HBM[31]
##############################################################################################################################################################

############################## Setting up Project Variables ##############################
# Points to top directory of Git repository
MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
COMMON_REPO ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%rtl_kernels/rtl_vadd/*}')
PWD = $(shell readlink -f .)
# XF_PROJ_ROOT = $(shell readlink -f $(COMMON_REPO))
# XF_PROJ_ROOT = /home/oj2zf/Documents/ActsOfAGraph/Makefile/common/includes/opencl/opencl.mk:
XF_PROJ_ROOT = /home/oj2zf/Documents/Vitis_Accel_Examples-master

TARGET := hw
HOST_ARCH := x86
SYSROOT := 

include ./utils.mk

XSA := 
ifneq ($(PLATFORM), )
XSA := $(call device2xsa, $(PLATFORM))
endif
TEMP_DIR := ./_x.$(TARGET).$(XSA)
BUILD_DIR := ./build_dir.$(TARGET).$(XSA)

# SoC variables
RUN_APP_SCRIPT = ./run_app.sh
PACKAGE_OUT = ./package.$(TARGET)

LAUNCH_EMULATOR = $(PACKAGE_OUT)/launch_$(TARGET).sh
RESULT_STRING = TEST PASSED

VPP := v++
VPP_PFLAGS := 
CMD_ARGS = $(BUILD_DIR)/vadd.xclbin
SDCARD := sd_card

include $(XF_PROJ_ROOT)/common/includes/opencl/opencl.mk
include config.mk

CXXFLAGS += $(opencl_CXXFLAGS) -Wall -O0 -g -std=c++1y
LDFLAGS += $(opencl_LDFLAGS)

########################## Checking if PLATFORM in whitelist #######################
PLATFORM_BLOCKLIST += nodma 

############################## Setting up Host Variables ##############################
#Include Required Host Source Files
CXXFLAGS += -I$(XF_PROJ_ROOT)/common/includes/xcl2
HOST_SRCS += $(XF_PROJ_ROOT)/common/includes/xcl2/xcl2.cpp
# HOST_SRCS += $(XF_PROJ_ROOT)/common/includes/xcl2/xcl2.cpp /home/oj2zf/Documents/Vitis_Accel_Examples-master/rtl_kernels/rtl_vadd/src/host.cpp 

# Host compiler global settings
CXXFLAGS += -fmessage-length=0
LDFLAGS += -lrt -lstdc++ 

ifneq ($(HOST_ARCH), x86)
	LDFLAGS += --sysroot=$(SYSROOT)
endif

############################## Setting up Kernel Variables ##############################
# Kernel compiler global settings
VPP_FLAGS += -t $(TARGET) --platform $(PLATFORM) --save-temps 
ifneq ($(TARGET), hw)
	VPP_FLAGS += -g
endif

EXECUTABLE = host
EMCONFIG_DIR = $(TEMP_DIR)
EMU_DIR = $(SDCARD)/data/emulation

############################## Setting Targets ##############################
CP = cp -rf

.PHONY: all clean cleanall docs emconfig
all: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig sd_card

.PHONY: host
host: $(EXECUTABLE)

.PHONY: build
build: check-vitis check-device $(BINARY_CONTAINERS)

.PHONY: xclbin
xclbin: build

############################## Setting Rules for Host (Building Host Executable) ##############################
# $(EXECUTABLE): $(HOST_SRCS) | check-xrt
		# $(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
# $(EXECUTABLE): $(HOST_SRCS) | check-xrt
		# $(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
# $(EXECUTABLE): $(HOST_SRCS) | check-xrt
		# $(CXX) -o $@ $^ $(CXXFLAGS) -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/SDx/2019.1/runtime/ -I/tools/Xilinx/Vivado/2019.1/include/ -std=c++0x $(CXXFLAGS) -pthread -march=native -lrt ./xcl.c -o host -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt $(LDFLAGS)
$(EXECUTABLE): $(HOST_SRCS) | check-xrt
		$(CXX) -o $@ $^ $(CXXFLAGS) -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/./Vitis_HLS/2021.2/include/ -I/tools/Xilinx/Vitis/2021.2/runtime/ -I/tools/Xilinx/Vivado/2021.2/include/ -std=c++0x $(CXXFLAGS) -pthread -march=native -lrt ./xcl.c -o host -L/opt/Xilinx/SDx/2021.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt $(LDFLAGS)

emconfig:$(EMCONFIG_DIR)/emconfig.json
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(PLATFORM) --od $(EMCONFIG_DIR)

############################## Setting Essential Checks and Running Rules ##############################
run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
ifeq ($(HOST_ARCH), x86)
	$(CP) $(EMCONFIG_DIR)/emconfig.json .
	XCL_EMULATION_MODE=$(TARGET) $(EXECUTABLE) $(CMD_ARGS)
else
	$(LAUNCH_EMULATOR) -run-app $(RUN_APP_SCRIPT) | tee run_app.log; exit $${PIPESTATUS[0]}
endif
else
ifeq ($(HOST_ARCH), x86)
	$(EXECUTABLE) $(CMD_ARGS)
endif
endif

.PHONY: test
test: $(EXECUTABLE)
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
ifeq ($(HOST_ARCH), x86)
	XCL_EMULATION_MODE=$(TARGET) $(EXECUTABLE) $(CMD_ARGS)
else
	$(LAUNCH_EMULATOR) -run-app $(RUN_APP_SCRIPT) | tee run_app.log; exit $${PIPESTATUS[0]}
endif
else
ifeq ($(HOST_ARCH), x86)
	$(EXECUTABLE) $(CMD_ARGS)
else
	$(ECHO) "Please copy the content of sd_card folder and data to an SD Card and run on the board"
endif
endif

############################## Preparing sdcard ##############################
sd_card: $(BINARY_CONTAINERS) $(EXECUTABLE) gen_run_app
ifneq ($(HOST_ARCH), x86)
	$(VPP) $(VPP_PFLAGS) -p $(BUILD_DIR)/vadd.xclbin -t $(TARGET) --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) --package.rootfs $(EDGE_COMMON_SW)/rootfs.ext4 --package.sd_file $(SD_IMAGE_FILE) --package.sd_file xrt.ini --package.sd_file $(RUN_APP_SCRIPT) --package.sd_file $(EXECUTABLE) -o vadd.xclbin
endif

# Building the kernel
##############################################################################################################################################################
# Building primitive kernels
$(XCLBIN)/topkernelP1.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP1 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP2.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP2 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP4.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP4 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP5.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP5 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)	
$(XCLBIN)/topkernelP6.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP6 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP7.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP7 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP8.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP8 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernelP9.$(TARGET).$(DSA).xo: $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -c -k topkernelP9 -I'$(<D)' -o'$@' $(KERNEL_TOP_PROC) $(KERNEL_UTILITY)
$(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernel_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS) --nk topkernel:1 -o'$@' $(+)
	
# Building kernel (sync)
$(XCLBIN)/topkernelS.$(TARGET).$(DSA).xo: $(KERNEL_TOP_SYNC) $(KERNEL_UTILITY)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelsync) -c -k topkernelS -I'$(<D)' -o'$@' $(KERNEL_TOP_SYNC)
$(XCLBIN)/topkernelS.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelsync) -l $(LDCLFLAGS_SYNC) --nk topkernelS:1 -o'$@' $(+)	
	
#CRITICAL REMOVEME ## (1by1by1byN: 1 PE Workers) ##
LDCLFLAGS_SLR += --slr topkernel_1:SLR0 --slr topkernel_2:SLR1 --slr topkernel_3:SLR2
$(XCLBIN)/topkernel_1by1by1by0_1and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernel_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_1by1by1by0_1and1) $(LDCLFLAGS_SLR) $(LDCLFLAGS_LOGICOPT) --nk topkernel:3 -o'$@' $(+)

## (1by1by1byN: 3 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_3PEs += --slr topkernelP1_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_3PEs += --slr topkernelP1_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_3PEs += --slr topkernelP1_3:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_3PEs += --slr topkernelS_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_3and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP1_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_3and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_3and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_3PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP1:3 --nk topkernelS:1 -o'$@' $(+)
	
## (1by1by1byN: 8 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_8PEs += --slr topkernelP8_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_8PEs += --slr topkernelS_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_8and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP8_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_8and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_8and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_8PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP8:1 --nk topkernelS:1 -o'$@' $(+)

## (1by1by1byN: 16 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_16PEs += --slr topkernelP6_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_16PEs += --slr topkernelP6_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_16PEs += --slr topkernelP4_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_16PEs += --slr topkernelS_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_16and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP4_OBJS) $(BINARY_CONTAINER_topkernelP6_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_16and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_16and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_16PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP6:2 --nk topkernelP4:1 --nk topkernelS:1 -o'$@' $(+)
		
## (1by1by1byN: 20 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_20PEs += --slr topkernelP8_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_20PEs += --slr topkernelP8_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_20PEs += --slr topkernelP4_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_20PEs += --slr topkernelS_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_20and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP4_OBJS) $(BINARY_CONTAINER_topkernelP8_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_20and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_20and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_20PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP8:2 --nk topkernelP4:1 --nk topkernelS:1 -o'$@' $(+)
	
## (1by1by1byN: 22 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_22PEs += --slr topkernelP8_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_22PEs += --slr topkernelP8_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_22PEs += --slr topkernelP6_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_22PEs += --slr topkernelS_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_22and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP6_OBJS) $(BINARY_CONTAINER_topkernelP8_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_22and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_22and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_22PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP8:2 --nk topkernelP6:1 --nk topkernelS:1 -o'$@' $(+)
	
## (1by1by1byN: 24 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_24PEs += --slr topkernelP9_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_24PEs += --slr topkernelP9_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_24PEs += --slr topkernelP6_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_24PEs += --slr topkernelS_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_24and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP9_OBJS) $(BINARY_CONTAINER_topkernelP6_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_24and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_24and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_24PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP9:2 --nk topkernelP6:1 --nk topkernelS:1 -o'$@' $(+)

## (1by1by1byN: 25 PE Workers) ##
LDCLFLAGS_SLR_1BY1BY1BYN_25PEs += --slr topkernelP9_1:SLR2
LDCLFLAGS_SLR_1BY1BY1BYN_25PEs += --slr topkernelP9_2:SLR1
LDCLFLAGS_SLR_1BY1BY1BYN_25PEs += --slr topkernelP7_1:SLR0
LDCLFLAGS_SLR_1BY1BY1BYN_25PEs += --slr topkernelS_1:SLR0
$(XCLBIN)/topkernel_1by1by1by0_25and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_topkernelP9_OBJS) $(BINARY_CONTAINER_topkernelP7_OBJS) $(BINARY_CONTAINER_topkernelsync_OBJS)
	mkdir -p $(BUILD_DIR)
	$(XOCC) $(VPP_FLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernel) -l $(LDCLFLAGS_HBM_PROC_1by1by1by0_25and1) $(LDCLFLAGS_HBM_SYNC_1by1by1by0_25and1) $(LDCLFLAGS_SLR_1BY1BY1BYN_25PEs) $(LDCLFLAGS_LOGICOPT) --nk topkernelP9:2 --nk topkernelP7:1 --nk topkernelS:1 -o'$@' $(+)
		
################################### xos and xclbins ###################################
CMD_ARGS = $(XCLBIN)/topkernel.$(TARGET).$(DSA).xclbin

EMCONFIG_DIR = $(XCLBIN)/$(DSA)

BINARY_CONTAINER_topkernelP1_OBJS += $(XCLBIN)/topkernelP1.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP2_OBJS += $(XCLBIN)/topkernelP2.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP4_OBJS += $(XCLBIN)/topkernelP4.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP5_OBJS += $(XCLBIN)/topkernelP5.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP6_OBJS += $(XCLBIN)/topkernelP6.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP7_OBJS += $(XCLBIN)/topkernelP7.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP8_OBJS += $(XCLBIN)/topkernelP8.$(TARGET).$(DSA).xo
BINARY_CONTAINER_topkernelP9_OBJS += $(XCLBIN)/topkernelP9.$(TARGET).$(DSA).xo

BINARY_CONTAINERS_1by1by1by0_1AND1 += $(XCLBIN)/topkernel_1by1by1by0_1and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_3AND1 += $(XCLBIN)/topkernel_1by1by1by0_3and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_8AND1 += $(XCLBIN)/topkernel_1by1by1by0_8and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_16AND1 += $(XCLBIN)/topkernel_1by1by1by0_16and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_20AND1 += $(XCLBIN)/topkernel_1by1by1by0_20and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_22AND1 += $(XCLBIN)/topkernel_1by1by1by0_22and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_24AND1 += $(XCLBIN)/topkernel_1by1by1by0_24and1.$(TARGET).$(DSA).xclbin
BINARY_CONTAINERS_1by1by1by0_25AND1 += $(XCLBIN)/topkernel_1by1by1by0_25and1.$(TARGET).$(DSA).xclbin

BINARY_CONTAINERS += BINARY_CONTAINERS_1by1by1by0_24AND1

CP = cp -rf

################################### make commands ###################################
.PHONY: all clean cleanall docs emconfig
# all_procandsync_1by1by1by0_1and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_1AND1) emconfig
all_procandsync_1by1by1by0_1and1: check-devices $(BINARY_CONTAINERS_1by1by1by0_1AND1) emconfig
# all_procandsync_1by1by1by0_3and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_3AND1) emconfig
all_procandsync_1by1by1by0_3and1: check-devices $(BINARY_CONTAINERS_1by1by1by0_3AND1) emconfig
all_procandsync_1by1by1by0_8and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_8AND1) emconfig
all_procandsync_1by1by1by0_16and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_16AND1) emconfig
all_procandsync_1by1by1by0_20and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_20AND1) emconfig
all_procandsync_1by1by1by0_22and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_22AND1) emconfig
# all_procandsync_1by1by1by0_24and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_24AND1) emconfig
all_procandsync_1by1by1by0_24and1: check-devices $(BINARY_CONTAINERS_1by1by1by0_24AND1) emconfig
all_procandsync_1by1by1by0_25and1: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_25AND1) emconfig

.PHONY: exe
exe: $(EXECUTABLE)

.PHONY: build
build: $(BINARY_CONTAINERS)
##############################################################################################################################################################

############################## Generating source files (Jinja 2) ##############################
generatesrcs:
	python gen.py $(XWARE) $(SETUP) $(ALGORITHM) $(DATASET) $(NUMSUPERCPUTHREADS) $(NUMCPUTHREADS) $(NUMSUBCPUTHREADS) $(NUMPARTITIONS) $(LOCKE) $(EVALUATION_TYPE) $(EVALUATION_PARAM0) $(NUM_PEs)

############################## Cleaning Rules ##############################
# Cleaning stuff
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	-$(RMDIR) src/*.ll *v++* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: clean
	-$(RMDIR) build_dir* sd_card*
	-$(RMDIR) package.*
	-$(RMDIR) _x* *xclbin.run_summary qemu-memory-_* emulation _vimage pl* start_simulation.sh *.xclbin
	-$(RMDIR) ./tmp_kernel_pack* ./packaged_kernel* 
