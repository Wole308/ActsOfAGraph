xilinx19sdx
XRT
./evaluate.sh

/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 

make all TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
make all TARGET=hw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
XCL_EMULATION_MODE=sw_emu ./host xclbin/krnl_vaddmul.sw_emu.xilinx_u280_xdma_201910_1.xclbin

nohup make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
./host xclbin/krnl_vaddmul.sw_emu.xilinx_u280_xdma_201910_1.xclbin

/tools/Xilinx/SDx/2019.1/bin/xcpp -I../../..//libs/xcl2 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/Vivado/2019.1/include/ -Wall -O0 -g -std=c++14 -fmessage-length=0 ../../..//libs/xcl2/xcl2.cpp src/host.cpp  -o 'host'  -L/opt/xilinx/xrt/lib/ -lOpenCL -lpthread -lrt -lstdc++ 

source /tools/Xilinx/Vivado/2020.1/settings64.sh
source /tools/Xilinx/Vitis/2020.1/settings64.sh
tools/Xilinx/Vitis/2020.1/samples/vadd
/tools/Xilinx/Vitis/2020.1/bin/v++ -t sw_emu --platform xilinx_u280_xdma_201920_3 -c -k krnl_vadd -I'./src' -o'vadd.sw_emu.xo' ./src/krnl_vadd.cpp
switching from SDx to Vitis: https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/buildingdevicebinary.html#zab1528399121187

vi ~/.bashrc

Next Paper: Disk, Insider

vitis tweaking flow URL (timing versus resources constraint), avoiding congestion: 
https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/buildingdevicebinary.html#zab1528399121187
https://forums.xilinx.com/t5/Implementation/run-congestion-command/td-p/910438
--xp (sdaccel command line utility webpage)
https://www.xilinx.com/html_docs/xilinx2019_1/sdaccel_doc/wrj1504034328013.html#uno1528577884586
https://www.xilinx.com/support/answers/66314.html
https://forums.xilinx.com/t5/Implementation/implementation-error-due-to-routing-congestion/td-p/474578
https://forums.xilinx.com/t5/Alveo-Accelerator-Cards/How-to-use-vivado-prop/td-p/1089904
https://forums.xilinx.com/t5/Implementation/What-does-Spread-Logic-mean/td-p/673141
https://www.xilinx.com/developer/articles/using-configuration-files-to-control-vitis-compilation.html
https://forums.xilinx.com/t5/Vivado-TCL-Community/how-to-get-the-list-of-all-possible-strategies-with-tcl-commands/td-p/913712 *
https://forums.xilinx.com/t5/Vitis-Acceleration-SDAccel-SDSoC/How-to-set-route-directives-in-Vitis-2019-2/td-p/1221096 *
https://forums.xilinx.com/t5/Vitis-Acceleration-SDAccel-SDSoC/Vivado-implementation-strategies-in-Vitis/td-p/1046022 *


XOCC := $(XILINX_VITIS)/bin/v++
CLFLAGS += -t $(TARGET) --platform $(DEVICE) --save-temps
TARGETS := hw
TARGET := $(TARGETS)
DEVICEPATH=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelproc) -l $(LDCLFLAGS_PROC) --nk topkernelproc:4 -o'$@' $(+)

$(XILINX_VITIS)/bin/v++ -t hw --platform $(DEVICE) --save-temps --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelproc) -l $(LDCLFLAGS_PROC) --nk topkernelproc:4 -o'$@' $(+)

/tools/Xilinx/Vitis/2020.1/bin/v++ -t hw --platform /opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm --save-temps --kernel_frequency 300 --temp_dir ./_x.hw.xilinx_u280_xdma_201920_3/topkernelproc -c -k topkernelproc -I'acts/acts' -I'acts/actsutility/' -o'xclbin/topkernelproc.hw.xilinx_u280_xdma_201920_3.xo' acts/acts/actsproc.cpp acts/actsutility/actsutility.cpp			
/tools/Xilinx/Vitis/2020.1/bin/v++ -t hw --platform /opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm --save-temps --kernel_frequency 300 --temp_dir ./_x.hw.xilinx_u280_xdma_201920_3/topkernelproc -l --sp topkernelproc_1.m_axi_gmem0:HBM[0]  --sp topkernelproc_2.m_axi_gmem0:HBM[1]  --sp topkernelproc_3.m_axi_gmem0:HBM[2]  --sp topkernelproc_4.m_axi_gmem0:HBM[3] --nk topkernelproc:4 -o'xclbin/topkernelproc.hw.xilinx_u280_xdma_201920_3.xclbin' xclbin/topkernelproc.hw.xilinx_u280_xdma_201920_3.xo		
v++ -t sw_emu --platform xilinx_u200_xdma_201830_2 -c -k vadd -I'./src' -o'vadd.sw_emu.xo' ./src/vadd.cpp