#!/bin/bash
#!/bin/bash

DEVICEPATH=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm

./evaluate.sh
faketime -f "-1y" make hw PLATFORM=$DEVICEPATH SYNFREQUENCY=300
cp xclbin/acts.hw.xilinx_u280_xdma_201920_3.xclbin /home/oj2zf/Documents/ActsOfAGraph/outputs/xclbins/golden.xclbin
cp v++_vmult_vadd.link.log /home/oj2zf/Documents/ActsOfAGraph/outputs/xclbins/golden.log


