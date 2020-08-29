HBM Simple Vector Addition
======================

This is a simple example of vector addition to describe how to use HLS kernels with HBM (High Bandwidth Memory) for achieving high throughput.

***KEY CONCEPTS:*** High Bandwidth Memory, Multiple HBM Banks

***KEYWORDS:*** HBM, XCL_MEM_TOPOLOGY, cl_mem_ext_ptr_t

## SUPPORTED PLATFORMS
Platform | Board             | Software Version
---------|-------------------|-----------------
xilinx_u50_xdma|Xilinx Alveo U50|SDx 2019.1
xilinx_u280_xdma|Xilinx Alveo U280|SDx 2019.1


##  DESIGN FILES
Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

```
src/host.cpp
src/krnl_vadd.cpp
```

##  COMMAND LINE ARGUMENTS
Once the environment has been configured, the application can be executed by
```
./host <krnl_vadd XCLBIN>
```


/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 

make all TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
make all TARGET=hw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
XCL_EMULATION_MODE=sw_emu ./host xclbin/krnl_vaddmul.sw_emu.xilinx_u280_xdma_201910_1.xclbin

nohup make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
./host xclbin/krnl_vaddmul.sw_emu.xilinx_u280_xdma_201910_1.xclbin

/tools/Xilinx/SDx/2019.1/bin/xcpp -I../../..//libs/xcl2 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/Vivado/2019.1/include/ -Wall -O0 -g -std=c++14 -fmessage-length=0 ../../..//libs/xcl2/xcl2.cpp src/host.cpp  -o 'host'  -L/opt/xilinx/xrt/lib/ -lOpenCL -lpthread -lrt -lstdc++ 


