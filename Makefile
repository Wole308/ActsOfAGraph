# faketime -f "-1y" make all TARGET=sw_emu PLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# faketime -f "-1y" make run TARGET=sw_emu PLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# v++ -t sw_emu --platform /opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm --save-temps  -g -c -k topkernelP9 --temp_dir ./_x.sw_emu.xilinx_u280_xdma_201920_3  -I'/home/oj2zf/Documents/ActsOfAGraph/acts/acts' -o'_x.sw_emu.xilinx_u280_xdma_201920_3/vP0.xo' '/home/oj2zf/Documents/ActsOfAGraph/acts/acts/acts_all.cpp'			

# datasets 
# https://sparse.tamu.edu/DIMACS10/rgg_n_2_24_s0 (16,777,216 vertices, 265,114,400 edges, undirected)
# https://sparse.tamu.edu/DIMACS10/europe_osm (50,912,018 vertices, 108,109,320 edges, undirected)
# https://sparse.tamu.edu/GenBank/kmer_V2a (55,042,369 vertices, 117,217,600 edges, undirected)
# https://sparse.tamu.edu/DIMACS10/hugebubbles-00020 (21,198,119 vertices, 63,580,358 edges, undirected)
# https://sparse.tamu.edu/GenBank/kmer_U1a (67,716,231 vertices, 138,778,562 edges, undirected)

# $ xbutil reset --device 0000:65:00.1
# Performing 'HOT Reset' on 
  # -[0000:65:00.1]
# Are you sure you wish to proceed? [Y/n]: Y
# Successfully reset Device[0000:65:00.1]
# To figure out which xbutil I was using, I used 'which':
# $ which xbutil
# /opt/xilinx/xrt/bin/xbutil

############################## Help Section ##############################
.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
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
	$(ECHO) "  make run TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to run application in emulation."
	$(ECHO) ""
	$(ECHO) "  make build TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to build xclbin application."
	$(ECHO) ""
	$(ECHO) "  make host"
	$(ECHO) "      Command to build host application."
	$(ECHO) ""

########################## C++ Sources #######################
DEVICE := $(DEVICES)
XCLBIN := ./xclbin
SYNFREQUENCY := 300
DSA = xilinx_u280_xdma_201920_3

# RELREF = ../
KERNEL_TOP_ALL += $(RELREF)acts/acts/acts_all.cpp
KERNEL_TOP_DEBUG += $(RELREF)acts/acts/mydebug.cpp
KERNEL_TOP_UTILITY += $(RELREF)acts/actsutility/actsutility.cpp
HOST_SRCS_ACTS += $(RELREF)examples/hostprocess.cpp
HOST_SRCS_ACTS += $(RELREF)src/algorithm/algorithm.cpp
HOST_SRCS_ACTS += $(RELREF)examples/helperfunctions/loadgraph.cpp
HOST_SRCS_ACTS += $(RELREF)examples/helperfunctions/loadedges_random.cpp
HOST_SRCS_ACTS += $(RELREF)examples/helperfunctions/evalparams.cpp
HOST_SRCS_ACTS += $(RELREF)examples/app/app.cpp
HOST_SRCS_ACTS += $(RELREF)examples/test/test.cpp
HOST_SRCS_ACTS += $(RELREF)src/graphs/graph.cpp
HOST_SRCS_ACTS += $(RELREF)src/stats/stats.cpp
HOST_SRCS_ACTS += $(RELREF)src/utility/utility.cpp
HOST_SRCS_ACTS += $(RELREF)src/dataset/dataset.cpp
HOST_SRCS_ACTS += $(RELREF)kernels/swkernel.cpp
HOST_SRCS_ACTS += $(RELREF)kernels/goclkernel.cpp
HOST_SRCS_ACTS += $(RELREF)src/graphs/creategraphs.cpp
HOST_SRCS_ACTS += $(RELREF)src/graphs/createundirectedgraph.cpp

