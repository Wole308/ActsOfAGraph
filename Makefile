# faketime -f "-1y" make all TARGET=hw PLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# faketime -f "-1y" make all TARGET=sw_emu PLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# faketime -f "-1y" make run TARGET=sw_emu PLATFORM=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# v++ -t sw_emu --platform /opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm --save-temps  -g -c -k topkernelP9 --temp_dir ./_x.sw_emu.xilinx_u280_xdma_201920_3  -I'/home/oj2zf/Documents/acts-billion-nodes/acts/acts' -o'_x.sw_emu.xilinx_u280_xdma_201920_3/vP0.xo' '/home/oj2zf/Documents/acts-billion-nodes/acts/acts/acts_all.cpp'			

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

# make run TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
# make build TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm

# FPGA specs: 142, 4330 (total num BAMs in FPGA board), 1440 (total num BAMs in SLR region)

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
KERNEL_TOP_ALL += $(RELREF)acts_templates/acts_kernel.cpp
# KERNEL_TOP_ALL += $(RELREF)acts_templates/acts_kernel___debugging.cpp
HOST_SRCS_ACTS += $(RELREF)host_srcs/hostprocess.cpp
# HOST_SRCS_ACTS += $(RELREF)host_srcs/hostprocess___allinone.cpp
HOST_SRCS_ACTS += $(RELREF)host_srcs/prepare_graph.cpp
HOST_SRCS_ACTS += $(RELREF)host_srcs/act_pack.cpp
HOST_SRCS_ACTS += $(RELREF)host_srcs/app.cpp
HOST_SRCS_ACTS += $(RELREF)host_srcs/utility.cpp
HOST_SRCS_ACTS += $(RELREF)host_srcs/algorithm.cpp
HOST_SRCS_ACTS += $(RELREF)host_srcs/app_hw.cpp

############################## Setting up Project Variables ##############################
# Points to top directory of Git repository
MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
COMMON_REPO ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%sys_opt/slr_assign/*}')
PWD = $(shell readlink -f .)
# XF_PROJ_ROOT = $(shell readlink -f $(COMMON_REPO))
XF_PROJ_ROOT = /home/oj2zf/Documents/Vitis_Accel_Examples-master

# TARGET := sw_emu
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
# VPP_FLAGS += -t $(TARGET) --platform $(PLATFORM) --save-temps -g
# VPP_LDFLAGS += --profile.data all:all:all
# VPP_LDFLAGS += --profile.memory=all 

EXECUTABLE = ./slr_assign
EMCONFIG_DIR = $(TEMP_DIR)

CONNECTIVITY_FILE = connectivity_1w.cfg
# CONNECTIVITY_FILE = connectivity_6w.cfg
# CONNECTIVITY_FILE = connectivity_12w.cfg

############################## Declaring Binary Containers ##############################
BINARY_CONTAINER_vmult_vadd_OBJS += $(TEMP_DIR)/kernel.xo
BINARY_CONTAINERS += $(XCLBIN)/acts.$(TARGET).$(DSA).xclbin
VPP_LDFLAGS_vmult_vadd += --config $(RELREF)connectivity_files/$(CONNECTIVITY_FILE)
KERNEL_NAME = top_function

############################## Setting Targets ##############################
CP = cp -rf

.PHONY: all clean cleanall docs emconfig
hw: clean cleanall check-platform check-device $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig

.PHONY: host
host: $(EXECUTABLE)

.PHONY: build
build: check-vitis check-device $(BINARY_CONTAINERS)

.PHONY: xclbin
xclbin: build

############################## Setting Rules for Binary Containers (Building Kernels) ##############################

$(TEMP_DIR)/kernel.xo: $(KERNEL_TOP_ALL)
	mkdir -p $(TEMP_DIR)
	$(VPP) $(VPP_FLAGS) -c -k $(KERNEL_NAME) --temp_dir $(TEMP_DIR)  -I'$(<D)' -o'$@' '$^'
$(XCLBIN)/acts.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_vmult_vadd_OBJS)
	mkdir -p $(BUILD_DIR)
	$(VPP) $(VPP_FLAGS) -l $(VPP_LDFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(TEMP_DIR) $(VPP_LDFLAGS_vmult_vadd) -o'$(BUILD_DIR)/vmult_vadd.link.xclbin' $(+)
	$(VPP) -p $(BUILD_DIR)/vmult_vadd.link.xclbin -t $(TARGET) --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) -o $(XCLBIN)/acts.$(TARGET).$(DSA).xclbin	

############################## Setting Rules for Host (Building Host Executable) ##############################
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
actsobj: clean build_acts 
actsGP_debug: clean build_acts run_nthreads_debug

build_acts:
	g++ $(HOST_TOP) $(HOST_SRCS_ACTS) $(KERNEL_TOP_ALL) -std=c++11 -pthread -march=native -lrt -o host
		
run_nthreads:
	./actsobj

run_nthreads_debug:
	gdb ./actsobj
	
############################## Generating source files (Jinja 2) ##############################
generatesrcs:
	python gen.py $(XWARE) $(ALGORITHM) $(EVALUATION_TYPE) $(NUM_PEs) $(TESTKERNEL)

############################## Cleaning Rules ##############################
# Cleaning stuff
clean:
	rm -rf host
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	-$(RMDIR) src/*.ll *v++* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: clean
	-$(RMDIR) build_dir*
	-$(RMDIR) package.*
	-$(RMDIR) _x* *xclbin.run_summary qemu-memory-_* emulation _vimage pl* start_simulation.sh *.xclbin

