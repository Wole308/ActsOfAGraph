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