############################## Setting up Project Variables ##############################
# Points to top directory of Git repository
MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
COMMON_REPO ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%sys_opt/slr_assign/*}')
PWD = $(shell readlink -f .)
# XF_PROJ_ROOT = $(shell readlink -f $(COMMON_REPO))
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
CMD_ARGS = $(BUILD_DIR)/vmult_vadd.xclbin

include $(XF_PROJ_ROOT)/common/includes/opencl/opencl.mk
CXXFLAGS += $(opencl_CXXFLAGS) -Wall -O0 -g -std=c++1y
LDFLAGS += $(opencl_LDFLAGS)

########################## Checking if PLATFORM in whitelist #######################
PLATFORM_BLOCKLIST += _u25_ u30 zc vck samsung _u2_ x3522pv nodma 
############################## Setting up Host Variables ##############################
#Include Required Host Source Files
CXXFLAGS += -I$(XF_PROJ_ROOT)/common/includes/xcl2
# HOST_SRCS += $(XF_PROJ_ROOT)/common/includes/xcl2/xcl2.cpp ./src/host.cpp 
HOST_SRCS += $(XF_PROJ_ROOT)/common/includes/xcl2/xcl2.cpp $(HOST_SRCS_ACTS)
# Host compiler global settings
CXXFLAGS += -fmessage-length=0
LDFLAGS += -lrt -lstdc++ 

############################## Setting up Kernel Variables ##############################
# Kernel compiler global settings
VPP_FLAGS += -t $(TARGET) --platform $(PLATFORM) --save-temps 
ifneq ($(TARGET), hw)
	VPP_FLAGS += -g
endif

EXECUTABLE = ./slr_assign
EMCONFIG_DIR = $(TEMP_DIR)

############################## Declaring Binary Containers ##############################
# BINARY_CONTAINERS += $(BUILD_DIR)/vmult_vadd.xclbin

BINARY_CONTAINER_vmult_vadd_OBJS += $(TEMP_DIR)/vP0.xo
BINARY_CONTAINER_vmult_vadd_OBJS += $(TEMP_DIR)/vP1.xo
BINARY_CONTAINER_vmult_vadd_OBJS += $(TEMP_DIR)/vP2.xo
BINARY_CONTAINER_vmult_vadd_OBJS += $(TEMP_DIR)/vS.xo
BINARY_CONTAINER_vmult_vsingle_OBJS += $(TEMP_DIR)/vS.xo

# BINARY_CONTAINERS += $(XCLBIN)/topkernel_1by1by1by0_1and1.$(TARGET).$(DSA).xclbin
# VPP_LDFLAGS_vmult_vadd += --config connectivity_files/connectivity_1w.cfg
# KERNELS_NAME = TOPS_topkernelS

BINARY_CONTAINERS += $(XCLBIN)/topkernel_1by1by1by0_3and1.$(TARGET).$(DSA).xclbin
VPP_LDFLAGS_vmult_vadd += --config connectivity_files/connectivity_3w.cfg
KERNELP0_NAME = TOPP0_U_topkernelP1
KERNELP1_NAME = TOPP1_U_topkernelP1
KERNELP2_NAME = TOPP2_U_topkernelP1
KERNELS_NAME = TOPS_topkernelS

# BINARY_CONTAINERS += $(XCLBIN)/topkernel_1by1by1by0_16and1.$(TARGET).$(DSA).xclbin
# VPP_LDFLAGS_vmult_vadd += --config connectivity_files/connectivity_16w.cfg
# KERNELP0_NAME = TOPP0_U_topkernelP6
# KERNELP1_NAME = TOPP1_U_topkernelP6
# KERNELP2_NAME = TOPP2_U_topkernelP4
# KERNELS_NAME = TOPS_topkernelS

# BINARY_CONTAINERS += $(XCLBIN)/topkernel_1by1by1by0_18and1.$(TARGET).$(DSA).xclbin
# VPP_LDFLAGS_vmult_vadd += --config connectivity_files/connectivity_18w.cfg
# KERNELP0_NAME = TOPP0_U_topkernelP6
# KERNELP1_NAME = TOPP1_U_topkernelP6
# KERNELP2_NAME = TOPP2_U_topkernelP6
# KERNELS_NAME = TOPS_topkernelS

# BINARY_CONTAINERS += $(XCLBIN)/topkernel_1by1by1by0_20and1.$(TARGET).$(DSA).xclbin
# VPP_LDFLAGS_vmult_vadd += --config connectivity_files/connectivity_20w.cfg
# KERNELP0_NAME = TOPP0_U_topkernelP7
# KERNELP1_NAME = TOPP1_U_topkernelP7
# KERNELP2_NAME = TOPP2_U_topkernelP6
# KERNELS_NAME = TOPS_topkernelS

# BINARY_CONTAINERS += $(XCLBIN)/topkernel_1by1by1by0_22and1.$(TARGET).$(DSA).xclbin
# VPP_LDFLAGS_vmult_vadd += --config connectivity_files/connectivity_22w.cfg
# KERNELP0_NAME = TOPP0_U_topkernelP8
# KERNELP1_NAME = TOPP1_U_topkernelP8
# KERNELP2_NAME = TOPP2_U_topkernelP6
# KERNELS_NAME = TOPS_topkernelS

# BINARY_CONTAINERS += $(XCLBIN)/topkernel_1by1by1by0_24and1.$(TARGET).$(DSA).xclbin
# VPP_LDFLAGS_vmult_vadd += --config connectivity_files/connectivity_24w.cfg
# KERNELP0_NAME = TOPP0_U_topkernelP9
# KERNELP1_NAME = TOPP1_U_topkernelP9
# KERNELP2_NAME = TOPP2_U_topkernelP6
# KERNELS_NAME = TOPS_topkernelS

############################## Setting Targets ##############################
CP = cp -rf

.PHONY: all clean cleanall docs emconfig
# all: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
all: check-platform check-device $(BINARY_CONTAINERS) emconfig

all_procandsync_1by1by1by0_1and1: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
all_procandsync_1by1by1by0_3and1: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
all_procandsync_1by1by1by0_16and1: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
all_procandsync_1by1by1by0_18and1: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
all_procandsync_1by1by1by0_20and1: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
all_procandsync_1by1by1by0_22and1: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
all_procandsync_1by1by1by0_24and1: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
# all_procandsync_1by1by1by0_25and1: check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS_1by1by1by0_25AND1) emconfig

.PHONY: host
host: $(EXECUTABLE)

.PHONY: build
build: check-vitis check-device $(BINARY_CONTAINERS)

.PHONY: xclbin
xclbin: build

############################## Setting Rules for Binary Containers (Building Kernels) ##############################
$(TEMP_DIR)/vP0.xo: $(RELREF)acts/acts/acts_allP0.cpp
	mkdir -p $(TEMP_DIR)
	$(VPP) $(VPP_FLAGS) -c -k $(KERNELP0_NAME) --temp_dir $(TEMP_DIR)  -I'$(<D)' -o'$@' '$^'
$(TEMP_DIR)/vP1.xo: $(RELREF)acts/acts/acts_allP1.cpp
	mkdir -p $(TEMP_DIR)
	$(VPP) $(VPP_FLAGS) -c -k $(KERNELP1_NAME) --temp_dir $(TEMP_DIR)  -I'$(<D)' -o'$@' '$^'
$(TEMP_DIR)/vP2.xo: $(RELREF)acts/acts/acts_allP2.cpp
	mkdir -p $(TEMP_DIR)
	$(VPP) $(VPP_FLAGS) -c -k $(KERNELP2_NAME) --temp_dir $(TEMP_DIR)  -I'$(<D)' -o'$@' '$^'
$(TEMP_DIR)/vS.xo: $(RELREF)acts/acts/acts_allS.cpp
	mkdir -p $(TEMP_DIR)
	$(VPP) $(VPP_FLAGS) -c -k $(KERNELS_NAME) --temp_dir $(TEMP_DIR)  -I'$(<D)' -o'$@' '$^'
$(XCLBIN)/topkernel_1by1by1by0_1and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_vmult_vsingle_OBJS)
	mkdir -p $(BUILD_DIR)
	$(VPP) $(VPP_FLAGS) -l $(VPP_LDFLAGS) --temp_dir $(TEMP_DIR) $(VPP_LDFLAGS_vmult_vadd) -o'$(BUILD_DIR)/vmult_vadd.link.xclbin' $(+)
	$(VPP) -p $(BUILD_DIR)/vmult_vadd.link.xclbin -t $(TARGET) --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) -o $(XCLBIN)/topkernel_1by1by1by0_1and1.$(TARGET).$(DSA).xclbin
$(XCLBIN)/topkernel_1by1by1by0_3and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_vmult_vadd_OBJS)
	mkdir -p $(BUILD_DIR)
	$(VPP) $(VPP_FLAGS) -l $(VPP_LDFLAGS) --temp_dir $(TEMP_DIR) $(VPP_LDFLAGS_vmult_vadd) -o'$(BUILD_DIR)/vmult_vadd.link.xclbin' $(+)
	$(VPP) -p $(BUILD_DIR)/vmult_vadd.link.xclbin -t $(TARGET) --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) -o $(XCLBIN)/topkernel_1by1by1by0_3and1.$(TARGET).$(DSA).xclbin
$(XCLBIN)/topkernel_1by1by1by0_16and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_vmult_vadd_OBJS)
	mkdir -p $(BUILD_DIR)
	$(VPP) $(VPP_FLAGS) -l $(VPP_LDFLAGS) --temp_dir $(TEMP_DIR) $(VPP_LDFLAGS_vmult_vadd) -o'$(BUILD_DIR)/vmult_vadd.link.xclbin' $(+)
	$(VPP) -p $(BUILD_DIR)/vmult_vadd.link.xclbin -t $(TARGET) --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) -o $(XCLBIN)/topkernel_1by1by1by0_16and1.$(TARGET).$(DSA).xclbin
$(XCLBIN)/topkernel_1by1by1by0_22and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_vmult_vadd_OBJS)
	mkdir -p $(BUILD_DIR)
	$(VPP) $(VPP_FLAGS) -l $(VPP_LDFLAGS) --temp_dir $(TEMP_DIR) $(VPP_LDFLAGS_vmult_vadd) -o'$(BUILD_DIR)/vmult_vadd.link.xclbin' $(+)
	$(VPP) -p $(BUILD_DIR)/vmult_vadd.link.xclbin -t $(TARGET) --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) -o $(XCLBIN)/topkernel_1by1by1by0_22and1.$(TARGET).$(DSA).xclbin
$(XCLBIN)/topkernel_1by1by1by0_20and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_vmult_vadd_OBJS)
	mkdir -p $(BUILD_DIR)
	$(VPP) $(VPP_FLAGS) -l $(VPP_LDFLAGS) --temp_dir $(TEMP_DIR) $(VPP_LDFLAGS_vmult_vadd) -o'$(BUILD_DIR)/vmult_vadd.link.xclbin' $(+)
	$(VPP) -p $(BUILD_DIR)/vmult_vadd.link.xclbin -t $(TARGET) --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) -o $(XCLBIN)/topkernel_1by1by1by0_20and1.$(TARGET).$(DSA).xclbin
$(XCLBIN)/topkernel_1by1by1by0_24and1.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_vmult_vadd_OBJS)
	mkdir -p $(BUILD_DIR)
	$(VPP) $(VPP_FLAGS) -l $(VPP_LDFLAGS) --temp_dir $(TEMP_DIR) $(VPP_LDFLAGS_vmult_vadd) -o'$(BUILD_DIR)/vmult_vadd.link.xclbin' $(+)
	$(VPP) -p $(BUILD_DIR)/vmult_vadd.link.xclbin -t $(TARGET) --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) -o $(XCLBIN)/topkernel_1by1by1by0_24and1.$(TARGET).$(DSA).xclbin
	
############################## Setting Rules for Host (Building Host Executable) ##############################
# $(EXECUTABLE): $(HOST_SRCS) | check-xrt
		# $(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
$(EXECUTABLE): $(HOST_SRCS) | check-xrt
		$(CXX) -o $@ $^ $(CXXFLAGS) -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/./Vitis_HLS/2021.2/include/ -I/tools/Xilinx/Vitis/2021.2/runtime/ -I/tools/Xilinx/Vivado/2021.2/include/ -pthread -march=native -lrt xcl.c -o host -L/opt/Xilinx/SDx/2021.2/runtime/lib/x86_64 -lOpenCL -pthread -lrt $(LDFLAGS)			

emconfig:$(EMCONFIG_DIR)/emconfig.json
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(PLATFORM) --od $(EMCONFIG_DIR)

############################## Setting Essential Checks and Running Rules ##############################
run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	$(CP) $(EMCONFIG_DIR)/emconfig.json .
	XCL_EMULATION_MODE=$(TARGET) $(EXECUTABLE) $(CMD_ARGS)
else
	$(EXECUTABLE) $(CMD_ARGS)
endif

.PHONY: test
test: $(EXECUTABLE)
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=$(TARGET) $(EXECUTABLE) $(CMD_ARGS)
else
	$(EXECUTABLE) $(CMD_ARGS)
endif

############################## CPU Multithreaded Implementation ##############################
demo_acts_nthreads: clean build_acts_nthreads run_nthreads
demo_acts_nthreads_debug: clean build_acts_nthreads run_nthreads_debug

build_acts_nthreads:
	g++ $(HOST_TOP) $(HOST_SRCS_ACTS) $(KERNEL_TOP_ALL) $(KERNEL_TOP_DEBUG) $(KERNEL_TOP_UTILITY) $(GRAPH_CPP) $(SRFLAGS) -I$(SORTREDUCE_INCLUDE) -I$(GRAPH_SRC) -L$(SORTREDUCE_LIB) -std=c++11 -pthread -march=native -lrt -o acts_nthreads
	
run_nthreads:
	./acts_nthreads

run_nthreads_debug:
	gdb ./acts_nthreads
	
############################## Generating source files (Jinja 2) ##############################
generatesrcs:
	python gen.py $(XWARE) $(SETUP) $(ALGORITHM) $(DATASET) $(NUMSUPERCPUTHREADS) $(NUMCPUTHREADS) $(NUMSUBCPUTHREADS) $(NUMPARTITIONS) $(LOCKE) $(EVALUATION_TYPE) $(EVALUATION_PARAM0) $(NUM_PEs) $(TESTKERNEL) $(TESTKERNELARG) 

############################## Cleaning Rules ##############################
# Cleaning stuff
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	-$(RMDIR) src/*.ll *v++* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: clean
	-$(RMDIR) build_dir*
	-$(RMDIR) package.*
	-$(RMDIR) _x* *xclbin.run_summary qemu-memory-_* emulation _vimage pl* start_simulation.sh *.xclbin

